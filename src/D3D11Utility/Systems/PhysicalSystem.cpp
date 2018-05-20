//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Systems\PhysicalSystem.h>
#include  <D3D11Utility\Systems\ComponentManager.h>


//----------------------------------------------------------------------------------
// using
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  D3D11Utility::Systems;
using  namespace  D3D11Utility::Physical;
using  namespace  DirectX;


//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
SystemId  PhysicalSystem::STATIC_SYSTEM_ID = STATIC_ID_INVALID;


void  PhysicalSystem::Update( float  ms )
{
		for ( auto physical : m_pComponentManager->GetComponents<IPhysics>() )
		{
				physical->Update();
		}
}