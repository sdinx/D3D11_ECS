//----------------------------------------------------------------------------------
// file : DebugSystem.h
// desc : �f�o�b�O���̂ݗL���ɂȂ�V�X�e��
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
// TODO: �Q�[���X�s�[�h�A�t���[���X�s�[�h����̎���.
// TODO: ���\�[�X�̓ǂݍ��݁E����󋵂̎���.
// TODO: �f�o�b�O�J�����̎���.
// TODO: �f�o�b�O�E�B���h�E�̎���.
// TODO: �ǂ��ł��Z�[�u�E���[�h�̎���.
// TODO: �X�N���[���E���[�r�[�L���v�`���[�̎���.
// TODO: CPU�EGPU���׏󋵂̎���.
// TODO: BGM�ESE�̊m�F�̎���.
// TODO: �����蔻��̉����̎���.
// TODO: �Q�[���G�f�B�b�g�̎���.

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