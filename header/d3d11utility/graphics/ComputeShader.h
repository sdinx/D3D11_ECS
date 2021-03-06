//----------------------------------------------------------------------------------
// file : ComputeShader.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_GRAPHICS_COMPUTE_SHADER_
#define  _INCLUDED_D3D11_UTILITY_GRAPHICS_COMPUTE_SHADER_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Graphics\IShader.h>

namespace  D3D11Utility
{

		namespace  Graphics
		{
				class  ComputeShader :public  IShader
				{
				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------
						ComputeShader() = delete;
						ComputeShader( ID3D11ComputeShader*  pCS );
						~ComputeShader();

				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------
						ID3D11ComputeShader*  m_pComputeShader;

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

						// 描画を行う前に呼び出す
						void  UpdateShader();
						void  Release();

				}; // class ComputeShader

		}// namespace Graphics

}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_GRAPHICS_COMPUTE_SHADER_