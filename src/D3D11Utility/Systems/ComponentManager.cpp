//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <D3D11Utility/Systems/Timer.h>


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
						component->Release();
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
		UINT  i = 0;
		UINT  listSize = m_componentTable.size();

		//static  Timer  timer;

		for ( ; i < listSize; i++ )
				for ( auto& component : m_componentTable[i] )
				{
						if ( component != nullptr )
								component->Update();
				}

		//system( "CLS" );
		//printf( "経過時間: %f", timer.GetElapsed<Timer::Milliseconds>() );
		//timer.Reset();

}// end Update()