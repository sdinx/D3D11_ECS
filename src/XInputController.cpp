#include <XInputController.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PI	3.141592654f
#define ToRadian(degree) ((degree) * (PI / 180.0f))
#define ToDegree(radian) ((radian) * (180.0f / PI))

#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)	// Windows8以上
#define FNC_XINPUTGETSTATEEX			((const char*)100)	// XInputGetStateEX用
//#define FNC_XINPUTWAITFORGUIDEBUTTON	((const char*)101)	// XInputWaitForGuideButton用
//#define FNC_XINPUTCANCELGUIDEBUTTONWAIT	((const char*)102)	// XInputCancelGuideButtonWait用
//#define FNC_XINPUTPOWEROFFCONTROLLER	((const char*)103)	// XInputPowerOffController用
// 他にも104と108があるが不明
#endif

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct CONTROLLER_STATE {
	XINPUT_STATE state;						// 現在の入力情報
	XINPUT_STATE old;						// 前回の入力情報
	XINPUT_VIBRATION vibration;				// バイブレーション情報
#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)		// Windows8以上
	XINPUT_BATTERY_INFORMATION battery;		// バッテリー情報
#endif
	bool bConnected;						// 接続確認情報
} CONTROLLER_STATE;

//*****************************************************************************
// グローバル
//*****************************************************************************
CONTROLLER_STATE *controller;	// コントローラー構造体
BYTE maxControllers = CMAX_4;	// 最大コントローラー数(XInputが認識できるのは4つまで)

BYTE	deadZoneLT			= 255 / 10;		// 左トリガーのしきい値
BYTE	deadZoneRT			= 255 / 10;		// 右トリガーのしきい値
SHORT	deadZoneLS			= 32767 / 5;	// 左スティックのしきい値
SHORT	deadZoneRS			= 32767 / 5;	// 右スティックのしきい値
FLOAT	angularDeadZoneLX	= 5.0f;			// 左スティックのX軸のしきい値
FLOAT	angularDeadZoneLY	= 5.0f;			// 左スティックのY軸のしきい値
FLOAT	angularDeadZoneRX	= 5.0f;			// 右スティックのX軸のしきい値
FLOAT	angularDeadZoneRY	= 5.0f;			// 右スティックのY軸のしきい値

#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)	// Windows8以上
/* XInputDLL関連 */
HMODULE XInput_dll = nullptr;
using XINPUTGETSTATEEX				= DWORD(WINAPI *)(DWORD dwUserIndex, XINPUT_STATE *pState);
//using XINPUTWAITFORGUIDEBUTTON		= DWORD(WINAPI *)(DWORD dwUserIndex, DWORD dwFlag, LPVOID pVoid);
//using XINPUTCANCELGUIDEBUTTONWAIT	= DWORD(WINAPI *)(DWORD dwUserIndex);
//using XINPUTPOWEROFFCONTROLLER		= DWORD(WINAPI *)(DWORD dwUserIndex);
XINPUTGETSTATEEX			XInputGetStateEx = nullptr;
//XINPUTWAITFORGUIDEBUTTON	XInputWaitForGuideButton = nullptr;
//XINPUTCANCELGUIDEBUTTONWAIT XInputCancelGuideButtonWait = nullptr;
//XINPUTPOWEROFFCONTROLLER	XInputPowerOffController = nullptr;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void LoadXInputDLL(void);	// XInputDLLのロード
void FreeXInputDLL(void);	// XInputDLLの解放

//=============================================================================
// XInputDLLのロード
//=============================================================================
void LoadXInputDLL(void) {
	if (XInput_dll = ::LoadLibrary(XINPUT_DLL)) {
		XInputGetStateEx = (XINPUTGETSTATEEX)::GetProcAddress(XInput_dll, FNC_XINPUTGETSTATEEX);
	}
	else {
		::FreeLibrary(XInput_dll);
	}
}

//=============================================================================
// XInputDLLの解放
//=============================================================================
void FreeXInputDLL(void) {
	if (XInput_dll) {
		::FreeLibrary(XInput_dll);
	}
}

//=============================================================================
// コントローラー初期化処理(Windows8以上)
//=============================================================================
void InitController(ControllerMax cMax, bool bUseDLL) {
	if (bUseDLL) {
		LoadXInputDLL();	// XInputDLLのロード
	}
	if (cMax >= CMAX_1 && cMax <= CMAX_4) {
		maxControllers = cMax;
	}
	controller = new CONTROLLER_STATE[maxControllers];
	SecureZeroMemory(controller, sizeof(CONTROLLER_STATE) * maxControllers);
}
#else

//=============================================================================
// コントローラー初期化処理(Windows7以下)
//=============================================================================
void InitController(ControllerMax cMax) {
	if (cMax >= CMAX_1 && cMax <= CMAX_4) {
		maxControllers = cMax;
	}
	controller = new CONTROLLER_STATE[maxControllers];
	SecureZeroMemory(controller, sizeof(CONTROLLER_STATE) * maxControllers);
}
#endif

//=============================================================================
// コントローラー終了処理
//=============================================================================
void UninitController(void) {
	delete[] controller;
#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)	// Windows8以上
	FreeXInputDLL();		// XInputDLLの解放
	XInputEnable(false);	// XInput無効化
#endif
}

//=============================================================================
// コントローラー更新処理
//=============================================================================
HRESULT UpdateController(void) {
	DWORD Result;

	for (BYTE i = 0; i < maxControllers; i++) {
		SecureZeroMemory(&controller[i].old, sizeof(XINPUT_STATE));		// 前回の入力情報のメモリ初期化
		controller[i].old = controller[i].state;						// 現在の入力情報を前回の入力情報へ保存
		SecureZeroMemory(&controller[i].state, sizeof(XINPUT_STATE));	// 現在の入力情報のメモリ初期化
		
#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)	// Windows8以上
		/* コントローラー情報取得 */
		if (XInput_dll) {											// XInputDLLがある場合
			Result = XInputGetStateEx(i, &controller[i].state);		// 入力情報取得(ガイドボタンあり)
		}
		else {														// XInputDLLがない場合
			Result = XInputGetState(i, &controller[i].state);		// 入力情報取得(ガイドボタンなし)
		}
		SecureZeroMemory(&controller[i].battery, sizeof(XINPUT_BATTERY_INFORMATION));		// バッテリー情報のメモリ初期化
		XInputGetBatteryInformation(i, BATTERY_DEVTYPE_GAMEPAD, &controller[i].battery);	// バッテリー情報取得
#else
		/* コントローラー情報取得 */
		Result = XInputGetState(i, &controller[i].state);			// 入力情報取得(ガイドボタンなし)
#endif
		/* 接続確認 */
		if (Result == ERROR_SUCCESS) {
			controller[i].bConnected = true;	// 接続されている
		}
		else {
			controller[i].bConnected = false;	// 接続されていない
		}
	}

	return S_OK;
}

//=============================================================================
// ボタンのプレス状態を取得
//=============================================================================
bool GetControllerButtonPress(WORD wBtn, ControllerNumber cNum) {
	if (cNum >= CNUM_1 && cNum < maxControllers) {
		if (controller[cNum].bConnected) {
			return (controller[cNum].state.Gamepad.wButtons & wBtn) ? true : false;
		}
	}
	return 0;
}

//=============================================================================
// ボタンのトリガー状態を取得
//=============================================================================
bool GetControllerButtonTrigger(WORD wBtn, ControllerNumber cNum) {
	if (cNum >= CNUM_1 && cNum < maxControllers) {
		if (controller[cNum].bConnected) {
			if ((controller[cNum].state.Gamepad.wButtons & wBtn) != (controller[cNum].old.Gamepad.wButtons & wBtn)) {
				return (controller[cNum].state.Gamepad.wButtons & wBtn) ? true : false;
			}
		}
	}
	return 0;
}

//=============================================================================
// ボタンのリリース状態を取得
//=============================================================================
bool GetControllerButtonRelease(WORD wBtn, ControllerNumber cNum) {
	if (cNum >= CNUM_1 && cNum < maxControllers) {
		if (controller[cNum].bConnected) {
			if ((controller[cNum].state.Gamepad.wButtons & wBtn) != (controller[cNum].old.Gamepad.wButtons & wBtn)) {
				return (controller[cNum].old.Gamepad.wButtons & wBtn) ? true : false;
			}
		}
	}
	return 0;
}

//=============================================================================
// トリガーのプレス状態を取得
//=============================================================================
bool GetControllerTriggerPress(LPCSTR pLR, ControllerNumber cNum) {
	if (cNum >= CNUM_1 && cNum < maxControllers) {
		if (controller[cNum].bConnected) {
			if (strcmp(pLR, XIP_LEFT) == 0) {
				return (controller[cNum].state.Gamepad.bLeftTrigger > deadZoneLT) ? true : false;
			}
			else if (strcmp(pLR, XIP_RIGHT) == 0) {
				return (controller[cNum].state.Gamepad.bRightTrigger > deadZoneRT) ? true : false;
			}
		}
	}
	return 0;
}

//=============================================================================
// トリガーのトリガー状態を取得
//=============================================================================
bool GetControllerTriggerTrigger(LPCSTR pLR, ControllerNumber cNum) {
	if (cNum >= CNUM_1 && cNum < maxControllers) {
		if (controller[cNum].bConnected) {
			if (strcmp(pLR, XIP_LEFT) == 0) {
				if ((controller[cNum].state.Gamepad.bLeftTrigger > deadZoneLT) != (controller[cNum].old.Gamepad.bLeftTrigger > deadZoneLT)) {
					return (controller[cNum].state.Gamepad.bLeftTrigger > deadZoneLT) ? true : false;
				}
			}
			else if (strcmp(pLR, XIP_RIGHT) == 0) {
				if ((controller[cNum].state.Gamepad.bRightTrigger > deadZoneRT) != (controller[cNum].old.Gamepad.bRightTrigger > deadZoneRT)) {
					return (controller[cNum].state.Gamepad.bRightTrigger > deadZoneRT) ? true : false;
				}
			}
		}
	}
	return 0;
}

//=============================================================================
// トリガーのリリース状態を取得
//=============================================================================
bool GetControllerTriggerRelease(LPCSTR pLR, ControllerNumber cNum) {
	if (cNum >= CNUM_1 && cNum < maxControllers) {
		if (controller[cNum].bConnected) {
			if (strcmp(pLR, XIP_LEFT) == 0) {
				if ((controller[cNum].state.Gamepad.bLeftTrigger > deadZoneLT) != (controller[cNum].old.Gamepad.bLeftTrigger > deadZoneLT)) {
					return (controller[cNum].old.Gamepad.bLeftTrigger > deadZoneLT) ? true : false;
				}
			}
			else if (strcmp(pLR, XIP_RIGHT) == 0) {
				if ((controller[cNum].state.Gamepad.bRightTrigger > deadZoneRT) != (controller[cNum].old.Gamepad.bRightTrigger > deadZoneRT)) {
					return (controller[cNum].old.Gamepad.bRightTrigger > deadZoneRT) ? true : false;
				}
			}
		}
	}
	return 0;
}

//=============================================================================
// スティックのプレス状態を取得
//=============================================================================
bool GetControllerStickPress(LPCSTR pLR, LPCSTR pUDLR, ControllerNumber cNum) {
	if (cNum >= CNUM_1 && cNum < maxControllers) {
		if (controller[cNum].bConnected) {
			if (strcmp(pLR, XIP_LEFT) == 0) {
				if (strcmp(pUDLR, XIP_S_UP) == 0) {
					return (controller[cNum].state.Gamepad.sThumbLY > deadZoneLS) ? true : false;
				}
				else if (strcmp(pUDLR, XIP_S_DOWN) == 0) {
					return (controller[cNum].state.Gamepad.sThumbLY < -deadZoneLS) ? true : false;
				}
				else if (strcmp(pUDLR, XIP_S_LEFT) == 0) {
					return (controller[cNum].state.Gamepad.sThumbLX < -deadZoneLS) ? true : false;
				}
				else if (strcmp(pUDLR, XIP_S_RIGHT) == 0) {
					return (controller[cNum].state.Gamepad.sThumbLX > deadZoneLS) ? true : false;
				}
			}
			else if (strcmp(pLR, XIP_RIGHT) == 0) {
				if (strcmp(pUDLR, XIP_S_UP) == 0) {
					return (controller[cNum].state.Gamepad.sThumbRY > deadZoneRS) ? true : false;
				}
				else if (strcmp(pUDLR, XIP_S_DOWN) == 0) {
					return (controller[cNum].state.Gamepad.sThumbRY < -deadZoneRS) ? true : false;
				}
				else if (strcmp(pUDLR, XIP_S_LEFT) == 0) {
					return (controller[cNum].state.Gamepad.sThumbRX < -deadZoneRS) ? true : false;
				}
				else if (strcmp(pUDLR, XIP_S_RIGHT) == 0) {
					return (controller[cNum].state.Gamepad.sThumbRX > deadZoneRS) ? true : false;
				}
			}
		}
	}
	return 0;
}

//=============================================================================
// スティックのトリガー状態を取得
//=============================================================================
bool GetControllerStickTrigger(LPCSTR pLR, LPCSTR pUDLR, ControllerNumber cNum) {
	if (cNum >= CNUM_1 && cNum < maxControllers) {
		if (controller[cNum].bConnected) {
			if (strcmp(pLR, XIP_LEFT) == 0) {
				if (strcmp(pUDLR, XIP_S_UP) == 0) {
					if ((controller[cNum].state.Gamepad.sThumbLY > deadZoneLS) != (controller[cNum].old.Gamepad.sThumbLY > deadZoneLS)) {
						return (controller[cNum].state.Gamepad.sThumbLY > deadZoneLS) ? true : false;
					}
				}
				else if (strcmp(pUDLR, XIP_S_DOWN) == 0) {
					if ((controller[cNum].state.Gamepad.sThumbLY < -deadZoneLS) != (controller[cNum].old.Gamepad.sThumbLY < -deadZoneLS)) {
						return (controller[cNum].state.Gamepad.sThumbLY < -deadZoneLS) ? true : false;
					}
				}
				else if (strcmp(pUDLR, XIP_S_LEFT) == 0) {
					if ((controller[cNum].state.Gamepad.sThumbLX < -deadZoneLS) != (controller[cNum].old.Gamepad.sThumbLX < -deadZoneLS)) {
						return (controller[cNum].state.Gamepad.sThumbLX < -deadZoneLS) ? true : false;
					}
				}
				else if (strcmp(pUDLR, XIP_S_RIGHT) == 0) {
					if ((controller[cNum].state.Gamepad.sThumbLX > deadZoneLS) != (controller[cNum].old.Gamepad.sThumbLX > deadZoneLS)) {
						return (controller[cNum].state.Gamepad.sThumbLX > deadZoneLS) ? true : false;
					}
				}
			}
			else if (strcmp(pLR, XIP_RIGHT) == 0) {
				if (strcmp(pUDLR, XIP_S_UP) == 0) {
					if ((controller[cNum].state.Gamepad.sThumbRY > deadZoneRS) != (controller[cNum].old.Gamepad.sThumbRY > deadZoneRS)) {
						return (controller[cNum].state.Gamepad.sThumbRY > deadZoneRS) ? true : false;
					}
				}
				else if (strcmp(pUDLR, XIP_S_DOWN) == 0) {
					if ((controller[cNum].state.Gamepad.sThumbRY < -deadZoneRS) != (controller[cNum].old.Gamepad.sThumbRY < -deadZoneRS)) {
						return (controller[cNum].state.Gamepad.sThumbRY < -deadZoneRS) ? true : false;
					}
				}
				else if (strcmp(pUDLR, XIP_S_LEFT) == 0) {
					if ((controller[cNum].state.Gamepad.sThumbRX < -deadZoneRS) != (controller[cNum].old.Gamepad.sThumbRX < -deadZoneRS)) {
						return (controller[cNum].state.Gamepad.sThumbRX < -deadZoneRS) ? true : false;
					}
				}
				else if (strcmp(pUDLR, XIP_S_RIGHT) == 0) {
					if ((controller[cNum].state.Gamepad.sThumbRX > deadZoneRS) != (controller[cNum].old.Gamepad.sThumbRX > deadZoneRS)) {
						return (controller[cNum].state.Gamepad.sThumbRX > deadZoneRS) ? true : false;
					}
				}
			}
		}
	}
	return 0;
}

//=============================================================================
// スティックのリリース状態を取得
//=============================================================================
bool GetControllerStickRelease(LPCSTR pLR, LPCSTR pUDLR, ControllerNumber cNum) {
	if (cNum >= CNUM_1 && cNum < maxControllers) {
		if (controller[cNum].bConnected) {
			if (strcmp(pLR, XIP_LEFT) == 0) {
				if (strcmp(pUDLR, XIP_S_UP) == 0) {
					if ((controller[cNum].state.Gamepad.sThumbLY > deadZoneLS) != (controller[cNum].old.Gamepad.sThumbLY > deadZoneLS)) {
						return (controller[cNum].old.Gamepad.sThumbLY > deadZoneLS) ? true : false;
					}
				}
				else if (strcmp(pUDLR, XIP_S_DOWN) == 0) {
					if ((controller[cNum].state.Gamepad.sThumbLY < -deadZoneLS) != (controller[cNum].old.Gamepad.sThumbLY < -deadZoneLS)) {
						return (controller[cNum].old.Gamepad.sThumbLY < -deadZoneLS) ? true : false;
					}
				}
				else if (strcmp(pUDLR, XIP_S_LEFT) == 0) {
					if ((controller[cNum].state.Gamepad.sThumbLX < -deadZoneLS) != (controller[cNum].old.Gamepad.sThumbLX < -deadZoneLS)) {
						return (controller[cNum].old.Gamepad.sThumbLX < -deadZoneLS) ? true : false;
					}
				}
				else if (strcmp(pUDLR, XIP_S_RIGHT) == 0) {
					if ((controller[cNum].state.Gamepad.sThumbLX > deadZoneLS) != (controller[cNum].old.Gamepad.sThumbLX > deadZoneLS)) {
						return (controller[cNum].old.Gamepad.sThumbLX > deadZoneLS) ? true : false;
					}
				}
			}
			else if (strcmp(pLR, XIP_RIGHT) == 0) {
				if (strcmp(pUDLR, XIP_S_UP) == 0) {
					if ((controller[cNum].state.Gamepad.sThumbRY > deadZoneRS) != (controller[cNum].old.Gamepad.sThumbRY > deadZoneRS)) {
						return (controller[cNum].old.Gamepad.sThumbRY > deadZoneRS) ? true : false;
					}
				}
				else if (strcmp(pUDLR, XIP_S_DOWN) == 0) {
					if ((controller[cNum].state.Gamepad.sThumbRY < -deadZoneRS) != (controller[cNum].old.Gamepad.sThumbRY < -deadZoneRS)) {
						return (controller[cNum].old.Gamepad.sThumbRY < -deadZoneRS) ? true : false;
					}
				}
				else if (strcmp(pUDLR, XIP_S_LEFT) == 0) {
					if ((controller[cNum].state.Gamepad.sThumbRX < -deadZoneRS) != (controller[cNum].old.Gamepad.sThumbRX < -deadZoneRS)) {
						return (controller[cNum].old.Gamepad.sThumbRX < -deadZoneRS) ? true : false;
					}
				}
				else if (strcmp(pUDLR, XIP_S_RIGHT) == 0) {
					if ((controller[cNum].state.Gamepad.sThumbRX > deadZoneRS) != (controller[cNum].old.Gamepad.sThumbRX > deadZoneRS)) {
						return (controller[cNum].old.Gamepad.sThumbRX > deadZoneRS) ? true : false;
					}
				}
			}
		}
	}
	return 0;
}

//=============================================================================
// スティックの軸の状態を取得
//=============================================================================
FLOAT GetControllerStickAxis(LPCSTR pLR, LPCSTR pAxis, ControllerNumber cNum) {
	if (cNum >= CNUM_1 && cNum < maxControllers) {
		if (controller[cNum].bConnected) {
			if (strcmp(pLR, XIP_LEFT) == 0) {
				float normalizedX = fmaxf(-1.0f, controller[cNum].state.Gamepad.sThumbLX / 32767.0f);
				float normalizedY = fmaxf(-1.0f, controller[cNum].state.Gamepad.sThumbLY / 32767.0f);
				float magnitude = sqrtf(normalizedX * normalizedX + normalizedY * normalizedY);
				if (magnitude > 1.0f) {
					magnitude = 1.0f;
				}
				float deadZone = deadZoneLS / 32767.0f;
				float angle = atan2f(normalizedY, normalizedX);
				angle = ToDegree(angle);
				if ((angle > 90.0f - angularDeadZoneLY && angle < 90.0f + angularDeadZoneLY) ||
					(angle < -90.0f + angularDeadZoneLY && angle > -90.0f - angularDeadZoneLY)) {
					normalizedX = 0;
				}
				else if ((angle < angularDeadZoneLX && angle > -angularDeadZoneLX) ||
					angle > 180.0f - angularDeadZoneLX || angle < -180.0f + angularDeadZoneLX) {
					normalizedY = 0;
				}
				if (magnitude > deadZone) {
					if (strcmp(pAxis, XIP_S_X) == 0) {
						return (normalizedX);
					}
					else if (strcmp(pAxis, XIP_S_Y) == 0) {
						return (normalizedY);
					}
				}
			}
			else if (strcmp(pLR, XIP_RIGHT) == 0) {
				float normalizedX = fmaxf(-1.0f, controller[cNum].state.Gamepad.sThumbRX / 32767.0f);
				float normalizedY = fmaxf(-1.0f, controller[cNum].state.Gamepad.sThumbRY / 32767.0f);
				float magnitude = sqrtf(normalizedX * normalizedX + normalizedY * normalizedY);
				if (magnitude > 1.0f) {
					magnitude = 1.0f;
				}
				float deadZone = deadZoneRS / 32767.0f;
				float angle = atan2f(normalizedY, normalizedX);
				angle = ToDegree(angle);
				if ((angle > 90.0f - angularDeadZoneRY && angle < 90.0f + angularDeadZoneRY) ||
					(angle < -90.0f + angularDeadZoneRY && angle > -90.0f - angularDeadZoneRY)) {
					normalizedX = 0;
				}
				else if ((angle < angularDeadZoneRX && angle > -angularDeadZoneRX) ||
					angle > 180.0f - angularDeadZoneRX || angle < -180.0f + angularDeadZoneRX) {
					normalizedY = 0;
				}
				if (magnitude > deadZone) {
					if (strcmp(pAxis, XIP_S_X) == 0) {
						return (normalizedX);
					}
					else if (strcmp(pAxis, XIP_S_Y) == 0) {
						return (normalizedY);
					}
				}
			}
		}
	}
	return 0;
}

//=============================================================================
// スケーリングされたスティックの軸の状態を取得
//=============================================================================
FLOAT GetControllerStickAxisScaled(LPCSTR pLR, LPCSTR pAxis, ControllerNumber cNum) {
	if (cNum >= CNUM_1 && cNum < maxControllers) {
		if (controller[cNum].bConnected) {
			if (strcmp(pLR, XIP_LEFT) == 0) {
				float normalizedX = fmaxf(-1.0f, controller[cNum].state.Gamepad.sThumbLX / 32767.0f);
				float normalizedY = fmaxf(-1.0f, controller[cNum].state.Gamepad.sThumbLY / 32767.0f);
				float magnitude = sqrtf(normalizedX * normalizedX + normalizedY * normalizedY);
				if (magnitude > 1.0f) {
					magnitude = 1.0f;
				}
				float deadZone = deadZoneLS / 32767.0f;
				float scale = (magnitude - deadZone) / (1.0f - deadZone) / magnitude;
				float outX = normalizedX * scale;
				float outY = normalizedY * scale;
				float angle = atan2f(outY, outX);
				angle = ToDegree(angle);
				if ((angle > 90.0f - angularDeadZoneLY && angle < 90.0f + angularDeadZoneLY) ||
					(angle < -90.0f + angularDeadZoneLY && angle > -90.0f - angularDeadZoneLY)) {
					outX = 0;
				}
				else if ((angle < angularDeadZoneLX && angle > -angularDeadZoneLX) ||
					angle > 180.0f - angularDeadZoneLX || angle < -180.0f + angularDeadZoneLX) {
					outY = 0;
				}
				if (magnitude > deadZone) {
					if (strcmp(pAxis, XIP_S_X) == 0) {
						return (outX);
					}
					else if (strcmp(pAxis, XIP_S_Y) == 0) {
						return (outY);
					}
				}
			}
			else if (strcmp(pLR, XIP_RIGHT) == 0) {
				float normalizedX = fmaxf(-1.0f, controller[cNum].state.Gamepad.sThumbRX / 32767.0f);
				float normalizedY = fmaxf(-1.0f, controller[cNum].state.Gamepad.sThumbRY / 32767.0f);
				float magnitude = sqrtf(normalizedX * normalizedX + normalizedY * normalizedY);
				if (magnitude > 1.0f) {
					magnitude = 1.0f;
				}
				float deadZone = deadZoneRS / 32767.0f;
				float scale = (magnitude - deadZone) / (1.0f - deadZone) / magnitude;
				float outX = normalizedX * scale;
				float outY = normalizedY * scale;
				float angle = atan2f(outY, outX);
				angle = ToDegree(angle);
				if ((angle > 90.0f - angularDeadZoneRY && angle < 90.0f + angularDeadZoneRY) || 
					(angle < -90.0f + angularDeadZoneRY && angle > -90.0f - angularDeadZoneRY)) {
					outX = 0;
				}
				else if ((angle < angularDeadZoneRX && angle > -angularDeadZoneRX) || 
					angle > 180.0f - angularDeadZoneRX || angle < -180.0f + angularDeadZoneRX) {
					outY = 0;
				}
				if (magnitude > deadZone) {
					if (strcmp(pAxis, XIP_S_X) == 0) {
						return (outX);
					}
					else if (strcmp(pAxis, XIP_S_Y) == 0) {
						return (outY);
					}
				}
			}
		}
	}
	return 0;
}

//=============================================================================
// バイブレーション設定
//=============================================================================
void SetControllerVibration(LPCSTR pLRB, WORD wPow, ControllerNumber cNum) {
	if (cNum >= CNUM_1 && cNum < maxControllers) {
		if (controller[cNum].bConnected) {
			SecureZeroMemory(&controller[cNum].vibration, sizeof(XINPUT_VIBRATION));	// バイブレーション情報のメモリ初期化
			if (strcmp(pLRB, XIP_LEFT) == 0) {
				controller[cNum].vibration.wLeftMotorSpeed = wPow;
			}
			else if (strcmp(pLRB, XIP_RIGHT) == 0) {
				controller[cNum].vibration.wRightMotorSpeed = wPow;
			}
			else if (strcmp(pLRB, XIP_BOTH) == 0) {
				controller[cNum].vibration.wLeftMotorSpeed = wPow;
				controller[cNum].vibration.wRightMotorSpeed = wPow;
			}
			XInputSetState(cNum, &controller[cNum].vibration);
		}
	}
}

#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)	// Windows8以上
//=============================================================================
// バッテリー情報取得
//=============================================================================
BYTE GetControllerBattery(ControllerNumber cNum) {
	if (cNum >= CNUM_1 && cNum < maxControllers) {
		if (controller[cNum].bConnected) {
			return (controller[cNum].battery.BatteryLevel);
		}
	}
	return 0;
}
#endif