//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <D3D11Utility\System\ComponentManager.h>
#include  <D3D11Utility\System\EntityManager.h>
#include  <D3D11Utility\Component.h>
#include  <D3D11Utility\Entity.h>


//----------------------------------------------------------------------------------
// using namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;


//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
ComponentIdList  ComponentManager::m_componentIdList;
ComponentTable  ComponentManager::m_componentTable;


//----------------------------------------------------------------------------------
// func: default contstructor
// 
//----------------------------------------------------------------------------------
ComponentManager::ComponentManager()
{

}


//----------------------------------------------------------------------------------
// func: AddComponent(const  EntityId, const  T*) : void
// 始めてテーブルに登録されるコンポーネントの場合,
// リストの末尾にIDを追加.
//----------------------------------------------------------------------------------
template<typename  T>
void  ComponentManager::AddComponent( const  EntityId  entityId, const  T*  component )
{
		ComponentId id;
		std::size_t  listSize = m_componentIdList.size();

		// ID の存在確認のみなので空ループで回してる
		for ( id : m_componentIdList; id != T::STATIC_COMPONENT_ID )
		{
				/* NOTHING */
		}// end for

		// IDリストのリサイズ
		if ( id == listSize )
				m_componentIdList.push_back( id );

		// コンポーネントの追加
		m_componentTable[entityId.entityId].push_back( T );

}// end AddComponent(const T*) : void


//----------------------------------------------------------------------------------
// func: GetComponent(const EntityId&, const UINT) : T*
// コンポーネントの存在を判定しているが, 
// TODO: 速度向上の為, Releaseでは比較しないかも.
//----------------------------------------------------------------------------------
template<typename  T>
T*  ComponentManager::GetComponent( const  EntityId  entityId, const  UINT  componentType )
{
		if ( m_componentTable.size() > entityId.entityId && m_componentTable[entityId.entityId].size() > m_componentIdList[componentType] )
				return  m_componentTable[entityId][m_componentIdList[componentType]];

		return  nullptr;
}// end GetComponent(const EntityId&, const UINT) : T*


//----------------------------------------------------------------------------------
// func: RemoveComponent() : void
// コンポーネントの解放処理だけでなく,
// 一時的に更新コンポーネントや描画コンポーネントを,
// 外して処理速度の改善を図ったりする.
//----------------------------------------------------------------------------------
template<typename  T>
void  ComponentManager::RemoveComponent()
{

}// end RemoveComponent() : void


//----------------------------------------------------------------------------------
// func: Update() : void
// Entityに登録されているコンポーネントを更新する
//----------------------------------------------------------------------------------
void  ComponentManager::Update()
{
		UINT  i = 0;
		UINT  listSize = m_componentTable.size();

		for ( ; i < listSize; i++ )
				for ( auto& component : m_componentTable[i] )
				{
						component.Update();
				}
}