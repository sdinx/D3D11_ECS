//----------------------------------------------------------------------------------
// file: Texture1D.h
// desc: 1Dテクスチャの作成用クラス ( RTV はこれで作らない ).
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
/* NOTHING */

#ifndef  _INCLUDED_D3D11_UTILITY_TEXTURE_1D_
#define  _INCLUDED_D3D11_UTILITY_TEXTURE_1D_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <d3d11utility/Platform.h>
#include  <d3d11utility/Object.h>
#include  <d3d11.h>
#include  <memory>
#include  <wrl/client.h>

namespace  D3D11Utility
{
		namespace  Graphics
		{
				class  Texture1D :public  Object
				{

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						Texture1D() = delete;
						Texture1D( DXGI_FORMAT  format, uint  width, uint bindFlags = D3D11_BIND_SHADER_RESOURCE, bool isDynamic = 0 );
						virtual  ~Texture1D();

				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------

						Microsoft::WRL::ComPtr<ID3D11Texture1D>  m_pTextureResource;
						Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>  m_pUAV;
						Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  m_pSRV;

				public:
						//----------------------------------------------------------------------------------
						// public variables
						//----------------------------------------------------------------------------------
						/* NOTHING */

				private:
						//----------------------------------------------------------------------------------
						// private methods
						//----------------------------------------------------------------------------------
						/* NOTHING */

				public:
						//----------------------------------------------------------------------------------
						// public methods
						//----------------------------------------------------------------------------------

						/* getter */
						ID3D11Texture1D*  GetTextureResource() { return  m_pTextureResource.Get(); }
						ID3D11UnorderedAccessView*  GetUnorderedAccessView() { return  m_pUAV.Get(); }
						ID3D11ShaderResourceView*  GetShaderResourceView() { return  m_pSRV.Get(); }
						/* utilities */
						void  Release();

				};// class Texture1D

		}// namespace Graphics
}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_TEXTURE_1D_