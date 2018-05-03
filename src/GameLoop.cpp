//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <GameUtility.h>
#include  <D3D11Utility\System\IDirect3DRenderer.h>
#include  <D3D11Utility\Camera.h>
#include  <D3D11Utility\Renderable.h>

#include  <D3D11Utility\Entity.h>
#include  <D3D11Utility\System\ComponentManager.h>
using namespace D3D11Utility;
void  GameUtility::GameLoop()
{
		static  const  EntityId  entityId = EntityManager::CreateEntity( "TestEntity" );
		static  const  EntityId  entityId2 = EntityManager::CreateEntity( "Test2Entity" );
		static  Entity*  entity = EntityManager::GetEntity( entityId );
		static  D3D11Utility::IDirect3DRenderer  pd3dRenderer;
		static  BOOL  isInit = true;
		if ( isInit ) 
		{
				D3D11Utility::Camera::SetConstantBuffer();
				D3D11Utility::Renderable::SetConstantBuffer();

				entity->SetTag( "entity" );
				//entity->AddComponent<Camera>( &camera );
				//Camera*  cam = entity->GetComponent<Camera>();
				ComponentManager::GetComponent<Camera>( entityId, Camera::STATIC_COMPONENT_ID );
				isInit = false;
		}

		pd3dRenderer.Rendering();
}