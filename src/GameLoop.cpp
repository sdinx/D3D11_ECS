//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <GameUtility.h>
#include  <D3D11Utility\System\IDirect3DRenderer.h>
#include  <D3D11Utility\Camera.h>
#include  <D3D11Utility\Renderable.h>

#include  <D3D11Utility\Entity.h>
#include  <D3D11Utility\System\ComponentManager.h>
#include  <D3D11Utility\System\EntityManager.h>
using namespace D3D11Utility;


void  GameUtility::GameLoop()
{
		static  ComponentManager  componentManager;
		static  EntityManager  entityManager( &componentManager );
		static  const  EntityId  entityId = entityManager.CreateEntity( "TestEntity" );
		static  const  EntityId  entityId2 = entityManager.CreateEntity( "Test2Entity" );
		static  Entity*  entity = entityManager.GetEntity( entityId );
		static  D3D11Utility::IDirect3DRenderer  pd3dRenderer;
		static  BOOL  isInit = true;
		if ( isInit ) 
		{
				D3D11Utility::Camera::SetConstantBuffer();
				D3D11Utility::Renderable::SetConstantBuffer();

				entity->SetTag( "entity" );
				Camera*  camera = new  Camera;
				Renderable*  ren = new  Renderable( PRIMITIVE_TYPE::PT_PLANE );
				entity->AddComponent<Camera>();
				entity->AddComponent<Renderable>( PT_PLANE );
				Camera*  cam = entity->GetComponent<Camera>();
				Renderable*  asd = entity->GetComponent<Renderable>();
				isInit = false;
		}

		pd3dRenderer.Rendering();
}