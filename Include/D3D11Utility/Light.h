//----------------------------------------------------------------------------------
// file : Light.h
// desc : Light object.
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_SCENE_
#define  _INCLUDED_D3D11_UTILITY_SCENE_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Transform.h>


namespace  D3D11Utility
{

		class  Light
		{
				struct  CBufferDirectionLight
				{
						Vector4  direction;
						Vector4  color;
				};

				struct  CBufferPointLight
				{
						Vector3  position;
						Vector4  color;
				};

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				Light();
				~Light();


		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------

				static  ComponentId  STATIC_COMPONENT_ID;
				static  const  UINT  s_nCBDirectionSlot = 2;
				static  const  UINT  s_nCBPointSlot = 3;
				static  ID3D11Buffer  *s_pConstantBufferDirection;
				static  ID3D11Buffer  *s_pConstantBufferPoint;

				CBufferDirectionLight  m_cbufferDirection;
				CBufferPointLight  m_cbufferPoint;

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

				/* derived virtual */
				void  HandleMessage( const  GameUtility::Message&  msg );
				void  HandleMessage( const  GameUtility::Message&  msg, Value  var )
				{}
				void  Update();


		};// class  Light

}// namespace  D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_SCENE_