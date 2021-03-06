//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility.h>
#include  <game/scene/Tutorial.h>
#include  <d3d11utility/components/Renderable.h>
#include  <d3d11utility/components/Transform.h>
#include  <d3d11utility/components/DirectionLight.h>
#include  <d3d11utility/components/PointLight.h>
#include  <d3d11utility/components/SpotLight.h>
#include  <d3d11utility/graphics/Material.h>
#include  <d3d11utility/systems/SystemInclude.h>
#include  <d3d11utility/physical/BulletPhysics.h>
#include  <d3d11utility/DIKeyboard.h>
#include  <d3d11utility/XInputController.h>
#include  <d3d11utility/systems/BulletEngine.h>


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
		Release();
}


void  Tutorial::InputFPSCamera()
{
		auto&  move = m_FPSCamera->GetComponent<Transform>()->GetPosition();
		Transform*  camTransform = m_FPSCamera->GetComponent<Transform>();
		static  bool  isMouse = false;
		static  bool  isShowMouse = false;
		static  Quaternion&  rotate = camTransform->GetRotation();
		Quaternion  q;
		static  float  dx = 0.0f;
		static  float  dy = 0.0f;
		float  angle_x = 0.0f;
		float  angle_y = 0.0f;

		auto  mx = -Input::MouseAxisX();
		auto  my = -Input::MouseAxisY();

		dx += ( float ) mx / 10.0f;
		dy += ( float ) my / 10.0f;


		if ( isMouse )
		{
				if ( mx != 0.0f || my != 0.0f )
				{
						angle_x += dy;
						angle_y += dx;
						//camTransform->SetEuler( angle_x, angle_y, 0.0f );
				}

				q.setEuler( ToRadian( dx ), ToRadian( dy ), ToRadian( 0 ) );
				rotate = rotate.slerp( q, 1 );
		}

		if ( Input::IsKeyTrigger( DIK_RETURN ) )
		{
				ShowCursor( isMouse );
				isMouse = !isMouse;
		}

		if ( Input::IsKeyPress( DIK_W ) || GetControllerButtonPress( XIP_D_UP ) )
		{
				move += Vector3( 0, 0, 0.002f );
		}
		else if ( Input::IsKeyPress( DIK_S ) || GetControllerButtonPress( XIP_D_DOWN ) )
		{
				move += Vector3( 0, 0, -0.002f );
		}

		if ( Input::IsKeyPress( DIK_A ) || GetControllerButtonPress( XIP_D_LEFT ) )
		{
				move += Vector3( -0.002f, 0, 0 );
		}
		else if ( Input::IsKeyPress( DIK_D ) || GetControllerButtonPress( XIP_D_RIGHT ) )
		{
				move += Vector3( 0.002f, 0, 0 );
		}

		if ( Input::IsKeyPress( DIK_Q ) )
				move += Vector3( 0, -0.002f, 0 );
		else if ( Input::IsKeyPress( DIK_E ) )
				move += Vector3( 0, 0.002f, 0 );

		if ( isMouse )
				SetCursorPos( GetSystemMetrics( SM_CXSCREEN ) / 2, GetSystemMetrics( SM_CYSCREEN ) / 2 );
}


void  Tutorial::Awake()
{
		/* システムクラス生成 */
		m_pComponentManager = std::make_unique<ComponentManager>();
		m_pEntityManager = std::make_unique<EntityManager>( m_pComponentManager.get() );
		m_pSystemManager = std::make_unique<SystemManager>( m_pComponentManager.get(), m_pEntityManager.get() );

		m_pd3dRenderer = _Singleton<IDirect3DRenderer>::GetInstance();
		m_pd3dRenderer->SetComponentManager( m_pComponentManager.get() );
		m_pTextureManager = _Singleton<TextureManager>::GetInstance();
		m_pDebugSystem = m_pSystemManager->AddSystem<DebugSystem>();
		auto  btEngine = m_pSystemManager->AddSystem<BulletEngine>();


		/* FBXモデル読み込み */
		//Graphics::VertexBuffer*  vtxMutant = m_pd3dRenderer->LoadFbxModel( "res/mutant.fbx" );
		Graphics::VertexBuffer*  vtxRifle = m_pd3dRenderer->LoadFbxModel( "res/rifle.fbx" );
		Graphics::VertexBuffer*  vtxFubuking = m_pd3dRenderer->LoadFbxModel( "res/fubuking.fbx" );
		Graphics::VertexBuffer*  vtxMutant = m_pd3dRenderer->LoadFbxModel( "res/mutant.fbx" );


		/* テクスチャ作成 */
		Graphics::TextureId  texId = m_pTextureManager->CreateTexture( L"res/blick_diffuse.png" );
		Graphics::TextureId  texNormalId = m_pTextureManager->CreateTexture( L"res/blick_normal.png" );
		Graphics::TextureId  texGroundId = m_pTextureManager->CreateTexture( L"res/ground.jpg" );
		Graphics::TextureId  texRifleDiffuseId = m_pTextureManager->CreateTexture( L"res/rifle_diff.png" );
		Graphics::TextureId  texSkyId = m_pTextureManager->CreateTexture( L"res/skysphere.jpg" );
		Graphics::TextureId  texFubukiId = m_pTextureManager->CreateTexture( L"res/fubuking.png" );
		Graphics::TextureId  texMutantNorId = m_pTextureManager->CreateTexture( L"res/Mutant_Normal.png" );

		/* シェーダ作成 */
		Graphics::VertexShader*  vs = m_pd3dRenderer->CreateVertexShader( L"Shader/Default.fx", "VSFunc" );
		Graphics::PixelShader*  ps = m_pd3dRenderer->CreatePixelShader( L"Shader/Default.fx", "PSFunc" );
		Graphics::PixelShader*  psSmooth = m_pd3dRenderer->CreatePixelShader( L"Shader/Smoothing.hlsl", "main" );
		//Graphics::PixelShader*  psBump = m_pd3dRenderer->CreatePixelShader( L"Shader/BumpMapping.hlsl", "main" );
		//Graphics::PixelShader*  psToon = m_pd3dRenderer->CreatePixelShader( L"Shader/BumpMapping.hlsl", "CelShading" );
		//Graphics::VertexShader*  vsToon = m_pd3dRenderer->CreateVertexShader( L"Shader/BumpMapping.fx", "OutlineVS" );

		/* マテリアル作成 */
		Graphics::Material*  matDefault = m_pd3dRenderer->CreateMaterial( "Default Material" );
		Graphics::Material*  matPtLight = m_pd3dRenderer->CreateMaterial( "PointLight Material", Vector3( 1, 1, 1 ), Vector3( 1, 1, 1 ), Vector4( 1, 1, 1, 1 ), Vector4( 1, 1, 1, 1 ) );

		// 定数バッファの初期化
		Camera::SetConstantBuffer();
		Renderable::SetConstantBuffer();
		Graphics::Material::SetConstantBuffer();
		DirectionLight::SetConstantBuffer();
		//PointLight::SetConstantBuffer();
		//SpotLight::SetConstantBuffer();


		/* ゲームシステムクラス生成 */

		// 弾丸のベースとなるエンティティを作成
		static  const  EntityId  bulletId = m_pEntityManager->CreateEntity( "Bullet" );
		Entity*  bulletEntity = m_pEntityManager->GetEntity( bulletId );
		Transform*  bulletTrans = bulletEntity->AddComponent<Transform>();
		Renderable*  bulletRender = bulletEntity->AddComponent<Renderable>( eMeshId::eCube, Graphics::eFrontSolid );
		bulletRender->SetVertexShader( vs );
		bulletRender->SetPixelShader( psSmooth );
		{/* Parameter */
				bulletRender->SetDiffuseTexId( texId );
				bulletRender->SetNormalTexId( texNormalId );
				bulletRender->SetMaterial( matDefault );
				bulletTrans->SetPosition( Vector3( 0, 5, 0 ) );
		}


		/* Init Cube */
		static  const  EntityId  cubeId = m_pEntityManager->CreateEntity( "Cube" );
		Entity*  cubeEntity = m_pEntityManager->GetEntity( cubeId );
		Transform*  cubeTrans = cubeEntity->AddComponent<Transform>();
		Renderable*  cubeRender = cubeEntity->AddComponent<Renderable>( eMeshId::eCube, Graphics::eNoneSolid );
		cubeRender->SetVertexShader( vs );
		cubeRender->SetPixelShader( ps );
		cubeRender->SetDiffuseTexId( texGroundId );
		{/* Parameter */
				cubeTrans->SetPosition( 0, -10.0f, 0 );
				cubeTrans->SetLocalScale( Vector3( 250, 0.2f, 250 ) );
				cubeRender->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );
				cubeRender->SetMaterial( matDefault );
				btRigidBody*  rb = btEngine->CreateRigidBody<btBoxShape>( cubeTrans, 0.0f, 0.5f, btVector3( 0, 0, 0 ), btVector3( 125.0f, 0.1f, 125.0f ) );
				cubeEntity->AddComponent<Physical::BulletPhysics>( rb, btEngine );
		}

		/* Init SkySphere */
		static  const  EntityId  sphereId = m_pEntityManager->CreateEntity( "SkySphere" );
		Entity*  sphereEntity = m_pEntityManager->GetEntity( sphereId );
		Transform*  sphereTrans = sphereEntity->AddComponent<Transform>();
		Renderable*  sphereRender = sphereEntity->AddComponent<Renderable>( eMeshId::eSphere, Graphics::eBackSolid );
		sphereRender->SetVertexShader( vs );
		sphereRender->SetPixelShader( ps );
		{/* Parameter */
				sphereTrans->SetPosition( Vector3( 0, 0, 0 ) );
				sphereTrans->SetLocalScale( Vector3( 300.f, 300.f, 300.f ) );
				sphereTrans->SetEuler( 0, 0, 0 );
				sphereRender->SetMaterial( matDefault );
				sphereRender->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );
				sphereRender->SetDiffuseTexId( texSkyId );
		}


		/* Init Rifle */
		static  const  EntityId  rifleId = m_pEntityManager->CreateEntity( "Rifle" );
		Entity*  rifleEntity = m_pEntityManager->GetEntity( rifleId );
		rifleEntity->SetTag( "Player" );
		Transform*  rifleTrans = rifleEntity->AddComponent<Transform>();
		Renderable*  rifleRender = rifleEntity->AddComponent<Renderable>( vtxRifle );
		{
				rifleRender->SetVertexShader( vs );
				rifleRender->SetPixelShader( ps );
				rifleRender->SetDiffuseTexId( texRifleDiffuseId );

				rifleTrans->SetLocalPosition( 0, 2, 0 );
				rifleTrans->SetScale( Vector3( 0.01f, 0.01f, 0.01f ) );
				rifleTrans->SetLocalEuler( 270, 180, 0 );
		}



		/* Init Player */
		static  const  EntityId  playerId = m_pEntityManager->CreateEntity( "Player" );
		m_playerEntity = m_pEntityManager->GetEntity( playerId );
		m_playerEntity->SetTag( "Player" );
		Transform*  trans2 = m_playerEntity->AddComponent<Transform>();
		Renderable*  playerRender = m_playerEntity->AddComponent<Renderable>( vtxFubuking );
		playerRender->SetVertexShader( vs );
		playerRender->SetPixelShader( psSmooth );
		{/* Parameter */
				playerRender->SetDiffuseTexId( texFubukiId );
				playerRender->SetNormalTexId( texMutantNorId );
				playerRender->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );
				Vector3&  scale2 = trans2->GetScale();
				Vector3&  pos2 = trans2->GetPosition();
				pos2.m_floats[2] += 5.0f;
				scale2 = Vector3( .03f, .03f, .03f );
		}

		/* Init Camera */
		static  const  EntityId  cameraId = m_pEntityManager->CreateEntity( "Camera" );
		Entity*  cameraEntity = m_pEntityManager->GetEntity( cameraId );
		Transform*  camTrans = cameraEntity->AddComponent<Transform>();
		Camera*  cam = cameraEntity->AddComponent<Camera>( camTrans );
		//rifleTrans->SetParent( camTrans );
		{/* Parameter */
				camTrans->SetPosition( 0.0f, 2.0f, 0.0f );
				cam->HandleMessage( Message( Camera::MSG_UPDATE_ALL ) );
		}
		PointLight::SetMainCamera( cam );

		/* Init light */
		const  EntityId  ptLightId = m_pEntityManager->CreateEntity( "Point Light" );
		Entity*  ptLightEntity = m_pEntityManager->GetEntity( ptLightId );
		Transform*  ptLightTrans = ptLightEntity->AddComponent<Transform>();
		PointLight*  ptLight = ptLightEntity->AddComponent<PointLight>( Vector3( 0.5f, 6, -0.2f ), 1.0f );
		Renderable*  ptLightRender = ptLightEntity->AddComponent<Renderable>( eMeshId::eSphere );
		{
				ptLightTrans->SetPosition( 0.5f, 6, -0.2f );
				ptLightTrans->SetScale( 0.2f );
				ptLightRender->SetVertexShader( vs );
				ptLightRender->SetPixelShader( ps );
				ptLightRender->SetMaterial( matPtLight );
		}

		/* Init light */
		const  EntityId  ptLight2Id = m_pEntityManager->CreateEntity( "Point Light 2" );
		Entity*  ptLight2Entity = m_pEntityManager->GetEntity( ptLight2Id );
		Transform*  ptLight2Trans = ptLight2Entity->AddComponent<Transform>();
		PointLight*  ptLight2 = ptLight2Entity->AddComponent<PointLight>( Vector3( -0.8f, 3, -0.2f ), 0.3f );
		Renderable*  ptLight2Render = ptLight2Entity->AddComponent<Renderable>( eMeshId::eSphere );
		{
				ptLight2Trans->SetPosition( -0.8f, 3, -0.2f );
				ptLight2Trans->SetScale( 0.5f );
				ptLight2Render->SetVertexShader( vs );
				ptLight2Render->SetPixelShader( ps );
				ptLight2Render->SetMaterial( matPtLight );
		}

		m_FPSCamera = cam;

		GameScene.SetMethodState( &Scene::BaseScene::Init );
}


void  Tutorial::Init()
{
		m_isInit = true;

		GameScene.SetMethodState( &Scene::BaseScene::Update );
}


void  Tutorial::Update()
{
		/* Update inputs */
		UpdateController();
		Input::UpdateKeyboard();
		Input::UpdateMouse();

		InputFPSCamera();

		Vector3&  trans = m_playerEntity->GetComponent<Transform>()->GetPosition();
		static  Vector3  euler( 0, 0, 0 );

		if ( Input::IsKeyPress( DIK_UP ) || GetControllerButtonPress( XIP_D_UP ) )
		{
				trans.m_floats[2] += 0.01f;
		}
		else if ( Input::IsKeyPress( DIK_DOWN ) || GetControllerButtonPress( XIP_D_DOWN ) )
		{
				trans.m_floats[2] += -0.01f;
		}

		if ( Input::IsKeyPress( DIK_LEFT ) || GetControllerButtonPress( XIP_D_LEFT ) )
		{
				euler.m_floats[1] += 0.1f;
		}
		else if ( Input::IsKeyPress( DIK_RIGHT ) || GetControllerButtonPress( XIP_D_RIGHT ) )
		{
				euler.m_floats[1] += -0.1f;
		}

		m_playerEntity->GetComponent<Transform>()->SetEuler( euler );

		if ( Input::IsKeyTrigger( DIK_SPACE ) )
		{
				//v.m_floats[1] += 10;
		}


		if ( Input::IsKeyPress( DIK_ESCAPE ) )
		{
				//GameScene.SetMethodState( &Scene::BaseScene::Release );
				exit( 0 );
		}

		/* Update systems */
		m_pComponentManager->Update( 0 );
		m_pSystemManager->Update( 0 );
		m_pd3dRenderer->Rendering();
		m_pDebugSystem->RenderImGui();
}


void  Tutorial::Release()
{
		if ( m_isInit )
		{

		}// if ( m_isInit == true )

		m_isInit = false;
}