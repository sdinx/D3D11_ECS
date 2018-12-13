//#define  _RENDERING_TIME_COUNT_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <d3d11utility\components/Camera.h>
#include  <d3d11utility\components/Renderable.h>
#include  <d3d11utility\components/PointLight.h>
#include  <d3d11utility/graphics/Material.h>
#include  <d3d11utility\Systems\ComponentManager.h>
#include  <d3d11utility\Systems\IDirect3DRenderer.h>
#include  <d3d11utility\Systems\TextureManager.h>
#include  <d3d11utility\Timer.h>
#include  <game/GameUtility.h>
#include  <comdef.h>


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
		for ( auto& buffer : m_vertexBufferList )
		{
				SafeDelete( buffer );
		}
		m_vertexBufferList.clear();

		for ( auto& shader : m_vertexShaderList )
		{
				SafeDelete( shader );
		}
		m_vertexShaderList.clear();

		for ( auto& shader : m_geometryShaderList )
		{
				SafeDelete( shader );
		}
		m_geometryShaderList.clear();

		for ( auto& shader : m_pixelShaderList )
		{
				SafeDelete( shader );
		}
		m_pixelShaderList.clear();

		m_renderTagets.clear();
		m_renderTagets.shrink_to_fit();

		SafeRelease( m_pVtxBuffer );
		SafeRelease( m_sampler );

		for ( auto& rs : m_rasterStates )
				SafeRelease( rs );

		m_fbxLoaderMap.clear();
		FbxLoader::StaticRelease();

		m_pID3D.~shared_ptr();
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
		pd3dDeviceContext->ClearDepthStencilView( m_pDSView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

		pd3dDeviceContext->OMSetRenderTargets( RT_ARRAY_COUNTS, rtvs, m_pDSView.Get() );

		/* Begin first rendering */
		{
				static  uint  nCount = 1;
				static  float  fLastTime = 0.0f;
				static  float  fAll = 0;
				Timer  timer;
				uint  i = 0;
				Renderable*  render;
				auto  textureManager = _Singleton<TextureManager>::GetInstance();
				pd3dDeviceContext->OMSetDepthStencilState( m_pDepthStencilState.Get(), 0 );

				for ( auto renderable : m_componentManager->GetComponents<Renderable>() )
				{
						renderable->HandleMessage( Renderable::MSG_UPDATE_CBUFFER );
						render = renderable->GetComponent<Renderable>();
						textureManager->SetDiffuse( render->m_diffuseId );
						textureManager->SetNormal( render->m_normalId );

						pd3dDeviceContext->RSSetState( m_rasterStates[render->m_nRasterMode] );
						m_materialList[render->m_materialId]->UpdateConstantBuffer();

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

		/* Begin light rendering */
		{
				ID3D11RenderTargetView*  srv = m_pClusterRTView.Get();
				pd3dDeviceContext->ClearRenderTargetView( m_pClusterRTView.Get(), m_fClearColors );
				pd3dDeviceContext->OMSetRenderTargets( 1, &srv, nullptr );

				m_pClusterVShader->UpdateShader();
				//m_pClusterGShader->UpdateShader();
				m_pClusterPShader->UpdateShader();

				pd3dDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

				pd3dDeviceContext->RSSetState( m_rasterStates[Graphics::eNoneSolid] );
				pd3dDeviceContext->PSSetShaderResources( 0, 0, nullptr );
				pd3dDeviceContext->PSSetSamplers( 0, 0, nullptr );

				Renderable*  ptRender = nullptr;
				//for ( auto ptLight : m_componentManager->GetComponents<PointLight>() )
				//{
				//		ptRender = ptLight->GetComponent<Renderable>();
				//		pd3dDeviceContext->UpdateSubresource( ptRender->s_pConstantBuffer, 0, nullptr, &ptRender->m_cbuffer, 0, 0 );
				//
				//		m_pVertexPtLight->BindBuffer();
				//}

				auto  strBuffer = PointLight::GetStructuredBuffer();
				auto  ptLights = strBuffer->Map();
				for ( int i = 0; i < PointLight::s_nLightCounts; i++ )
						ptLights[i] = PointLight::s_instanceLights[i];

				strBuffer->Unmap();
				ID3D11ShaderResourceView*  srvLight = strBuffer->GetShaderResourceView();
				pd3dDeviceContext->VSSetShaderResources( 3, 1, &srvLight );
				pd3dDeviceContext->PSSetShaderResources( 3, 1, &srvLight );


				m_pVertexPtLight->BindBuffer();
				pd3dDeviceContext->DrawInstanced( m_pVertexPtLight->GetVertexCounts(), PointLight::s_nLightCounts, 0, 0 );

				pd3dDeviceContext->GSSetShader( nullptr, nullptr, 0 );

				// todo: ライトをインスタンス描画する.

		}/* Done light rendering */

		 /* Begin deferred rendering */
		{

				ID3D11RenderTargetView*  defRtv = m_pRTView.Get();
				pd3dDeviceContext->ClearRenderTargetView( m_pRTView.Get(), m_fClearColors );
				pd3dDeviceContext->OMSetRenderTargets( 1, &defRtv, nullptr );
				//pd3dDeviceContext->ClearDepthStencilView( m_pDSView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

				uint stride = 36;
				uint offset = 0;
				m_pVShader->UpdateShader();
				pd3dDeviceContext->IASetVertexBuffers( 0, 1, &m_pVtxBuffer, &stride, &offset );
				pd3dDeviceContext->RSSetState( m_rasterStates[Graphics::eNoneSolid] );

				constexpr  uint  RT_TEXTURE_COUNTS = RT_ARRAY_COUNTS + 3;
				ID3D11ShaderResourceView*  srvs[RT_TEXTURE_COUNTS];
				int  nSRViewCounts = 0;
				for ( auto& rt : m_renderTagets )
				{
						srvs[nSRViewCounts] = rt.m_pSRView;
						nSRViewCounts++;
				}

				srvs[nSRViewCounts] = m_pDSShaderResourceView.Get();

				nSRViewCounts++;
				srvs[nSRViewCounts] = m_pSTShaderResourceView.Get();

				nSRViewCounts++;
				srvs[nSRViewCounts] = m_pClusterRTShaderResourceView.Get();

				pd3dDeviceContext->PSSetShaderResources( 0, RT_TEXTURE_COUNTS, srvs );
				pd3dDeviceContext->PSSetSamplers( 0, 1, &m_sampler );
				m_pPShader->UpdateShader();

				pd3dDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
				pd3dDeviceContext->Draw( 4, 0 );

				ID3D11ShaderResourceView*  srvsClear[RT_TEXTURE_COUNTS];
				ID3D11SamplerState*  ssClear = nullptr;
				for ( uint nClear = 0; nClear < RT_TEXTURE_COUNTS; nClear++ )
				{
						srvsClear[nClear] = nullptr;
				}

				pd3dDeviceContext->PSSetShaderResources( 0, RT_TEXTURE_COUNTS, srvsClear );
				pd3dDeviceContext->PSSetSamplers( 0, 1, &ssClear );

		}/* Done deferred rendering */

		// デバッグ時は ImGUI を描画するため, あとで描画コマンドを実行する.
#ifndef  _DEBUG
		m_pID3D->EndRender();
#endif // ! _DEBUG

}// end Rendering()const


HRESULT  IDirect3DRenderer::CreateMultipleRenderTargetView()
{
		HRESULT  hr;

		LoadFbxModel( "res/cube.fbx" );
		LoadFbxModel( "res/sphere.fbx" );

		hr = m_pID3D->GetSwapChain()->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* ) &m_pRTTexture );
		if ( FAILED( hr ) )
				return hr;

		// レンダーターゲットビューを生成
		hr = pd3dDevice->CreateRenderTargetView( m_pRTTexture.Get(), NULL, &m_pRTView );
		if ( FAILED( hr ) )
				return hr;

		// レンダーターゲットのシェーダリソースビューを生成
		hr = pd3dDevice->CreateShaderResourceView( m_pRTTexture.Get(), NULL, &m_pRTShaderResourceView );
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


		// クラスタ用 SRV 作成.
		{
				D3D11_TEXTURE3D_DESC  tex3dDesc;
				ZeroMemory( &tex3dDesc, sizeof( tex3dDesc ) );
				tex3dDesc.Width = screen.x;
				tex3dDesc.Height = screen.y;
				tex3dDesc.Depth = 16;
				tex3dDesc.MipLevels = 1;
				tex3dDesc.Format = DXGI_FORMAT_R32G32_UINT;// R32 : offset, G32 : [PointLightCount, SpotLightCount]
				tex3dDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
				tex3dDesc.Usage = D3D11_USAGE_DYNAMIC;
				tex3dDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				tex3dDesc.MiscFlags = 0;

				hr = pd3dDevice->CreateTexture3D( &tex3dDesc, nullptr, &m_pClusterRTTexture );
				if ( FAILED( hr ) )
				{
						printf( "<IDirect3DRenderer> CreateMultiRenderTargetView() failed : 232" );
						return  hr;
				}// end if

				D3D11_RENDER_TARGET_VIEW_DESC  rtDesc;
				rtDesc.Format = tex3dDesc.Format;
				rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
				rtDesc.Texture3D.WSize = 16;
				rtDesc.Texture3D.FirstWSlice = 0;
				rtDesc.Texture3D.MipSlice = 0;

				hr = pd3dDevice->CreateRenderTargetView( m_pClusterRTTexture.Get(), nullptr, &m_pClusterRTView );

				D3D11_SHADER_RESOURCE_VIEW_DESC  srvDesc;
				ZeroMemory( &srvDesc, sizeof( srvDesc ) );
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
				srvDesc.Texture3D.MipLevels = tex3dDesc.MipLevels;
				srvDesc.Texture3D.MostDetailedMip = 0;
				srvDesc.Format = tex3dDesc.Format;

				hr = pd3dDevice->CreateShaderResourceView( m_pClusterRTTexture.Get(), nullptr, &m_pClusterRTShaderResourceView );

				// UAV作成
				D3D11_UNORDERED_ACCESS_VIEW_DESC  uavDesc;
				ZeroMemory( &uavDesc, sizeof( uavDesc ) );
				uavDesc.Format = tex3dDesc.Format;
				uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
				uavDesc.Texture3D.WSize = 16;
				uavDesc.Texture3D.FirstWSlice = 0;
				uavDesc.Texture3D.MipSlice = 0;

				pd3dDevice->CreateUnorderedAccessView( m_pClusterRTTexture.Get(), nullptr, &m_pClusterUAV );

		}

		// クラスタ用 インプットレイアウト.
		{
				auto&  loader = m_fbxLoaderMap.at( std::hash<std::string>()( "res/sphere.fbx" ) );
				auto  vertices = loader.GetModelContainer( 0 ).vertices;

				const  uint  size = vertices.size();
				DirectX::XMFLOAT3*  vertex = new  DirectX::XMFLOAT3[size];
				for ( uint i = 0; i < size; i++ )
						vertex[i] = DirectX::XMFLOAT3( vertices[i].m_floats[0], vertices[i].m_floats[1], vertices[i].m_floats[2] );

				Graphics::VertexBuffer*  vtxBuffer = new  Graphics::VertexBuffer( vertex, size, sizeof( DirectX::XMFLOAT3 ), m_vertexBufferList.size(), loader );
				m_vertexBufferList.emplace_back( vtxBuffer );
				m_pVertexPtLight = m_vertexBufferList.back();

				D3D11_INPUT_ELEMENT_DESC  clLayout[ ] = {
						{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				};
				UINT  numClusterLayouts = ARRAYSIZE( clLayout );
				m_pClusterVShader = CreateVertexShader( L"shader/Light.hlsl", "vsmain", "vs_5_0", numClusterLayouts, clLayout );
				m_pClusterGShader = CreateGeometryShader( L"shader/Light.hlsl", "gsmain" );
				m_pClusterPShader = CreatePixelShader( L"shader/Light.hlsl", "psmain" );

				// ポイントライトの構造体バッファを作成
				PointLight::SetConstantBuffer();

		}


		// サンプラーステート.
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


		// ディファードレンダリング用頂点バッファ
		{
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
		}


		// カリング
		{
				D3D11_RASTERIZER_DESC  rdc = { };
				rdc.CullMode = D3D11_CULL_NONE;
				rdc.FillMode = D3D11_FILL_SOLID;
				rdc.FrontCounterClockwise = TRUE;
				pd3dDevice->CreateRasterizerState( &rdc, &m_rasterStates[Graphics::eRasterMode::eNoneSolid] );

				rdc.CullMode = D3D11_CULL_BACK;
				pd3dDevice->CreateRasterizerState( &rdc, &m_rasterStates[Graphics::eRasterMode::eBackSolid] );

				rdc.CullMode = D3D11_CULL_FRONT;
				pd3dDevice->CreateRasterizerState( &rdc, &m_rasterStates[Graphics::eRasterMode::eFrontSolid] );

				rdc.CullMode = D3D11_CULL_NONE;
				rdc.FillMode = D3D11_FILL_WIREFRAME;
				pd3dDevice->CreateRasterizerState( &rdc, &m_rasterStates[Graphics::eRasterMode::eNoneWireframe] );
		}


		// デプスステンシルステート作成.
		D3D11_DEPTH_STENCIL_DESC  dsDesc = CD3D11_DEPTH_STENCIL_DESC( CD3D11_DEFAULT() );
		hr = pd3dDevice->CreateDepthStencilState( &dsDesc, &m_pDepthStencilState );
		if ( FAILED( hr ) )
		{
				std::cout << "<IDirect3DRenderer> CreateMultiRenderTargetView() failed : 273\n";
		}

		return  hr;

}// end CreateMultiRenderTargetView() : HRESULT


Graphics::VertexBuffer*  IDirect3DRenderer::CreateVertexBuffer( const  FbxLoader&  fbxLoader )
{
		// モデルの情報を取得 ( 0番目のメッシュ情報 )
		ModelContainer  container = fbxLoader.GetModelContainer( 0 );

		int  i = 0;

		size_t  vertexCount = container.vertices.size();
		VERTEX*  vertices = new  VERTEX[vertexCount];
		for ( auto vertex : container.vertices )
		{
				vertices[i].position = DirectX::XMFLOAT3( vertex.m_floats[0], vertex.m_floats[1], vertex.m_floats[2] );;
				i++;
		}

		i = 0;
		for ( auto texcoord : container.texcoords )
		{
				vertices[i].texcoord = texcoord;
				i++;
		}

		i = 0;
		for ( auto normal : container.normals )
		{
				vertices[i].normal = DirectX::XMFLOAT3( normal.m_floats[0], normal.m_floats[1], normal.m_floats[2] );
				i++;
		}

		Graphics::VertexBuffer*  vtxBuffer = new  Graphics::VertexBuffer( vertices, vertexCount, m_vertexBufferList.size(), fbxLoader );
		m_vertexBufferList.emplace_back( vtxBuffer );

		return  m_vertexBufferList.back();
}


Graphics::VertexShader*  IDirect3DRenderer::CreateVertexShader( LPCWSTR  szFileName, LPCSTR  szEntryPoint, LPCSTR  szVSModel, UINT  numLayouts, D3D11_INPUT_ELEMENT_DESC*  layouts )
{
		_bstr_t  bstr( szFileName );
		std::string  strShader = bstr;
		strShader += "-";
		strShader += szEntryPoint;
		const  size_t  shaderHash = std::hash<std::string>()( strShader );
		const  Graphics::ShaderId  shaderId = m_vertexShaderList.size();

		// 重複で生成を防止
		for ( auto vs : m_vertexShaderList )
		{
				if ( vs->entryPointHash == shaderHash )
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
		m_vertexShaderList.back()->entryPointHash = shaderHash;
		m_vertexShaderList.back()->m_shaderId = shaderId;

		return  m_vertexShaderList.back();
}


Graphics::GeometryShader*  IDirect3DRenderer::CreateGeometryShader( LPCWSTR  szFileName, LPCSTR  szEntryPoint, LPCSTR  szGSModel )
{
		_bstr_t  bstr( szFileName );
		std::string  strShader = bstr;
		strShader += "-";
		strShader += szEntryPoint;
		const  size_t  shaderHash = std::hash<std::string>()( strShader );
		const  Graphics::ShaderId  shaderId = m_geometryShaderList.size();

		// 重複で生成を防止
		for ( auto gs : m_geometryShaderList )
		{
				if ( gs->entryPointHash == shaderHash )
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
		m_geometryShaderList.back()->entryPointHash = shaderHash;
		m_geometryShaderList.back()->m_shaderId = shaderId;

		return  m_geometryShaderList.back();
}


Graphics::PixelShader*  IDirect3DRenderer::CreatePixelShader( LPCWSTR  szFileName, LPCSTR  szEntryPoint, LPCSTR  szPSModel )
{
		_bstr_t  bstr( szFileName );
		std::string  strShader = bstr;
		strShader += "-";
		strShader += szEntryPoint;
		const  size_t  shaderHash = std::hash<std::string>()( strShader );
		const  Graphics::ShaderId  shaderId = m_pixelShaderList.size();

		// 重複で生成を防止
		for ( auto ps : m_pixelShaderList )
		{
				if ( ps->entryPointHash == shaderHash )
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
		m_pixelShaderList.back()->entryPointHash = shaderHash;
		m_pixelShaderList.back()->m_shaderId = shaderId;
		m_pixelShaderList.back()->m_name = strShader;

		return  m_pixelShaderList.back();
}


Graphics::Material*  IDirect3DRenderer::CreateMaterial( std::string  name )
{
		Graphics::Material*  material = new  Graphics::Material( m_materialList.size() );
		material->SetName( name );
		m_materialList.emplace_back( material );


		return  m_materialList.back();
}


Graphics::Material*  IDirect3DRenderer::CreateMaterial( std::string  name, Vector3  _ambient, Vector3  _diffuse, Vector4  _specular, Vector4  _emissive )
{
		Graphics::Material*  material = new  Graphics::Material( m_materialList.size(), _ambient, _diffuse, _specular, _emissive );
		material->SetName( name );
		m_materialList.emplace_back( material );

		return  m_materialList.back();
}


Graphics::Material*  IDirect3DRenderer::GetMaterial( Graphics::MaterialId  index )const
{
		return  m_materialList[index];
}


Graphics::VertexBuffer*  IDirect3DRenderer::GetVertexBuffer( Graphics::MeshId  index )const
{
		if ( ( size_t ) index < m_vertexBufferList.size() )
				return  m_vertexBufferList[index];

		printf( "<IDirect3DRenderer> GetVertexBuffer( %d ) invalid value.\n", index );
		return  nullptr;
}


Graphics::VertexShader*  IDirect3DRenderer::GetVertexShader( size_t  index )const
{
		if ( index < m_vertexShaderList.size() )
				return  m_vertexShaderList[index];

		printf( "<IDirect3DRenderer> GetVertexShader( %d ) invalid value.\n", index );
		return  nullptr;
}


Graphics::GeometryShader*  IDirect3DRenderer::GetGeometryShader( size_t  index )const
{
		if ( index < m_geometryShaderList.size() )
				return  m_geometryShaderList[index];

		printf( "<IDirect3DRenderer> GetGeometryShader( %d ) invalid value.\n", index );
		return  nullptr;
}


Graphics::PixelShader*  IDirect3DRenderer::GetPixelShader( size_t  index )const
{
		if ( index < m_pixelShaderList.size() )
				return  m_pixelShaderList[index];

		printf( "<IDirect3DRenderer> GetPixelShader( %d ) invalid value.\n", index );
		return  nullptr;
}


FbxLoader  IDirect3DRenderer::GetFbxLoader( std::string  fileName )const
{
		size_t  meshHash = std::hash<std::string>()( fileName );

#if  defined  (DEBUG) || (_DEBUG)
		return  m_fbxLoaderMap.at( meshHash );// 範囲外指定時に例外を投げる.
#else// RELEASE
		return  m_fbxLoaderMap[meshHash];// 範囲外指定時に例外を投げないで要素数拡張.
#endif
}


Graphics::VertexBuffer*  IDirect3DRenderer::LoadFbxModel( FbxString  fileName )
{
		std::string  strName = fileName;
		size_t  meshHash = std::hash<std::string>()( strName );

		m_fbxLoaderMap.emplace( meshHash, fileName );

		return  CreateVertexBuffer( m_fbxLoaderMap.at( meshHash ) );
}