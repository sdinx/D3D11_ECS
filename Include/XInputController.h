#ifndef INCLUDED_XINPUTCONTROLLER
#define INCLUDED_XINPUTCONTROLLER

#define  _WIN32_WINNT  _WIN32_WINNT_WIN7

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include  <Windows.h>
#include  <XInput.h>
#include  <cstring>
#include  <cmath>

//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)	// Windows8�ȏ�
#pragma comment(lib, "Xinput.lib")
#else									// Windows7�ȉ�
#pragma comment(lib, "Xinput9_1_0.lib")
#endif

//*****************************************************************************
// #pragma deprecated�΍�
//*****************************************************************************
#if(_WIN32_WINNT >= _WIN32_WINNT_WIN10)	// Windows10�ȏ�
#pragma warning(disable : 4995)
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************
/* �ő�R���g���[���[���w��p */
enum ControllerMax {
	CMAX_1 = 1,	// �ő�R���g���[���[��1
	CMAX_2,		// �ő�R���g���[���[��2
	CMAX_3,		// �ő�R���g���[���[��3
	CMAX_4		// �ő�R���g���[���[��4
};

/* �R���g���[���[�w��p */
enum ControllerNumber{
	CNUM_1,	// �R���g���[���[1
	CNUM_2,	// �R���g���[���[2
	CNUM_3,	// �R���g���[���[3
	CNUM_4	// �R���g���[���[4
};

/* �{�^�� */
#define XIP_D_UP			XINPUT_GAMEPAD_DPAD_UP			// ���{�^��
#define XIP_D_DOWN			XINPUT_GAMEPAD_DPAD_DOWN		// ���{�^��
#define XIP_D_LEFT			XINPUT_GAMEPAD_DPAD_LEFT		// ���{�^��
#define XIP_D_RIGHT			XINPUT_GAMEPAD_DPAD_RIGHT		// ���{�^��
#define XIP_A				XINPUT_GAMEPAD_A				// A�{�^��
#define XIP_B				XINPUT_GAMEPAD_B				// B�{�^��
#define XIP_X				XINPUT_GAMEPAD_X				// X�{�^��
#define XIP_Y				XINPUT_GAMEPAD_Y				// Y�{�^��
#define XIP_START			XINPUT_GAMEPAD_START			// START�{�^��
#define XIP_BACK			XINPUT_GAMEPAD_BACK				// BACK�{�^��
#define XIP_LSB				XINPUT_GAMEPAD_LEFT_THUMB		// ���X�e�B�b�N�{�^��
#define XIP_RSB				XINPUT_GAMEPAD_RIGHT_THUMB		// �E�X�e�B�b�N�{�^��
#define XIP_LB				XINPUT_GAMEPAD_LEFT_SHOULDER	// L�{�^��
#define XIP_RB				XINPUT_GAMEPAD_RIGHT_SHOULDER	// R�{�^��
#define XIP_ANY			( 0xffff )

/* ���E���� */
#define XIP_LEFT			"XIP_LEFT"			// ��
#define XIP_RIGHT			"XIP_RIGHT"			// �E
#define XIP_BOTH			"XIP_BOTH"			// ����(�o�C�u���[�V������p)

/* �X�e�B�b�N */
#define XIP_S_UP			"XIP_S_UP"			// �X�e�B�b�N��
#define XIP_S_DOWN			"XIP_S_DOWN"		// �X�e�B�b�N��
#define XIP_S_LEFT			"XIP_S_LEFT"		// �X�e�B�b�N��
#define XIP_S_RIGHT			"XIP_S_RIGHT"		// �X�e�B�b�N��
#define XIP_S_X				"XIP_S_X"			// �X�e�B�b�NX��
#define XIP_S_Y				"XIP_S_Y"			// �X�e�B�b�NY��

/* �o�C�u���[�V���� */
#define XIP_VIBRATION_MAX	65535				// ���[�^�[�̍ő�U��
#define XIP_VIBRATION_HIGH	65535/4*3			// ���[�^�[�̋��U��
#define XIP_VIBRATION_MID	65535/2				// ���[�^�[�̒��U��
#define XIP_VIBRATION_LOW	65535/4				// ���[�^�[�̎�U��
#define XIP_VIBRATION_OFF	0					// ���[�^�[�̒�~

#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)	// Windows8�ȏ�
/* �o�b�e���[ */
#define XIP_BATTERY_FULL	BATTERY_LEVEL_FULL		// �o�b�e���[��
#define XIP_BATTERY_MEDIUM	BATTERY_LEVEL_MEDIUM	// �o�b�e���[��
#define XIP_BATTERY_LOW		BATTERY_LEVEL_LOW		// �o�b�e���[��
#define XIP_BATTERY_EMPTY	BATTERY_LEVEL_EMPTY		// �o�b�e���[��

/* �K�C�h�{�^�� */
#define XINPUT_GAMEPAD_GUIDE	0x0400
#define XIP_GUIDE			XINPUT_GAMEPAD_GUIDE	// �K�C�h�{�^��
#endif

//*****************************************************************************
// �O���[�o��
//*****************************************************************************
extern  BYTE	 deadZoneLT;			// ���g���K�[�̂������l
extern  BYTE	 deadZoneRT;			// �E�g���K�[�̂������l
extern  SHORT	 deadZoneLS;			// ���X�e�B�b�N�̂������l
extern  SHORT	 deadZoneRS;			// �E�X�e�B�b�N�̂������l
extern  FLOAT	 angularDeadZoneLX;	// ���X�e�B�b�N��X���̂������l
extern  FLOAT	 angularDeadZoneLY;	// ���X�e�B�b�N��Y���̂������l
extern  FLOAT	 angularDeadZoneRX;	// �E�X�e�B�b�N��X���̂������l
extern  FLOAT	 angularDeadZoneRY;	// �E�X�e�B�b�N��Y���̂������l

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)	// Windows8�ȏ�
void	InitController(ControllerMax cMax, bool bUseDLL = false);	// �R���g���[���[����������
#else									// Windows7�ȉ�
void	InitController(ControllerMax cMax);							// �R���g���[���[����������
#endif
void	UninitController(void);										// �R���g���[���[�I������

HRESULT	UpdateController(void);										// �R���g���[���[�X�V����

bool	GetControllerButtonPress(WORD wBtn, ControllerNumber cNum = CNUM_1);	// �{�^���̃v���X��Ԃ��擾
bool	GetControllerButtonTrigger(WORD wBtn, ControllerNumber cNum = CNUM_1);	// �{�^���̃g���K�[��Ԃ��擾
bool	GetControllerButtonRelease(WORD wBtn, ControllerNumber cNum = CNUM_1);	// �{�^���̃����[�X��Ԃ��擾

bool	GetControllerTriggerPress(LPCSTR pLR, ControllerNumber cNum = CNUM_1);		// �g���K�[�̃v���X��Ԃ��擾
bool	GetControllerTriggerTrigger(LPCSTR pLR, ControllerNumber cNum = CNUM_1);	// �g���K�[�̃g���K�[��Ԃ��擾
bool	GetControllerTriggerRelease(LPCSTR pLR, ControllerNumber cNum = CNUM_1);	// �g���K�[�̃����[�X��Ԃ��擾

bool	GetControllerStickPress(LPCSTR pLR, LPCSTR pUDLR, ControllerNumber cNum = CNUM_1);		// �X�e�B�b�N�̃v���X��Ԃ��擾
bool	GetControllerStickTrigger(LPCSTR pLR, LPCSTR pUDLR, ControllerNumber cNum = CNUM_1);	// �X�e�B�b�N�̃g���K�[��Ԃ��擾
bool	GetControllerStickRelease(LPCSTR pLR, LPCSTR pUDLR, ControllerNumber cNum = CNUM_1);	// �X�e�B�b�N�̃����[�X��Ԃ��擾

FLOAT	GetControllerStickAxis(LPCSTR pLR, LPCSTR pAxis, ControllerNumber cNum = CNUM_1);		// �X�e�B�b�N�̎��̏�Ԃ��擾
FLOAT	GetControllerStickAxisScaled(LPCSTR pLR, LPCSTR pAxis, ControllerNumber cNum = CNUM_1);	// �X�P�[�����O���ꂽ�X�e�B�b�N�̎��̏�Ԃ��擾

void	SetControllerVibration(LPCSTR pLRB, WORD wPow, ControllerNumber cNum = CNUM_1);	// �o�C�u���[�V�����ݒ�

#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)	// Windows8�ȏ�
BYTE	GetControllerBattery(ControllerNumber cNum = CNUM_1);	// �o�b�e���[���擾
#endif

#endif