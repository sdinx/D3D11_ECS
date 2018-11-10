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
				class  EntityManager;
				class  SystemManager;
				class  TextureManager;
				class  PhysicalSystem;
				class  BulletEngine;

		}// namespace Systems

		class  Entity;
		class  IEntity;
		class  Component;
		class  IComponent;
		class  GameObject;
		class  Transform;

}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_INTERFACE_