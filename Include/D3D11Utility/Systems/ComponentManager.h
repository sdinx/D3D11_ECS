//----------------------------------------------------------------------------------
// file : ComponentManager.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_COMPONENT_MANAGER_
#define  _INCLUDED_D3D11_UTILITY_COMPONENT_MANAGER_

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
// TODO: 自作ハッシュテーブルでキャッシュミスを減らしたアクセスの高速化が必要

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
						// note: 一時的にフレンドクラス化
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
						//! @brief:     始めてテーブルに登録されるコンポーネントの場合, リストの末尾にIDを追加.
						//! @param:     (param) 追加するコンポーネントのコンストラクタ引数を指定.
						//! @note:     予め全コンポーネントに固有IDを割り振ると高速化できるかも.
						//----------------------------------------------------------------------------------
						template<class  T, typename  ...P>
						void  AddComponent( const  EntityId  entity, P&&... param )
						{
								const  UINT  entityId = entity.index;
								UINT  nComponentPosition = 0;
								ComponentId  componentId = T::GetStaticComponentId();
								
								// 初登録コンポーネントにstatic_idを割り当て
								if ( componentId == COMPONENT_ID_INVALID )
								{
										componentId = m_componentTable.size();
										T::SetStaticComponentId( componentId );
										m_componentTable.resize( componentId + 1 );

										std::cout << "<ComponentManager> Add new component : componentId = " << componentId << ", Type = " << typeid( T ).name() << std::endl;
								}

								// コンポーネントの格納される位置
								nComponentPosition = m_componentTable[componentId].size();

								// コンポーネントIDテーブルのサイズ拡張
								if ( componentId >= ( int ) m_entityComponetIdTable[entityId].size() )
										m_entityComponetIdTable[entityId].resize( componentId + 1, COMPONENT_ID_INVALID );

								// 既にコンポーネントが存在している
								else  if ( m_entityComponetIdTable[entityId][componentId] != COMPONENT_ID_INVALID )
										return;

								m_entityComponetIdTable[entityId][componentId] = nComponentPosition;

								// コンポーネント生成
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

								// エンティティとコンポーネントの存在確認
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
						//! @brief:     型と位置の指定でコンポーネントを取得する
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
						 //! @brief:     コンポーネントの解放処理だけでなく,
						 //! @brief:     一時的に更新コンポーネントや描画コンポーネントを,
						 //! @brief:     外して処理速度の改善を図ったりする.
						 //----------------------------------------------------------------------------------
						template<typename  T>
						void  RemoveComponent( const  EntityId  entity )
						{
								const  UINT  entityId = entityId.index;
								const  ComponentId  componentId = T::GetStaticComponentId();

								// エンティティとコンポーネントの存在確認
								if ( m_entityComponetIdTable.size() > entityId && m_entityComponetIdTable[entityId].size() > componentId )
								{
										const  UINT  numComponent = m_entityComponetIdTable[entityId][componentId];
										m_entityComponetIdTable[entityId][componentId] = COMPONENT_ID_INVALID;
										m_componentTable[componentId].erase( m_componentTable[componentId].begin() + numComponent );
								}

						}// end RemoveComponent(const  EntityId) : void

						//----------------------------------------------------------------------------------
						//! @func:     GetComponents() : std::vector<Component*>, template<T>
						//! @brief:     指定したコンポーネント型の配列を返す
						//! @TODO:     配列を直接返さずに行えるように変更する必要がある
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