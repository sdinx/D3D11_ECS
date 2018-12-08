//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <d3d11utility/Graphics/Material.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  D3D11Utility::Graphics;
using  namespace  DirectX;

//----------------------------------------------------------------------------------
// static  variables
//----------------------------------------------------------------------------------
const  UINT  Material::s_nConstantBufferSlot;
ID3D11Buffer*  Material::s_pConstantBuffer = nullptr;


Material::Material() :
		materialId( -1 ),
		ambient( 1, 1, 1 ),
		diffuse( 1, 1, 1 ),
		specular( 1, 1, 1, 0.3f ),
		emissive( 0, 0, 0, 0 )
{

}


Material::Material( const  Material&  mat ) :
		materialId( mat.materialId ),
		ambient( mat.ambient ),
		diffuse( mat.diffuse ),
		specular( mat.specular ),
		emissive( mat.emissive )
{

}


Material::Material( const  MaterialId  _id ) :
		materialId( _id ),
		ambient( 1, 1, 1 ),
		diffuse( 1, 1, 1 ),
		specular( 1, 1, 1, 0.3f ),
		emissive( 0, 0, 0, 0 )
{

}


Material::Material( Vector3  _ambient, Vector3  _diffuse, Vector4  _specular, Vector4  _emissive ) :
		materialId( -1 ),
		ambient( _ambient ),
		diffuse( _diffuse ),
		specular( _specular ),
		emissive( _emissive )
{

}


Material::Material( const  MaterialId  _id, Vector3  _ambient, Vector3  _diffuse, Vector4  _specular, Vector4  _emissive ) :
		materialId( _id ),
		ambient( _ambient ),
		diffuse( _diffuse ),
		specular( _specular ),
		emissive( _emissive )
{

}


Material::~Material()
{

}


void  Material::SetConstantBuffer()
{
		if ( s_pConstantBuffer == nullptr )
		{
				CreateConstantBuffer( s_pConstantBuffer, sizeof( CBufferMaterial ) );
		}
}


void  Material::UpdateConstantBuffer()
{
		m_cbuffer.g_ambient = ambient;
		m_cbuffer.g_diffuse = diffuse;
		m_cbuffer.g_specular = specular;
		m_cbuffer.g_emissive = emissive;

		pd3dDeviceContext->UpdateSubresource( s_pConstantBuffer, 0, nullptr, &m_cbuffer, 0, 0 );
		pd3dDeviceContext->VSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
		pd3dDeviceContext->GSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
		pd3dDeviceContext->PSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
}