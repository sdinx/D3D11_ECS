//----------------------------------------------------------------------------------
// file : VertexBuffer.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_VERTEX_BUFFER_
#define  _INCLUDED_D3D11_UTILITY_VERTEX_BUFFER_

//----------------------------------------------------------------------------------
// pragma
//----------------------------------------------------------------------------------
#pragma comment(lib, "d3d11.lib")

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <d3d11.h>
#include  <DirectXMath.h>
#include  <D3D11Utility.h>


namespace  D3D11Utility
{

		namespace  Graphics
		{
				class  CVertexBuffer
				{
				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						CVertexBuffer() = delete;
						CVertexBuffer( VERTEX*  pVertices, UINT  numVertexCounts );
						CVertexBuffer( POLYGON_TYPE  polygonType );
						~CVertexBuffer();


				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------

						// �v���~�e�B�u�̎��
						D3D_PRIMITIVE_TOPOLOGY  primitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
						// ���_��
						UINT  m_numVertexCounts;
						// ���_�f�[�^
						VERTEX  *m_pVertices = NULL;
						// ���_�o�b�t�@
						ID3D11Buffer*  m_pVertexBuffer = NULL;
						UINT  m_nStride;
						UINT  m_nOffset = 0;


				public:
						//----------------------------------------------------------------------------------
						// public variables
						//----------------------------------------------------------------------------------


				private:
						//----------------------------------------------------------------------------------
						// private functions
						//----------------------------------------------------------------------------------


				public:
						//----------------------------------------------------------------------------------
						// public functions
						//----------------------------------------------------------------------------------

						// ���_�o�b�t�@�̐����Ɛݒ�
						HRESULT  CreateVertexBuffer();
						// �`����s���Ƃ��ɌĂяo��
						VOID  BindBuffer();
						VOID  Release();

				}; // class VertexBuffer

		}// namespace Graphics
}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_VERTEX_BUFFER_