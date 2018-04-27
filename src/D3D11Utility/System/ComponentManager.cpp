//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <D3D11Utility\System\ComponentManager.h>
#include  <D3D11Utility\Component.h>
#include  <D3D11Utility\Entity.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;

//----------------------------------------------------------------------------------
// func: default contstructor
// 
//----------------------------------------------------------------------------------
ComponentManager::ComponentManager()
{

}


//----------------------------------------------------------------------------------
// func: AddComponent(const  T*)
// �n�߂ăe�[�u���ɓo�^�����R���|�[�l���g�̏ꍇ,
// ���X�g�̖�����ID��ǉ�
//----------------------------------------------------------------------------------
template<typename  T>
void  ComponentManager::AddComponent( const  T*  component )
{

}// end ComponentManager(const T*)


//----------------------------------------------------------------------------------
// func: GetComponent(const EntityId&, const UINT) : T*
// �R���|�[�l���g�̑��݂𔻒肵�Ă��邪, 
// TODO: ���x����̈�, Release�ł͔�r���Ȃ�����
//----------------------------------------------------------------------------------
template<typename  T>
T*  ComponentManager::GetComponent( const  EntityId  entityId, const  UINT  componentType )
{
		if ( m_componentTable.size() > entityId.entityId && m_componentTable[entityId.entityId].size() > componentType )
		{
				return  m_componentTable[entityId][componentType];
		}

		return  nullptr;
}// end ComponentManager(const EntityId&, const UINT) : T*