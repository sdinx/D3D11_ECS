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
				// func: AddComponent(const  EntityId, const  T*) : void
				// 始めてテーブルに登録されるコンポーネントの場合,
				// リストの末尾にIDを追加.
				// TODO: 予め全コンポーネントに固有IDを割り振ると高速化できるかも
				//----------------------------------------------------------------------------------
				template<typename  T>
				void  AddComponent( const  EntityId  entityId, T*  component )
				{
						// 初登録コンポーネントにstatic_idを割り当て
						if ( STATIC_ID_INVALID == component->GetStaticId() )
						{
								component->SetStaticId( m_componentIdList.size() );
								m_componentIdList.push_back( component->GetStaticId() );
						}

						// コンポーネントの追加
						m_componentTable[entityId.entityId].push_back( new  T( *component ) );

				}// end AddComponent(const EntityId, T*) : void

				//----------------------------------------------------------------------------------
				// func: GetComponent(const EntityId&, const UINT) : T*
				// コンポーネントの存在を判定しているが, 
				// TODO: 速度向上の為, Releaseでは比較しないかも.
				//----------------------------------------------------------------------------------
				template<typename  T>
				T*  GetComponent( const  EntityId  entityId )
				{
						UINT  componentType = ( UINT ) T::STATIC_COMPONENT_ID;

						// コンポーネントとエンティティの存在確認
						if ( componentType != ( UINT ) STATIC_ID_INVALID && m_componentTable.size() > entityId.entityId && m_componentTable[entityId.entityId].size() > ( UINT ) m_componentIdList[componentType] )
								return  dynamic_cast< T* >( m_componentTable[entityId.entityId][m_componentIdList[componentType]] );

						return  nullptr;
				}// end GetComponent(const EntityId) : T*

				 //----------------------------------------------------------------------------------
				 // func: RemoveComponent() : void
				 // コンポーネントの解放処理だけでなく,
				 // 一時的に更新コンポーネントや描画コンポーネントを,
				 // 外して処理速度の改善を図ったりする.
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