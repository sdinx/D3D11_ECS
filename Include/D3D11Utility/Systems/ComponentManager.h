//----------------------------------------------------------------------------------
// file : ComponentManager.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_COMPONENT_MANAGER_
#define  _INCLUDED_D3D11_UTILITY_COMPONENT_MANAGER_

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
// TODO: コンポーネントテーブルを用意する必要あり
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

		using  ComponentId = int;
		using  ComponentIdList = std::vector<ComponentId>;
		using  ComponentTable = std::vector<std::vector<Component*>>;


		namespace  Systems
		{

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
						// brief: 始めてテーブルに登録されるコンポーネントの場合, リストの末尾にIDを追加.
						// param: (param) 追加するコンポーネントのコンストラクタ引数を指定.
						// note: 予め全コンポーネントに固有IDを割り振ると高速化できるかも.
						//----------------------------------------------------------------------------------
						template<class  T, typename  ...P>
						void  AddComponent( const  EntityId  entityId, P&&... param )
						{
								// 初登録コンポーネントにstatic_idを割り当て
								if ( STATIC_ID_INVALID == T::STATIC_COMPONENT_ID )
								{
										T::STATIC_COMPONENT_ID = m_componentIdList.size();
										m_componentIdList.push_back( T::STATIC_COMPONENT_ID );
								}

								// コンポーネントの追加
								m_componentTable[entityId.entityId].push_back( new  T( std::forward<P>( param )... ) );
								T*  component = dynamic_cast< T* >( m_componentTable[entityId.entityId].back() );
								component->m_parentsEntityId = entityId;
								component->m_managerInstance = this;

						}// end AddComponent(const EntityId, T*) : void

						//----------------------------------------------------------------------------------
						// func: GetComponent( const EntityId& ) : T*
						// note: 速度向上の為, Releaseでは存在確認しないかも.
						// note: 型チェックのために動的キャストにしているが静的キャストにするかもしれない.
						//----------------------------------------------------------------------------------
						template<class  T>
						T*  GetComponent( const  EntityId  entityId )
						{
								UINT  componentType = ( UINT ) T::STATIC_COMPONENT_ID;

								// エンティティの存在確認
								if ( m_componentTable.size() > entityId.entityId )
								{
										// コンポーネントの存在確認
										for ( auto component : m_componentTable[entityId.entityId] )
												if ( component->GetStaticId() == T::STATIC_COMPONENT_ID )
												{
														return  dynamic_cast< T* >( component );
												}
								}

								return  nullptr;
						}// end GetComponent(const EntityId) : T*

						 //----------------------------------------------------------------------------------
						 // func: RemoveComponent() : void
						 // brief: コンポーネントの解放処理だけでなく,
						 // brief: 一時的に更新コンポーネントや描画コンポーネントを,
						 // brief: 外して処理速度の改善を図ったりする.
						 //----------------------------------------------------------------------------------
						template<typename  T>
						void  RemoveComponent( const  EntityId  entityId )
						{
								UINT  componentType = ( UINT ) T::STATIC_COMPONENT_ID;
								UINT  nFind = 0;

								// エンティティの存在確認
								if ( m_componentTable.size() > entityId.entityId )
								{
										// コンポーネントの存在確認
										for ( auto component : m_componentTable[entityId.entityId] )
												if ( component->GetStaticId() == T::STATIC_COMPONENT_ID )
												{
														m_componentTable[entityId.entityId].erase( m_componentTable[entityId.entityId].begin() + nFind );
														break;
												}
										nFind++;
								}
						}// end RemoveComponent(const  EntityId) : void

						void  AddEntity( const  EntityId  entityId );
						void  Release();
						void  Update();


				};// class  ComponentManager

		}// namespace Systems
}// namespace  D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_COMPONENT_MANAGER_