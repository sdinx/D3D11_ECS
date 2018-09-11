//----------------------------------------------------------------------------------
// file : Transform.h
// desc : コンテナとして列計算に必要な位置や回転や大きさ
// などの情報を持っているコンポーネント
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
// TODO: 親と子の関係のなるよう変更.
// TODO: ローカル行列とワールド行列の計算頻度をオブジェクトごとに段階分けする.

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
				// TODO: オブジェクトに必要な座標や行列情報の変数を用意

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
				bool  m_isMessages[MSG_UPDATE_ALL];// オブジェクトの移動をチェックする変数.
				Transform*  m_pParent;// 親のワールド空間.
				Matrix4x4  m_multiplyWorld;// 親との空間を掛け合わせたワールド空間.

				// 子に影響しないローカル空間.
				Matrix4x4  m_localWorld;
				Vector3  m_localPosition;
				Vector3  m_localEuler;
				Vector3  m_localScale;

				// 子に影響するワールド空間.
				Matrix4x4  m_world;
				Vector3  m_position;
				Vector3  m_euler;
				Vector3  m_scale;
				Vector3  m_translation;// 移動行列.


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
				void  SetLocalEuler( float x, float y, float z )// オイラー角
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
				void  SetEuler( float x, float y, float z )// オイラー角
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