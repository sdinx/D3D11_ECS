//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility.h>
#include  <d3d11utility\Graphics\PixelShader.h>
#include  <d3d11utility\Systems\IDirect3D.h>
#include  <iostream>

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


PixelShader::PixelShader() :
		m_pPixelShader( nullptr ),
		m_pSamplerState( nullptr )
{
		CreatePixelShader( DEFAULT_SHADER );
		CreateSamplerState();
}


PixelShader::PixelShader( ID3D11PixelShader*  pPS ) :
		m_pPixelShader( pPS ),
		m_pSamplerState( nullptr )
{
		CreateSamplerState();
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


HRESULT  PixelShader::CreateSamplerState()
{
		HRESULT  hr = S_OK;

		// SamplerState作成
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory( &samplerDesc, sizeof( samplerDesc ) );
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		hr = pd3dDevice->CreateSamplerState( &samplerDesc, &m_pSamplerState );
		if ( FAILED( hr ) )
		{
				std::cout << "<PixelShader> CreateSamplerState() : Failed to create a sampler state.\n";
				return  hr;
		}

		return  hr;
}


void  PixelShader::UpdateShader()
{
		pd3dDeviceContext->PSSetShader( m_pPixelShader, nullptr, 0 );
		pd3dDeviceContext->PSSetSamplers( 0, 1, &m_pSamplerState );
}


void  PixelShader::Release()
{
		SafeRelease( m_pPixelShader );
}