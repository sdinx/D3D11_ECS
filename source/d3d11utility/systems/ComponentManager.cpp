//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <d3d11utility/systems/ComponentManager.h>
#include  <d3d11utility/Timer.h>
#include  <omp.h>


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
void  ComponentManager::AddEntity( const  EntityId  entity )
{
		m_entityComponetIdTable.resize( entity.index + 1 );
}


 //----------------------------------------------------------------------------------
 // func: Release() : void
 //----------------------------------------------------------------------------------
void  ComponentManager::Release()
{
		for ( auto entityTable : m_entityComponetIdTable )
				entityTable.clear();

		m_entityComponetIdTable.clear();

		for ( auto componentTable : m_componentTable )
		{
				for ( auto component : componentTable )
						component->Release();
				componentTable.clear();
		}

		m_componentTable.clear();
		m_componentTable.shrink_to_fit();
}// end Release()


//----------------------------------------------------------------------------------
// func: Update() : void
// brief: Entity�ɓo�^����Ă���R���|�[�l���g���X�V����
//----------------------------------------------------------------------------------
void  ComponentManager::Update( float  ms )
{
		int  i = 0, k = 0;
		int  listSize = ( int ) m_componentTable.size();

		for ( i = 0; i < listSize; i++ ) {
				for ( k = 0; k < ( int ) m_componentTable[i].size(); k++ ) {
						m_componentTable[i][k]->Update();
				}
		}

}// end Update()