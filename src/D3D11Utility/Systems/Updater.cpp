//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Entity.h>
#include  <D3D11Utility\Transform.h>
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <D3D11Utility\Systems\EntityManager.h>
#include  <D3D11Utility\Systems\Updater.h>
#include  <D3D11Utility\Systems\Timer.h>
#include  <GameUtility.h>


//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  D3D11Utility::Systems;


//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
SystemId  Updater::STATIC_SYSTEM_ID = STATIC_ID_INVALID;


Updater::Updater()
{

}


Updater::~Updater()
{

}


void  Updater::Update( float  ms )
{
		size_t  i = 0;
		auto  transform = m_pComponentManager->GetComponents<Transform>();
//#pragma omp parallel for num_threads(1)
		for ( i = 0; i < transform.size(); i++ )
		{
				transform[i]->Update();
		}
}