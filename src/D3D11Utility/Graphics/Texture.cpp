//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\Graphics\Texture.h>


//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  Graphics;


Texture::Texture( ID3D11Texture2D*  texture, ID3D11ShaderResourceView*  shaderResourceView, ID3D11SamplerState*  sampler ) :
		m_texture( texture ),
		m_shaderResourceView( shaderResourceView ),
		m_sampler( sampler )
{

}


Texture::~Texture()
{

}


void  Texture::SetTexture()
{
		pd3dDeviceContext->PSSetShaderResources( 0, 1, &m_shaderResourceView );
		// pd3dDeviceContext->PSSetSamplers( 0, 1, &m_sampler );
}// end SetTexture() : void


void  Texture::Release()
{

}// end Release() : void