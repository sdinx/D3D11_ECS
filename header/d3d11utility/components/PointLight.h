//----------------------------------------------------------------------------------
// file : PointLight.h
// desc : point light component.
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_POINT_LIGHT_
#define  _INCLUDED_D3D11_UTILITY_POINT_LIGHT_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <d3d11utility\components/Transform.h>


namespace  D3D11Utility
{

		class  PointLight :public  Component
		{

				struct  CBufferPointLight
				{
						Vector3  position;
						Vector4  ambient;
						Vector4  diffuse;
				};

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				PointLight();
				~PointLight();


		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------

				static  ComponentId  STATIC_COMPONENT_ID;
				static  const  uint  s_nConstantBufferSlot = eCbufferId::eCbufferPointLight;
				static  ID3D11Buffer  *s_pConstantBuffer;

				CBufferPointLight  m_cbuffer;

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
				void  SetAmbient( Vector4  v4Color )
				{
						m_cbuffer.ambient = v4Color;
				}
				void  SetDiffuse( Vector4  v4Color )
				{
						m_cbuffer.diffuse = v4Color;
				}

				/* derived virtual */
				void  Update();


		};// class  PointLight

}// namespace  D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_POINT_LIGHT_