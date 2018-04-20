//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <Mesh.h>
#include  <D3D11Utility.h>
#include  <DirectXMath.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  Graphics;
using  namespace  DirectX;

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
CONSTANTBUFFER*  Mesh::s_pCBuffer = nullptr;

//----------------------------------------------------------------------------------
// struct
//----------------------------------------------------------------------------------
struct  ConstantBufferForPerFrame
{
		DirectX::XMMATRIX  world;
};


Mesh::Mesh( PRIMITIVE_TYPE  primitiveType )
{
		m_isDraw = TRUE;
		m_pVertexShader = new  CVertexShader();

		VERTEX  vertices[ ] = {
				Vector3( 1.0f,1.0f,1.0f ),
				Vector3( 2.0f,0.0f,1.0f ),
				Vector3( 0.0f,0.0f,1.0f ),
		};
		UINT  numVertices = ARRAYSIZE( vertices );

		m_pVertexBuffer = new  CVertexBuffer( vertices, numVertices );
}


VOID  Mesh::SetConstantBuffer()
{
		if ( s_pCBuffer == nullptr )
		{
				s_pCBuffer = new  CONSTANTBUFFER;
				CreateConstantBuffer( s_pCBuffer->pCB, s_pCBuffer->nCBSlot, sizeof( ConstantBufferForPerFrame ) );
		}
}


VOID  Mesh::Rendering()const
{
		m_pVertexShader->UpdateShader();

		ConstantBufferForPerFrame  cbuffer;
		cbuffer.world = XMLoadFloat4x4( &m_mtxWorld );
		pd3dDeviceContext->UpdateSubresource( s_pCBuffer->pCB, 0, nullptr, &cbuffer, 0, 0 );
		pd3dDeviceContext->VSSetConstantBuffers( s_pCBuffer->nCBSlot, 1, &s_pCBuffer->pCB );
		// pd3dDeviceContext->PSSetConstantBuffers( s_pCBuffer->nCBSlot, 1, &s_pCBuffer->pCB );

		m_pVertexBuffer->BindBuffer();
}