//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility.h>
#include  <d3d11utility\Graphics\ComputeShader.h>
#include  <d3d11utility\Systems\IDirect3D.h>
#include  <iostream>

//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------
#define  DEFAULT_FUNCTION  "main"
#define  DEFAULT_VERSION  "cs_5_0"

//----------------------------------------------------------------------------------
// using namespace
//----------------------------------------------------------------------------------
using namespace D3D11Utility;
using namespace Graphics;


ComputeShader::ComputeShader( ID3D11ComputeShader*  pCS ) :
		m_pComputeShader( pCS )
{

}


ComputeShader::~ComputeShader()
{
		Release();
}


void  ComputeShader::UpdateShader()
{
		pd3dDeviceContext->CSSetShader( m_pComputeShader, nullptr, 0 );
}


void  ComputeShader::Release()
{
		SafeRelease( m_pComputeShader );
}