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
// �n�߂ăe�[�u���ɓo�^�����R���|�[�l���g�̏ꍇ,
// ���X�g�̖�����ID��ǉ�.
//----------------------------------------------------------------------------------
template<typename  T>
void  ComponentManager::AddComponent( const  EntityId  entityId, T*  component )
{
		// ���o�^�R���|�[�l���g��static_id�����蓖��
		if ( STATIC_ID_INVALID == component->GetStaticId() )
				component->SetStaticId( m_componentIdList.size() );

		// �R���|�[�l���g�̒ǉ�
		m_componentTable[entityId.entityId].push_back( new  T( *component ) );

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
// Entity�ɓo�^����Ă���R���|�[�l���g���X�V����
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