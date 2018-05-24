//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\Graphics\PixelShader.h>
#include  <D3D11Utility\Systems\IDirect3D.h>

//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------
#define  DEFAULT_SHADER  L"Shader/Default.fx"
#define  DEFAULT_FUNCTION  "PSFunc"
#define  DEFAULT_VERSION  "ps_5_0"

//----------------------------------------------------------------------------------
// using namespace
//----------------------------------------------------------------------------------
using namespace D3D11Utility;
using namespace Graphics;


PixelShader::PixelShader()
{
		CreatePixelShader( DEFAULT_SHADER );
}


PixelShader::PixelShader( ID3D11PixelShader*  pPS ) :
		m_pPixelShader( pPS )
{

}


PixelShader::~PixelShader()
{
		Release();
}


HRESULT  PixelShader::CreatePixelShader( LPCWSTR  szFileName )
{
		HRESULT  hr = S_OK;

		// 頂点シェーダのコンパイル
		ID3DBlob*  pPSBlob = nullptr;

		hr = CompileShaderFromFile( szFileName, DEFAULT_FUNCTION, DEFAULT_VERSION, &pPSBlob );
		if ( FAILED( hr ) )
		{
				OutputDebugString( TEXT( "<PixelShader> FAILED CompileShaderFromFile \n" ) );
				return  hr;
		}


		// 頂点シェーダーの生成
		hr = pd3dDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShader );
		if ( FAILED( hr ) )
		{
				OutputDebugString( TEXT( "<PixelShader> FAILED CreatePixelShader \n" ) );
				SafeRelease( pPSBlob );
				return  hr;
		}


		return  hr;
}


void  PixelShader::UpdateShader()
{
		pd3dDeviceContext->PSSetShader( m_pPixelShader, nullptr, 0 );
}


void  PixelShader::Release()
{
		SafeRelease( m_pPixelShader );
}