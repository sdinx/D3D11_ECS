//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <Game\Scene\Tutorial.h>
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\Renderable.h>
#include  <D3D11Utility\Transform.h>
#include  <DIKeyboard.h>
#include  <XInputController.h>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#if  defined(DEBUG)||(_DEBUG)
#pragma  comment(lib, "BulletCollision_vs2010_debug.lib")
#pragma  comment(lib, "BulletDynamics_vs2010_debug.lib")
#pragma  comment(lib, "LinearMath_vs2010_debug.lib")
#else
#pragma  comment(lib, "BulletCollision_vs2010.lib")
#pragma  comment(lib, "BulletDynamics_vs2010.lib")
#pragma  comment(lib, "LinearMath_vs2010.lib")
#endif


//----------------------------------------------------------------------------------
// pragmas
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  Scene;
using  namespace  D3D11Utility::Systems;
using  namespace  GameUtility;


Tutorial::Tutorial()
{
		btSphereShape* shape = new btSphereShape( 10 );//”¼Œa
}


Tutorial::~Tutorial()
{

}


void  Tutorial::InputFPSCamera()
{
		auto&  pos = m_FPSCamera->GetComponent<Camera>()->GetTarget();
		auto&  move = m_FPSCamera->GetComponent<Camera>()->GetPosition();
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
						m_FPSCamera->SetLookRotation( dy, dx, 0.0f );
				}

		if ( Input::KeyTrigger( DIK_RETURN ) )
		{
				isMouse = !isMouse;
				ShowCursor( isMouse );
		}

		if ( Input::KeyPress( DIK_W ) || GetControllerButtonPress( XIP_D_UP ) )
		{
				m_FPSCamera->SetTranslation( Vector3( 0, 0, 0.02f ) );
		}
		else if ( Input::KeyPress( DIK_S ) || GetControllerButtonPress( XIP_D_DOWN ) )
		{
				m_FPSCamera->SetTranslation( Vector3( 0, 0, -0.02f ) );
		}

		if ( Input::KeyPress( DIK_A ) || GetControllerButtonPress( XIP_D_LEFT ) )
		{
				m_FPSCamera->SetTranslation( Vector3( -0.02f, 0, 0 ) );
		}
		else if ( Input::KeyPress( DIK_D ) || GetControllerButtonPress( XIP_D_RIGHT ) )
		{
				m_FPSCamera->SetTranslation( Vector3( 0.02f, 0, 0 ) );
		}

		m_FPSCamera->HandleMessage( Message( Camera::MSG_UPDATE_ALL ) );

		if ( isMouse )
				SetCursorPos( GetSystemMetrics( SM_CXSCREEN ) / 2, GetSystemMetrics( SM_CYSCREEN ) / 2 );
}


void  Tutorial::Awake()
{
		m_pComponentManager.reset( new  ComponentManager() );
		m_pd3dRenderer.reset( new  IDirect3DRenderer( m_pComponentManager.get() ) );
		m_pSystemManager.reset( new  SystemManager( m_pComponentManager.get() ) );
		m_pEntityManager.reset( new  EntityManager( m_pComponentManager.get() ) );
		m_pTextureManager.reset( new  TextureManager );
		m_pSystemManager->AddSystem<DebugSystem>();

		Graphics::TextureId  texId = m_pTextureManager->CreateTexture( L"res/0.png" );
		Graphics::TextureId  texRifleDiffuseId = m_pTextureManager->CreateTexture( L"res/rifle_diff.png" );
		Graphics::TextureId  texSkyId = m_pTextureManager->CreateTexture( L"res/skysphere.jpg" );

		Graphics::VertexShader*  vs = m_pd3dRenderer->CreateVertexShader( L"Shader/Default.fx", "VSFunc" );
		//Graphics::ShaderId  gsId = m_pd3dRenderer->CreateGeometryShader( L"Shader/Default.fx", "GSFunc" );
		Graphics::PixelShader*  ps = m_pd3dRenderer->CreatePixelShader( L"Shader/Default.fx", "PSFunc" );

		Camera::SetConstantBuffer();
		Renderable::SetConstantBuffer();


		static  const  EntityId  groundId = m_pEntityManager->CreateEntity( "Ground" );
		Entity*  groundEntity = m_pEntityManager->GetEntity( groundId );
		groundEntity->AddComponent<Renderable>( "res/sphere.fbx", D3D11_CULL_NONE );
		groundEntity->AddComponent<Transform>();
		Renderable*  groundRender = groundEntity->GetComponent<Renderable>();
		Transform*  groundTrans = groundEntity->GetComponent<Transform>();
		groundRender->SetVertexShader( vs );
		groundRender->SetPixelShader( ps );
		{/* Parameter */

		}


		/* Init Cube */
		static  const  EntityId  cubeId = m_pEntityManager->CreateEntity( "Cube" );
		Entity*  cubeEntity = m_pEntityManager->GetEntity( cubeId );
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
		static  const  EntityId  sphereId = m_pEntityManager->CreateEntity( "Sphere" );
		Entity*  sphereEntity = m_pEntityManager->GetEntity( sphereId );
		sphereEntity->AddComponent<Renderable>( "res/sphere.fbx", D3D11_CULL_BACK );
		sphereEntity->AddComponent<Transform>();
		Renderable*  sphereRender = sphereEntity->GetComponent<Renderable>();
		Transform*  sphereTrans = sphereEntity->GetComponent<Transform>();
		sphereRender->SetVertexShader( vs );
		sphereRender->SetPixelShader( ps );
		{/* Parameter */
				sphereTrans->SetPosition( Vector3( 0, 0, 0 ) );
				sphereTrans->SetLocalScale( Vector3( 300.f, 300.f, 300.f ) );
				sphereTrans->SetLocalEuler( 180, 0, 0 );
				sphereTrans->HandleMessage( Message( Transform::MSG_UPDATE_LOCAL ) );
				sphereRender->SetDiffuse( Vector4( 1, 1, 0, 0 ) );
				sphereRender->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );
				sphereRender->SetDiffuse( Vector4( 0.7f, 0.7f, 0.7f, 1 ) );
				sphereRender->SetTextureId( texSkyId, m_pTextureManager.get() );
		}


		/* Init Rifle */
		static  const  EntityId  rifleId = m_pEntityManager->CreateEntity( "Rifle" );
		Entity*  rifleEntity = m_pEntityManager->GetEntity( rifleId );
		rifleEntity->SetTag( "Player" );
		rifleEntity->AddComponent<Renderable>( "res/rifle.fbx" );
		rifleEntity->AddComponent<Transform>();
		Renderable*  rifleRender = rifleEntity->GetComponent<Renderable>();
		Transform*  rifleTrans = rifleEntity->GetComponent<Transform>();
		{
		rifleRender->SetVertexShader( vs );
		rifleRender->SetPixelShader( ps );
		rifleRender->SetTextureId( texRifleDiffuseId, m_pTextureManager.get() );
		rifleTrans->SetScale( Vector3( 0.01f, 0.01f, 0.01f ) );
		rifleTrans->SetLocalEuler( 270, 180, 0 );
		rifleTrans->HandleMessage( Message( Transform::MSG_UPDATE_LOCAL ) );
		}



		/* Init Player */
		static  const  EntityId  playerId = m_pEntityManager->CreateEntity( "Player" );
		Entity*  playerEntity = m_pEntityManager->GetEntity( playerId );
		playerEntity->SetTag( "Player" );
		playerEntity->AddComponent<Renderable>( "humanoid.fbx" );
		playerEntity->AddComponent<Transform>();
		Renderable*  playerRender = playerEntity->GetComponent<Renderable>();
		Transform*  trans2 = playerEntity->GetComponent<Transform>();
		playerRender->SetVertexShader( vs );
		playerRender->SetPixelShader( ps );
		{/* Parameter */
				playerRender->SetTextureId( texId, m_pTextureManager.get() );
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
		static  const  EntityId  cameraId = m_pEntityManager->CreateEntity( "Camera" );
		Entity*  cameraEntity = m_pEntityManager->GetEntity( cameraId );
		cameraEntity->AddComponent<Camera>();
		cameraEntity->AddComponent<Transform>();
		Camera*  cam = cameraEntity->GetComponent<Camera>();
		{/* Parameter */
				cam->SetPosition( Vector3( 0.0f, 0.0f, 0.0f ) );
				cam->SetTarget( Vector3( 0.0f, 0.0f, 0.75f ) );
				cam->HandleMessage( GameUtility::Message( Camera::MSG_UPDATE_ALL ) );
		}

		m_FPSCamera = cam;

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
		m_pSystemManager->Update( 0 );
		m_pd3dRenderer->Rendering();

		InputFPSCamera();



		if ( Input::KeyPress( DIK_L ) )
		{

		}
		else
		{

		}

}