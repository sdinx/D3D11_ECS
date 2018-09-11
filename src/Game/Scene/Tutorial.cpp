//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <Game\Scene\Tutorial.h>
#include  <Game\Manager\BulletManager.h>
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\Renderable.h>
#include  <D3D11Utility\Transform.h>
#include  <D3D11Utility\Systems\SystemInclude.h>
#include  <D3D11Utility\Physical\BulletPhysics.h>
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

}


Tutorial::~Tutorial()
{

}


void  Tutorial::InputFPSCamera()
{
		auto&  move = m_FPSCamera->GetComponent<Camera>()->GetPosition();
		Transform*  camTransform = m_FPSCamera->GetComponent<Transform>();
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
				//move += Vector3( 0, 0, 0.02f );
				camTransform->SetTranslation( Vector3( 0, 0, 0.02f ) );
		}
		else if ( Input::KeyPress( DIK_S ) || GetControllerButtonPress( XIP_D_DOWN ) )
		{
				//move += Vector3( 0, 0, -0.02f );
				camTransform->SetTranslation( Vector3( 0, 0, -0.02f ) );
		}

		if ( Input::KeyPress( DIK_A ) || GetControllerButtonPress( XIP_D_LEFT ) )
		{
				//move += Vector3( -0.02f, 0, 0 );
				camTransform->SetTranslation( Vector3( -0.02f, 0, 0 ) );
		}
		else if ( Input::KeyPress( DIK_D ) || GetControllerButtonPress( XIP_D_RIGHT ) )
		{
				//move += Vector3( 0.02f, 0, 0 );
				camTransform->SetTranslation( Vector3( 0.02f, 0, 0 ) );
		}

		m_FPSCamera->HandleMessage( Message( Camera::MSG_UPDATE_ALL ) );

		if ( isMouse )
				SetCursorPos( GetSystemMetrics( SM_CXSCREEN ) / 2, GetSystemMetrics( SM_CYSCREEN ) / 2 );
}


void  Tutorial::Awake()
{
		/* システムクラス生成 */
		m_pComponentManager.reset( new  ComponentManager() );
		m_pd3dRenderer.reset( new  IDirect3DRenderer( m_pComponentManager.get() ) );
		m_pEntityManager.reset( new  EntityManager( m_pComponentManager.get() ) );
		m_pSystemManager.reset( new  SystemManager( m_pComponentManager.get(), m_pEntityManager.get() ) );
		m_pTextureManager.reset( new  TextureManager );
		m_pSystemManager->AddSystem<Updater>();
		m_pSystemManager->AddSystem<DebugSystem>();
		auto  btEngine = m_pSystemManager->AddSystem<BulletEngine>();


		/* テクスチャ作成 */
		Graphics::TextureId  texId = m_pTextureManager->CreateTexture( L"res/0.png" );
		Graphics::TextureId  texGroundId = m_pTextureManager->CreateTexture( L"res/ground.jpg" );
		Graphics::TextureId  texRifleDiffuseId = m_pTextureManager->CreateTexture( L"res/rifle_diff.png" );
		Graphics::TextureId  texSkyId = m_pTextureManager->CreateTexture( L"res/skysphere.jpg" );

		/* シェーダ作成 */
		Graphics::VertexShader*  vs = m_pd3dRenderer->CreateVertexShader( L"Shader/Default.fx", "VSFunc" );
		//Graphics::ShaderId  gsId = m_pd3dRenderer->CreateGeometryShader( L"Shader/Default.fx", "GSFunc" );
		Graphics::PixelShader*  ps = m_pd3dRenderer->CreatePixelShader( L"Shader/Default.fx", "PSFunc" );
		Graphics::PixelShader*  psSmooth = m_pd3dRenderer->CreatePixelShader( L"Shader/Smoothing.hlsl", "PSSmooth" );

		// 定数バッファの初期化
		Camera::SetConstantBuffer();
		Renderable::SetConstantBuffer();


		/* ゲームシステムクラス生成 */

		// 弾丸のベースとなるエンティティを作成
		static  const  EntityId  bulletId = m_pEntityManager->CreateEntity( "Bullet" );
		Entity*  bulletEntity = m_pEntityManager->GetEntity( bulletId );
		bulletEntity->AddComponent<Renderable>( "res/sphere.fbx", D3D11_CULL_FRONT );
		bulletEntity->AddComponent<Transform>();
		Renderable*  bulletRender = bulletEntity->GetComponent<Renderable>();
		Transform*  bulletTrans = bulletEntity->GetComponent<Transform>();
		bulletRender->SetVertexShader( vs );
		bulletRender->SetPixelShader( ps );
		{/* Parameter */
				bulletRender->SetTextureId( texRifleDiffuseId, m_pTextureManager.get() );
				bulletRender->SetDiffuse( Vector4( 0.1f, 0.1f, 0.2f, 1.0f ) );
				bulletTrans->SetPosition( Vector3( 0, 5, 0 ) );
		}

		BulletManager*  bulletManager = m_pSystemManager->AddSystem<BulletManager>( bulletEntity );
		

		/* Init Cube */
		static  const  EntityId  cubeId = m_pEntityManager->CreateEntity( "Cube" );
		Entity*  cubeEntity = m_pEntityManager->GetEntity( cubeId );
		cubeEntity->AddComponent<Renderable>( "res/cube.fbx" );
		cubeEntity->AddComponent<Transform>();
		Renderable*  cubeRender = cubeEntity->GetComponent<Renderable>();
		Transform*  cubeTrans = cubeEntity->GetComponent<Transform>();
		cubeRender->SetVertexShader( vs );
		cubeRender->SetPixelShader( psSmooth );
		{/* Parameter */
				cubeTrans->SetLocalScale( Vector3( 250, 0.2f, 250 ) );
				cubeRender->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );
				cubeRender->SetDiffuse( Vector4( 1, 0.5f, 0.5f, 1 ) );
				btRigidBody*  rb = btEngine->CreateRigidBody<btBoxShape>( cubeTrans, 0.1f, 0.5f, btVector3( 0, 0, 0 ), btVector3( 125.0f, 0.1f, 125.0f ) );
				cubeEntity->AddComponent<Physical::BulletPhysics>( rb, btEngine );
		}


		/* Init SkySphere */
		static  const  EntityId  sphereId = m_pEntityManager->CreateEntity( "SkySphere" );
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
				rifleTrans->SetPosition( Vector3( 0.7f, -0.7f, 0.8f ) );
				rifleTrans->SetLocalPosition( Vector3( 0, -0, 0 ) );
				rifleTrans->SetScale( Vector3( 0.01f, 0.01f, 0.01f ) );
				rifleTrans->SetLocalEuler( 270, 180, 0 );
		}



		/* Init Player */
		static  const  EntityId  playerId = m_pEntityManager->CreateEntity( "Player" );
		Entity*  playerEntity = m_pEntityManager->GetEntity( playerId );
		playerEntity->SetTag( "Player" );
		playerEntity->AddComponent<Renderable>( "res/humanoid.fbx" );
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
				pos2.z += 5.0f;
				scale2 = Vector3( .03f, .03f, .03f );
		}

		/* Init Camera */
		static  const  EntityId  cameraId = m_pEntityManager->CreateEntity( "Camera" );
		Entity*  cameraEntity = m_pEntityManager->GetEntity( cameraId );
		Transform*  camTrans = cameraEntity->AddComponent<Transform>();
		cameraEntity->AddComponent<Camera>( camTrans );
		Camera*  cam = cameraEntity->GetComponent<Camera>();
		rifleTrans->SetParent( camTrans );
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

		InputFPSCamera();

		if ( Input::KeyPress( DIK_W ) || GetControllerButtonPress( XIP_D_UP ) )
		{
				//v.m_floats[2] += 0.01f;
		}
		else if ( Input::KeyPress( DIK_S ) || GetControllerButtonPress( XIP_D_DOWN ) )
		{
				//v.m_floats[2] += -0.01f;
		}

		if ( Input::KeyPress( DIK_A ) || GetControllerButtonPress( XIP_D_LEFT ) )
		{
				//v.m_floats[0] += -0.01f;
		}
		else if ( Input::KeyPress( DIK_D ) || GetControllerButtonPress( XIP_D_RIGHT ) )
		{
				//v.m_floats[0] += 0.01f;
		}

		if ( Input::KeyTrigger( DIK_SPACE ) )
		{
				//v.m_floats[1] += 10;
		}


		if ( Input::KeyPress( DIK_ESCAPE ) )
		{
				GameScene.SetMethodState( &Scene::BaseScene::Release );
		}

		/* Update systems */
		m_pSystemManager->Update( 0 );
		m_pd3dRenderer->Rendering();
}


void  Tutorial::Release()
{
		exit( 0 );
}