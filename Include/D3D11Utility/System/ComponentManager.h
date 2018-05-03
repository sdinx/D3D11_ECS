//----------------------------------------------------------------------------------
// file : ComponentManager.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_COMPONENT_MANAGER_
#define  _INCLUDED_D3D11_UTILITY_COMPONENT_MANAGER_

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
// TODO: �R���|�[�l���g�e�[�u����p�ӂ���K�v����
// TODO: ����n�b�V���e�[�u���ŃL���b�V���~�X�����炵���A�N�Z�X�̍��������K�v

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\D3D11Utility.h>
#include  <vector>
#include  <list>


namespace  D3D11Utility
{
		//----------------------------------------------------------------------------------
		// �O���`
		//----------------------------------------------------------------------------------
		class  Component;
		struct  EntityId;

		//----------------------------------------------------------------------------------
		// type defined
		//----------------------------------------------------------------------------------
		using  ComponentId = int;
		using  ComponentIdList = std::list<ComponentId>;
		using  ComponentTable = std::vector<std::vector<Component*>>;


		class  ComponentManager
		{

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				ComponentManager();
				virtual  ~ComponentManager();

		private:
				//----------------------------------------------------------------------------------
				// private  variables
				//----------------------------------------------------------------------------------
				static  ComponentIdList  m_componentIdList;
				static  ComponentTable  m_componentTable;


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
				template<typename  T>
				static  void  AddComponent( const  EntityId  entityId, T*  component );
				template<typename  T>
				static  T*  GetComponent( const  EntityId  entityId, const  UINT  componentType );
				template<typename  T>
				static  void  RemoveComponent();
				static  void  Release();
				static  void  Update();


		};// class  ComponentManager

}// namespace  D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_COMPONENT_MANAGER_