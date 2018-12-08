//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility.h>
#include  <d3d11utility\Graphics\Texture.h>


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
		Release();
}


void  Texture::SetTexture( uint  slot )
{
		pd3dDeviceContext->PSSetShaderResources( slot, 1, &m_shaderResourceView );
		pd3dDeviceContext->PSSetSamplers( slot, 1, &m_sampler );
}// end SetTexture() : void


void  Texture::Release()
{
		SafeRelease( m_sampler );
		SafeRelease( m_texture );
		SafeRelease( m_shaderResourceView );
}// end Release() : void