//----------------------------------------------------------------------------------
// file : Transform.h
// desc : �R���e�i�Ƃ��ė�v�Z�ɕK�v�Ȉʒu���]��傫��
// �Ȃǂ̏��������Ă���R���|�[�l���g
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
// TODO: �e�Ǝq�̊֌W�̂Ȃ�悤�ύX.
// TODO: ���[�J���s��ƃ��[���h�s��̌v�Z�p�x���I�u�W�F�N�g���Ƃɒi�K��������.

#ifndef  _INCLUDED_D3D11_UTILITY_TRANSFORM_
#define  _INCLUDED_D3D11_UTILITY_TRANSFORM_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Component.h>
#include  <D3D11Utility\D3D11Utility.h>


namespace  D3D11Utility
{
		static  auto  ToDegree = [ ]( auto  rad )
		{
				return  ( float ) rad*180.0f / DirectX::XM_PI;
		};
		static  auto  ToRadian = [ ]( auto  deg )
		{
				return  DirectX::XM_PI*( float ) deg / 180.0f;
		};

		class  Transform :public  Component
		{
				// TODO: �I�u�W�F�N�g�ɕK�v�ȍ��W��s����̕ϐ���p��

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------

				Transform();
				Transform( Transform*  parent );

				enum  MSG_TRANSFORM
				{
						MSG_NONE = 0,
						MSG_UPDATE_LOCAL,
						MSG_UPDATE_ALL,
				};// enum MSG_CAMERA

		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------

				static  ComponentId  STATIC_COMPONENT_ID;
				bool  isChange;// true �̂Ƃ��ɍs����X�V����
				Transform*  m_pParent;// �e�̃��[���h���

				// �q�ɉe�����Ȃ����[�J�����
				Matrix4x4  m_localWorld;
				Vector3  m_localPosition;
				Vector3  m_localEuler;
				Vector3  m_localScale;

				// �q�ɉe�����郏�[���h���
				Matrix4x4  m_world;
				Vector3  m_position;
				Vector3  m_euler;
				Vector3  m_scale;
				Vector3  m_translation;// �ړ��s��


		public:
				//----------------------------------------------------------------------------------
				// public variables
				//----------------------------------------------------------------------------------
				/* NOTHING */

		private:
				//----------------------------------------------------------------------------------
				// private methods
				//----------------------------------------------------------------------------------
				/* NOTHING */

		public:
				//----------------------------------------------------------------------------------
				// public methods
				//----------------------------------------------------------------------------------

				/* static */
				static  ComponentId  GetStaticComponentId()
				{
						return  STATIC_COMPONENT_ID;
				}
				static  void  SetStaticComponentId( ComponentId  id )
				{
						if ( STATIC_COMPONENT_ID == STATIC_ID_INVALID )
						{
								STATIC_COMPONENT_ID = id;
								// TODO: need output debug string.
						}
				}
				static  DirectX::XMMATRIX  MultiplyRootTransform( Transform*  parent );

				/* derived virtual */
				void  HandleMessage( const  GameUtility::Message&  msg );
				void  HandleMessage( const  GameUtility::Message&  msg, Value  var )
				{}
				void  Update();

				/* Getter local world */
				Matrix4x4&  GetLocalWorld()
				{
						return  m_localWorld;
				}
				Vector3&  GetLocalPosition()
				{
						return  m_localPosition;
				}
				Vector3&  GetLocalEuler()
				{
						return  m_localEuler;
				}
				Vector3&  GetLocalScale()
				{
						return  m_localScale;
				}
				/* Getter world */
				Matrix4x4&  GetWorld()
				{
						return  m_world;
				}
				Vector3&  GetPosition()
				{
						return  m_position;
				}
				Vector3&  GetEuler()
				{
						return  m_euler;
				}
				Vector3&  GetScale()
				{
						return  m_scale;
				}
				Vector3&  GetTranslation()
				{
						return  m_translation;
				}

				const  Matrix4x4  GetWorldMatrix();

				/* Setter */
				void  SetParent( Transform*  pParent )
				{
						if ( m_parentsEntityId == pParent->m_parentsEntityId )
								return;

						m_pParent = pParent;
				}

				/* Setter local world */
				void  SetLocalWorld( Matrix4x4  world )
				{
						m_localWorld = world;
				}
				void  SetLocalPosition( Vector3  position )
				{
						m_localPosition = position;
				}
				void  SetLocalEuler( Vector3  euler )
				{
						m_localEuler = euler;
				}
				void  SetLocalEuler( float x, float y, float z )// �I�C���[�p
				{
						m_localEuler = Vector3( x, y, z );
				}
				void  SetLocalScale( Vector3  scale )
				{
						m_localScale = scale;
				}
				/* Setter world */
				void  SetWorld( Matrix4x4  world )
				{
						m_world = world;
				}
				void  SetPosition( Vector3  position )
				{
						m_position = position;
				}
				void  SetEuler( Vector3  euler )
				{
						m_euler = euler;
				}
				void  SetEuler( float x, float y, float z )// �I�C���[�p
				{
						m_euler = Vector3( x, y, z );
				}
				void  SetScale( Vector3  scale )
				{
						m_scale = scale;
				}
				void  SetTranslation( Vector3  translation )
				{
						m_translation = translation;
				}

		public:
				//----------------------------------------------------------------------------------
				// operator
				//----------------------------------------------------------------------------------
				/* NOTHING */

		};

}

#endif // ! _INCLUDED_D3D11_UTILITY_TRANSFORM_