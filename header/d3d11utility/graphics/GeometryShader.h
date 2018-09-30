//----------------------------------------------------------------------------------
// file : GeometryShader.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_GRAPHICS_GEOMETRY_SHADER_
#define  _INCLUDED_D3D11_UTILITY_GRAPHICS_GEOMETRY_SHADER_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Graphics\IShader.h>


namespace  D3D11Utility
{

		namespace  Graphics
		{
				class  GeometryShader :public  IShader
				{
				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------
						GeometryShader();
						GeometryShader( ID3D11GeometryShader*  pGS );
						~GeometryShader();

				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------
						ID3D11GeometryShader*  m_pGeometryShader = nullptr;


				public:
						//----------------------------------------------------------------------------------
						// public variables
						//----------------------------------------------------------------------------------


				private:
						//----------------------------------------------------------------------------------
						// private methods
						//----------------------------------------------------------------------------------


				public:
						//----------------------------------------------------------------------------------
						// public methods
						//----------------------------------------------------------------------------------

						// �V�F�[�_�̃R���p�C���ƒ��_�V�F�[�_�̍쐬
						HRESULT  CreateGeometryShader( LPCWSTR  szFileName = nullptr );
						// �`����s���O�ɌĂяo��
						void  UpdateShader();
						void  Release();

				}; // class GeometryShader

		}// namespace Graphics

}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_GRAPHICS_GEOMETRY_SHADER_