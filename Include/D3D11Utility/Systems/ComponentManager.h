//----------------------------------------------------------------------------------
// file : ComponentManager.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_COMPONENT_MANAGER_
#define  _INCLUDED_D3D11_UTILITY_COMPONENT_MANAGER_

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
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

		using  EntityComponentTable = std::vector<std::vector<ComponentId>>;
		using  ComponentTable = std::vector<std::vector<Component*>>;


		namespace  Systems
		{

				class  ComponentManager
				{
						// note: �ꎞ�I�Ƀt�����h�N���X��
						friend  class  IDirect3DRenderer;

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

						EntityComponentTable  m_entityComponetIdTable;
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
						//! @func:     AddComponent( const  EntityId,  P&&... ) : void, template<T, P>
						//! @brief:     �n�߂ăe�[�u���ɓo�^�����R���|�[�l���g�̏ꍇ, ���X�g�̖�����ID��ǉ�.
						//! @param:     (param) �ǉ�����R���|�[�l���g�̃R���X�g���N�^�������w��.
						//! @note:     �\�ߑS�R���|�[�l���g�ɌŗLID������U��ƍ������ł��邩��.
						//----------------------------------------------------------------------------------
						template<class  T, typename  ...P>
						void  AddComponent( const  EntityId  entity, P&&... param )
						{
								const  UINT  entityId = entity.index;
								UINT  nComponentPosition = 0;
								ComponentId  componentId = T::GetStaticComponentId();
								
								// ���o�^�R���|�[�l���g��static_id�����蓖��
								if ( componentId == COMPONENT_ID_INVALID )
								{
										componentId = m_componentTable.size();
										T::SetStaticComponentId( componentId );
										m_componentTable.resize( componentId + 1 );

										std::cout << "<ComponentManager> Add new component : componentId = " << componentId << ", Type = " << typeid( T ).name() << std::endl;
								}

								// �R���|�[�l���g�̊i�[�����ʒu
								nComponentPosition = m_componentTable[componentId].size();

								// �R���|�[�l���gID�e�[�u���̃T�C�Y�g��
								if ( componentId >= ( int ) m_entityComponetIdTable[entityId].size() )
										m_entityComponetIdTable[entityId].resize( componentId + 1, COMPONENT_ID_INVALID );

								// ���ɃR���|�[�l���g�����݂��Ă���
								else  if ( m_entityComponetIdTable[entityId][componentId] != COMPONENT_ID_INVALID )
										return;

								m_entityComponetIdTable[entityId][componentId] = nComponentPosition;

								// �R���|�[�l���g����
								Component*  component = new  T( std::forward<P>( param )... );
								component->m_parentsEntityId = entity;
								component->m_pComponentManager = this;
								component->m_componentId = componentId;

								m_componentTable[componentId].push_back( component );
								// TODO: need to output debug string.


						}// end AddComponent(const EntityId, T*) : void

						//----------------------------------------------------------------------------------
						//! @func:     GetComponent( const EntityId& ) : T*
						//----------------------------------------------------------------------------------
						template<class  T>
						T*  GetComponent( const  EntityId  entity )
						{
								const  UINT  entityId = entity.index;
								const  ComponentId  componentId = T::GetStaticComponentId();

								// �G���e�B�e�B�ƃR���|�[�l���g�̑��݊m�F
								if ( m_entityComponetIdTable.size() > entityId && m_entityComponetIdTable[entityId].size() > ( UINT ) componentId )
								{
										const  INT  numComponents = m_entityComponetIdTable[entityId][componentId];
										if ( numComponents == COMPONENT_ID_INVALID )
										{
												std::cout << "<ComponentManager> numComponents invalid : entityId = " << entityId << ", componentId = " << componentId << std::endl;
												return  nullptr;
										}
										return  dynamic_cast< T* >( m_componentTable[componentId][numComponents] );
								}

								std::cout << "<ComponentManager> numComponents invalid : entityId = " << entityId << ", componentId = " << componentId << std::endl;
								return  nullptr;
						}// end GetComponent(const EntityId) : T*

						//----------------------------------------------------------------------------------
						//! @func:     GetComponent( const UINT ) : T*
						//! @brief:     �^�ƈʒu�̎w��ŃR���|�[�l���g���擾����
						//----------------------------------------------------------------------------------
						template<class  T>
						T*  GetComponent( const  UINT  numComponents )
						{
								const  ComponentId  componentId = T::GetStaticComponentId();

								if ( componentId != COMPONENT_ID_INVALID&& componentId < ( int ) m_componentTable.size() && numComponents < m_componentTable[componentId].size() )
										return  dynamic_cast< T* >( m_componentTable[componentId][numComponents] );

								return  nullptr;
						}// end GetComponent(const EntityId) : T*

						 //----------------------------------------------------------------------------------
						 //! @func:     RemoveComponent() : void
						 //! @brief:     �R���|�[�l���g�̉�����������łȂ�,
						 //! @brief:     �ꎞ�I�ɍX�V�R���|�[�l���g��`��R���|�[�l���g��,
						 //! @brief:     �O���ď������x�̉��P��}�����肷��.
						 //----------------------------------------------------------------------------------
						template<typename  T>
						void  RemoveComponent( const  EntityId  entity )
						{
								const  UINT  entityId = entityId.index;
								const  ComponentId  componentId = T::GetStaticComponentId();

								// �G���e�B�e�B�ƃR���|�[�l���g�̑��݊m�F
								if ( m_entityComponetIdTable.size() > entityId && m_entityComponetIdTable[entityId].size() > componentId )
								{
										const  UINT  numComponent = m_entityComponetIdTable[entityId][componentId];
										m_entityComponetIdTable[entityId][componentId] = COMPONENT_ID_INVALID;
										m_componentTable[componentId].erase( m_componentTable[componentId].begin() + numComponent );
								}

						}// end RemoveComponent(const  EntityId) : void

						//----------------------------------------------------------------------------------
						//! @func:     GetComponents() : std::vector<Component*>, template<T>
						//! @brief:     �w�肵���R���|�[�l���g�^�̔z���Ԃ�
						//! @TODO:     �z��𒼐ڕԂ����ɍs����悤�ɕύX����K�v������
						//----------------------------------------------------------------------------------
						template  <class  T>
						std::vector<Component*>  GetComponents()
						{
								ComponentId  componentId = T::GetStaticComponentId();

								if ( componentId == STATIC_ID_INVALID )
								{
										std::cout << "<ComponentManager> GetComponents failed." << std::endl;
								}

								return  m_componentTable[componentId];
						}

						void  AddEntity( const  EntityId  entity );
						void  Release();
						void  Update();


				};// class  ComponentManager

		}// namespace Systems
}// namespace  D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_COMPONENT_MANAGER_