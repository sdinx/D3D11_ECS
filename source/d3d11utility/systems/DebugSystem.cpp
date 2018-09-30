//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <d3d11utility\Systems\DebugSystem.h>
#include  <game/GameUtility.h>


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


void  DebugSystem::Release()
{
		_fcloseall();
		FreeConsole();
}