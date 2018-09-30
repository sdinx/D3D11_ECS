#ifndef  _INCLUDED_DIKEYBOARD_
#define  _INCLUDED_DIKEYBOARD_
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include  <dinput.h>

#define NUM_KEY_MAX			256
#define LIMIT_COUNT_REPEEAT	20

namespace Input
{

		void  UpdateInput();
		BOOL  WINAPI  DI_Init( HWND hWnd, HINSTANCE hInstance );
		HRESULT  InitMouse( HWND hWnd );
		VOID  WINAPI  DI_Term();
		VOID  UninitMouse();
		VOID  WINAPI  UpdateKeyboard();
		HRESULT  UpdateMouse();

		// Get key functions
		bool IsKeyPress( int nKey );
		bool IsKeyTrigger( int nKey );
		bool IsKeyRepeat( int nKey );
		bool IsKeyRelease( int nKey );
		float KeyVertical();
		float KeyHorizontal();

		// Get mouse functions
		bool MouseLeftPress( void );
		bool MouseLeftTrigger( void );
		bool MouseRightPress( void );
		bool MouseRightTrigger( void );
		bool MouseCenterPress( void );
		bool MouseCenterTrigger( void );
		float MouseAxisX( void );
		float MouseAxisY( void );
		float MouseAxisZ( void );
		float MouseX( void );
		float MouseY( void );

}

#endif // ! _INCLUDED_DIKEYBOARD_
