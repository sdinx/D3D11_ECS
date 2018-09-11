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
						MSG_UPDATE_MATRIX,
						MSG_UPDATE_PARENT,
						MSG_UPDATE_ALL,
				};// enum MSG_TRANSFORM

		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------

				static  ComponentId  STATIC_COMPONENT_ID;
				bool  m_isMessages[MSG_UPDATE_ALL];// �I�u�W�F�N�g�̈ړ����`�F�b�N����ϐ�.
				Transform*  m_pParent;// �e�̃��[���h���.
				Matrix4x4  m_multiplyWorld;// �e�Ƃ̋�Ԃ��|�����킹�����[���h���.

				// �q�ɉe�����Ȃ����[�J�����.
				Matrix4x4  m_localWorld;
				Vector3  m_localPosition;
				Vector3  m_localEuler;
				Vector3  m_localScale;

				// �q�ɉe�����郏�[���h���.
				Matrix4x4  m_world;
				Vector3  m_position;
				Vector3  m_euler;
				Vector3  m_scale;
				Vector3  m_translation;// �ړ��s��.


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

				/* Update */
				void  UpdateLocalMatrix();
				void  UpdateMatrix();

				/* Getter local world */
				Matrix4x4  GetLocalWorld()
				{
						//m_isMessages[MSG_UPDATE_LOCAL] = true;
						return  m_localWorld;
				}
				Vector3&  GetLocalPosition()
				{
						m_isMessages[MSG_UPDATE_LOCAL] = true;
						return  m_localPosition;
				}
				Vector3&  GetLocalEuler()
				{
						m_isMessages[MSG_UPDATE_LOCAL] = true;
						return  m_localEuler;
				}
				Vector3&  GetLocalScale()
				{
						m_isMessages[MSG_UPDATE_LOCAL] = true;
						return  m_localScale;
				}
				/* Getter world */
				Matrix4x4  GetWorld()
				{
						//m_isMessages[MSG_UPDATE_MATRIX] = true;
						return  m_world;
				}
				Vector3&  GetPosition()
				{
						m_isMessages[MSG_UPDATE_MATRIX] = true;
						return  m_position;
				}
				Vector3&  GetEuler()
				{
						m_isMessages[MSG_UPDATE_MATRIX] = true;
						return  m_euler;
				}
				Vector3&  GetScale()
				{
						m_isMessages[MSG_UPDATE_MATRIX] = true;
						return  m_scale;
				}
				Vector3&  GetTranslation()
				{
						m_isMessages[MSG_UPDATE_MATRIX] = true;
						return  m_translation;
				}

				const  Matrix4x4  GetWorldMatrix()
				{
						return  m_multiplyWorld;
				}

				/* Setter */
				void  SetParent( Transform*  pParent )
				{
						if ( m_parentsEntityId == pParent->m_parentsEntityId )
								return;

						m_isMessages[MSG_UPDATE_MATRIX] = true;
						m_pParent = pParent;
				}
				Transform*  GetParentTransform()
				{
						return  m_pParent;
				}

				/* Setter local world */
				void  SetLocalWorld( Matrix4x4  world )
				{
						m_isMessages[MSG_UPDATE_LOCAL] = true;
						m_localWorld = world;
				}
				void  SetLocalPosition( Vector3  position )
				{
						m_isMessages[MSG_UPDATE_LOCAL] = true;
						m_localPosition = position;
				}
				void  SetLocalEuler( Vector3  euler )
				{
						m_isMessages[MSG_UPDATE_LOCAL] = true;
						m_localEuler = euler;
				}
				void  SetLocalEuler( float x, float y, float z )// �I�C���[�p
				{
						m_isMessages[MSG_UPDATE_LOCAL] = true;
						m_localEuler = Vector3( x, y, z );
				}
				void  SetLocalScale( Vector3  scale )
				{
						m_isMessages[MSG_UPDATE_LOCAL] = true;
						m_localScale = scale;
				}
				/* Setter world */
				void  SetWorld( Matrix4x4  world )
				{
						m_isMessages[MSG_UPDATE_MATRIX] = true;
						m_world = world;
				}
				void  SetPosition( Vector3  position )
				{
						m_isMessages[MSG_UPDATE_MATRIX] = true;
						m_position = position;
				}
				void  SetEuler( Vector3  euler )
				{
						m_isMessages[MSG_UPDATE_MATRIX] = true;
						m_euler = euler;
				}
				void  SetEuler( float x, float y, float z )// �I�C���[�p
				{
						m_isMessages[MSG_UPDATE_MATRIX] = true;
						m_euler = Vector3( x, y, z );
				}
				void  SetScale( Vector3  scale )
				{
						m_isMessages[MSG_UPDATE_MATRIX] = true;
						m_scale = scale;
				}
				void  SetTranslation( Vector3  translation )
				{
						m_isMessages[MSG_UPDATE_MATRIX] = true;
						m_translation += translation;
				}

				bool  GetChange( MSG_TRANSFORM  nMessage )
				{
						return  m_isMessages[nMessage];
				}
				void  SetChange( bool  is, MSG_TRANSFORM  nMessage )
				{
						m_isMessages[nMessage] = is;
				}

		public:
				//----------------------------------------------------------------------------------
				// operator
				//----------------------------------------------------------------------------------
				/* NOTHING */

		};

}

#endif // ! _INCLUDED_D3D11_UTILITY_TRANSFORM_