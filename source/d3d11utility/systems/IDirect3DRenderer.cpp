//#define  _RENDERING_TIME_COUNT_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <d3d11utility\components/Camera.h>
#include  <d3d11utility\components/Renderable.h>
#include  <d3d11utility\Systems\ComponentManager.h>
#include  <d3d11utility\Systems\IDirect3DRenderer.h>
#include  <d3d11utility\Systems\TextureManager.h>
#include  <d3d11utility\Timer.h>
#include  <game/GameUtility.h>


//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  D3D11Utility::Systems;


IDirect3DRenderer::IDirect3DRenderer( ComponentManager*  pComponentManagerInstance ) :
		m_componentManager( pComponentManagerInstance ),
		m_pID3D( _Singleton<IDirect3D>::GetInstance() )
{
		CreateMultipleRenderTargetView();
}


IDirect3DRenderer::~IDirect3DRenderer()
{
		Release();
}


void  IDirect3DRenderer::Release()
{
		m_vertexShaderList.clear();
		m_geometryShaderList.clear();
		m_pixelShaderList.clear();

		m_renderTagets.clear();
		m_renderTagets.shrink_to_fit();

}


void  IDirect3DRenderer::Rendering()const
{
		ID3D11RenderTargetView*  rtvs[RT_ARRAY_COUNTS];

		int  i = 0;
		for ( auto& rt : m_renderTagets )
		{
				rtvs[i] = rt.m_pRTView;
				pd3dDeviceContext->ClearRenderTargetView( rt.m_pRTView, m_fClearColors );
				i++;
		}
		pd3dDeviceContext->ClearDepthStencilView( m_pDSView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

		pd3dDeviceContext->OMSetRenderTargets( RT_ARRAY_COUNTS, rtvs, m_pDSView );

		{/* Begin first rendering */
				static  uint  nCount = 1;
				static  float  fLastTime = 0.0f;
				static  float  fAll = 0;
				Timer  timer;
				uint  i = 0;
				Renderable*  render;
				auto  textureManager = _Singleton<TextureManager>::GetInstance();
				pd3dDeviceContext->OMSetDepthStencilState( m_pDepthStencilState, 0 );

				for ( auto renderable : m_componentManager->GetComponents<Renderable>() )
				{
						renderable->HandleMessage( Renderable::MSG_UPDATE_CBUFFER );
						render = renderable->GetComponent<Renderable>();
						textureManager->SetTexture( render->m_textureId );
						render->Rendering();
				}

#ifdef  _RENDERING_TIME_COUNT_
				// 描画時間計測
#pragma 
				{
						float  fThisTime = timer.GetElapsed<Timer::Milliseconds>();
						printf( "Loop time: %f ms ( %f )\n", fThisTime, fThisTime - fLastTime );
						fAll += fThisTime;
						printf( "Average time: %f ms\n", fAll / ( float ) nCount );
						nCount++;
						system( "CLS" );
						fLastTime = fThisTime;
				}
#endif // _RENDERING_TIME_COUNT_

		}/* Done first rendering */

		{/* Begin deferred rendering */

				pd3dDeviceContext->OMSetRenderTargets( 1, &m_pRTView, nullptr );
				pd3dDeviceContext->ClearRenderTargetView( m_pRTView, m_fClearColors );
				//pd3dDeviceContext->ClearDepthStencilView( m_pDSView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

				pd3dDeviceContext->OMSetDepthStencilState( m_pDepthStencilState, 0 );

				uint stride = 36;
				uint offset = 0;
				m_pVShader->UpdateShader();
				pd3dDeviceContext->IASetVertexBuffers( 0, 1, &m_pVtxBuffer, &stride, &offset );
				pd3dDeviceContext->RSSetState( m_rasterState );

				constexpr  uint  RT_TEXTURE_COUNTS = RT_ARRAY_COUNTS + 2;
				ID3D11ShaderResourceView*  srvs[RT_TEXTURE_COUNTS];
				ID3D11SamplerState*  ss[RT_TEXTURE_COUNTS];
				int  nSRViewCounts = 0;
				for ( auto& rt : m_renderTagets )
				{
						srvs[nSRViewCounts] = rt.m_pSRView;
						ss[nSRViewCounts] = m_sampler;
						nSRViewCounts++;
				}

				srvs[nSRViewCounts] = m_pDSShaderResourceView;
				ss[nSRViewCounts] = m_sampler;

				nSRViewCounts++;
				srvs[nSRViewCounts] = m_pSTShaderResourceView;
				ss[nSRViewCounts] = m_sampler;

				pd3dDeviceContext->PSSetShaderResources( 0, RT_TEXTURE_COUNTS, srvs );
				pd3dDeviceContext->PSSetSamplers( 0, RT_TEXTURE_COUNTS, ss );
				m_pPShader->UpdateShader();

				pd3dDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
				pd3dDeviceContext->Draw( 4, 0 );

				ID3D11ShaderResourceView*  srvsClear[RT_TEXTURE_COUNTS];
				ID3D11SamplerState*  ssClear[RT_TEXTURE_COUNTS];
				for ( uint nClear = 0; nClear < RT_TEXTURE_COUNTS; nClear++ )
				{
						srvsClear[nClear] = nullptr;
						ssClear[nClear] = nullptr;
				}

				pd3dDeviceContext->PSSetShaderResources( 0, RT_TEXTURE_COUNTS, srvsClear );
				pd3dDeviceContext->PSSetSamplers( 0, RT_TEXTURE_COUNTS, ssClear );

		}/* Done deferred rendering */

		m_pID3D->EndRender();

}// end Rendering()const


HRESULT  IDirect3DRenderer::CreateMultipleRenderTargetView()
{
		HRESULT  hr;

		hr = m_pID3D->GetSwapChain()->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* ) &m_pRTTexture );
		if ( FAILED( hr ) )
				return hr;

		// レンダーターゲットビューを生成
		hr = pd3dDevice->CreateRenderTargetView( m_pRTTexture, NULL, &m_pRTView );
		if ( FAILED( hr ) )
				return hr;

		// レンダーターゲットのシェーダリソースビューを生成
		hr = pd3dDevice->CreateShaderResourceView( m_pRTTexture, NULL, &m_pRTShaderResourceView );
		if ( FAILED( hr ) )
				return  hr;

		m_pDSView = m_pID3D->GetDepthStencilView();
		m_pDSTexture = m_pID3D->GetDSTexture();
		m_pDSShaderResourceView = m_pID3D->GetDSShaderResourceView();
		m_pSTShaderResourceView = m_pID3D->GetSTShaderResourceView();

		POINT  screen = m_pID3D->GetScreenSize();

		D3D11_TEXTURE2D_DESC  texDesc;
		ZeroMemory( &texDesc, sizeof( texDesc ) );
		texDesc.Width = screen.x;
		texDesc.Height = screen.y;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		D3D11_TEXTURE2D_DESC  texDescs[RT_ARRAY_COUNTS] = { texDesc,texDesc,texDesc };
		const  size_t  descSize = ARRAYSIZE( texDescs );

		// 法線
		texDescs[0].Format = DXGI_FORMAT_R8G8_UNORM;// float2 にエンコード・デコードできるので2byteでいい.
		texDescs[0].BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		// ディフューズ
		texDescs[1].Format = DXGI_FORMAT_R8G8B8A8_UNORM;// 0.0~1.0 の範囲の値なので4byte.
		texDescs[1].BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		// スペキュラ
		texDescs[2].Format = DXGI_FORMAT_R8_UNORM;// ディフューズ同様でAの値は強度を表す.
		texDescs[2].BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		m_renderTagets.resize( descSize );

		for ( int i = 0; i < descSize; i++ )
		{
				hr = pd3dDevice->CreateTexture2D( &texDescs[i], nullptr, &m_renderTagets[i].m_pTexture );
				if ( FAILED( hr ) )
				{
						printf( "<IDirect3DRenderer> CreateMultiRenderTargetView() failed : 210" );
						return  hr;
				}// end if

				hr = pd3dDevice->CreateRenderTargetView( m_renderTagets[i].m_pTexture, nullptr, &m_renderTagets[i].m_pRTView );
				hr = pd3dDevice->CreateShaderResourceView( m_renderTagets[i].m_pTexture, nullptr, &m_renderTagets[i].m_pSRView );

		}// end for

		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory( &sampDesc, sizeof( sampDesc ) );
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		pd3dDevice->CreateSamplerState( &sampDesc, &m_sampler );


		D3D11_INPUT_ELEMENT_DESC  layout[ ] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT  numLayouts = ARRAYSIZE( layout );
		m_pVShader = CreateVertexShader( L"shader/Deferred.hlsl", "vsmain", "vs_5_0", numLayouts, layout );
		m_pPShader = CreatePixelShader( L"shader/Deferred.hlsl", "psmain" );

		float  vertex[4 * 9] = {
				-1.0f, 1.0f, 0.0f,  0.0f,0.0f,  1.0f,1.0f,1.0f,1.0f,
				1.0f, 1.0f, 0.0f,  1.0f,0.0f,  1.0f,1.0f,1.0f,1.0f,
				-1.0f, -1.0f, 0.0f,  0.0f,1.0f,  1.0f,1.0f,1.0f,1.0f,
				1.0f, -1.0f, 0.0f,  1.0f,1.0f,  1.0f,1.0f,1.0f,1.0f,
		};

		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof( bd ) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( vertex );
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory( &InitData, sizeof( InitData ) );
		InitData.pSysMem = vertex;
		pd3dDevice->CreateBuffer( &bd, &InitData, &m_pVtxBuffer );

		D3D11_RASTERIZER_DESC  rdc = { };
		rdc.CullMode = D3D11_CULL_NONE;
		rdc.FillMode = D3D11_FILL_SOLID;
		rdc.FrontCounterClockwise = TRUE;
		pd3dDevice->CreateRasterizerState( &rdc, &m_rasterState );

		D3D11_DEPTH_STENCIL_DESC  dsDesc = CD3D11_DEPTH_STENCIL_DESC( CD3D11_DEFAULT() );
		hr = pd3dDevice->CreateDepthStencilState( &dsDesc, &m_pDepthStencilState );
		if ( FAILED( hr ) )
		{
				std::cout << "<IDirect3DRenderer> CreateMultiRenderTargetView() failed : 273\n";
		}

		return  hr;

}// end CreateMultiRenderTargetView() : HRESULT


Graphics::VertexShader*  IDirect3DRenderer::CreateVertexShader( LPCWSTR  szFileName, LPCSTR  szEntryPoint, LPCSTR  szVSModel, UINT  numLayouts, D3D11_INPUT_ELEMENT_DESC*  layouts )
{
		const  size_t  shaderHash = std::hash<LPCWSTR>()( szFileName );
		const  Graphics::ShaderId  shaderId = m_vertexShaderList.size();
		const  size_t  hash = std::hash<LPCSTR>()( szEntryPoint );

		// 重複で生成を防止
		for ( auto vs : m_vertexShaderList )
		{
				if ( vs->entryPointHash == hash )
						return  vs;
		}

		HRESULT  hr = S_OK;
		ID3DBlob*  pVSBlob = nullptr;
		ID3D11VertexShader*  pVertexShader = nullptr;
		ID3D11InputLayout*  pInputLayout = nullptr;

		hr = D3D11Utility::CompileShaderFromFile( szFileName, szEntryPoint, szVSModel, &pVSBlob );
		if ( FAILED( hr ) )
		{
				std::cout << "<IDirect3DRenderer>  Failed to compile a vertex shader by: " << szEntryPoint << std::endl;
				SafeRelease( pVSBlob );
				return  nullptr;
		}

		// 頂点シェーダーの生成
		hr = pd3dDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &pVertexShader );
		if ( FAILED( hr ) )
		{
				std::cout << "<IDirect3DRenderer>  Create vertex shader failed." << std::endl;
				SafeRelease( pVSBlob );
				return  nullptr;
		}

		if ( numLayouts == 0 )
		{
				// 入力レイアウトの定義
				const  D3D11_INPUT_ELEMENT_DESC  elementDescs[ ] = {
						{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				};
				const  UINT  numElements = ARRAYSIZE( elementDescs );

				hr = pd3dDevice->CreateInputLayout( elementDescs, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pInputLayout );
				SafeRelease( pVSBlob );
				if ( FAILED( hr ) )
				{
						std::cout << "<IDirect3DRenderer>  Create input layout failed." << std::endl;
						return  nullptr;
				}
		}
		else
		{
				hr = pd3dDevice->CreateInputLayout( layouts, numLayouts, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pInputLayout );
				SafeRelease( pVSBlob );
				if ( FAILED( hr ) )
				{
						std::cout << "<IDirect3DRenderer>  Create input layout failed." << std::endl;
						return  nullptr;
				}
		}

		// シェーダ生成
		m_vertexShaderList.push_back( new  Graphics::VertexShader( pVertexShader, pInputLayout ) );
		// ハッシュ値と固有IDの設定
		m_vertexShaderList.back()->entryPointHash = hash;
		m_vertexShaderList.back()->m_shaderId = shaderId;

		return  m_vertexShaderList.back();
}


Graphics::GeometryShader*  IDirect3DRenderer::CreateGeometryShader( LPCWSTR  szFileName, LPCSTR  szEntryPoint, LPCSTR  szGSModel )
{
		const  size_t  shaderHash = std::hash<LPCWSTR>()( szFileName );
		const  Graphics::ShaderId  shaderId = m_geometryShaderList.size();
		const  size_t  hash = std::hash<LPCSTR>()( szEntryPoint );

		// 重複で生成を防止
		for ( auto gs : m_geometryShaderList )
		{
				if ( gs->entryPointHash == hash )
						return  gs;
		}

		HRESULT  hr = S_OK;
		ID3DBlob*  pGSBlob = nullptr;
		ID3D11GeometryShader*  pGeometryShader = nullptr;

		hr = D3D11Utility::CompileShaderFromFile( szFileName, szEntryPoint, szGSModel, &pGSBlob );
		if ( FAILED( hr ) )
		{
				std::cout << "<IDirect3DRenderer>  Failed to compile a geometry shader by: " << szEntryPoint << std::endl;
				SafeRelease( pGSBlob );
				return  nullptr;
		}

		// 頂点シェーダーの生成
		hr = pd3dDevice->CreateGeometryShader( pGSBlob->GetBufferPointer(), pGSBlob->GetBufferSize(), NULL, &pGeometryShader );
		SafeRelease( pGSBlob );
		if ( FAILED( hr ) )
		{
				std::cout << "<IDirect3DRenderer>  Failed to create a geometry shader." << std::endl;
				return  nullptr;
		}

		// シェーダ生成
		m_geometryShaderList.push_back( new  Graphics::GeometryShader( pGeometryShader ) );
		// ハッシュ値と固有IDの設定
		m_geometryShaderList.back()->entryPointHash = hash;
		m_geometryShaderList.back()->m_shaderId = shaderId;

		return  m_geometryShaderList.back();
}


Graphics::PixelShader*  IDirect3DRenderer::CreatePixelShader( LPCWSTR  szFileName, LPCSTR  szEntryPoint, LPCSTR  szPSModel )
{
		std::string  s;

		const  size_t  shaderHash = std::hash<LPCWSTR>()( szFileName );
		const  Graphics::ShaderId  shaderId = m_pixelShaderList.size();
		const  size_t  hash = std::hash<LPCSTR>()( szEntryPoint );

		// 重複で生成を防止
		for ( auto ps : m_pixelShaderList )
		{
				if ( ps->entryPointHash == hash )
						return  ps;
		}

		HRESULT  hr = S_OK;
		ID3DBlob*  pPSBlob = nullptr;
		ID3D11PixelShader*  pPixelShader = nullptr;

		hr = D3D11Utility::CompileShaderFromFile( szFileName, szEntryPoint, szPSModel, &pPSBlob );
		if ( FAILED( hr ) )
		{
				std::cout << "<IDirect3DRenderer>  Failed to compile a pixel shader by: " << szEntryPoint << std::endl;
				SafeRelease( pPSBlob );
				return  nullptr;
		}

		// ピクセルシェーダーの生成
		hr = pd3dDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &pPixelShader );
		SafeRelease( pPSBlob );
		if ( FAILED( hr ) )
		{
				std::cout << "<IDirect3DRenderer>  Failed to create a pixel shader.\n";
				return  nullptr;
		}

		// シェーダ生成
		m_pixelShaderList.push_back( new  Graphics::PixelShader( pPixelShader ) );
		// ハッシュ値と固有IDの設定
		m_pixelShaderList.back()->entryPointHash = hash;
		m_pixelShaderList.back()->m_shaderId = shaderId;

		return  m_pixelShaderList.back();
}


Graphics::VertexShader*  IDirect3DRenderer::GetVertexShader( size_t  index )
{
		if ( index < m_vertexShaderList.size() )
				return  m_vertexShaderList[index];

		printf( "<IDirect3DRenderer> GetVertexShader( %d ) invalid value.\n", index );
		return  nullptr;
}


Graphics::GeometryShader*  IDirect3DRenderer::GetGeometryShader( size_t  index )
{
		if ( index < m_geometryShaderList.size() )
				return  m_geometryShaderList[index];

		printf( "<IDirect3DRenderer> GetGeometryShader( %d ) invalid value.\n", index );
		return  nullptr;
}


Graphics::PixelShader*  IDirect3DRenderer::GetPixelShader( size_t  index )
{
		if ( index < m_pixelShaderList.size() )
				return  m_pixelShaderList[index];

		printf( "<IDirect3DRenderer> GetPixelShader( %d ) invalid value.\n", index );
		return  nullptr;
}