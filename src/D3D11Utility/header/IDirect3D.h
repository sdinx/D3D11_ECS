//----------------------------------------------------------------------------------
// file : Direct3D.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_INTERFACE_D3D11_UTILITY_DIRECT3D_
#define  _INCLUDED_INTERFACE_D3D11_UTILITY_DIRECT3D_

//----------------------------------------------------------------------------------
// pragma
//----------------------------------------------------------------------------------
#pragma  warning(disable:4005)
#pragma  comment(lib,  "d3d11.lib")

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <d3d11.h>
#include  "C_Singleton.h"


namespace  D3D11Utility
{

		class  IDirect3D :public _Singleton<IDirect3D>
		{
				//----------------------------------------------------------------------------------
				// friend and etc...
				//----------------------------------------------------------------------------------
				friend  class  _Singleton<IDirect3D>;
				// friend  class  IDirect3DRenderer;

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				IDirect3D();
				virtual ~IDirect3D();


		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------

#if  defined UNICODE || _UNICODE
				LPCWSTR  m_szWindowClass = L"Direct3D_11";
#else  // �}���`�o�C�g
				LPCSTR  m_szWindowClass = "Direct3D_11";
#endif // _UNICODE

				INT  m_nScreenWidth;
				INT  m_nScreenHeight;
				UINT  m_nSwapChainCount = 2;// �_�u���o�b�t�@
				UINT  m_nMultiSampleCount = 1;
				UINT  m_nMultiSampleQuality = 0;
				FLOAT  m_fClearColors[4] = { 0.0f,0.125f,0.3f,1.0f };
				D3D_DRIVER_TYPE  m_driverType = D3D_DRIVER_TYPE_NULL;
				D3D_FEATURE_LEVEL  m_featureLevel = D3D_FEATURE_LEVEL_11_0;
				DXGI_FORMAT  m_depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
				DXGI_FORMAT  m_swapChainFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

				//----------------------------------------------------------------------------------
				// utility variables
				//----------------------------------------------------------------------------------
				HWND  m_hWnd;
				HINSTANCE  m_hInstance;
				ID3D11Device*  m_pd3dDevice = NULL;
				ID3D11DeviceContext*  m_pd3dDeviceContext = NULL;
				IDXGISwapChain*  m_pSwapChain = NULL;
				ID3D11RenderTargetView* m_pRTView = NULL;
				ID3D11DepthStencilView*  m_pDSView = NULL;
				ID3D11Texture2D*  m_pRTTexture = NULL;
				ID3D11Texture2D*  m_pDSTexture = NULL;
				ID3D11ShaderResourceView*  m_pRTShaderResourceView = NULL;
				ID3D11ShaderResourceView*  m_pDSShaderResourceView = NULL;


		private:
				//----------------------------------------------------------------------------------
				// private functions
				//----------------------------------------------------------------------------------

				// �E�B���h�E�v���V�[�W��
				static  LRESULT  CALLBACK  WndProc( HWND  hWnd, UINT  message, WPARAM  wParam, LPARAM  lParam );


		public:
				//----------------------------------------------------------------------------------
				// public funcitons
				//----------------------------------------------------------------------------------
				/* NOTHING */
				
				//----------------------------------------------------------------------------------
				// Init
				// Create �֐������s���Ă���C���X�^���X���g�p����.
				//----------------------------------------------------------------------------------

				// �����Ŏw�肵���傫���ŃE�B���h�E���쐬.
				HRESULT  CreateScreen( INT  screenWidth, INT  screenHeight, HINSTANCE  hInstance = GetModuleHandle( NULL ) );
				// �f�o�C�X���쐬
				HRESULT  CreateDevice();
				// �f�t�H���g�̐[�x�X�e���V���o�b�t�@�쐬
				HRESULT  CreateDefaultDepthStencil();

				//----------------------------------------------------------------------------------
				// Utility
				//----------------------------------------------------------------------------------
				VOID  MainLoop();
				// �X�N���[���T�C�Y�̍X�V
				VOID  SetWindow( INT  screenWidth, INT  screenHeight );
				// �����_�����O�̃N���A
				VOID  BeginRender();
				// �����_�����O�̕\��
				VOID  EndRender();

				//----------------------------------------------------------------------------------
				// Release
				//----------------------------------------------------------------------------------

				// Create �����S�Ă��������
				VOID  Release();
				// Release ��͐V�����E�B���h�E���쐬
				VOID  ReleaseScreen();
				// Release ��͐V���������_�[�^�[�Q�b�g���쐬
				VOID  ReleaseDefaultRenderTarget();
				// Release ��͐V�����[�x�X�e���V���o�b�t�@���쐬
				VOID  ReleaseDefaultDepthStencil();

				//----------------------------------------------------------------------------------
				// Getter
				//----------------------------------------------------------------------------------
				ID3D11Device* GetDevice() { return  m_pd3dDevice; }
				ID3D11DeviceContext* GetDeviceContext() { return  m_pd3dDeviceContext; }
				IDXGISwapChain* GetSwapChain() { return  m_pSwapChain; }
				ID3D11RenderTargetView* GetRenderTargetView() { return  m_pRTView; }
				ID3D11DepthStencilView* GetDepthStencilView() { return  m_pDSView; }
				POINT  GetScreenSize() { return{ m_nScreenWidth, m_nScreenHeight }; }

		}; // class IDirect3D

} // namespace D3D11Utility

#endif // ! _INCLUDED_INTERFACE_D3D11_UTILITY_DIRECT3D_