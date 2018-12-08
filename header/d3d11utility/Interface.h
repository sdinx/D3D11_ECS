//----------------------------------------------------------------------------------
// file : Interface.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_INTERFACE_
#define  _INCLUDED_D3D11_UTILITY_INTERFACE_

//----------------------------------------------------------------------------------
// defined
//----------------------------------------------------------------------------------
#define  STATIC_ID_INVALID  ( -1 )


namespace  D3D11Utility
{

		namespace  Systems
		{

				//----------------------------------------------------------------------------------
				// ‘O’ñ’è‹`
				//----------------------------------------------------------------------------------
				class  IDirect3D;
				class  IDirect3DRenderer;
				class  ComponentManager;
				class  DebugSystem;
				class  EntityManager;
				class  FbxLoader;
				class  SystemManager;
				class  TextureManager;
				class  PhysicalSystem;
				class  BulletEngine;

		}// namespace Systems

		namespace  Graphics
		{

				enum eRasterMode
				{
						eNoneSolid,
						eBackSolid,
						eFrontSolid,
						eNoneWireframe,
						MaxRasterMode
				};// enum eRasterMode

				class  Material;
				using  MaterialId = int;

		}// namespace Graphics

		class  Entity;
		class  IEntity;
		class  Component;
		class  IComponent;
		class  GameObject;
		class  Transform;

}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_INTERFACE_