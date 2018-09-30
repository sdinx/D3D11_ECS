#include  <DIKeyboard.h>
#include  <XInputController.h>

// Global Area
LPDIRECTINPUT8		 g_pDI = NULL;;			// DirectInput �I�u�W�F�N�g
LPDIRECTINPUTDEVICE8 g_pdiKeyboard = NULL;	// DirectInput �f�o�C�X(�L�[�{�[�h)
LPDIRECTINPUTDEVICE8 g_pDIDevMouse = NULL;		// DirectInput �f�o�C�X(�}�E�X)

#define  INPUT_FPS  ( 60.0f )
static  bool  g_isInput = true;
static  bool  g_isInputExit = false;

POINT  g_MousePoint;
DIMOUSESTATE2  g_mouseState;
DIMOUSESTATE2  g_mouseStateTrigger;				// �}�E�X�̏�Ԃ��󂯎�郏�[�N
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


VOID WINAPI  Input::DI_Term() {// DirectInput �V�X�e���̏I��
	if (g_pDI) {
		if (g_pdiKeyboard) {
			g_pdiKeyboard->Unacquire();	// DirectInput �f�o�C�X�ւ̃A�N�Z�X������B
			g_pdiKeyboard->Release();	// DirectInput �f�o�C�X������B
			g_pdiKeyboard = NULL;		// DirectInput �f�o�C�X����ɂ���B
		}

		UninitMouse();

		g_pDI->Release();	// DirectInput �I�u�W�F�N�g�̉���B
		g_pDI = NULL;		// DirectInput �I�u�W�F�N�g����ɂ���B
	}
}

VOID WINAPI  Input::UpdateKeyboard()
{
	BYTE	aKeyState[NUM_KEY_MAX];

	if (g_pdiKeyboard) { g_pdiKeyboard->Acquire(); }

	// �L�[�{�[�h�̃f�[�^�擾
	if (FAILED(g_pdiKeyboard->GetDeviceState(sizeof(aKeyState), (LPVOID)&aKeyState))) {
		// ���̃��\�b�h��2 �̃p�����[�^���󂯎��B���ꂼ��f�o�C�X�X�e�[�g�f�[�^���i�[����o�b�t�@�̃T�C�Y�A���̃o�b�t�@�ւ̃|�C���^�B
		// �L�[�{�[�h�̏ꍇ�́A��ɕ����Ȃ� 256 �o�C�g�̃o�b�t�@��錾�B
		return;
	}
	// �ďo�����_�ɉ�����Ă������̃L�[�ɉ����ł���B�o�b�t�@���̊e�v�f�́A1 �̃L�[��\���B
	// �v�f�̏�ʃr�b�g��1 �Ȃ�Όďo�����_�ł��̃L�[��������Ă���A����ȊO�Ȃ�΃L�[�͉�����Ă��Ȃ��B
	// ����̃L�[�̏�Ԃ��`�F�b�N����ɂ́ADirectInput �L�[�{�[�h�f�o�C�X���g�p���āA����̃L�[�̃o�b�t�@�ɃC���f�b�N�X���쐬����B

	for (int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
	{// �L�[�g���K�[�E�����[�X���𐶐�

		g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey])&aKeyState[nCnKey];
		g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey])&~aKeyState[nCnKey];

		// �L�[���s�[�g���𐶐�
		if (aKeyState[nCnKey])
		{
			if (g_aKeyStateRepeatCnt[nCnKey] < LIMIT_COUNT_REPEEAT)
			{
				g_aKeyStateRepeatCnt[nCnKey]++;
				if (g_aKeyStateRepeatCnt[nCnKey] == 1 || g_aKeyStateRepeatCnt[nCnKey] >= LIMIT_COUNT_REPEEAT)
				{// �L�[�������n�߂��ŏ��̃t���[���A�܂��͈�莞�o�߂����炫�[���s�[�g���
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
		// �L�[�v���X���L��
		g_aKeyState[nCnKey] = aKeyState[nCnKey];
	}
}

BOOL WINAPI  Input::DI_Init(HWND hWnd, HINSTANCE hInstance) {
	// DirectInput �I�u�W�F�N�g�쐬
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&g_pDI, NULL))) {
		// ����1�F�I�u�W�F�N�g���쐬����A�v���P�[�V�����ւ̃C���X�^���X�n���h�����w��; 
		// ����2�FDirectInput �̃V�X�e���o�[�W�������w��;
		// ����3�F�Ԃ����C���^�[�t�F�C�X���w��B�قƂ�ǂ́AIID_IDirectInput8 ��n���čŐV�o�[�W�������擾����;
		// ����4�F�ďo��������ɍs����ƁA�ϐ�������������ėL���ȃC���^�[�t�F�C�X�|�C���^���i�[�����;
		// ����5�FCOM �W���Ɏg�p���鐧��I�u�W�F�N�g��IUnknown �C���^�[�t�F�C�X�̃A�h���X���w��B�قƂ�ǂ͎g�p���Ȃ��̂�NULL �ł���;
		return FALSE;
	}

	// DirectInput �L�[�{�[�h�f�o�C�X�쐬
	if (FAILED(g_pDI->CreateDevice(GUID_SysKeyboard, &g_pdiKeyboard, NULL))) {
		// ����1�F�쐬����f�o�C�X�̃O���[�o����ӎ��ʎq(GUID)���w��B�V�X�e���L�[�{�[�h���g�p���邽�߁AGUID_SysKeyboard ��n��;
		// ����2�F�ďo��������ɍs����ƁA���̕ϐ�������������ăC���^�[�t�F�C�X�|�C���^���i�[�����;
		// ����3�FDI �I�u�W�F�N�g�쐬�̎��Ɠ��l�ŁA�W�����g�p���Ȃ��̂ŁA�ʏ��NULL ���w��;
		DI_Term();
		return FALSE;
	}

	// �L�[�{�[�h�f�[�^�t�H�[�}�b�g�̐ݒ�
	if (FAILED(g_pdiKeyboard->SetDataFormat(&c_dfDIKeyboard))) {
		// ��`�ς݂� c_dfDIKeyboard �O���[�o�X�ϐ��� DirectInput ���񋟂���f�[�^�t�H�[�}�b�g���w�肷��B
		DI_Term();
		return FALSE;
	}

	// �L�[�{�[�h�̓���ݒ�
	if (FAILED(g_pdiKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) {
		// ����1�F�f�o�C�X�Ɋ֘A�t����E�B���h�E�ւ̃n���h�����w��; 
		// ����2�F�������x�������肷��t���O�̑g�ݍ��킹���w��;
		DI_Term();
		return FALSE;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�̊l��
	if (g_pdiKeyboard) { g_pdiKeyboard->Acquire(); }
	// �E�B���h�E����A�N�e�B�u�ɂȂ�ƃA�N�Z�X�̍Ċl���i�X�V�j����K�v������

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
// �}�E�X�̏�����
//=============================================================================
HRESULT Input::InitMouse( HWND hWnd )
{
		HRESULT hr;

		// �f�o�C�X�I�u�W�F�N�g���쐬
		hr = g_pDI->CreateDevice( GUID_SysMouse, &g_pDIDevMouse, NULL );
		if ( FAILED( hr ) || g_pDIDevMouse == NULL )
		{
				MessageBox( hWnd, TEXT( "�}�E�X���˂��I" ), TEXT( "�x���I" ), MB_ICONWARNING );
				return hr;
		}

		// �f�[�^�t�H�[�}�b�g��ݒ�
		hr = g_pDIDevMouse->SetDataFormat( &c_dfDIMouse2 );
		if ( FAILED( hr ) )
		{
				MessageBox( hWnd, TEXT( "�}�E�X�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B" ), TEXT( "�x���I" ), MB_ICONWARNING );
				return hr;
		}

		// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
		hr = g_pDIDevMouse->SetCooperativeLevel( hWnd, ( DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) );
		if ( FAILED( hr ) )
		{
				MessageBox( hWnd, TEXT( "�}�E�X�̋������[�h��ݒ�ł��܂���ł����B" ), TEXT( "�x���I" ), MB_ICONWARNING );
				return hr;
		}

		// �f�o�C�X�̐ݒ�
		{
				DIPROPDWORD dipdw;

				dipdw.diph.dwSize = sizeof( dipdw );
				dipdw.diph.dwHeaderSize = sizeof( dipdw.diph );
				dipdw.diph.dwObj = 0;
				dipdw.diph.dwHow = DIPH_DEVICE;
				dipdw.dwData = DIPROPAXISMODE_REL;

				hr = g_pDIDevMouse->SetProperty( DIPROP_AXISMODE, &dipdw.diph );
		}

		// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
		g_pDIDevMouse->Acquire();

		return S_OK;
}

//=============================================================================
// �}�E�X�̏I������
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
// �}�E�X�̍X�V����
//=============================================================================
HRESULT  Input::UpdateMouse()
{
		HRESULT hr;
		DIMOUSESTATE2 mouseStateOld;

		// �O��̃f�[�^��ۑ�
		mouseStateOld = g_mouseState;

		GetCursorPos( &g_MousePoint );
		ScreenToClient( g_hWnd, &g_MousePoint );


		// �f�o�C�X����f�[�^���擾
		hr = g_pDIDevMouse->GetDeviceState( sizeof( g_mouseState ), &g_mouseState );
		if ( SUCCEEDED( hr ) )
		{
				// �g���K�[�̎擾
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
// �}�E�X�f�[�^�擾(���v���X)
//=============================================================================
bool  Input::MouseLeftPress( void )
{
		return ( ( g_mouseState.rgbButtons[0] & 0x80 ) != 0 );
}

//=============================================================================
// �}�E�X�f�[�^�擾(���g���K�[)
//=============================================================================
bool  Input::MouseLeftTrigger( void )
{
		return ( ( g_mouseStateTrigger.rgbButtons[0] & 0x80 ) != 0 );
}

//=============================================================================
// �}�E�X�f�[�^�擾(�E�v���X)
//=============================================================================
bool  Input::MouseRightPress( void )
{
		return ( ( g_mouseState.rgbButtons[1] & 0x80 ) != 0 );
}

//=============================================================================
// �}�E�X�f�[�^�擾(�E�g���K�[)
//=============================================================================
bool  Input::MouseRightTrigger( void )
{
		return ( ( g_mouseStateTrigger.rgbButtons[1] & 0x80 ) != 0 );
}

//=============================================================================
// �}�E�X�f�[�^�擾(�����v���X)
//=============================================================================
bool  Input::MouseCenterPress( void )
{
		return ( ( g_mouseState.rgbButtons[2] & 0x80 ) != 0 );
}

//=============================================================================
// �}�E�X�f�[�^�擾(�����g���K�[)
//=============================================================================
bool  Input::MouseCenterTrigger( void )
{
		return ( ( g_mouseState.rgbButtons[2] & 0x80 ) != 0 );
}

//=============================================================================
// �}�E�X�f�[�^�擾(�w���ړ�)
//=============================================================================
float  Input::MouseAxisX( void )
{
		return  ( float ) g_mouseState.lX;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�x���ړ�)
//=============================================================================
float  Input::MouseAxisY( void )
{
		return  ( float ) g_mouseState.lY;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�y���ړ�)
//=============================================================================
float  Input::MouseAxisZ( void )
{
		return  ( float ) g_mouseState.lZ;
}
//=============================================================================
// �}�E�X���W�擾(X)
//=============================================================================
float  Input::MouseX( void )
{
		return  ( float ) g_MousePoint.x;
}

//=============================================================================
// �}�E�X���W�擾(Y)
//=============================================================================
float  Input::MouseY( void )
{
		return  ( float ) g_MousePoint.y;
}