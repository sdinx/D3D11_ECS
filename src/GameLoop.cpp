//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <GameUtility.h>
#include  <D3D11Utility\Systems\IDirect3DRenderer.h>
#include  <D3D11Utility\Camera.h>
#include  <D3D11Utility\Renderable.h>
#include  <D3D11Utility\Transform.h>
#include  <DIKeyboard.h>

#include  <D3D11Utility\Entity.h>
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <D3D11Utility\Systems\EntityManager.h>
#include  <D3D11Utility\Systems\SystemManager.h>
#include  <D3D11Utility\Systems\TextureManager.h>
#include  <D3D11Utility\Systems\FbxLoader.h>
#include  <D3D11Utility\Systems\DebugSystem.h>
#include  <D3D11Utility\Systems\Timer.h>


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
// note: std::unique_ptr 明示的なリリースじゃないとメモリリークする可能性がある?

static Camera*  s_camera = nullptr;

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

		static  const  EntityId  playerId = pEntityManager->CreateEntity( "Player" );
		static  const  EntityId  backGroundId = pEntityManager->CreateEntity( "BackGround" );
		static  const  EntityId  cameraId = pEntityManager->CreateEntity( "Camera" );
		Entity*  playerEntity = pEntityManager->GetEntity( playerId );
		Entity*  backGroundEntity = pEntityManager->GetEntity( backGroundId );
		Entity*  cameraEntity = pEntityManager->GetEntity( cameraId );

		Camera::SetConstantBuffer();
		Renderable::SetConstantBuffer();

		/* Init Background */
		backGroundEntity->AddComponent<Renderable>( PRMTV_PLANE );
		backGroundEntity->AddComponent<Transform>();

		Renderable*  backRender = backGroundEntity->GetComponent<Renderable>();
		Transform*  backTrans = backGroundEntity->GetComponent<Transform>();
		backTrans->SetPosition( Vector3( 0, 0, 6.0f ) );
		backTrans->SetScale( Vector3( 2, 2, 2 ) );
		backTrans->SetRotation( 45, 45, 0 );
		backRender->SetColor( Vector4( 1, 1, 0, 0 ) );
		backRender->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );
		backRender->SetVertexShader( vs );
		backRender->SetPixelShader( ps );
		backRender->SetColor( Vector4( 1, 0.5f, 0.5f, 1 ) );


		/* Init Player */
		playerEntity->SetTag( "Player" );
		playerEntity->AddComponent<Renderable>( "WaterMill_No1.fbx" );
		playerEntity->AddComponent<Transform>();

		Renderable*  playerRender = playerEntity->GetComponent<Renderable>();
		playerRender->SetTextureId( texId, pTextureManager.get() );
		playerRender->SetVertexShader( vs );
		playerRender->SetPixelShader( ps );

		playerRender->SetColor( Vector4( 0.5f, 0.5f, 0.5f, 0 ) );
		playerRender->HandleMessage( Message( Renderable::MSG_UPDATE_CBUFFER ) );

		Transform*  trans2 = playerEntity->GetComponent<Transform>();
		Vector3&  scale2 = trans2->GetScale();
		Vector3&  pos2 = trans2->GetPosition();

		pos2.y -= 3.0f;
		pos2.z += 5.0f;

		scale2 = Vector3( .05f, .05f, .05f );


		/* Init Camera */
		cameraEntity->AddComponent<Camera>();
		cameraEntity->AddComponent<Transform>();

		Camera*  cam = cameraEntity->GetComponent<Camera>();
		cam->SetPosition( Vector3( 0.0f, 0.0f, -0.75f ) );
		cam->SetTarget( Vector3( 0.0f, 0.0f, 0.0f ) );
		cam->HandleMessage( GameUtility::Message( Camera::MSG_UPDATE_ALL ) );

		s_camera = cam;
}


void  GameUtility::GameLoop()
{

		//UpdateController();
		Input::UpdateKeyboard();
		Input::UpdateMouse();

		pSystemManager->Update( 0 );
		pd3dRenderer->Rendering();
		auto& pos = s_camera->GetComponent<Camera>()->GetTarget();
		auto& move = s_camera->GetComponent<Camera>()->GetPosition();
		static Vector3  s_vecCamera = Vector3( 0, 0, 0 );

		auto mx = Input::MouseAxisX();
		auto my = Input::MouseAxisY();
		if ( mx != 0 )
		{
				pos.x += ( float ) mx / 100.0f;
		}
		if ( my != 0 )
		{
				pos.y -= ( float ) my / 100.0f;
		}

		if ( Input::KeyPress( DIK_W ) )
		{
				move.z += .01f;
				pos.z += .01f;
		}
		else if ( Input::KeyPress( DIK_S ) )
		{
				move.z -= .01f;
				pos.z -= .01f;
		}

		if ( Input::KeyPress( DIK_A ) )
		{
				//move.x += .01f;
				//pos.x += .01f;
		}
		else if ( Input::KeyPress( DIK_D ) )
		{
				//move.x -= .01f;
				//pos.x -= .01f;
		}

		s_camera->HandleMessage( Message( Camera::MSG_UPDATE_ALL ) );

}