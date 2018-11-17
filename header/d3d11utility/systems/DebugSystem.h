//----------------------------------------------------------------------------------
// file : DebugSystem.h
// desc : デバッグ時のみ有効になるシステム
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
// TODO: ゲームスピード、フレームスピード制御の実装.
// TODO: リソースの読み込み・解放状況の実装.
// TODO: デバッグカメラの実装.
// TODO: デバッグウィンドウの実装.
// TODO: どこでもセーブ・ロードの実装.
// TODO: スクリーン・ムービーキャプチャーの実装.
// TODO: CPU・GPU負荷状況の実装.
// TODO: BGM・SEの確認の実装.
// TODO: 当たり判定の可視化の実装.
// TODO: ゲームエディットの実装.

#ifndef  _INCLUDED_D3D11_UTILITY_SYSTEMS_DEBUG_SYSTEM_
#define  _INCLUDED_D3D11_UTILITY_SYSTEMS_DEBUG_SYSTEM_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility/Systems/ISystem.h>
#include  <memory>

namespace  D3D11Utility
{
		namespace  Systems
		{

				class  DebugSystem :public  ISystem
				{
				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						DebugSystem();
						~DebugSystem();

				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------

						static  SystemId  STATIC_SYSTEM_ID;

						std::shared_ptr<IDirect3D>  m_pd3dInterface;

				public:
						//----------------------------------------------------------------------------------
						// public variables
						//----------------------------------------------------------------------------------
						/* NOTHING */

				private:
						//----------------------------------------------------------------------------------
						// private methods
						//----------------------------------------------------------------------------------
						
						void  SetupConsole();

				public:
						//----------------------------------------------------------------------------------
						// public methods
						//----------------------------------------------------------------------------------

						static  SystemId  GetStaticSystemId()
						{
								return  STATIC_SYSTEM_ID;
						}
						static  void  SetStaticSystemId( SystemId  systemId )
						{
								if ( STATIC_SYSTEM_ID == STATIC_ID_INVALID )
										STATIC_SYSTEM_ID = systemId;
						}

						SystemId  GetSystemId()const
						{
								return  STATIC_SYSTEM_ID;
						}
						void  Update( FLOAT  ms );
						void  RenderImGui();
						void  Release();

				};// class DebugSystem

		}// namespace Systems
}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_SYSTEMS_DEBUG_SYSTEM_