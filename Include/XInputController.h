#ifndef INCLUDED_XINPUTCONTROLLER
#define INCLUDED_XINPUTCONTROLLER

#define  _WIN32_WINNT  _WIN32_WINNT_WIN7

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include  <Windows.h>
#include  <XInput.h>
#include  <cstring>
#include  <cmath>

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)	// Windows8以上
#pragma comment(lib, "Xinput.lib")
#else									// Windows7以下
#pragma comment(lib, "Xinput9_1_0.lib")
#endif

//*****************************************************************************
// #pragma deprecated対策
//*****************************************************************************
#if(_WIN32_WINNT >= _WIN32_WINNT_WIN10)	// Windows10以上
#pragma warning(disable : 4995)
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************
/* 最大コントローラー数指定用 */
enum ControllerMax {
	CMAX_1 = 1,	// 最大コントローラー数1
	CMAX_2,		// 最大コントローラー数2
	CMAX_3,		// 最大コントローラー数3
	CMAX_4		// 最大コントローラー数4
};

/* コントローラー指定用 */
enum ControllerNumber{
	CNUM_1,	// コントローラー1
	CNUM_2,	// コントローラー2
	CNUM_3,	// コントローラー3
	CNUM_4	// コントローラー4
};

/* ボタン */
#define XIP_D_UP			XINPUT_GAMEPAD_DPAD_UP			// ↑ボタン
#define XIP_D_DOWN			XINPUT_GAMEPAD_DPAD_DOWN		// ↓ボタン
#define XIP_D_LEFT			XINPUT_GAMEPAD_DPAD_LEFT		// ←ボタン
#define XIP_D_RIGHT			XINPUT_GAMEPAD_DPAD_RIGHT		// →ボタン
#define XIP_A				XINPUT_GAMEPAD_A				// Aボタン
#define XIP_B				XINPUT_GAMEPAD_B				// Bボタン
#define XIP_X				XINPUT_GAMEPAD_X				// Xボタン
#define XIP_Y				XINPUT_GAMEPAD_Y				// Yボタン
#define XIP_START			XINPUT_GAMEPAD_START			// STARTボタン
#define XIP_BACK			XINPUT_GAMEPAD_BACK				// BACKボタン
#define XIP_LSB				XINPUT_GAMEPAD_LEFT_THUMB		// 左スティックボタン
#define XIP_RSB				XINPUT_GAMEPAD_RIGHT_THUMB		// 右スティックボタン
#define XIP_LB				XINPUT_GAMEPAD_LEFT_SHOULDER	// Lボタン
#define XIP_RB				XINPUT_GAMEPAD_RIGHT_SHOULDER	// Rボタン
#define XIP_ANY			( 0xffff )

/* 左右識別 */
#define XIP_LEFT			"XIP_LEFT"			// 左
#define XIP_RIGHT			"XIP_RIGHT"			// 右
#define XIP_BOTH			"XIP_BOTH"			// 両方(バイブレーション専用)

/* スティック */
#define XIP_S_UP			"XIP_S_UP"			// スティック↑
#define XIP_S_DOWN			"XIP_S_DOWN"		// スティック↓
#define XIP_S_LEFT			"XIP_S_LEFT"		// スティック←
#define XIP_S_RIGHT			"XIP_S_RIGHT"		// スティック→
#define XIP_S_X				"XIP_S_X"			// スティックX軸
#define XIP_S_Y				"XIP_S_Y"			// スティックY軸

/* バイブレーション */
#define XIP_VIBRATION_MAX	65535				// モーターの最大振動
#define XIP_VIBRATION_HIGH	65535/4*3			// モーターの強振動
#define XIP_VIBRATION_MID	65535/2				// モーターの中振動
#define XIP_VIBRATION_LOW	65535/4				// モーターの弱振動
#define XIP_VIBRATION_OFF	0					// モーターの停止

#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)	// Windows8以上
/* バッテリー */
#define XIP_BATTERY_FULL	BATTERY_LEVEL_FULL		// バッテリー満
#define XIP_BATTERY_MEDIUM	BATTERY_LEVEL_MEDIUM	// バッテリー中
#define XIP_BATTERY_LOW		BATTERY_LEVEL_LOW		// バッテリー小
#define XIP_BATTERY_EMPTY	BATTERY_LEVEL_EMPTY		// バッテリー空

/* ガイドボタン */
#define XINPUT_GAMEPAD_GUIDE	0x0400
#define XIP_GUIDE			XINPUT_GAMEPAD_GUIDE	// ガイドボタン
#endif

//*****************************************************************************
// グローバル
//*****************************************************************************
extern  BYTE	 deadZoneLT;			// 左トリガーのしきい値
extern  BYTE	 deadZoneRT;			// 右トリガーのしきい値
extern  SHORT	 deadZoneLS;			// 左スティックのしきい値
extern  SHORT	 deadZoneRS;			// 右スティックのしきい値
extern  FLOAT	 angularDeadZoneLX;	// 左スティックのX軸のしきい値
extern  FLOAT	 angularDeadZoneLY;	// 左スティックのY軸のしきい値
extern  FLOAT	 angularDeadZoneRX;	// 右スティックのX軸のしきい値
extern  FLOAT	 angularDeadZoneRY;	// 右スティックのY軸のしきい値

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)	// Windows8以上
void	InitController(ControllerMax cMax, bool bUseDLL = false);	// コントローラー初期化処理
#else									// Windows7以下
void	InitController(ControllerMax cMax);							// コントローラー初期化処理
#endif
void	UninitController(void);										// コントローラー終了処理

HRESULT	UpdateController(void);										// コントローラー更新処理

bool	GetControllerButtonPress(WORD wBtn, ControllerNumber cNum = CNUM_1);	// ボタンのプレス状態を取得
bool	GetControllerButtonTrigger(WORD wBtn, ControllerNumber cNum = CNUM_1);	// ボタンのトリガー状態を取得
bool	GetControllerButtonRelease(WORD wBtn, ControllerNumber cNum = CNUM_1);	// ボタンのリリース状態を取得

bool	GetControllerTriggerPress(LPCSTR pLR, ControllerNumber cNum = CNUM_1);		// トリガーのプレス状態を取得
bool	GetControllerTriggerTrigger(LPCSTR pLR, ControllerNumber cNum = CNUM_1);	// トリガーのトリガー状態を取得
bool	GetControllerTriggerRelease(LPCSTR pLR, ControllerNumber cNum = CNUM_1);	// トリガーのリリース状態を取得

bool	GetControllerStickPress(LPCSTR pLR, LPCSTR pUDLR, ControllerNumber cNum = CNUM_1);		// スティックのプレス状態を取得
bool	GetControllerStickTrigger(LPCSTR pLR, LPCSTR pUDLR, ControllerNumber cNum = CNUM_1);	// スティックのトリガー状態を取得
bool	GetControllerStickRelease(LPCSTR pLR, LPCSTR pUDLR, ControllerNumber cNum = CNUM_1);	// スティックのリリース状態を取得

FLOAT	GetControllerStickAxis(LPCSTR pLR, LPCSTR pAxis, ControllerNumber cNum = CNUM_1);		// スティックの軸の状態を取得
FLOAT	GetControllerStickAxisScaled(LPCSTR pLR, LPCSTR pAxis, ControllerNumber cNum = CNUM_1);	// スケーリングされたスティックの軸の状態を取得

void	SetControllerVibration(LPCSTR pLRB, WORD wPow, ControllerNumber cNum = CNUM_1);	// バイブレーション設定

#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)	// Windows8以上
BYTE	GetControllerBattery(ControllerNumber cNum = CNUM_1);	// バッテリー情報取得
#endif

#endif