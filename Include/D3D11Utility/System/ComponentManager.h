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
#include  <D3D11Utility\Component.h>
#include  <D3D11Utility\IEntity.h>
#include  <D3D11Utility\D3D11Utility.h>
#include  <vector>
#include  <list>


namespace  D3D11Utility
{

		//----------------------------------------------------------------------------------
		// type defined
		//----------------------------------------------------------------------------------
		using  ComponentId = int;
		using  ComponentIdList = std::vector<ComponentId>;
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
				ComponentIdList  m_componentIdList;
				ComponentTable  m_componentTable;


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

				//----------------------------------------------------------------------------------
				// func: AddComponent( const  EntityId,  P&&... ) : void, template<T, P>
				// brief: �n�߂ăe�[�u���ɓo�^�����R���|�[�l���g�̏ꍇ, ���X�g�̖�����ID��ǉ�.
				// param: (param) �ǉ�����R���|�[�l���g�̃R���X�g���N�^�������w��.
				// note: �\�ߑS�R���|�[�l���g�ɌŗLID������U��ƍ������ł��邩��.
				//----------------------------------------------------------------------------------
				template<class  T, typename  ...P>
				void  AddComponent( const  EntityId  entityId, P&&... param )
				{
						// ���o�^�R���|�[�l���g��static_id�����蓖��
						if ( STATIC_ID_INVALID == T::STATIC_COMPONENT_ID )
						{
								T::STATIC_COMPONENT_ID = m_componentIdList.size();
								m_componentIdList.push_back( T::STATIC_COMPONENT_ID );
						}

						// �R���|�[�l���g�̒ǉ�
						m_componentTable[entityId.entityId].push_back( new  T( std::forward<P>( param )... ) );

				}// end AddComponent(const EntityId, T*) : void

				//----------------------------------------------------------------------------------
				// func: GetComponent( const EntityId& ) : T*
				// note: ���x����̈�, Release�ł͑��݊m�F���Ȃ�����.
				//----------------------------------------------------------------------------------
				template<class  T>
				T*  GetComponent( const  EntityId  entityId )
				{
						UINT  componentType = ( UINT ) T::STATIC_COMPONENT_ID;

						// �R���|�[�l���g�ƃG���e�B�e�B�̑��݊m�F
						if ( componentType != ( UINT ) STATIC_ID_INVALID && m_componentTable.size() > entityId.entityId && m_componentTable[entityId.entityId].size() > ( UINT ) m_componentIdList[componentType] )
								return  dynamic_cast< T* >( m_componentTable[entityId.entityId][m_componentIdList[componentType]] );

						return  nullptr;
				}// end GetComponent(const EntityId) : T*

				 //----------------------------------------------------------------------------------
				 // func: RemoveComponent() : void
				 // �R���|�[�l���g�̉�����������łȂ�,
				 // �ꎞ�I�ɍX�V�R���|�[�l���g��`��R���|�[�l���g��,
				 // �O���ď������x�̉��P��}�����肷��.
				 //----------------------------------------------------------------------------------
				template<typename  T>
				void  RemoveComponent( const  EntityId  entityId )
				{

				}// end RemoveComponent(const  EntityId) : void

				void  AddEntity( const  EntityId  entityId );
				void  Release();
				void  Update();


		};// class  ComponentManager

}// namespace  D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_COMPONENT_MANAGER_