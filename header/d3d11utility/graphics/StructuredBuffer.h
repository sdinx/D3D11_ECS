//----------------------------------------------------------------------------------
// file : StrucuturedBuffer.h
// desc : 構造体バッファ作成用のクラス.
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_STRUCTURED_BUFFER_
#define  _INCLUDED_D3D11_UTILITY_STRUCTURED_BUFFER_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <d3d11utility/Platform.h>
#include  <d3d11.h>
#include  <memory>
#include  <wrl/client.h>
#include  <../shader/header/ShaderDefines.h>

namespace  D3D11Utility
{
		namespace  Graphics
		{

				template<class  T>
				class  StructuredBuffer  final
				{

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------
						StructuredBuffer() = delete;
						StructuredBuffer( const  int  nDataCounts, uint  bindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE, bool  dynamic = false );
						~StructuredBuffer();

				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------
						const  int  m_nDataCounts;
						Microsoft::WRL::ComPtr<ID3D11Buffer>  m_pBuffer;
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
						int  GetDataCounts() { return  m_nDataCounts; }
						ID3D11Buffer*  GetBuffer() { return  m_pBuffer.Get(); }
						ID3D11UnorderedAccessView*  GetUnorderedAccessView() { return  m_pUAV.Get(); }
						ID3D11ShaderResourceView*  GetShaderResourceView() { return  m_pSRV.Get(); }

						/* utilities */
						T*  Map();
						void  Unmap();
						void  Release();

				};// class  StructuredBuffer

				template<class  T>
				StructuredBuffer<T>::StructuredBuffer( const  int  nDataCounts, uint  bindFlags, bool  dynamic ) :
						m_nDataCounts( nDataCounts )
				{
						CD3D11_BUFFER_DESC  desc(
								sizeof( T )*nDataCounts,
								bindFlags,
								dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT,
								dynamic ? D3D11_CPU_ACCESS_WRITE : 0,
								D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
								sizeof( T )
						);

						pd3dDevice->CreateBuffer( &desc, 0, &m_pBuffer );

						if ( bindFlags & D3D11_BIND_UNORDERED_ACCESS )
								pd3dDevice->CreateUnorderedAccessView( m_pBuffer.Get(), 0, &m_pUAV );

						if ( bindFlags & D3D11_BIND_SHADER_RESOURCE )
								pd3dDevice->CreateShaderResourceView( m_pBuffer.Get(), 0, &m_pSRV );

				}


				template<class  T>
				StructuredBuffer<T>::~StructuredBuffer()
				{
						Release();
				}


				template<class  T>
				T*  StructuredBuffer<T>::Map()
				{
						D3D11_MAPPED_SUBRESOURCE  mapRes;
						pd3dDeviceContext->Map( m_pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes );

						return  static_cast< T* >( mapRes.pData );
				}


				template<class  T>
				void  StructuredBuffer<T>::Unmap()
				{
						pd3dDeviceContext->Unmap( m_pBuffer.Get(), 0 );
				}


				template<class  T>
				void  StructuredBuffer<T>::Release()
				{

				}

		}// namespace  Graphics
}// namespace  D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_STRUCTURED_BUFFER_