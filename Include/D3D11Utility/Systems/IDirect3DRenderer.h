//----------------------------------------------------------------------------------
// file : IDirect3DRenderer.h
// desc : 
//----------------------------------------------------------------------------------


#ifndef  _INCLUDED_INTERFACE_D3D11_UTILITY_DIRECT3D_RENDERER_
#define  _INCLUDED_INTERFACE_D3D11_UTILITY_DIRECT3D_RENDERER_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Interface.h>
#include  <D3D11Utility\Systems\IDirect3D.h>
#include  <D3D11Utility\Systems\ISystem.h>
#include  <IGraphicsRenderer.h>
#include  <IRenderable.h>


namespace  D3D11Utility
{
		namespace  Systems
		{

				class  IDirect3DRenderer :public  IGraphicsRenderer,  public  ISystem
				{

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						IDirect3DRenderer();
						~IDirect3DRenderer();


				private:
						//----------------------------------------------------------------------------------
						// private  variables
						//----------------------------------------------------------------------------------

						static  SystemId  STATIC_SYSTEM_ID;
						std::shared_ptr<IDirect3D>  m_pID3D;
						ComponentManager*  m_componentManagerInstance;


				public:
						//----------------------------------------------------------------------------------
						// public  variables
						//----------------------------------------------------------------------------------
						/* NOTHING */

				private:
						//----------------------------------------------------------------------------------
						// private  methods
						//----------------------------------------------------------------------------------
						/* NOTHING */

				public:
						//----------------------------------------------------------------------------------
						// public  methods
						//----------------------------------------------------------------------------------

						static  void  SetStaticSystemId( SystemId  id )
						{
								STATIC_SYSTEM_ID = id;
						}
						static  SystemId  GetStaticSystemId()
						{
								return  STATIC_SYSTEM_ID;
						}
						SystemId  GetSystemId()const
						{
								return  STATIC_SYSTEM_ID;
						}
						VOID  Release();
						VOID  Update( float  ms )
						{
								Rendering();
						}
						VOID  Rendering()const;

				};// class IDirect3DRenderer

		}// namespace Systems
}// namespace D3D11Utility

#endif // ! _INCLUDED_INTERFACE_D3D11_UTILITY_DIRECT3D_RENDERER_