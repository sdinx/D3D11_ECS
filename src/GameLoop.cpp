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

static Camera*  s_camera = nullptr;
static Transform*  s_camTrans = nullptr;
static Transform*  s_waterWheelTrans = nullptr;
static Transform*  s_waterGateLTrans = nullptr;
static Transform*  s_waterGateRTrans = nullptr;


void  GameUtility::GameInit()
{
		componentManager.reset( new  ComponentManager() );
		pd3dRenderer.reset( new  IDirect3DRenderer( componentManager.get() ) );
		pSystemManager.reset( new  SystemManager( componentManager.get() ) );
		pEntityManager.reset( new  EntityManager( componentManager.get() ) );
		pTextureManager.reset( new  TextureManager );

		Graphics::TextureId  texId = pTextureManager->CreateTexture( TEXT( "res/0.png" ) );

		static  const  EntityId  playerId = pEntityManager->CreateEntity( "Player" );
		static  const  EntityId  backGroundId = pEntityManager->CreateEntity( "BackGround" );
		static  const  EntityId  cameraId = pEntityManager->CreateEntity( "Camera" );
		static  const  EntityId  waterGateLId = pEntityManager->CreateEntity( "waterGateLeft" );
		static  const  EntityId  waterGateRId = pEntityManager->CreateEntity( "waterGateRight" );
		static  const  EntityId  waterWheelId = pEntityManager->CreateEntity( "WaterWheel" );
		Entity*  playerEntity = pEntityManager->GetEntity( playerId );
		Entity*  backGroundEntity = pEntityManager->GetEntity( backGroundId );
		Entity*  cameraEntity = pEntityManager->GetEntity( cameraId );
		Entity*  waterGateLEntity = pEntityManager->GetEntity( waterGateLId );
		Entity*  waterGateREntity = pEntityManager->GetEntity( waterGateRId );
		Entity*  waterWheelEntity = pEntityManager->GetEntity( waterWheelId );

		Camera::SetConstantBuffer();
		Renderable::SetConstantBuffer();


		/* Init Background */
		backGroundEntity->AddComponent<Renderable>( PRMTV_PLANE );
		backGroundEntity->AddComponent<Transform>();

		Renderable*  backRender = backGroundEntity->GetComponent<Renderable>();
		backRender->SetColor( Vector4( 1, 1, 0, 0 ) );
		backRender->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );

		backRender->SetColor( Vector4( 1, 0.5f, 0.5f, 1 ) );

		/* Init WaterGate+*/
		waterGateLEntity->AddComponent<Renderable>( "WaterGate_No3.fbx" );
		waterGateLEntity->AddComponent<Transform>();

		s_waterGateLTrans = waterGateLEntity->GetComponent<Transform>();
		s_waterGateLTrans->SetPosition( Vector3( 7, 0, 1 ) );
		s_waterGateLTrans->SetScale( Vector3( 0.05f, 0.05f, 0.05f ) );

		/* Init WaterGateR*/
		waterGateREntity->AddComponent<Renderable>( "WaterGate_No3.fbx" );
		waterGateREntity->AddComponent<Transform>();

		s_waterGateRTrans = waterGateREntity->GetComponent<Transform>();
		s_waterGateRTrans->SetPosition( Vector3( 7.1f, 0, 1 ) );
		s_waterGateRTrans->SetRotation( Vector3( 0, ToRadian( 180 ), 0 ) );
		s_waterGateRTrans->SetScale( Vector3( 0.05f, 0.05f, 0.05f ) );


		/* Init WaterWheel*/
		waterWheelEntity->AddComponent<Renderable>( "WaterMill_No1.fbx" );
		waterWheelEntity->AddComponent<Transform>();

		Renderable*  waterWheelRender = waterWheelEntity->GetComponent<Renderable>();

		s_waterWheelTrans = waterWheelEntity->GetComponent<Transform>();
		s_waterWheelTrans->SetScale( Vector3( 0.05f, 0.05f, 0.05f ) );
		s_waterWheelTrans->SetPosition( Vector3( 10, 0, 3 ) );
		Vector3&  waterWheelRotat = s_waterWheelTrans->GetRotation();
		waterWheelRotat.x = ToRadian( 90 );
		waterWheelRotat.y = ToRadian( -60 );


		/* Init Player */
		playerEntity->SetTag( "Player" );
		playerEntity->AddComponent<Renderable>( "Cup_No1.fbx" );
		playerEntity->AddComponent<Transform>();

		Renderable*  playerRender = playerEntity->GetComponent<Renderable>();
		playerRender->SetTextureId( texId, pTextureManager.get() );

		playerRender->SetColor( Vector4( 0.5f, 0.5f, 0.5f, 0 ) );
		playerRender->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );


		Transform*  trans2 = playerEntity->GetComponent<Transform>();
		Vector3&  scale2 = trans2->GetScale();
		Vector3&  pos2 = trans2->GetPosition();

		pos2.y -= 3.0f;
		pos2.z += 5.0f;

		scale2 = Vector3( .5f, .5f, .5f );


		/* Init Camera */
		cameraEntity->AddComponent<Camera>();
		cameraEntity->AddComponent<Transform>();

		Camera*  cam = cameraEntity->GetComponent<Camera>();
		cam->SetPosition( Vector3( 0.0f, 0.0f, -0.75f ) );
		cam->SetTarget( Vector3( 0.0f, 0.0f, 0.0f ) );
		cam->HandleMessage( GameUtility::Message( Camera::MSG_UPDATE_ALL ) );

		
		s_camera = cam;

		s_camTrans = trans2;
}


void  GameUtility::GameLoop()
{
		Vector3&  pos = s_camera->GetPosition();
		Vector3&  tar = s_camera->GetTarget();
		Vector3&  move = s_camTrans->GetPosition();
		Vector3&  rot = s_camTrans->GetRotation();
		Vector3&  waterWheelRotat = s_waterWheelTrans->GetAngle();
		FLOAT  rotate = 0;
		static  BOOL  isRotate = false;

		UpdateController();
		pSystemManager->Update( 0 );
		pd3dRenderer->Rendering();

		
		if ( rot.z <= ToRadian( -50 ) )
				isRotate = true;
		if ( isRotate )
		{// 水車ぐるぐる
				waterWheelRotat.y += ToRadian( 0.1 );
				Vector3&  gateRotL = s_waterGateLTrans->GetRotation();
				Vector3&  gateRotR = s_waterGateRTrans->GetRotation();
				Vector3&  gateTransL = s_waterGateLTrans->GetTranslation();
				Vector3&  gateTransR = s_waterGateRTrans->GetTranslation();
				if ( gateRotL.y <= ToRadian( 130 ) )
				{
						gateRotL.y += ToRadian( 0.1 );
						gateRotR.y -= ToRadian( 0.1 );
						gateTransL.z -= 0.0002f;
						gateTransR.z -= 0.0002f;
						gateTransL.x -= 0.0007f;
						gateTransR.x += 0.0007f;
				}// end if
		}// end if
		if ( GetControllerButtonPress( XIP_RB ) )
		{
				rotate -= ToRadian( 1 );
		}
		if ( GetControllerButtonPress( XIP_LB ) )
		{
				rotate += ToRadian( 1 );
		}
		if ( GetControllerButtonPress( XIP_D_UP ) )
		{

		}
		else if ( GetControllerButtonPress( XIP_D_DOWN ) )
		{

		}
		if ( GetControllerButtonPress( XIP_D_RIGHT ) )
		{
				pos.x += 0.001f;
				tar.x += 0.001f;
				move.x += 0.001f;
		}
		else if ( GetControllerButtonPress( XIP_D_LEFT ) )
		{
				pos.x -= 0.001f;
				tar.x -= 0.001f;
				move.x -= 0.001f;
		}
		if ( GetControllerButtonPress( XIP_ANY ) )
		{
				rot.z += ToRadian( rotate );
				s_camera->HandleMessage( Message( Camera::MSG_UPDATE_ALL ) );
		}

}