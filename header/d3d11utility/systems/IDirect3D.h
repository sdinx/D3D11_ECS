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
#include  <vector>
#include  "Singleton.h"


namespace  D3D11Utility
{

		namespace  Systems
		{

				class  IDirect3D :public _Singleton<IDirect3D>
				{
						//----------------------------------------------------------------------------------
						// friend and etc...
						//----------------------------------------------------------------------------------
						friend  class  _Singleton<IDirect3D>;

						struct  RenderTarget
						{
								enum  eViews
								{
										Depth_Stencil = 0,
										Normal,
										Diffuse,
										Specular,
								};

								RenderTarget() :
										m_pRTView( nullptr ),
										m_pTexture( nullptr ),
										m_pSRView( nullptr )
								{}
								eViews  m_numViews;
								ID3D11RenderTargetView* m_pRTView = nullptr;
								ID3D11Texture2D*  m_pTexture = nullptr;
								ID3D11ShaderResourceView*  m_pSRView = nullptr;
						};

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------
						IDirect3D();
						virtual ~IDirect3D();

						using  eRTViews = RenderTarget::eViews;

				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------

#if  defined  (UNICODE)  ||  (_UNICODE)
						LPCWSTR  m_szWindowClass = L"Direct3D_11";
#else  // Multibyte
						LPCSTR  m_szWindowClass = "Direct3D_11";
#endif // _UNICODE

						INT  m_nScreenWidth;
						INT  m_nScreenHeight;
						UINT  m_nSwapChainCount = 2;// ダブルバッファ
						UINT  m_nMultiSampleCount = 1;
						UINT  m_nMultiSampleQuality = 0;
						FLOAT  m_fClearColors[4] = { 0.0f,0.125f,0.3f,1.0f };
						D3D_DRIVER_TYPE  m_driverType = D3D_DRIVER_TYPE_NULL;
						D3D_FEATURE_LEVEL  m_featureLevel = D3D_FEATURE_LEVEL_11_0;
						DXGI_FORMAT  m_depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
						DXGI_FORMAT  m_swapChainFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

						//----------------------------------------------------------------------------------
						// device
						//----------------------------------------------------------------------------------
						HWND  m_hWnd;
						HINSTANCE  m_hInstance;
						ID3D11Device*  m_pd3dDevice = nullptr;
						ID3D11DeviceContext*  m_pd3dDeviceContext = nullptr;
						IDXGISwapChain*  m_pSwapChain = nullptr;
						ID3D11RenderTargetView* m_pRTView = nullptr;
						ID3D11DepthStencilView*  m_pDSView = nullptr;
						ID3D11Texture2D*  m_pRTTexture = nullptr;
						ID3D11Texture2D*  m_pDSTexture = nullptr;
						ID3D11ShaderResourceView*  m_pRTShaderResourceView = nullptr;
						ID3D11ShaderResourceView*  m_pDSShaderResourceView = nullptr;
						std::vector<RenderTarget>  m_renderTagets;


				private:
						//----------------------------------------------------------------------------------
						// private methods
						//----------------------------------------------------------------------------------

						// ウィンドウプロシージャ
						static  LRESULT  CALLBACK  WndProc( HWND  hWnd, UINT  message, WPARAM  wParam, LPARAM  lParam );


				public:
						//----------------------------------------------------------------------------------
						// public methods
						//----------------------------------------------------------------------------------

						//----------------------------------------------------------------------------------
						// Init
						// Create 関数を実行してからインスタンスを使用する.
						//----------------------------------------------------------------------------------

						// 引数で指定した大きさでウィンドウを作成.
						HRESULT  CreateScreen( INT  screenWidth, INT  screenHeight, HINSTANCE  hInstance = GetModuleHandle( nullptr ) );
						// デバイスを作成
						HRESULT  CreateDevice();
						// デフォルトの深度ステンシルバッファ作成
						HRESULT  CreateDefaultDepthStencil();
						// レンダーターゲットビューの生成 ( MRT用 )
						HRESULT  CreateMultipleRenderTargetView();

						//----------------------------------------------------------------------------------
						// Utility
						//----------------------------------------------------------------------------------
						VOID  MainLoop();
						// スクリーンサイズの更新
						VOID  SetWindow( INT  screenWidth, INT  screenHeight );
						// レンダリングのクリア
						VOID  BeginRender();
						// レンダリングの表示
						VOID  EndRender();

						//----------------------------------------------------------------------------------
						// Release
						//----------------------------------------------------------------------------------

						// Create した全てを解放処理
						VOID  Release();
						// Release 後は新しいウィンドウを作成
						VOID  ReleaseScreen();
						// Release 後は新しいレンダーターゲットを作成
						VOID  ReleaseDefaultRenderTarget();
						// Release 後は新しい深度ステンシルバッファを作成
						VOID  ReleaseDefaultDepthStencil();

						//----------------------------------------------------------------------------------
						// Getter
						//----------------------------------------------------------------------------------
						HWND  GetWindowHandle() { return  m_hWnd; }
						ID3D11Device* GetDevice() { return  m_pd3dDevice; }
						ID3D11DeviceContext* GetDeviceContext() { return  m_pd3dDeviceContext; }
						IDXGISwapChain* GetSwapChain() { return  m_pSwapChain; }
						ID3D11RenderTargetView* GetRenderTargetView() { return  m_pRTView; }
						ID3D11DepthStencilView* GetDepthStencilView() { return  m_pDSView; }
						ID3D11ShaderResourceView* GetRTShaderResourceView() { return  m_pRTShaderResourceView; }
						ID3D11ShaderResourceView* GetDSShaderResourceView() { return  m_pDSShaderResourceView; }
						POINT  GetScreenSize() { return{ m_nScreenWidth, m_nScreenHeight }; }
						RenderTarget&  GetRenderTargets( UINT  index ) { return  m_renderTagets[index]; }

				}; // class IDirect3D

		}// namespace Systems
} // namespace D3D11Utility

#endif // ! _INCLUDED_INTERFACE_D3D11_UTILITY_DIRECT3D_