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
// �n�߂ăe�[�u���ɓo�^�����R���|�[�l���g�̏ꍇ,
// ���X�g�̖�����ID��ǉ�.
//----------------------------------------------------------------------------------
template<typename  T>
void  ComponentManager::AddComponent( const  EntityId  entityId, const  T*  component )
{
		ComponentId id;
		std::size_t  listSize = m_componentIdList.size();

		// ID �̑��݊m�F�݂̂Ȃ̂ŋ󃋁[�v�ŉ񂵂Ă�
		for ( id : m_componentIdList; id != T::STATIC_COMPONENT_ID )
		{
				/* NOTHING */
		}// end for

		// ID���X�g�̃��T�C�Y
		if ( id == listSize )
				m_componentIdList.push_back( id );

		// �R���|�[�l���g�̒ǉ�
		m_componentTable[entityId.entityId].push_back( T );

}// end AddComponent(const T*) : void


//----------------------------------------------------------------------------------
// func: GetComponent(const EntityId&, const UINT) : T*
// �R���|�[�l���g�̑��݂𔻒肵�Ă��邪, 
// TODO: ���x����̈�, Release�ł͔�r���Ȃ�����.
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
// �R���|�[�l���g�̉�����������łȂ�,
// �ꎞ�I�ɍX�V�R���|�[�l���g��`��R���|�[�l���g��,
// �O���ď������x�̉��P��}�����肷��.
//----------------------------------------------------------------------------------
template<typename  T>
void  ComponentManager::RemoveComponent()
{

}// end RemoveComponent() : void


//----------------------------------------------------------------------------------
// func: Update() : void
// Entity�ɓo�^����Ă���R���|�[�l���g���X�V����
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