//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\Graphics\GeometryShader.h>
#include  <D3D11Utility\Systems\IDirect3D.h>

//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------
#define  DEFAULT_SHADER  L"Shader/Default.fx"
#define  DEFAULT_FUNCTION  "GSFunc"
#define  DEFAULT_VERSION  "gs_5_0"

//----------------------------------------------------------------------------------
// using namespace
//----------------------------------------------------------------------------------
using namespace D3D11Utility;
using namespace Graphics;


GeometryShader::GeometryShader()
{
		CreateGeometryShader( DEFAULT_SHADER );
}


GeometryShader::~GeometryShader()
{
		Release();
}


HRESULT  GeometryShader::CreateGeometryShader( LPCWSTR  szFileName )
{
		HRESULT  hr = S_OK;

		// 頂点シェーダのコンパイル
		ID3DBlob*  pGSBlob = nullptr;

		hr = CompileShaderFromFile( szFileName, DEFAULT_FUNCTION, DEFAULT_VERSION, &pGSBlob );
		if ( FAILED( hr ) )
		{
				OutputDebugString( TEXT( "<GeometryShader> FAILED CompileShaderFromFile \n" ) );
				return  hr;
		}


		// 頂点シェーダーの生成
		hr = pd3dDevice->CreateGeometryShader( pGSBlob->GetBufferPointer(), pGSBlob->GetBufferSize(), NULL, &m_pGeometryShader );
		if ( FAILED( hr ) )
		{
				OutputDebugString( TEXT( "<GeometryShader> FAILED CreateGeometryShader \n" ) );
				SafeRelease( pGSBlob );
				return  hr;
		}


		return  hr;
}


void  GeometryShader::UpdateShader()
{
		pd3dDeviceContext->GSSetShader( m_pGeometryShader, nullptr, 0 );
}


void  GeometryShader::Release()
{
		SafeRelease( m_pGeometryShader );
}