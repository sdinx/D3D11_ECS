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
// �R���|�[�l���g�̑��݂𔻒肵�Ă��邪, 
// ���x����̈ה�r����K�v�͂Ȃ�
//----------------------------------------------------------------------------------
template<typename  T>
T*  ComponentManager::GetComponent( const  EntityId&  entityId, const  UINT  componentType )
{
		// �G���e�B�e�BID�͑��݂���O��Ȃ̂Ŕ�r�͂��Ȃ�
		if ( m_componentTable[entityId.entityId].size() > componentType )
		{
				return  m_componentTable[entityId][componentType];
		}

		return  nullptr;
}// end ComponentManager(const EntityId&, const UINT) : T*