#include <XInputController.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PI	3.141592654f
#define ToRadian(degree) ((degree) * (PI / 180.0f))
#define ToDegree(radian) ((radian) * (180.0f / PI))

#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)	// Windows8�ȏ�
#define FNC_XINPUTGETSTATEEX			((const char*)100)	// XInputGetStateEX�p
//#define FNC_XINPUTWAITFORGUIDEBUTTON	((const char*)101)	// XInputWaitForGuideButton�p
//#define FNC_XINPUTCANCELGUIDEBUTTONWAIT	((const char*)102)	// XInputCancelGuideButtonWait�p
//#define FNC_XINPUTPOWEROFFCONTROLLER	((const char*)103)	// XInputPowerOffController�p
// ���ɂ�104��108�����邪�s��
#endif

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct CONTROLLER_STATE {
	XINPUT_STATE state;						// ���݂̓��͏��
	XINPUT_STATE old;						// �O��̓��͏��
	XINPUT_VIBRATION vibration;				// �o�C�u���[�V�������
#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)		// Windows8�ȏ�
	XINPUT_BATTERY_INFORMATION battery;		// �o�b�e���[���
#endif
	bool bConnected;						// �ڑ��m�F���
} CONTROLLER_STATE;

//*****************************************************************************
// �O���[�o��
//*****************************************************************************
CONTROLLER_STATE *controller;	// �R���g���[���[�\����
BYTE maxControllers = CMAX_4;	// �ő�R���g���[���[��(XInput���F���ł���̂�4�܂�)

BYTE	deadZoneLT			= 255 / 10;		// ���g���K�[�̂������l
BYTE	deadZoneRT			= 255 / 10;		// �E�g���K�[�̂������l
SHORT	deadZoneLS			= 32767 / 5;	// ���X�e�B�b�N�̂������l
SHORT	deadZoneRS			= 32767 / 5;	// �E�X�e�B�b�N�̂������l
FLOAT	angularDeadZoneLX	= 5.0f;			// ���X�e�B�b�N��X���̂������l
FLOAT	angularDeadZoneLY	= 5.0f;			// ���X�e�B�b�N��Y���̂������l
FLOAT	angularDeadZoneRX	= 5.0f;			// �E�X�e�B�b�N��X���̂������l
FLOAT	angularDeadZoneRY	= 5.0f;			// �E�X�e�B�b�N��Y���̂������l

#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)	// Windows8�ȏ�
/* XInputDLL�֘A */
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
// �v���g�^�C�v�錾
//*****************************************************************************
void LoadXInputDLL(void);	// XInputDLL�̃��[�h
void FreeXInputDLL(void);	// XInputDLL�̉��

//=============================================================================
// XInputDLL�̃��[�h
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
// XInputDLL�̉��
//=============================================================================
void FreeXInputDLL(void) {
	if (XInput_dll) {
		::FreeLibrary(XInput_dll);
	}
}

//=============================================================================
// �R���g���[���[����������(Windows8�ȏ�)
//=============================================================================
void InitController(ControllerMax cMax, bool bUseDLL) {
	if (bUseDLL) {
		LoadXInputDLL();	// XInputDLL�̃��[�h
	}
	if (cMax >= CMAX_1 && cMax <= CMAX_4) {
		maxControllers = cMax;
	}
	controller = new CONTROLLER_STATE[maxControllers];
	SecureZeroMemory(controller, sizeof(CONTROLLER_STATE) * maxControllers);
}
#else

//=============================================================================
// �R���g���[���[����������(Windows7�ȉ�)
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
// �R���g���[���[�I������
//=============================================================================
void UninitController(void) {
	delete[] controller;
#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)	// Windows8�ȏ�
	FreeXInputDLL();		// XInputDLL�̉��
	XInputEnable(false);	// XInput������
#endif
}

//=============================================================================
// �R���g���[���[�X�V����
//=============================================================================
HRESULT UpdateController(void) {
	DWORD Result;

	for (BYTE i = 0; i < maxControllers; i++) {
		SecureZeroMemory(&controller[i].old, sizeof(XINPUT_STATE));		// �O��̓��͏��̃�����������
		controller[i].old = controller[i].state;						// ���݂̓��͏���O��̓��͏��֕ۑ�
		SecureZeroMemory(&controller[i].state, sizeof(XINPUT_STATE));	// ���݂̓��͏��̃�����������
		
#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)	// Windows8�ȏ�
		/* �R���g���[���[���擾 */
		if (XInput_dll) {											// XInputDLL������ꍇ
			Result = XInputGetStateEx(i, &controller[i].state);		// ���͏��擾(�K�C�h�{�^������)
		}
		else {														// XInputDLL���Ȃ��ꍇ
			Result = XInputGetState(i, &controller[i].state);		// ���͏��擾(�K�C�h�{�^���Ȃ�)
		}
		SecureZeroMemory(&controller[i].battery, sizeof(XINPUT_BATTERY_INFORMATION));		// �o�b�e���[���̃�����������
		XInputGetBatteryInformation(i, BATTERY_DEVTYPE_GAMEPAD, &controller[i].battery);	// �o�b�e���[���擾
#else
		/* �R���g���[���[���擾 */
		Result = XInputGetState(i, &controller[i].state);			// ���͏��擾(�K�C�h�{�^���Ȃ�)
#endif
		/* �ڑ��m�F */
		if (Result == ERROR_SUCCESS) {
			controller[i].bConnected = true;	// �ڑ�����Ă���
		}
		else {
			controller[i].bConnected = false;	// �ڑ�����Ă��Ȃ�
		}
	}

	return S_OK;
}

//=============================================================================
// �{�^���̃v���X��Ԃ��擾
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
// �{�^���̃g���K�[��Ԃ��擾
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
// �{�^���̃����[�X��Ԃ��擾
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
// �g���K�[�̃v���X��Ԃ��擾
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
// �g���K�[�̃g���K�[��Ԃ��擾
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
// �g���K�[�̃����[�X��Ԃ��擾
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
// �X�e�B�b�N�̃v���X��Ԃ��擾
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
// �X�e�B�b�N�̃g���K�[��Ԃ��擾
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
// �X�e�B�b�N�̃����[�X��Ԃ��擾
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
// �X�e�B�b�N�̎��̏�Ԃ��擾
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
// �X�P�[�����O���ꂽ�X�e�B�b�N�̎��̏�Ԃ��擾
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
// �o�C�u���[�V�����ݒ�
//=============================================================================
void SetControllerVibration(LPCSTR pLRB, WORD wPow, ControllerNumber cNum) {
	if (cNum >= CNUM_1 && cNum < maxControllers) {
		if (controller[cNum].bConnected) {
			SecureZeroMemory(&controller[cNum].vibration, sizeof(XINPUT_VIBRATION));	// �o�C�u���[�V�������̃�����������
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

#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)	// Windows8�ȏ�
//=============================================================================
// �o�b�e���[���擾
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