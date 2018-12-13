//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <d3d11utility\components\Transform.h>
#include  <d3d11utility\systems\DebugSystem.h>
#include  <d3d11utility/systems/IDirect3DRenderer.h>
#include  <d3d11utility/systems/ComponentManager.h>
#include  <d3d11utility/Object.h>
#include  <game/GameUtility.h>
#include  <imgui/imgui.h>
#include  <imgui/imgui_impl_dx11.h>
#include  <imgui/imgui_impl_win32.h>


//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  D3D11Utility::Systems;
using  namespace  GameUtility;
using  namespace  DirectX;


//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
SystemId  DebugSystem::STATIC_SYSTEM_ID = STATIC_ID_INVALID;


DebugSystem::DebugSystem()
{
#ifdef  _DEBUG

		SetupConsole();

		m_pd3dInterface = _Singleton<IDirect3D>::GetInstance();
		m_pd3dRenderer = _Singleton<IDirect3DRenderer>::GetInstance();

		// imgui 初期化
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO&  io = ImGui::GetIO();
		ImGui_ImplWin32_Init( m_pd3dInterface->GetWindowHandle() );
		ImGui_ImplDX11_Init( pd3dDevice, pd3dDeviceContext );
		ImGui::StyleColorsDark();

#endif
}


DebugSystem::~DebugSystem()
{
#ifdef  _DEBUG
		Release();
#endif
}


void  DebugSystem::SetupConsole()
{
		FILE*  fpConsole = nullptr;
		AllocConsole();
		freopen_s( &fpConsole, "CON", "w", stdout );
}


void  DebugSystem::Update( FLOAT  ms )
{
#ifdef  _DEBUG
		if ( m_isActive )
		{

		}
#endif
}


void  DebugSystem::RenderImGui()
{
#ifdef  _DEBUG
		// imgui のフレームを開始
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		int  i = 0;
		/* Material editor */
		ImGui::Begin( "Material editor" );
		{
				for ( auto& material : m_pd3dRenderer->m_materialList )
				{
						ImGui::SetNextTreeNodeOpen( true, ImGuiSetCond_Once );
						if ( ImGui::TreeNode( ( std::to_string( i ) + " : " + material->GetName() ).c_str() ) )
						{
								ImGui::SliderFloat3( "Ambient color", material->ambient, 0.0f, 1.0f );
								ImGui::SliderFloat3( "Diffuse color", material->diffuse, 0.0f, 1.0f );
						}
						ImGui::TreePop();
						i++;
				}// for ( auto& material : m_materialList )
		}
		ImGui::End();


		ImGui::Begin( "Component editor" );
		{
				Transform*  transform = nullptr;
				for ( auto& component : m_pComponentManager->GetComponents<Transform>() )
				{
						transform = component->GetComponent<Transform>();
						ImGui::SetNextTreeNodeOpen( true, ImGuiSetCond_Once );
						if ( ImGui::TreeNode( ( std::to_string( i ) + " : " + transform->GetEntityId().parent->GetName() ).c_str() ) )
						{
								ImGui::DragFloat3( "position", transform->m_position, 0.01f );
						}
						ImGui::TreePop();
						i++;
				}
		}
		ImGui::End();

		// 描画
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );

		m_pd3dInterface->EndRender();
#endif
}


void  DebugSystem::Release()
{

		_fcloseall();
		FreeConsole();
}