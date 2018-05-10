//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Systems\ComponentManager.h>


//----------------------------------------------------------------------------------
// defined
//----------------------------------------------------------------------------------
#define  STATIC_ID_INVALID  ( -1 )

//----------------------------------------------------------------------------------
// using namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  D3D11Utility::Systems;


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
		Release();
}


//----------------------------------------------------------------------------------
// func: AddEntity(const  EntityId) : void
// brief: �R���|�l���g�e�[�u���̃T�C�Y����ǉ�����
// brief: EntityList��ComponentTable�̗v�f���͋��ʂ���
//----------------------------------------------------------------------------------
void  ComponentManager::AddEntity( const  EntityId  entityId )
{
		m_componentTable.resize( entityId.entityId + 1 );
}


 //----------------------------------------------------------------------------------
 // func: Release() : void
 //----------------------------------------------------------------------------------
void  ComponentManager::Release()
{
		m_componentIdList.clear();

		m_componentTable.clear();
		m_componentTable.shrink_to_fit();
}// end Release()


//----------------------------------------------------------------------------------
// func: Update() : void
// brief: Entity�ɓo�^����Ă���R���|�[�l���g���X�V����
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
}// end Update()