//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\Graphics\VertexShader.h>
#include  <D3D11Utility\Systems\IDirect3D.h>

//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------
#define  DEFAULT_SHADER  L"Shader/Default.fx"
#define  DEFAULT_FUNCTION  "VSFunc"
#define  DEFAULT_VERSION  "vs_5_0"

//----------------------------------------------------------------------------------
// using namespace
//----------------------------------------------------------------------------------
using namespace D3D11Utility;
using namespace Graphics;


VertexShader::VertexShader()
{
		CreateVertexShader( DEFAULT_SHADER );
}


VertexShader::VertexShader( ID3D11VertexShader*  pVS, ID3D11InputLayout*  pIL ) :
		m_pVertexShader( pVS ),
		m_pInputLayout( pIL )
{

}


VertexShader::~VertexShader()
{
		Release();
}


HRESULT  VertexShader::CreateVertexShader( LPCWSTR  szFileName )
{
		HRESULT  hr = S_OK;

		// 頂点シェーダのコンパイル
		ID3DBlob*  pVSBlob = nullptr;

		hr = CompileShaderFromFile( szFileName, DEFAULT_FUNCTION, DEFAULT_VERSION, &pVSBlob );
		if ( FAILED( hr ) )
		{
				OutputDebugString( TEXT( "<VertexShader> FAILED CompileShaderFromFile \n" ) );
				return  hr;
		}


		// 頂点シェーダーの生成
		hr = pd3dDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVertexShader );
		if ( FAILED( hr ) )
		{
				OutputDebugString( TEXT( "<VertexShader> FAILED CreateVertexShader \n" ) );
				SafeRelease( pVSBlob );
				return  hr;
		}


		// 入力レイアウト生成
		hr = CreateInputLayout( pVSBlob );
		SafeRelease( pVSBlob );
		if ( FAILED( hr ) )
		{
				OutputDebugString( TEXT( "<VertexShader> FAILED CreateInputLayout \n" ) );
				return  hr;
		}


		return  hr;
}


HRESULT  VertexShader::CreateInputLayout( ID3DBlob* pVSBlob )
{
		HRESULT  hr = S_OK;


		// 入力レイアウトの定義
		D3D11_INPUT_ELEMENT_DESC  layout[ ] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT  numElements = ARRAYSIZE( layout );

		hr = pd3dDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pInputLayout );


		return  hr;
}


void  VertexShader::UpdateShader()
{
		pd3dDeviceContext->VSSetShader( m_pVertexShader, nullptr, 0 );

		// 入力アセンブラに入力レイアウトを設定
		pd3dDeviceContext->IASetInputLayout( m_pInputLayout );
}


void  VertexShader::Release()
{
		SafeRelease( m_pVertexShader );
		SafeRelease( m_pInputLayout );
}