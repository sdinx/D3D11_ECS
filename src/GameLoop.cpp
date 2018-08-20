//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
/* NOTHING */

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <GameUtility.h>
#include  <D3D11Utility\Systems\IDirect3DRenderer.h>
#include  <D3D11Utility\Camera.h>
#include  <D3D11Utility\Renderable.h>
#include  <D3D11Utility\Transform.h>
#include  <DIKeyboard.h>
#include  <XInputController.h>

#include  <D3D11Utility\Entity.h>
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <D3D11Utility\Systems\EntityManager.h>
#include  <D3D11Utility\Systems\SystemManager.h>
#include  <D3D11Utility\Systems\TextureManager.h>
#include  <D3D11Utility\Systems\FbxLoader.h>
#include  <D3D11Utility\Systems\DebugSystem.h>
#include  <D3D11Utility\Systems\Timer.h>

//----------------------------------------------------------------------------------
// pragma
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
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
static  Transform*  s_trans[3] = { nullptr };
static  UINT  numTrans = 0;


void  GameUtility::GameInit()
{
		componentManager.reset( new  ComponentManager() );
		pd3dRenderer.reset( new  IDirect3DRenderer( componentManager.get() ) );
		pSystemManager.reset( new  SystemManager( componentManager.get() ) );
		pEntityManager.reset( new  EntityManager( componentManager.get() ) );
		pTextureManager.reset( new  TextureManager );
		pSystemManager->AddSystem<DebugSystem>();

		Graphics::TextureId  texId = pTextureManager->CreateTexture( L"res/0.png" );

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
				cubeTrans->SetLocalScale( Vector3( 50, 50, 50 ) );
				cubeTrans->SetLocalPosition( Vector3( 3, 0, 0 ) );
				cubeTrans->SetLocalEuler( 45, 45, 0 );
				cubeTrans->HandleMessage( Message( Transform::MSG_UPDATE_LOCAL ) );
				cubeRender->SetColor( Vector4( 1, 1, 0, 0 ) );
				cubeRender->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );
				cubeRender->SetColor( Vector4( 1, 0.5f, 0.5f, 1 ) );
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
				sphereTrans->SetLocalScale( Vector3( 50, 50, 50 ) );
				sphereTrans->SetPosition( Vector3( -3, 0, -3 ) );
				sphereTrans->SetLocalEuler( 0, 0, 0 );
				sphereTrans->HandleMessage( Message( Transform::MSG_UPDATE_LOCAL ) );
				sphereRender->SetColor( Vector4( 1, 1, 0, 0 ) );
				sphereRender->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );
				sphereRender->SetColor( Vector4( 0.7f, 0.7f, 0.7f, 1 ) );
		}


		/* Init Player */
		static  const  EntityId  playerId = pEntityManager->CreateEntity( "Player" );
		Entity*  playerEntity = pEntityManager->GetEntity( playerId );
		playerEntity->SetTag( "Player" );
		playerEntity->AddComponent<Renderable>( "WaterMill_No1.fbx" );
		playerEntity->AddComponent<Transform>();
		Renderable*  playerRender = playerEntity->GetComponent<Renderable>();
		Transform*  trans2 = playerEntity->GetComponent<Transform>();
		playerRender->SetVertexShader( vs );
		playerRender->SetPixelShader( ps );
		{/* Parameter */
				playerRender->SetTextureId( texId, pTextureManager.get() );
				playerRender->SetColor( Vector4( 0.5f, 0.5f, 0.5f, 0 ) );
				playerRender->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );
				Vector3&  scale2 = trans2->GetScale();
				Vector3&  pos2 = trans2->GetPosition();
				pos2.y -= 3.0f;
				pos2.z += 5.0f;
				scale2 = Vector3( .05f, .05f, .05f );
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
		s_trans[0] = trans2;
		s_trans[1] = cubeTrans;
		s_trans[2] = sphereTrans;

		// eŽqŠÖŒW‚ÌÝ’è
		cubeTrans->SetParent( s_trans[0] );
		sphereTrans->SetParent( cubeTrans );
}


void  GameUtility::GameLoop()
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
				Vector3&  objPos = s_trans[numTrans]->GetLocalPosition();
				if ( Input::KeyPress( DIK_LEFT ) )
						objPos.x += -0.05f;
				else if ( Input::KeyPress( DIK_RIGHT ) )
						objPos.x += 0.05f;
				if ( Input::KeyPress( DIK_UP ) )
						objPos.z += 0.05f;
				else if ( Input::KeyPress( DIK_DOWN ) )
						objPos.z += -0.05f;
		}
		else
		{
				Vector3&  objPos = s_trans[numTrans]->GetTranslation();
				if ( Input::KeyPress( DIK_LEFT ) )
						objPos.x += -0.005f;
				else if ( Input::KeyPress( DIK_RIGHT ) )
						objPos.x += 0.005f;
				if ( Input::KeyPress( DIK_UP ) )
						objPos.z += 0.005f;
				else if ( Input::KeyPress( DIK_DOWN ) )
						objPos.z += -0.005f;
		}

		if ( Input::KeyPress( DIK_L ) )
		{
				Vector3&  rotate = s_trans[numTrans]->GetLocalEuler();
				if ( Input::KeyPress( DIK_Q ) )
				{
						rotate.y += 1.0f;
				}
				else if ( Input::KeyPress( DIK_E ) )
				{
						rotate.y += -1.0f;
				}
		}
		else
		{
				Vector3&  rotate = s_trans[numTrans]->GetEuler();
				if ( Input::KeyPress( DIK_Q ) )
				{
						rotate.y += 0.1f;
				}
				else if ( Input::KeyPress( DIK_E ) )
				{
						rotate.y += -0.1f;
				}
		}

		if ( Input::KeyTrigger( DIK_1 ) )
				numTrans = 0;
		if ( Input::KeyTrigger( DIK_2 ) )
				numTrans = 1;
		if ( Input::KeyTrigger( DIK_3 ) )
				numTrans = 2;

		for ( auto trans : s_trans )
				trans->HandleMessage( Message( Transform::MSG_UPDATE_LOCAL ) );

		s_camera->HandleMessage( Message( Camera::MSG_UPDATE_ALL ) );

		if ( isMouse )
				SetCursorPos( GetSystemMetrics( SM_CXSCREEN ) / 2, GetSystemMetrics( SM_CYSCREEN ) / 2 );
}