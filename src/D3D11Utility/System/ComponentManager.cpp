//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <D3D11Utility\System\ComponentManager.h>
#include  <D3D11Utility\Component.h>
#include  <D3D11Utility\Entity.h>,

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;


//----------------------------------------------------------------------------------
// func: AddComponent(const  T*)
// 
//----------------------------------------------------------------------------------
template<typename  T>
void  ComponentManager::AddComponent( const  T*  component )
{

}// end ComponentManager(const T*)


//----------------------------------------------------------------------------------
// func: GetComponent(const EntityId&, const UINT) : T*
// コンポーネントの存在を判定しているが, 
// 速度向上の為比較する必要はない
//----------------------------------------------------------------------------------
template<typename  T>
T*  ComponentManager::GetComponent( const  EntityId&  entityId, const  UINT  componentType )
{
		// エンティティIDは存在する前提なので比較はしない
		if ( m_componentTable[entityId.entityId].size() > componentType )
		{
				return  m_componentTable[entityId][componentType];
		}

		return  nullptr;
}// end ComponentManager(const EntityId&, const UINT) : T*