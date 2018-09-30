#include  <DIKeyboard.h>
#include  <XInputController.h>

// Global Area
LPDIRECTINPUT8		 g_pDI = NULL;;			// DirectInput オブジェクト
LPDIRECTINPUTDEVICE8 g_pdiKeyboard = NULL;	// DirectInput デバイス(キーボード)
LPDIRECTINPUTDEVICE8 g_pDIDevMouse = NULL;		// DirectInput デバイス(マウス)

#define  INPUT_FPS  ( 60.0f )
static  bool  g_isInput = true;
static  bool  g_isInputExit = false;

POINT  g_MousePoint;
DIMOUSESTATE2  g_mouseState;
DIMOUSESTATE2  g_mouseStateTrigger;				// マウスの状態を受け取るワーク
HWND  g_hWnd;

BYTE	g_aKeyState[NUM_KEY_MAX],
g_aKeyStateTrigger[NUM_KEY_MAX],
g_aKeyStateRepeat[NUM_KEY_MAX],
g_aKeyStateRelease[NUM_KEY_MAX];
int		g_aKeyStateRepeatCnt[NUM_KEY_MAX];


void  Input::UpdateInput()
{
		while ( g_isInputExit == false )
		{
				while ( g_isInput )
				{
						UpdateController();
						UpdateKeyboard();
						UpdateMouse();
				}
		}

}


VOID WINAPI  Input::DI_Term() {// DirectInput システムの終了
	if (g_pDI) {
		if (g_pdiKeyboard) {
			g_pdiKeyboard->Unacquire();	// DirectInput デバイスへのアクセスを解放。
			g_pdiKeyboard->Release();	// DirectInput デバイスを解放。
			g_pdiKeyboard = NULL;		// DirectInput デバイスを空にする。
		}

		UninitMouse();

		g_pDI->Release();	// DirectInput オブジェクトの解放。
		g_pDI = NULL;		// DirectInput オブジェクトを空にする。
	}
}

VOID WINAPI  Input::UpdateKeyboard()
{
	BYTE	aKeyState[NUM_KEY_MAX];

	if (g_pdiKeyboard) { g_pdiKeyboard->Acquire(); }

	// キーボードのデータ取得
	if (FAILED(g_pdiKeyboard->GetDeviceState(sizeof(aKeyState), (LPVOID)&aKeyState))) {
		// このメソッドは2 つのパラメータを受け取る。それぞれデバイスステートデータを格納するバッファのサイズ、そのバッファへのポインタ。
		// キーボードの場合は、常に符号なし 256 バイトのバッファを宣言。
		return;
	}
	// 呼出し時点に押されている特定のキーに応答できる。バッファ内の各要素は、1 つのキーを表す。
	// 要素の上位ビットが1 ならば呼出し時点でそのキーが押されており、それ以外ならばキーは押されていない。
	// 特定のキーの状態をチェックするには、DirectInput キーボードデバイスを使用して、特定のキーのバッファにインデックスを作成する。

	for (int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
	{// キートリガー・リリース情報を生成

		g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey])&aKeyState[nCnKey];
		g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey])&~aKeyState[nCnKey];

		// キーリピート情報を生成
		if (aKeyState[nCnKey])
		{
			if (g_aKeyStateRepeatCnt[nCnKey] < LIMIT_COUNT_REPEEAT)
			{
				g_aKeyStateRepeatCnt[nCnKey]++;
				if (g_aKeyStateRepeatCnt[nCnKey] == 1 || g_aKeyStateRepeatCnt[nCnKey] >= LIMIT_COUNT_REPEEAT)
				{// キーを押し始めた最初のフレーム、または一定時経過したらきーリピート情報
					g_aKeyStateRepeat[nCnKey] = aKeyState[nCnKey];
				}
				else {
					g_aKeyStateRepeat[nCnKey] = 0;
				}
			}
		}
		else {
			g_aKeyStateRepeatCnt[nCnKey] = 0;
			g_aKeyStateRepeat[nCnKey] = 0;
		}
		// キープレスを記憶
		g_aKeyState[nCnKey] = aKeyState[nCnKey];
	}
}

BOOL WINAPI  Input::DI_Init(HWND hWnd, HINSTANCE hInstance) {
	// DirectInput オブジェクト作成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&g_pDI, NULL))) {
		// 引数1：オブジェクトを作成するアプリケーションへのインスタンスハンドルを指定; 
		// 引数2：DirectInput のシステムバージョンを指定;
		// 引数3：返されるインターフェイスを指定。ほとんどは、IID_IDirectInput8 を渡して最新バージョンを取得する;
		// 引数4：呼出しが正常に行われると、変数が初期化されて有効なインターフェイスポインタが格納される;
		// 引数5：COM 集成に使用する制御オブジェクトのIUnknown インターフェイスのアドレスを指定。ほとんどは使用しないのでNULL でいい;
		return FALSE;
	}

	// DirectInput キーボードデバイス作成
	if (FAILED(g_pDI->CreateDevice(GUID_SysKeyboard, &g_pdiKeyboard, NULL))) {
		// 引数1：作成するデバイスのグローバル一意識別子(GUID)を指定。システムキーボードを使用するため、GUID_SysKeyboard を渡す;
		// 引数2：呼出しが正常に行われると、この変数が初期化されてインターフェイスポインタが格納される;
		// 引数3：DI オブジェクト作成の時と同様で、集成を使用しないので、通常はNULL を指定;
		DI_Term();
		return FALSE;
	}

	// キーボードデータフォーマットの設定
	if (FAILED(g_pdiKeyboard->SetDataFormat(&c_dfDIKeyboard))) {
		// 定義済みの c_dfDIKeyboard グローバス変数に DirectInput が提供するデータフォーマットを指定する。
		DI_Term();
		return FALSE;
	}

	// キーボードの動作設定
	if (FAILED(g_pdiKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) {
		// 引数1：デバイスに関連付けるウィンドウへのハンドルを指定; 
		// 引数2：協調レベルを決定するフラグの組み合わせを指定;
		DI_Term();
		return FALSE;
	}

	// キーボードへのアクセスの獲得
	if (g_pdiKeyboard) { g_pdiKeyboard->Acquire(); }
	// ウィンドウが非アクティブになるとアクセスの再獲得（更新）する必要がある

	g_hWnd = hWnd;

	return TRUE;
}

/* Get boolean */

bool  Input::IsKeyPress(int nKey) {
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}


bool  Input::IsKeyTrigger(int nKey) {
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}


bool  Input::IsKeyRepeat(int nKey) {
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}


bool  Input::IsKeyRelease(int nKey) {
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

/* get float value */

float  Input::KeyVertical() {
		return  float( g_aKeyState[DIK_W] & 0x80 ) + -float( g_aKeyState[DIK_S] & 0x80 );
}


float  Input::KeyHorizontal() {
		return  float( g_aKeyState[DIK_D] & 0x80 ) + -float( g_aKeyState[DIK_A] & 0x80 );
}

//=============================================================================
// マウスの初期化
//=============================================================================
HRESULT Input::InitMouse( HWND hWnd )
{
		HRESULT hr;

		// デバイスオブジェクトを作成
		hr = g_pDI->CreateDevice( GUID_SysMouse, &g_pDIDevMouse, NULL );
		if ( FAILED( hr ) || g_pDIDevMouse == NULL )
		{
				MessageBox( hWnd, TEXT( "マウスがねぇ！" ), TEXT( "警告！" ), MB_ICONWARNING );
				return hr;
		}

		// データフォーマットを設定
		hr = g_pDIDevMouse->SetDataFormat( &c_dfDIMouse2 );
		if ( FAILED( hr ) )
		{
				MessageBox( hWnd, TEXT( "マウスのデータフォーマットを設定できませんでした。" ), TEXT( "警告！" ), MB_ICONWARNING );
				return hr;
		}

		// 協調モードを設定（フォアグラウンド＆非排他モード）
		hr = g_pDIDevMouse->SetCooperativeLevel( hWnd, ( DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) );
		if ( FAILED( hr ) )
		{
				MessageBox( hWnd, TEXT( "マウスの協調モードを設定できませんでした。" ), TEXT( "警告！" ), MB_ICONWARNING );
				return hr;
		}

		// デバイスの設定
		{
				DIPROPDWORD dipdw;

				dipdw.diph.dwSize = sizeof( dipdw );
				dipdw.diph.dwHeaderSize = sizeof( dipdw.diph );
				dipdw.diph.dwObj = 0;
				dipdw.diph.dwHow = DIPH_DEVICE;
				dipdw.dwData = DIPROPAXISMODE_REL;

				hr = g_pDIDevMouse->SetProperty( DIPROP_AXISMODE, &dipdw.diph );
		}

		// キーボードへのアクセス権を獲得(入力制御開始)
		g_pDIDevMouse->Acquire();

		return S_OK;
}

//=============================================================================
// マウスの終了処理
//=============================================================================
void Input::UninitMouse( void )
{
		if ( g_pDIDevMouse )
		{
				g_pDIDevMouse->Unacquire();

				g_pDIDevMouse->Release();
				g_pDIDevMouse = NULL;
		}
}

//=============================================================================
// マウスの更新処理
//=============================================================================
HRESULT  Input::UpdateMouse()
{
		HRESULT hr;
		DIMOUSESTATE2 mouseStateOld;

		// 前回のデータを保存
		mouseStateOld = g_mouseState;

		GetCursorPos( &g_MousePoint );
		ScreenToClient( g_hWnd, &g_MousePoint );


		// デバイスからデータを取得
		hr = g_pDIDevMouse->GetDeviceState( sizeof( g_mouseState ), &g_mouseState );
		if ( SUCCEEDED( hr ) )
		{
				// トリガーの取得
				//g_mouseStateTrigger.lX = 
				//g_mouseStateTrigger.lY = 
				//g_mouseStateTrigger.lZ = 
				for ( int cnt = 0; cnt < 8; cnt++ )
				{
						g_mouseStateTrigger.rgbButtons[cnt] = ( ( mouseStateOld.rgbButtons[cnt] ^ g_mouseState.rgbButtons[cnt] ) & g_mouseState.rgbButtons[cnt] );
				}
		}
		else
		{
				g_pDIDevMouse->Acquire();
		}

		return S_OK;
}

//=============================================================================
// マウスデータ取得(左プレス)
//=============================================================================
bool  Input::MouseLeftPress( void )
{
		return ( ( g_mouseState.rgbButtons[0] & 0x80 ) != 0 );
}

//=============================================================================
// マウスデータ取得(左トリガー)
//=============================================================================
bool  Input::MouseLeftTrigger( void )
{
		return ( ( g_mouseStateTrigger.rgbButtons[0] & 0x80 ) != 0 );
}

//=============================================================================
// マウスデータ取得(右プレス)
//=============================================================================
bool  Input::MouseRightPress( void )
{
		return ( ( g_mouseState.rgbButtons[1] & 0x80 ) != 0 );
}

//=============================================================================
// マウスデータ取得(右トリガー)
//=============================================================================
bool  Input::MouseRightTrigger( void )
{
		return ( ( g_mouseStateTrigger.rgbButtons[1] & 0x80 ) != 0 );
}

//=============================================================================
// マウスデータ取得(中央プレス)
//=============================================================================
bool  Input::MouseCenterPress( void )
{
		return ( ( g_mouseState.rgbButtons[2] & 0x80 ) != 0 );
}

//=============================================================================
// マウスデータ取得(中央トリガー)
//=============================================================================
bool  Input::MouseCenterTrigger( void )
{
		return ( ( g_mouseState.rgbButtons[2] & 0x80 ) != 0 );
}

//=============================================================================
// マウスデータ取得(Ｘ軸移動)
//=============================================================================
float  Input::MouseAxisX( void )
{
		return  ( float ) g_mouseState.lX;
}

//=============================================================================
// マウスデータ取得(Ｙ軸移動)
//=============================================================================
float  Input::MouseAxisY( void )
{
		return  ( float ) g_mouseState.lY;
}

//=============================================================================
// マウスデータ取得(Ｚ軸移動)
//=============================================================================
float  Input::MouseAxisZ( void )
{
		return  ( float ) g_mouseState.lZ;
}
//=============================================================================
// マウス座標取得(X)
//=============================================================================
float  Input::MouseX( void )
{
		return  ( float ) g_MousePoint.x;
}

//=============================================================================
// マウス座標取得(Y)
//=============================================================================
float  Input::MouseY( void )
{
		return  ( float ) g_MousePoint.y;
}