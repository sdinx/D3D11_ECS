//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <GameUtility.h>
#include  <D3D11Utility\Systems\IDirect3DRenderer.h>
#include  <D3D11Utility\Camera.h>
#include  <D3D11Utility\Renderable.h>
#include  <D3D11Utility\Transform.h>
#include  <XInputController.h>

#include  <D3D11Utility\Entity.h>
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <D3D11Utility\Systems\EntityManager.h>
#include  <D3D11Utility\Systems\SystemManager.h>
using  namespace  D3D11Utility;
using  namespace  D3D11Utility::Systems;
using  namespace  GameUtility;


static  std::unique_ptr<ComponentManager>  componentManager;
static  std::unique_ptr<IDirect3DRenderer>  pd3dRenderer;
static  std::unique_ptr<EntityManager>  pEntityManager;
static  std::unique_ptr<SystemManager>  pSystemManager;

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
// note: std::unique_ptr 明示的なリリースじゃないとメモリリークする可能性がある

#include  <fbxsdk.h>
static Camera*  s_camera = nullptr;

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
		//entity2->AddComponent<Renderable>( PT_CUBE );

		Camera*  cam = entity->GetComponent<Camera>();
		cam->SetPosition( Vector3( 0.0f, 0.0f, -0.75f ) );
		cam->SetTarget( Vector3( 0.0f, 0.0f, 0.0f ) );
		cam->HandleMessage( GameUtility::Message( Camera::MSG_UPDATE_ALL ) );
		Renderable*  asd = entity->GetComponent<Renderable>();
		Renderable*  asd2 = entity2->GetComponent<Renderable>();
		Renderable*  asb = cam->GetComponent<Renderable>();

		Transform*  trans = entity->GetComponent<Transform>();
		Vector3&  pos = trans->GetPosition();
		Vector3&  move = trans->GetTranslation();
		Vector3&  rot = trans->GetRotation();
		Vector3&  scale = trans->GetScale();

		pos.z += 0.5f;
		move.x += 0.5f;
		rot.z += ToRadian( 90 );
		scale.y += 0.5f;

		asd->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );

		FbxManager*  fbxManager = FbxManager::Create();
		FbxScene*  fbxScene = FbxScene::Create( fbxManager, "fbxscene" );
		FbxString  fileName( "humanoid.fbx" );
		FbxImporter*  fbxImporter = FbxImporter::Create( fbxManager, "imp" );
		fbxImporter->Initialize( fileName.Buffer(), -1, fbxManager->GetIOSettings() );
		fbxImporter->Import( fbxScene );
		fbxImporter->Destroy();
		fbxScene->Destroy();
		fbxManager->Destroy();
		asd->SetColor( Vector4( 1, 0.5f, 0.5f, 1 ) );
		s_camera = cam;
}


void  GameUtility::GameLoop()
{
		Vector3&  pos = s_camera->GetPosition();

		UpdateController();
		pSystemManager->Update( 0 );
		pd3dRenderer->Rendering();

		if ( GetControllerButtonPress( XIP_D_UP ) )
		{
				pos.y += 0.001f;
		}
		else if ( GetControllerButtonPress( XIP_D_DOWN ) )
		{
				pos.y -= 0.001f;
		}
		if ( GetControllerButtonPress( XIP_D_RIGHT ) )
		{
				pos.x += 0.001f;
		}
		else if ( GetControllerButtonPress( XIP_D_LEFT ) )
		{
				pos.x -= 0.001f;
		}
		if ( GetControllerButtonPress( XIP_ANY ) )
				s_camera->HandleMessage( Message( Camera::MSG_UPDATE_ALL ) );


}