//----------------------------------------------------------------------------------
// file : ComponentManager.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_COMPONENT_MANAGER_
#define  _INCLUDED_D3D11_UTILITY_COMPONENT_MANAGER_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\D3D11Utility.h>
#include  <vector>
#include  <list>


namespace  D3D11Utility
{
		//----------------------------------------------------------------------------------
		// 前提定義
		//----------------------------------------------------------------------------------
		class  Component;
		struct EntityId;

		//----------------------------------------------------------------------------------
		// type defined
		//----------------------------------------------------------------------------------
		using  ComponentId = UINT;
		using  ComponentIdList = std::list<ComponentId>;
		using  ComponentTable = std::vector<std::vector<Component>>;


		class  ComponentManager
		{
				// TODO: コンポーネントテーブルを用意する必要あり
		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				ComponentManager();

		private:
				//----------------------------------------------------------------------------------
				// private  variables
				//----------------------------------------------------------------------------------
				ComponentIdList  m_componentIdList;
				ComponentTable  m_componentTable;


		public:
				//----------------------------------------------------------------------------------
				// public  variables
				//----------------------------------------------------------------------------------
				/* NOTHING */

		private:
				//----------------------------------------------------------------------------------
				// private  functions
				//----------------------------------------------------------------------------------
				/* NOTHING */


		public:
				//----------------------------------------------------------------------------------
				// public  functions
				//----------------------------------------------------------------------------------
				template<typename  T>
				static  void  AddComponent( const  T*  component );
				template<typename  T>
				static  T*  GetComponent( const  EntityId  entityId, const  UINT  componentType );


		};// class  ComponentManager

}// namespace  D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_COMPONENT_MANAGER_