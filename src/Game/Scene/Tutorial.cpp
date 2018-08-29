//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\D3D11Utility.h>
#include  <Game\Scene\Tutorial.h>
#include  <D3D11Utility\Camera.h>
#include  <D3D11Utility\Renderable.h>
#include  <D3D11Utility\Transform.h>
#include  <DIKeyboard.h>
#include  <XInputController.h>

#include  <D3D11Utility\Entity.h>
#include  <D3D11Utility\Systems\IDirect3DRenderer.h>
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <D3D11Utility\Systems\EntityManager.h>
#include  <D3D11Utility\Systems\SystemManager.h>
#include  <D3D11Utility\Systems\TextureManager.h>
#include  <D3D11Utility\Systems\FbxLoader.h>
#include  <D3D11Utility\Systems\DebugSystem.h>
#include  <D3D11Utility\Systems\Timer.h>


//----------------------------------------------------------------------------------
// pragmas
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  Scene;
using  namespace  D3D11Utility::Systems;
using  namespace  GameUtility;

//----------------------------------------------------------------------------------
// global variables
//----------------------------------------------------------------------------------
static  std::unique_ptr<ComponentManager>  componentManager;
static  std::unique_ptr<IDirect3DRenderer>  pd3dRenderer;
static  std::unique_ptr<EntityManager>  pEntityManager;
static  std::unique_ptr<SystemManager>  pSystemManager;
static  std::unique_ptr<TextureManager>  pTextureManager;
static  Camera*  s_camera = nullptr;

Tutorial::Tutorial()
{

}


Tutorial::~Tutorial()
{

}


void  Tutorial::Awake()
{
		componentManager.reset( new  ComponentManager() );
		pd3dRenderer.reset( new  IDirect3DRenderer( componentManager.get() ) );
		pSystemManager.reset( new  SystemManager( componentManager.get() ) );
		pEntityManager.reset( new  EntityManager( componentManager.get() ) );
		pTextureManager.reset( new  TextureManager );
		pSystemManager->AddSystem<DebugSystem>();

		Graphics::TextureId  texId = pTextureManager->CreateTexture( L"res/0.png" );
		Graphics::TextureId  texRifleDiffuseId = pTextureManager->CreateTexture( L"res/rifle_diff.png" );

		Graphics::VertexShader*  vs = pd3dRenderer->CreateVertexShader( L"Shader/Default.fx", "VSFunc" );
		//Graphics::ShaderId  gsId = pd3dRenderer->CreateGeometryShader( L"Shader/Default.fx", "GSFunc" );
		Graphics::PixelShader*  ps = pd3dRenderer->CreatePixelShader( L"Shader/Default.fx", "PSFunc" );

		Camera::SetConstantBuffer();
		Renderable::SetConstantBuffer();


		/* Init Cube */
		static  const  EntityId  cubeId = pEntityManager->CreateEntity( "Cube" );
		Entity*  cubeEntity = pEntityManager->GetEntity( cubeId );
		cubeEntity->AddComponent<Renderable>( "res/cube.fbx" );
		cubeEntity->AddComponent<Transform>();
		Renderable*  cubeRender = cubeEntity->GetComponent<Renderable>();
		Transform*  cubeTrans = cubeEntity->GetComponent<Transform>();
		cubeRender->SetVertexShader( vs );
		cubeRender->SetPixelShader( ps );
		{/* Parameter */
				cubeTrans->SetPosition( Vector3( 0, 1.0f, 6.0f ) );
				cubeTrans->SetLocalScale( Vector3( 1, 1, 1 ) );
				cubeTrans->SetLocalPosition( Vector3( 3, 0, 0 ) );
				cubeTrans->SetLocalEuler( 45, 45, 0 );
				cubeTrans->HandleMessage( Message( Transform::MSG_UPDATE_LOCAL ) );
				cubeRender->SetDiffuse( Vector4( 1, 1, 0, 0 ) );
				cubeRender->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );
				cubeRender->SetDiffuse( Vector4( 1, 0.5f, 0.5f, 1 ) );
		}


		/* Init Sphere */
		static  const  EntityId  sphereId = pEntityManager->CreateEntity( "Sphere" );
		Entity*  sphereEntity = pEntityManager->GetEntity( sphereId );
		sphereEntity->AddComponent<Renderable>( "res/sphere.fbx" );
		sphereEntity->AddComponent<Transform>();
		Renderable*  sphereRender = sphereEntity->GetComponent<Renderable>();
		Transform*  sphereTrans = sphereEntity->GetComponent<Transform>();
		sphereRender->SetVertexShader( vs );
		sphereRender->SetPixelShader( ps );
		{/* Parameter */
				sphereTrans->SetPosition( Vector3( 0, 1.0f, 6.0f ) );
				sphereTrans->SetLocalScale( Vector3( 1, 1, 1 ) );
				sphereTrans->SetPosition( Vector3( -3, 0, -3 ) );
				sphereTrans->SetLocalEuler( 0, 0, 0 );
				sphereTrans->HandleMessage( Message( Transform::MSG_UPDATE_LOCAL ) );
				sphereRender->SetDiffuse( Vector4( 1, 1, 0, 0 ) );
				sphereRender->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );
				sphereRender->SetDiffuse( Vector4( 0.7f, 0.7f, 0.7f, 1 ) );
		}


		/* Init Rifle */
		static  const  EntityId  rifleId = pEntityManager->CreateEntity( "Rifle" );
		Entity*  rifleEntity = pEntityManager->GetEntity( rifleId );
		rifleEntity->SetTag( "Player" );
		rifleEntity->AddComponent<Renderable>( "res/rifle.fbx" );
		rifleEntity->AddComponent<Transform>();
		Renderable*  rifleRender = rifleEntity->GetComponent<Renderable>();
		Transform*  rifleTrans = rifleEntity->GetComponent<Transform>();
		{
		rifleRender->SetVertexShader( vs );
		rifleRender->SetPixelShader( ps );
		rifleRender->SetTextureId( texRifleDiffuseId, pTextureManager.get() );
		rifleTrans->SetScale( Vector3( 0.01f, 0.01f, 0.01f ) );
		rifleTrans->SetLocalEuler( 270, 180, 0 );
		rifleTrans->HandleMessage( Message( Transform::MSG_UPDATE_LOCAL ) );
		}



		/* Init Player */
		static  const  EntityId  playerId = pEntityManager->CreateEntity( "Player" );
		Entity*  playerEntity = pEntityManager->GetEntity( playerId );
		playerEntity->SetTag( "Player" );
		playerEntity->AddComponent<Renderable>( "humanoid.fbx" );
		playerEntity->AddComponent<Transform>();
		Renderable*  playerRender = playerEntity->GetComponent<Renderable>();
		Transform*  trans2 = playerEntity->GetComponent<Transform>();
		playerRender->SetVertexShader( vs );
		playerRender->SetPixelShader( ps );
		{/* Parameter */
				playerRender->SetTextureId( texId, pTextureManager.get() );
				//playerRender->SetColor( Vector4( 0.5f, 0.5f, 0.5f, 0 ) );
				playerRender->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );
				Vector3&  scale2 = trans2->GetLocalScale();
				Vector3&  pos2 = trans2->GetPosition();
				pos2.y -= 3.0f;
				pos2.z += 5.0f;
				scale2 = Vector3( .03f, .03f, .03f );
				trans2->HandleMessage( Message( Transform::MSG_UPDATE_LOCAL ) );
				rifleTrans->SetParent( trans2 );
		}


		/* Init Camera */
		static  const  EntityId  cameraId = pEntityManager->CreateEntity( "Camera" );
		Entity*  cameraEntity = pEntityManager->GetEntity( cameraId );
		cameraEntity->AddComponent<Camera>();
		cameraEntity->AddComponent<Transform>();
		Camera*  cam = cameraEntity->GetComponent<Camera>();
		{/* Parameter */
				cam->SetPosition( Vector3( 0.0f, 0.0f, 0.0f ) );
				cam->SetTarget( Vector3( 0.0f, 0.0f, 0.75f ) );
				cam->HandleMessage( GameUtility::Message( Camera::MSG_UPDATE_ALL ) );
		}

		s_camera = cam;

		GameScene.SetMethodState( &Scene::BaseScene::Init );
}


void  Tutorial::Init()
{

		GameScene.SetMethodState( &Scene::BaseScene::Update );
}


void  Tutorial::Update()
{

		/* Update inputs */
		UpdateController();
		Input::UpdateKeyboard();
		Input::UpdateMouse();

		/* Update systems */
		pSystemManager->Update( 0 );
		pd3dRenderer->Rendering();

		auto&  pos = s_camera->GetComponent<Camera>()->GetTarget();
		auto&  move = s_camera->GetComponent<Camera>()->GetPosition();
		static  Vector3  s_vecCamera = Vector3( 0, 0, 0 );
		static  bool  isMouse = false;
		float  dx = 0.0f;
		float  dy = 0.0f;

		auto  mx = Input::MouseAxisX();
		auto  my = Input::MouseAxisY();

		dx = ( float ) mx / 10.0f;
		dy = ( float ) my / 10.0f;

		if ( isMouse )
				if ( mx != 0.0f || my != 0.0f )
				{
						s_camera->SetLookRotation( dy, dx, 0.0f );
				}

		if ( Input::KeyTrigger( DIK_RETURN ) )
		{
				isMouse = !isMouse;
				ShowCursor( isMouse );
		}

		if ( Input::KeyPress( DIK_W ) || GetControllerButtonPress( XIP_D_UP ) )
		{
				s_camera->SetTranslation( Vector3( 0, 0, 0.02f ) );
		}
		else if ( Input::KeyPress( DIK_S ) || GetControllerButtonPress( XIP_D_DOWN ) )
		{
				s_camera->SetTranslation( Vector3( 0, 0, -0.02f ) );
		}

		if ( Input::KeyPress( DIK_A ) || GetControllerButtonPress( XIP_D_LEFT ) )
		{
				s_camera->SetTranslation( Vector3( -0.02f, 0, 0 ) );
		}
		else if ( Input::KeyPress( DIK_D ) || GetControllerButtonPress( XIP_D_RIGHT ) )
		{
				s_camera->SetTranslation( Vector3( 0.02f, 0, 0 ) );
		}

		if ( Input::KeyPress( DIK_L ) )
		{

		}
		else
		{

		}

		s_camera->HandleMessage( Message( Camera::MSG_UPDATE_ALL ) );

		if ( isMouse )
				SetCursorPos( GetSystemMetrics( SM_CXSCREEN ) / 2, GetSystemMetrics( SM_CYSCREEN ) / 2 );
}