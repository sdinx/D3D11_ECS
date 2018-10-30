//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <d3d11utility\Systems\IDirect3D.h>
#include  <D3D11Utility.h>
#include  <d3d11utility\components/Renderable.h>
#include  <game/GameUtility.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility::Systems;


IDirect3D::IDirect3D()
{

}// end default constructor


IDirect3D::~IDirect3D()
{
		Release();
}// end destructor


LRESULT  CALLBACK  IDirect3D::WndProc( HWND  hWnd, UINT  message, WPARAM  wParam, LPARAM  lParam )
{
		switch ( message ) {
		case  WM_CLOSE:
				PostMessage( hWnd, WM_DESTROY, NULL, NULL );
				break;
		case  WM_DESTROY:
				PostQuitMessage( 0 );
				break;
		}

		return  DefWindowProc( hWnd, message, wParam, lParam );
}// end WndProc(HWND, UINT, WPARAM, LPARAM)


HRESULT  IDirect3D::CreateScreen( INT  screenWidth, INT  screenHeight, HINSTANCE  hInstance )
{
		m_nScreenWidth = screenWidth;
		m_nScreenHeight = screenHeight;
		m_hInstance = hInstance;

		WNDCLASSEX  wincEx = {
				sizeof( WNDCLASSEX ), CS_CLASSDC,
				WndProc, 0U, 0U, m_hInstance,
				LoadIcon( NULL, IDI_APPLICATION ),
				LoadCursor( NULL, IDC_ARROW ),
				NULL, NULL, m_szWindowClass,
				LoadIcon( NULL,IDI_WINLOGO )
		};
		if ( !RegisterClassEx( &wincEx ) ) { return E_FAIL; }

		m_hWnd = CreateWindow(
				m_szWindowClass, m_szWindowClass,
				WS_OVERLAPPEDWINDOW,
				100, 50, m_nScreenWidth, m_nScreenHeight,
				NULL, NULL, wincEx.hInstance, NULL
		);

		SetWindow( m_nScreenWidth, m_nScreenHeight );

		return  S_OK;
}// end CreateScreen(INT, INT, HINSTANCE)


HRESULT  IDirect3D::CreateDevice()
{
		HRESULT  hr = S_OK;

		// ウィンドウサイズを取得
		RECT  rc;
		GetClientRect( m_hWnd, &rc );
		INT  width = rc.right - rc.left;
		INT  height = rc.bottom - rc.top;

		UINT  createDeviceFlags = NULL;
#if  defined(DEBUG)||defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif //  defined(DEBUG)||defined(_DEBUG)

		// ドライバータイプ
		D3D_DRIVER_TYPE  driverTypes[ ] = {
				D3D_DRIVER_TYPE_HARDWARE, // ハードウェアドライバ
				D3D_DRIVER_TYPE_WARP, // ソフトウェアラスタライザ
				D3D_DRIVER_TYPE_REFERENCE, // リファレンスラスタライザ
		};
		UINT  numDriverTypes = ARRAYSIZE( driverTypes );

		// 機能レベル
		D3D_FEATURE_LEVEL  featureLevels[ ] = {
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
		};
		UINT  numFeatureLevels = ARRAYSIZE( featureLevels );

		// スワップチェインの設定
		DXGI_SWAP_CHAIN_DESC  sd;
		ZeroMemory( &sd, sizeof( sd ) );
		sd.BufferCount = m_nSwapChainCount;// ディスプレイバッファ数
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = m_swapChainFormat;// ディスプレイバッファのフォーマット
		sd.BufferDesc.RefreshRate.Numerator = 60;// リフレッシュレートの分子
		sd.BufferDesc.RefreshRate.Denominator = 1; // リフレッシュレートの分母
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
		sd.OutputWindow = m_hWnd;
		sd.SampleDesc.Count = m_nMultiSampleCount;
		sd.SampleDesc.Quality = m_nMultiSampleQuality;
		sd.Windowed = TRUE;

		// デバイスとスワップチェインの作成
		for ( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
		{
				m_driverType = driverTypes[driverTypeIndex];
				hr = D3D11CreateDeviceAndSwapChain(
						NULL,
						m_driverType,
						NULL,
						createDeviceFlags,
						featureLevels,
						numFeatureLevels,
						D3D11_SDK_VERSION,
						&sd,
						&m_pSwapChain,
						&m_pd3dDevice,
						&m_featureLevel,
						&m_pd3dDeviceContext
				);
				if ( SUCCEEDED( hr ) )
						break;
		}
		if ( FAILED( hr ) )
				return hr;

		// バックバッファ取得
		hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* ) &m_pRTTexture );
		if ( FAILED( hr ) )
				return hr;

		// レンダーターゲットビューを生成
		hr = m_pd3dDevice->CreateRenderTargetView( m_pRTTexture, NULL, &m_pRTView );
		if ( FAILED( hr ) )
				return hr;

		// レンダーターゲットのシェーダリソースビューを生成
		hr = m_pd3dDevice->CreateShaderResourceView( m_pRTTexture, NULL, &m_pRTShaderResourceView );
		if ( FAILED( hr ) )
				return  hr;


		// ピューポートの設定
		D3D11_VIEWPORT  vp;
		vp.Width = ( FLOAT ) width;
		vp.Height = ( FLOAT ) height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;

		// デバイスコンテキストにビューポートを設定
		m_pd3dDeviceContext->RSSetViewports( 1, &vp );

		return  S_OK;

}// end CreateDevice()


HRESULT  IDirect3D::CreateDefaultDepthStencil()
{
		HRESULT  hr = S_OK;

		DXGI_FORMAT  textureFormat = m_depthStencilFormat;
		DXGI_FORMAT  resourceFormat = m_depthStencilFormat;

		// テクスチャとシェーダリソースビューのフォーマットを適切なものに変更
		switch ( m_depthStencilFormat )
		{
		case  DXGI_FORMAT_D16_UNORM:
				{
						textureFormat = DXGI_FORMAT_R16_TYPELESS;
						resourceFormat = DXGI_FORMAT_R16_UNORM;
				}
				break;

		case  DXGI_FORMAT_D24_UNORM_S8_UINT:
				{
						textureFormat = DXGI_FORMAT_R24G8_TYPELESS;
						resourceFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
				}
				break;

		case  DXGI_FORMAT_D32_FLOAT:
				{
						textureFormat = DXGI_FORMAT_R32_TYPELESS;
						resourceFormat = DXGI_FORMAT_R32_FLOAT;
				}
				break;

		case  DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
				{
						textureFormat = DXGI_FORMAT_R32G8X24_TYPELESS;
						resourceFormat = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
				}
				break;
		}

		// 深度ステンシルテクスチャの生成
		D3D11_TEXTURE2D_DESC  td;
		ZeroMemory( &td, sizeof( D3D11_TEXTURE2D_DESC ) );
		td.Width = m_nScreenWidth;
		td.Height = m_nScreenHeight;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = textureFormat;
		td.SampleDesc.Count = m_nMultiSampleCount;
		td.SampleDesc.Quality = m_nMultiSampleQuality;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		// 震度ステンシルテクスチャの生成
		hr = m_pd3dDevice->CreateTexture2D( &td, NULL, &m_pDSTexture );
		if ( FAILED( hr ) )
				return  E_FAIL;

		// 深度ステンシルビューの設定
		D3D11_DEPTH_STENCIL_VIEW_DESC  dsvd;
		ZeroMemory( &dsvd, sizeof( D3D11_DEPTH_STENCIL_VIEW_DESC ) );
		dsvd.Format = m_depthStencilFormat;
		if ( m_nMultiSampleCount == 0 )
		{
				dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				dsvd.Texture2D.MipSlice = 0;
		}
		else
		{
				dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		}

		// 深度ステンシルビューの生成
		hr = m_pd3dDevice->CreateDepthStencilView( m_pDSTexture, &dsvd, &m_pDSView );
		if ( FAILED( hr ) )
				return  E_FAIL;

		// シェーダリソースビューの設定
		D3D11_SHADER_RESOURCE_VIEW_DESC  srvd;
		ZeroMemory( &srvd, sizeof( D3D11_SHADER_RESOURCE_VIEW_DESC ) );
		srvd.Format = resourceFormat;

		if ( m_nMultiSampleCount == 0 )
		{
				srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvd.Texture2D.MostDetailedMip = 0;
				srvd.Texture2D.MipLevels = 1;
		}
		else
		{
				srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		}

		// シェーダリソースビューの生成
		hr = m_pd3dDevice->CreateShaderResourceView( m_pDSTexture, &srvd, &m_pDSShaderResourceView );
		if ( FAILED( hr ) )
				return  E_FAIL;

		// デバイスコンテキストにレンダーターゲットを設定
		m_pd3dDeviceContext->OMSetRenderTargets( 1, &m_pRTView, m_pDSView );

		return  S_OK;
}// end CreateDefaultDepthStencil()


VOID  IDirect3D::MainLoop()
{
		// デバイスのポインタを名前空間のグローバル変数へ保持させる
		D3D11Utility::SetD3DDevices( m_pd3dDevice, m_pd3dDeviceContext );
		GameUtility::GameInit();

		MSG msg;
		ZeroMemory( &msg, sizeof( msg ) );

		while ( msg.message != WM_QUIT )
		{

				if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
				{
						TranslateMessage( &msg );
						DispatchMessage( &msg );
				}// end if
				else
				{
						GameUtility::GameLoop();
				}// end else

		}// end while

}// end MainLoop()


VOID  IDirect3D::SetWindow( INT  screenWidth, INT  screenHeight )
{
		m_nScreenWidth = screenWidth;
		m_nScreenHeight = screenHeight;

		RECT rcWin, rcClt;
		// ウィンドウサイズを再計算（Metricsだけでは、フレームデザインでクライアント領域サイズが変わってしまうので）
		GetWindowRect( m_hWnd, &rcWin );
		GetClientRect( m_hWnd, &rcClt );

		int width = ( rcWin.right - rcWin.left ) - ( rcClt.right - rcClt.left ) + m_nScreenWidth;
		int height = ( rcWin.bottom - rcWin.top ) - ( rcClt.bottom - rcClt.top ) + m_nScreenHeight;

		SetWindowPos(
				m_hWnd, NULL,
				GetSystemMetrics( SM_CXSCREEN ) / 2 - width / 2,
				GetSystemMetrics( SM_CYSCREEN ) / 2 - height / 2,
				width - 1, height - 1,
				SWP_NOZORDER );

		ShowWindow( m_hWnd, SW_SHOWDEFAULT );
		UpdateWindow( m_hWnd );
}// end SetWindow(INT, INT)


VOID  IDirect3D::BeginRender()
{
		// 最終描画結果のレンダーターゲット
		m_pd3dDeviceContext->OMSetRenderTargets( 1, &m_pRTView, m_pDSView );
		// ビューをクリア
		m_pd3dDeviceContext->ClearRenderTargetView( m_pRTView, m_fClearColors );
		m_pd3dDeviceContext->ClearDepthStencilView( m_pDSView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );



}


VOID  IDirect3D::EndRender()
{
		// レンダリングイメージの表示
		m_pSwapChain->Present( 0, 0 );
}// end EndRender()


VOID  IDirect3D::Release()
{
		if ( m_pd3dDeviceContext )
		{
				// ステートをクリア
				m_pd3dDeviceContext->ClearState();
				// デフォルトステート
				m_pd3dDeviceContext->Flush();
		}

		// レンダーターゲットを解放
		ReleaseDefaultRenderTarget();
		// 深度ステンシルバッファを解放
		ReleaseDefaultDepthStencil();

		// スワップチェインを解放
		SafeRelease( m_pSwapChain );
		// デバイスコンテキストを解放
		SafeRelease( m_pd3dDeviceContext );
		// デバイスを解放
		SafeRelease( m_pd3dDevice );

		ReleaseScreen();
}// end Release()


VOID  IDirect3D::ReleaseScreen()
{
		if ( m_hInstance != NULL )
				UnregisterClass( m_szWindowClass, m_hInstance );
}// end ReleaseScreen()


VOID  IDirect3D::ReleaseDefaultRenderTarget()
{
		SafeRelease( m_pRTShaderResourceView );
		SafeRelease( m_pRTView );
		SafeRelease( m_pRTTexture );
}// end ReleaseDefaultRenderTarget()


VOID  IDirect3D::ReleaseDefaultDepthStencil()
{
		SafeRelease( m_pDSShaderResourceView );
		SafeRelease( m_pDSView );
		SafeRelease( m_pDSTexture );
}// end ReleaseDefaultDepthStencil()