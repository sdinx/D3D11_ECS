#pragma  warning(disable:4005)
#pragma  comment(lib,  "d3d11.lib")


#include  <d3d11.h>
#include  <D3D11Utility.h>
#include  <d3d11utility\Systems\IDirect3D.h>
#include  <d3d11utility/XInputController.h>
#include  <d3d11utility/DIKeyboard.h>
#include  <StateTransition.h>
#include  <d3d11utility\Timer.h>


INT  WINAPI  WinMain( HINSTANCE  hInstance, HINSTANCE  nPrevInstance, PSTR  lpCmdLine, INT  nCmdShow )
{
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
		auto  d3d11 = _Singleton<D3D11Utility::Systems::IDirect3D>::GetInstance();

		if ( FAILED( d3d11->CreateScreen( D3D11Utility::SCREEN_WIDTH, D3D11Utility::SCREEN_HEIGHT, hInstance ) ) )
				return E_FAIL;

		if ( FAILED( d3d11->CreateDevice() ) )
				return  E_FAIL;

		if ( FAILED( d3d11->CreateDefaultDepthStencil() ) )
				return  E_FAIL;



		Input::DI_Init( d3d11->GetWindowHandle(), hInstance );
		Input::InitMouse( d3d11->GetWindowHandle() );
		InitController( ControllerMax::CMAX_1 );

		d3d11->MainLoop();

		UninitController();
		_CrtDumpMemoryLeaks();

		return  0;

}