#ifndef  _INCLUDED_DIKEYBOARD_
#define  _INCLUDED_DIKEYBOARD_
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <dinput.h>

#define NUM_KEY_MAX			256
#define LIMIT_COUNT_REPEEAT	20

namespace Input
{

		BOOL  WINAPI  DI_Init( HWND hWnd, HINSTANCE hInstance );
		HRESULT  InitMouse( HWND hWnd );
		VOID  WINAPI  DI_Term();
		VOID  UninitMouse();
		VOID  WINAPI  UpdateKeyboard();
		HRESULT  UpdateMouse();

		// Get key functions
		bool KeyPress( int nKey );
		bool KeyTrigger( int nKey );
		bool KeyRepeat( int nKey );
		bool KeyRelease( int nKey );

		// Get mouse functions
		bool MouseLeftPress( void );
		bool MouseLeftTrigger( void );
		bool MouseRightPress( void );
		bool MouseRightTrigger( void );
		bool MouseCenterPress( void );
		bool MouseCenterTrigger( void );
		long MouseAxisX( void );
		long MouseAxisY( void );
		long MouseAxisZ( void );
		long MouseX( void );
		long MouseY( void );

}

#endif // ! _INCLUDED_DIKEYBOARD_
