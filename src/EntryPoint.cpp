#pragma  warning(disable:4005)
#pragma  comment(lib,  "d3d11.lib")


#include  <d3d11.h>
#include  <IDirect3D.h>
#include  <D3D11Utility.h>


INT  WINAPI  WinMain( HINSTANCE  hInstance, HINSTANCE  nPrevInstance, PSTR  lpCmdLine, INT  nCmdShow )
{
		auto  d3d11 = _Singleton<D3D11Utility::IDirect3D>::GetInstance();


		if ( FAILED( d3d11->CreateScreen( D3D11Utility::SCREEN_WIDTH, D3D11Utility::SCREEN_HEIGHT, hInstance ) ) )
				return E_FAIL;


		if ( FAILED( d3d11->CreateDevice() ) )
				return  E_FAIL;


		if ( FAILED( d3d11->CreateDefaultDepthStencil() ) )
				return  E_FAIL;


		d3d11->MainLoop();

		// スマートポインタなので自動的に開放される
		// d3d11->Release();


		return  0;

}