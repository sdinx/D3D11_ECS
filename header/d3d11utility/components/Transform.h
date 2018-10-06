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
#include  <D3D11Utility.h>
#include  <d3d11utility\Component.h>


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
				Transform*  m_pParent;// �e�̃��[���h���.

				// �q�ɉe�����Ȃ����[�J�����.
				Matrix4x4  m_localWorld;
				Quaternion  m_localRotation;
				Vector3  m_localPosition;
				Vector3  m_localScale;

				// �q�ɉe�����郏�[���h���.
				Matrix4x4  m_world;
				Quaternion  m_rotation;
				Vector3  m_position;
				Vector3  m_scale;
				Vector3  m_up;// y���i�Ύ��j
				Vector3  m_forward;// z���i���j
				Vector3  m_right;// x���i�Ԏ��j


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
				void  HandleMessage( const  Message&  msg );
				void  Update();

				/* Update */
				void  UpdateLocalMatrix();
				void  UpdateMatrix();

				/* Getter local world */
				Matrix4x4  GetLocalWorld()
				{
						return  m_localWorld;
				}
				Vector3&  GetLocalPosition()
				{
						return  m_localPosition;
				}
				Quaternion&  GetLocalRotation()
				{
						return  m_localRotation;
				}
				Vector3&  GetLocalScale()
				{
						return  m_localScale;
				}
				/* Getter world */
				Matrix4x4  GetWorld()
				{
						return  m_world;
				}
				Vector3&  GetPosition()
				{
						return  m_position;
				}
				Quaternion&  GetRotation()
				{
						return  m_rotation;
				}
				Vector3&  GetScale()
				{
						return  m_scale;
				}
				Vector3&  GetUp()
				{
						return  m_up;
				}
				Vector3&  GetForward()
				{
						return  m_forward;
				}
				Vector3&  GetRight()
				{
						return  m_right;
				}

				/* Setter */
				void  SetParent( Transform*  pParent )
				{
						if ( m_parentsEntityId == pParent->m_parentsEntityId )
								return;

						m_pParent = pParent;
				}
				Transform*  GetParentTransform()
				{
						return  m_pParent;
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
				void  SetLocalPosition( float  x, float  y, float  z )
				{
						m_localPosition.m_floats[0] = x;
						m_localPosition.m_floats[1] = y;
						m_localPosition.m_floats[2] = z;
				}
				void  SetLocalRotation( Quaternion  q )
				{
						m_localRotation = q;
				}
				void  SetLocalEuler( Vector3  euler )
				{
						m_localRotation.setEuler( ToRadian( euler.m_floats[1] ), ToRadian( euler.m_floats[0] ), ToRadian( euler.m_floats[2] ) );
				}
				void  SetLocalEuler( float x, float y, float z )// �I�C���[�p
				{
						m_localRotation.setEuler( ToRadian( y ), ToRadian( x ), ToRadian( z ) );
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
				void  SetPosition( float  x, float  y, float  z )
				{
						m_position.m_floats[0] = x;
						m_position.m_floats[1] = y;
						m_position.m_floats[2] = z;
				}
				void  SetRotation( Quaternion  q )
				{
						m_rotation = q;
				}
				void  SetEuler( Vector3  euler )
				{
						m_rotation.setEuler( ToRadian( euler.m_floats[1] ), ToRadian( euler.m_floats[0] ), ToRadian( euler.m_floats[2] ) );
				}
				void  SetEuler( float x, float y, float z )// �I�C���[�p
				{
						m_rotation.setEuler( ToRadian( y ), ToRadian( x ), ToRadian( z ) );
				}
				void  SetScale( Vector3  scale )
				{
						m_scale = scale;
				}
				void  GetUp( Vector3  up )
				{
						m_up = up;
				}
				void  GetForward( Vector3  forward )
				{
						m_forward = forward;
				}
				void  GetRight( Vector3  right )
				{
						m_right = right;
				}

		public:
				//----------------------------------------------------------------------------------
				// operator
				//----------------------------------------------------------------------------------
				/* NOTHING */

		};

}

#endif // ! _INCLUDED_D3D11_UTILITY_TRANSFORM_