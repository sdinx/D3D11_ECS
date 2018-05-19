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
#include  <D3D11Utility\Systems\TextureManager.h>
using  namespace  D3D11Utility;
using  namespace  D3D11Utility::Systems;
using  namespace  GameUtility;


static  std::unique_ptr<ComponentManager>  componentManager;
static  std::unique_ptr<IDirect3DRenderer>  pd3dRenderer;
static  std::unique_ptr<EntityManager>  pEntityManager;
static  std::unique_ptr<SystemManager>  pSystemManager;
static  std::unique_ptr<TextureManager>  pTextureManager;

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
// note: std::unique_ptr 明示的なリリースじゃないとメモリリークする可能性がある

#include  <fbxsdk.h>
static Camera*  s_camera = nullptr;
static Transform*  s_camTrans = nullptr;

void  GameUtility::GameInit()
{
		componentManager.reset( new  ComponentManager() );
		pd3dRenderer.reset( new  IDirect3DRenderer( componentManager.get() ) );
		pSystemManager.reset( new  SystemManager( componentManager.get() ) );
		pEntityManager.reset( new  EntityManager( componentManager.get() ) );
		pTextureManager.reset( new  TextureManager );

		Graphics::TextureId  texId = pTextureManager->CreateTexture( TEXT( "res/0.png" ) );

		static  const  EntityId  entityId = pEntityManager->CreateEntity( "TestEntity" );
		static  const  EntityId  entityId2 = pEntityManager->CreateEntity( "Test2Entity" );
		static  const  EntityId  camId = pEntityManager->CreateEntity( "camera" );
		Entity*  entity = pEntityManager->GetEntity( entityId );
		Entity*  entity2 = pEntityManager->GetEntity( entityId2 );
		Entity*  camEntity = pEntityManager->GetEntity( camId );

		Camera::SetConstantBuffer();
		Renderable::SetConstantBuffer();

		entity->SetTag( "entity" );

		entity->AddComponent<Renderable>( PRMTV_PLANE );
		entity->AddComponent<Transform>();
		entity2->AddComponent<Renderable>( "WaterMill_No1.fbx" );
		entity2->AddComponent<Transform>();
		camEntity->AddComponent<Camera>();
		camEntity->AddComponent<Transform>();

		Camera*  cam = camEntity->GetComponent<Camera>();
		cam->SetPosition( Vector3( 0.0f, 0.0f, -0.75f ) );
		cam->SetTarget( Vector3( 0.0f, 0.0f, 0.0f ) );
		cam->HandleMessage( GameUtility::Message( Camera::MSG_UPDATE_ALL ) );

		Renderable*  asd = entity->GetComponent<Renderable>();
		Renderable*  asd2 = entity2->GetComponent<Renderable>();
		asd->SetTextureId( texId, pTextureManager.get() );

		Transform*  trans = entity->GetComponent<Transform>();
		Vector3&  pos = trans->GetPosition();
		Vector3&  move = trans->GetTranslation();
		Vector3&  rot = trans->GetRotation();
		Vector3&  scale = trans->GetScale();

		pos.z += 0.5f;
		move.x += 0.5f;
		rot.z += ToRadian( 0 );
		scale.y += 0.5f;


		Transform*  trans2 = entity2->GetComponent<Transform>();
		Vector3&  scale2 = trans2->GetScale();
		Vector3&  pos2 = trans2->GetPosition();

		pos2.y -= 3.0f;
		pos2.z += 5.0f;

		scale2 = Vector3( .5f, .5f, .5f );

		asd2->SetColor( Vector4( 0.5f, 0.5f, 0.5f, 0 ) );
		asd2->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );
		
		asd->SetColor( Vector4( 1, 1, 0, 0 ) );
		asd->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );

		asd->SetColor( Vector4( 1, 0.5f, 0.5f, 1 ) );
		s_camera = cam;

		s_camTrans = camEntity->GetComponent<Transform>();
}


void  GameUtility::GameLoop()
{
		Vector3&  pos = s_camera->GetPosition();
		Vector3&  tar = s_camera->GetTarget();
		Vector3&  move = s_camTrans->GetTranslation();

		UpdateController();
		pSystemManager->Update( 0 );
		pd3dRenderer->Rendering();

		if ( GetControllerButtonPress( XIP_RB ) )
		{
				pos.z += 0.001f;
				tar.z += 0.001f;
		}
		if ( GetControllerButtonPress( XIP_LB ) )
		{
				pos.z -= 0.001f;
				tar.z -= 0.001f;
		}
		if ( GetControllerButtonPress( XIP_D_UP ) )
		{
				pos.y -= 0.001f;
		}
		else if ( GetControllerButtonPress( XIP_D_DOWN ) )
		{
				pos.y += 0.001f;
		}
		if ( GetControllerButtonPress( XIP_D_RIGHT ) )
		{
				pos.x -= 0.001f;
		}
		else if ( GetControllerButtonPress( XIP_D_LEFT ) )
		{
				pos.x += 0.001f;
		}
		if ( GetControllerButtonPress( XIP_ANY ) )
		{
				s_camera->HandleMessage( Message( Camera::MSG_UPDATE_ALL ) );
		}

}