//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <GameUtility.h>
#include  <D3D11Utility\Systems\IDirect3DRenderer.h>
#include  <D3D11Utility\Camera.h>
#include  <D3D11Utility\Renderable.h>
#include  <D3D11Utility\Transform.h>

#include  <GameUtility.h>
#include  <D3D11Utility\Entity.h>
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <D3D11Utility\Systems\EntityManager.h>
#include  <D3D11Utility\Systems\SystemManager.h>
using namespace D3D11Utility;
using namespace D3D11Utility::Systems;
using namespace GameUtility;


static  std::unique_ptr<ComponentManager>  componentManager;
static  std::unique_ptr<IDirect3DRenderer>  pd3dRenderer;
static  std::unique_ptr<EntityManager>  pEntityManager;
static  std::unique_ptr<SystemManager>  pSystemManager;

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
// note: std::unique_ptr 明示的なリリースじゃないとメモリリークする可能性がある

#include  <fbxsdk.h>
void  GameUtility::GameInit()
{
		componentManager.reset( new  ComponentManager() );
		pd3dRenderer.reset( new  IDirect3DRenderer( componentManager.get() ) );
		pSystemManager.reset( new  SystemManager( componentManager.get() ) );
		pEntityManager.reset( new  EntityManager( componentManager.get() ) );

		static  const  EntityId  entityId = pEntityManager->CreateEntity( "TestEntity" );
		static  const  EntityId  entityId2 = pEntityManager->CreateEntity( "Test2Entity" );
		Entity*  entity = pEntityManager->GetEntity( entityId );
		Entity*  entity2 = pEntityManager->GetEntity( entityId2 );

		Camera::SetConstantBuffer();
		Renderable::SetConstantBuffer();

		entity->SetTag( "entity" );

		entity->AddComponent<Camera>();
		entity->AddComponent<Renderable>( PT_PLANE );
		entity->AddComponent<Transform>();
		entity2->AddComponent<Renderable>( PT_CUBE );

		Camera*  cam = entity->GetComponent<Camera>();
		cam->SetPosition( Vector3( 0.0f, 0.0f, -0.75f ) );
		cam->SetTarget( Vector3( 0.0f, 0.0f, 0.0f ) );
		cam->HandleMessage( GameUtility::Message( Camera::MSG_UPDATE_ALL ) );
		Renderable*  asd = entity->GetComponent<Renderable>();
		Renderable*  asd2 = entity2->GetComponent<Renderable>();
		Renderable*  asb = cam->GetComponent<Renderable>();

		Transform*  trans = entity->GetComponent<Transform>();
		Vector3&  move = trans->GetPosition();

		move.x += 5.0f;

		asd->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );

		FbxManager*  fbxManager = FbxManager::Create();
		FbxScene*  fbxScene = FbxScene::Create( fbxManager, "fbxscene" );
		FbxManager*  fbxManager2 = FbxManager::Create();
		FbxScene*  fbxScene2 = FbxScene::Create( fbxManager, "fbxscene2" );
		FbxString  fileName( "humanoid.fbx" );
		FbxImporter*  fbxImporter = FbxImporter::Create( fbxManager, "imp" );
		fbxImporter->Initialize( fileName.Buffer(), -1, fbxManager->GetIOSettings() );
		fbxImporter->Import( fbxScene );
		fbxImporter->Destroy();
}


void  GameUtility::GameLoop()
{
		pSystemManager->Update( 0 );
		pd3dRenderer->Rendering();
}