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


void  Texture:: SetTexture()
{
		pd3dDeviceContext->PSSetShaderResources( 0, 1, &m_SRView );
}// end SetTexture() : void


void  Texture::Release()
{

}// end Release() : void