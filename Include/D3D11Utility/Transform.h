//----------------------------------------------------------------------------------
// file : Transform.h
// desc : コンテナとして列計算に必要な位置や回転や大きさ
// などの情報を持っているコンポーネント
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_TRANSFORM_
#define  _INCLUDED_D3D11_UTILITY_TRANSFORM_

//----------------------------------------------------------------------------------
// Include
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

				Transform() :
						m_position( 0, 0, 0 ),
						m_translation( 0, 0, 0 ),
						m_rotation( 0, 0, 0 ),
						m_scale( 1, 1, 1 )
				{

				}

		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------

				static  ComponentId  STATIC_COMPONENT_ID;
				Vector3  m_position;
				Vector3  m_translation;
				Vector3  m_rotation;
				Vector3  m_scale;
				Matrix4x4  m_localWorld;


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
								// TODO: need  to output debug string.
						}
				}

				/* derived virtual */
				void  HandleMessage( const  GameUtility::Message&  msg )
				{}
				void  HandleMessage( const  GameUtility::Message&  msg, Value  var )
				{}
				void  Update();

				/* Setter and Getter */
				Vector3&  GetPosition()
				{
						return  m_position;
				}
				Vector3&  GetTranslation()
				{
						return  m_translation;
				}
				Vector3&  GetRotation()
				{
						return  m_rotation;
				}
				Vector3&  GetScale()
				{
						return  m_scale;
				}
				Matrix4x4&  GetLocalWorld()
				{
						return  m_localWorld;
				}
				void  SetPosition( Vector3  position )
				{
						m_position = position;
				}
				void  SetTranslation( Vector3  translation )
				{
						m_translation = translation;
				}
				void  SetRotation( Vector3  rotation )
				{
						m_position = rotation;
				}
				void  SetScale( Vector3  scale )
				{
						m_position = scale;
				}
				void  SetLocalWorld( Matrix4x4  localWorld )
				{
						m_localWorld = localWorld;
				}

		public:
				//----------------------------------------------------------------------------------
				// operator
				//----------------------------------------------------------------------------------
				/* NOTHING */

		};

}

#endif // ! _INCLUDED_D3D11_UTILITY_TRANSFORM_