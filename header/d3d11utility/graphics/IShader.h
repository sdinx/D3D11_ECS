//----------------------------------------------------------------------------------
// file: IShader.h
// desc: シェーダを管理する際に使うインターフェース  
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_GRAPHICS_INTERFACE_SHADER_
#define  _INCLUDED_D3D11_UTILITY_GRAPHICS_INTERFACE_SHADER_

//----------------------------------------------------------------------------------
// pragma
//----------------------------------------------------------------------------------
#pragma comment(lib, "d3d11.lib")

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Interface.h>
#include  <d3d11utility/Object.h>
#include  <d3d11.h>
#include  <string>

namespace  D3D11Utility
{

		namespace  Graphics
		{
				using  ShaderId = int;

				class  IShader :public  Object
				{
						friend  class  Systems::IDirect3DRenderer;

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------
						IShader()
						{}
						virtual  ~IShader()
						{}

				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------

						ShaderId  m_shaderId;
						uint  entryPointHash = 0;
						std::string  m_name;

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
						/* NOTHING */

				}; // class GeometryShader

		}// namespace Graphics
}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_GRAPHICS_INTERFACE_SHADER_