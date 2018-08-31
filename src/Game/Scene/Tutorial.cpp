//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <Game\Scene\Tutorial.h>
#include  <Game\Manager\BulletManager.h>
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


btDynamicsWorld* g_pDynamicsWorld = nullptr;
btRigidBody* g_pSphereBody = nullptr;
btRigidBody* g_pRBGround = nullptr;
btRigidBody* g_pHumanoid = nullptr;
Transform*  spTrans = nullptr;
Transform*  boxTrans = nullptr;
Transform*  s_playerTrans = nullptr;


void  InitBullet()
{
		btVector3  pos = btVector3( -1.0, 0.5, 0 );
		btScalar  mass = 0.03;
		btScalar  restitution = 0.8;

		// 衝突検出方法の設定 ( デフォルト設定 )
		btDefaultCollisionConfiguration*  config = new btDefaultCollisionConfiguration();
		btCollisionDispatcher*  dispatcher = new btCollisionDispatcher( config );

		// ブロードフェーズ法の設定 ( Dynamic AABB tree method )
		btDbvtBroadphase*  broadphase = new btDbvtBroadphase();

		// 剛体間リンクのソルバ設定
		btSequentialImpulseConstraintSolver*  solver = new btSequentialImpulseConstraintSolver();

		// Bullet ワールド作成
		g_pDynamicsWorld = new btDiscreteDynamicsWorld( dispatcher, broadphase, solver, config );
		g_pDynamicsWorld->setGravity( btVector3( 0, -.8, 0 ) );

		// 球体形状の設定
		btCollisionShape*  sphereShape = new btSphereShape( 1.0 );
		btCollisionShape*  boxShape = new btBoxShape( btVector3( 125, 0.1, 125 ) );

		// 慣性モーメントの計算
		btVector3  inertia( 0, 0, 0 );
		sphereShape->calculateLocalInertia( mass, inertia );

		btTransform  transform = btTransform::getIdentity();
		transform.setOrigin( pos );
		btTransform  playerTrans = btTransform::getIdentity();
		playerTrans.setOrigin( btVector3( 0, 50, 0 ) );

		// 剛体オブジェクト生成
		btDefaultMotionState*  motionState = new btDefaultMotionState( transform );
		btDefaultMotionState*  playerMotionState = new btDefaultMotionState( playerTrans );
		btRigidBody::btRigidBodyConstructionInfo  rbInfo( mass, playerMotionState, sphereShape, inertia );
		btRigidBody::btRigidBodyConstructionInfo  rbBoxInfo( 0, motionState, boxShape, inertia );
		g_pSphereBody = new btRigidBody( rbInfo );
		g_pRBGround = new btRigidBody( rbBoxInfo );

		g_pDynamicsWorld->addRigidBody( g_pSphereBody );
		g_pDynamicsWorld->addRigidBody( g_pRBGround );
}


void  UninitBullet()
{
		SafeDelete( g_pSphereBody->getMotionState() );
		g_pDynamicsWorld->removeRigidBody( g_pSphereBody );
		SafeDelete( g_pSphereBody );

		SafeDelete( g_pRBGround->getMotionState() );
		g_pDynamicsWorld->removeRigidBody( g_pRBGround );
		SafeDelete( g_pRBGround );


		SafeDelete( g_pDynamicsWorld->getBroadphase() );
		SafeDelete( g_pDynamicsWorld );
}


Tutorial::Tutorial()
{

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
		// Bullet Physics 初期化
		InitBullet();

		/* システムクラス生成 */
		m_pComponentManager.reset( new  ComponentManager() );
		m_pd3dRenderer.reset( new  IDirect3DRenderer( m_pComponentManager.get() ) );
		m_pSystemManager.reset( new  SystemManager( m_pComponentManager.get(), m_pEntityManager.get() ) );
		m_pEntityManager.reset( new  EntityManager( m_pComponentManager.get() ) );
		m_pTextureManager.reset( new  TextureManager );
		m_pSystemManager->AddSystem<DebugSystem>();

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
				boxTrans = cubeTrans;
				cubeTrans->SetLocalScale( Vector3( 250, 0.2f, 250 ) );
				cubeTrans->HandleMessage( Message( Transform::MSG_UPDATE_LOCAL ) );
				cubeRender->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );
				cubeRender->SetDiffuse( Vector4( 1, 0.5f, 0.5f, 1 ) );
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
				spTrans = sphereTrans;

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
				s_playerTrans = rifleTrans;
				rifleRender->SetVertexShader( vs );
				rifleRender->SetPixelShader( ps );
				rifleRender->SetTextureId( texRifleDiffuseId, m_pTextureManager.get() );
				rifleTrans->SetPosition( Vector3( 10.f, 60.f, 0.f ) );
				rifleTrans->SetLocalPosition( Vector3( 50.f, -50.f, 40.f ) );
				rifleTrans->SetScale( Vector3( 0.01f, 0.01f, 0.01f ) );
				rifleTrans->SetLocalEuler( 270, 180, 0 );
				rifleTrans->HandleMessage( Message( Transform::MSG_UPDATE_LOCAL ) );
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
				trans2->HandleMessage( Message( Transform::MSG_UPDATE_LOCAL ) );
				rifleTrans->SetParent( trans2 );
		}

		Transform  trans;
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
		g_pDynamicsWorld->stepSimulation( 1 / 300.f, 10 );

		/* Update inputs */
		UpdateController();
		Input::UpdateKeyboard();
		Input::UpdateMouse();

		/* Update systems */
		m_pSystemManager->Update( 0 );
		m_pd3dRenderer->Rendering();

		InputFPSCamera();


		btTransform&  btTrans = g_pSphereBody->getCenterOfMassTransform();
		btVector3&  v = btTrans.getOrigin();
		Vector3  pPos = Vector3( v.m_floats[0], v.m_floats[1], v.m_floats[2] );
		Vector3  rotate = m_FPSCamera->GetLookRotation();
		s_playerTrans->SetPosition( pPos );
		m_FPSCamera->SetPosition( pPos );
		s_playerTrans->SetEuler( m_FPSCamera->GetLookRotation() );
		btTransform&  btBoxTrans = g_pRBGround->getCenterOfMassTransform();
		btVector3&  v2 = btBoxTrans.getOrigin();
		boxTrans->SetPosition( Vector3( v2.m_floats[0], v2.m_floats[1], v2.m_floats[2] ) );


		if ( Input::KeyPress( DIK_W ) || GetControllerButtonPress( XIP_D_UP ) )
		{
				v.m_floats[2] += 0.01f;
		}
		else if ( Input::KeyPress( DIK_S ) || GetControllerButtonPress( XIP_D_DOWN ) )
		{
				v.m_floats[2] += -0.01f;
		}

		if ( Input::KeyPress( DIK_A ) || GetControllerButtonPress( XIP_D_LEFT ) )
		{
				v.m_floats[0] += -0.01f;
		}
		else if ( Input::KeyPress( DIK_D ) || GetControllerButtonPress( XIP_D_RIGHT ) )
		{
				v.m_floats[0] += 0.01f;
		}

		if ( Input::KeyTrigger( DIK_SPACE ) )
		{
				v.m_floats[1] += 10;
		}


		s_playerTrans->HandleMessage( Message( Transform::MSG_UPDATE_LOCAL ) );


		if ( Input::KeyPress( DIK_ESCAPE ) )
		{
				GameScene.SetMethodState( &Scene::BaseScene::Release );
		}

}


void  Tutorial::Release()
{
		UninitBullet();
		exit( 0 );
}