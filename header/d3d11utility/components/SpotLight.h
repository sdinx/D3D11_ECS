//----------------------------------------------------------------------------------
// file : SpotLight.h
// desc : point light component.
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_SPOT_LIGHT_
#define  _INCLUDED_D3D11_UTILITY_SPOT_LIGHT_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <d3d11utility\components/Transform.h>


namespace  D3D11Utility
{

		class  SpotLight :public  Component
		{

				struct  CBufferSpotLight
				{
						Vector3  position;
						Vector3  attenuate;
				};// struct CBufferSpotLight

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				SpotLight() {}
				SpotLight( Vector3  position, Vector3  attenuate );
				~SpotLight();


		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------

				static  ComponentId  STATIC_COMPONENT_ID;
				static  const  uint  s_nConstantBufferSlot = eCbufferId::eCBufferSpotLight;
				static  ID3D11Buffer  *s_pConstantBuffer;

				Transform*  m_transform;
				CBufferSpotLight  m_cbuffer;

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
				static  void  SetConstantBuffer();
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

				/* setter */

				/* derived virtual */
				void  HandleMessage( const  Message&  msg ) {}
				void  Update();

				/* original */
				void  UpdateConstantBuffer();

		};// class  SpotLight

}// namespace  D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_SPOT_LIGHT_