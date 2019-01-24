//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <d3d11utility/systems/ComponentManager.h>
#include  <d3d11utility/systems/IDirect3D.h>
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
		auto  d3dInterface = _Singleton<IDirect3D>::GetInstance();
		pd3dDevice = d3dInterface->GetDevice();
		pd3dDeviceContext = d3dInterface->GetDeviceContext();
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
// brief: コンポネントテーブルのサイズを一つ追加する
// brief: EntityListとComponentTableの要素数は共通する
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
				{
						SafeDelete( component );
				}
				componentTable.clear();
		}

		m_componentTable.clear();
		m_componentTable.shrink_to_fit();
}// end Release()


//----------------------------------------------------------------------------------
// func: Update() : void
// brief: Entityに登録されているコンポーネントを更新する
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