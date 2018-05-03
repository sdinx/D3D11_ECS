//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Component.h>
#include  <D3D11Utility\Entity.h>
#include  <D3D11Utility\System\ComponentManager.h>
#include  <D3D11Utility\System\EntityManager.h>


//----------------------------------------------------------------------------------
// defined
//----------------------------------------------------------------------------------
#define  STATIC_ID_INVALID  ( -1 )

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
// func: destructor
//----------------------------------------------------------------------------------
ComponentManager::~ComponentManager()
{
		ComponentManager::Release();
}


//----------------------------------------------------------------------------------
// func: AddComponent(const  EntityId, const  T*) : void
// 始めてテーブルに登録されるコンポーネントの場合,
// リストの末尾にIDを追加.
//----------------------------------------------------------------------------------
template<typename  T>
void  ComponentManager::AddComponent( const  EntityId  entityId, T*  component )
{
		// 初登録コンポーネントにstatic_idを割り当て
		if ( STATIC_ID_INVALID == component->GetStaticId() )
				component->SetStaticId( m_componentIdList.size() );

		// コンポーネントの追加
		m_componentTable[entityId.entityId].push_back( new  T( *component ) );

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
 // func: Release() : void
 //----------------------------------------------------------------------------------
void  ComponentManager::Release()
{
		m_componentIdList.clear();

		m_componentTable.clear();
		m_componentTable.shrink_to_fit();
}


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
						component->Update();
				}
}