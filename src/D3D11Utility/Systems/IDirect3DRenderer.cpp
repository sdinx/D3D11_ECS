//#define  _RENDERING_TIME_COUNT_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Camera.h>
#include  <D3D11Utility\Renderable.h>
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <D3D11Utility\Systems\IDirect3DRenderer.h>
#include  <GameUtility.h>
#include  <D3D11Utility\Systems\Timer.h>


//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  D3D11Utility::Systems;


IDirect3DRenderer::IDirect3DRenderer( ComponentManager*  pComponentManagerInstance ) :
		m_componentManager( pComponentManagerInstance ),
		m_pID3D( _Singleton<IDirect3D>::GetInstance() )
{

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
}


void  IDirect3DRenderer::Rendering()const
{
		m_pID3D->BeginRender();
		{/* Begin rendering */
				static  UINT  nCount = 1;
				static  float  fLastTime = 0.0f;
				static  float  fAll = 0;
				Timer  timer;
				int  i = 0;

				for ( auto renderable : m_componentManager->GetComponents<Renderable>() )
				{
						renderable->Update();
						renderable->Rendering();
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

		}/* Done rendering */
		m_pID3D->EndRender();

}// end Rendering()const


Graphics::VertexShader*  IDirect3DRenderer::CreateVertexShader( LPCWSTR  szFileName, LPCSTR  szEntryPoint, LPCSTR  szVSModel )
{
		const  size_t  shaderHash = std::hash<LPCSTR>()( szVSModel );
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


		// 入力レイアウトの定義
		const  D3D11_INPUT_ELEMENT_DESC  layout[ ] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		const  UINT  numElements = ARRAYSIZE( layout );

		hr = pd3dDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pInputLayout );
		SafeRelease( pVSBlob );
		if ( FAILED( hr ) )
		{
				std::cout << "<IDirect3DRenderer>  Create input layout failed." << std::endl;
				return  nullptr;
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
		const  size_t  shaderHash = std::hash<LPCSTR>()( szEntryPoint );
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
		const  size_t  shaderHash = std::hash<LPCSTR>()( szPSModel );
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