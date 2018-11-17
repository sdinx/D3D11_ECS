//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <d3d11utility\Systems\DebugSystem.h>
#include  <d3d11utility/systems/IDirect3DRenderer.h>
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

		static  uint  counter = 0;
		ImGui::Begin( "test" );
		ImGui::Text( "let press" );
		if ( ImGui::Button( "click here" ) )
				counter++;
		std::string  textCount = "count : " + std::to_string( counter );
		ImGui::Text( textCount.c_str() );
		ImGui::End();

		ImGui::Begin( "testd" );
		ImGui::Text( "let press" );
		if ( ImGui::Button( "click here" ) )
				counter++;
		std::string  stextCount = "count : " + std::to_string( counter );
		ImGui::Text( stextCount.c_str() );
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