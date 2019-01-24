//--------------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------------
#include  <D3D11Utility.h>
#include  <d3d11utility\Graphics\Texture1D.h>


//--------------------------------------------------------------------------------------------------
// using  namespace
//--------------------------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  Graphics;


//--------------------------------------------------------------------------------------------------
//! @func:					constructor( DXGI_FORMAT, uint, uint, bool )
//! @param 1:		( format ) データのサイズフォーマットを指定.
//! @param 2:		( width ) データ数.
//! @param 3:		( bindFlags ) テクスチャの用途に合わせたフラグ.
//! @param 4:		( isDynamic ) CPU 側でデータを変更する場合に true を指定.
//--------------------------------------------------------------------------------------------------
Texture1D::Texture1D( DXGI_FORMAT  format, uint  width, uint bindFlags, bool isDynamic )
{
		CD3D11_TEXTURE1D_DESC texDesc(
				format,
				width,
				1, 0,
				bindFlags,
				isDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT,
				isDynamic ? D3D11_CPU_ACCESS_WRITE : 0,
				0
		);

		HRESULT hr = pd3dDevice->CreateTexture1D( &texDesc, nullptr, &m_pTextureResource );
		if ( FAILED( hr ) )
				printf( "<Texture1D> Failed constructor( DXGI_FORMAT, uint, uint, bool )" );

		if ( bindFlags & D3D11_BIND_UNORDERED_ACCESS )
				pd3dDevice->CreateUnorderedAccessView( m_pTextureResource.Get(), 0, &m_pUAV );

		if ( bindFlags & D3D11_BIND_SHADER_RESOURCE )
				pd3dDevice->CreateShaderResourceView( m_pTextureResource.Get(), 0, &m_pSRV );

}


Texture1D::~Texture1D()
{
		Release();
}


void  Texture1D::Release()
{

}