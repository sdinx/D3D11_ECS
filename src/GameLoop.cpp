//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <GameUtility.h>
#include  <D3D11Utility\Systems\IDirect3DRenderer.h>
#include  <D3D11Utility\Camera.h>
#include  <D3D11Utility\Renderable.h>

#include  <GameUtility.h>
#include  <D3D11Utility\Entity.h>
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <D3D11Utility\Systems\EntityManager.h>
#include  <D3D11Utility\Systems\SystemManager.h>
using namespace D3D11Utility;
using namespace D3D11Utility::Systems;
using namespace GameUtility;


static  ComponentManager  componentManager;
static  std::unique_ptr<SystemManager>  pSystemManager;
static  std::unique_ptr<EntityManager>  pEntityManager;


void  GameUtility::GameInit()
{
		pSystemManager.reset( new  SystemManager( &componentManager ) );
		pSystemManager->AddSystem<IDirect3DRenderer>();
		pEntityManager.reset( new  EntityManager( &componentManager ) );

		static  const  EntityId  entityId = pEntityManager->CreateEntity( "TestEntity" );
		static  const  EntityId  entityId2 = pEntityManager->CreateEntity( "Test2Entity" );
		Entity*  entity = pEntityManager->GetEntity( entityId );
		Entity*  entity2 = pEntityManager->GetEntity( entityId2 );

		D3D11Utility::Camera::SetConstantBuffer();
		D3D11Utility::Renderable::SetConstantBuffer();

		entity->SetTag( "entity" );
		Camera*  camera = new  Camera;
		Renderable*  ren = new  Renderable( PRIMITIVE_TYPE::PT_PLANE );
		entity->AddComponent<Camera>();
		entity->AddComponent<Renderable>( PT_PLANE );
		entity2->AddComponent<Renderable>( PT_PLANE );

		Camera*  cam = entity->GetComponent<Camera>();
		cam->SetPosition( Vector3( 0.0f, 0.0f, -0.75f ) );
		cam->SetTarget( Vector3( 0.0f, 0.0f, 0.0f ) );
		cam->HandleMessage( GameUtility::Message( MSG_UPDATE_ALL ) );
		Renderable*  asd = entity->GetComponent<Renderable>();
		Renderable*  asd2 = entity2->GetComponent<Renderable>();
		Renderable*  asb = cam->GetComponent<Renderable>();
}


void  GameUtility::GameLoop()
{
		pSystemManager->Update( 0 );
}