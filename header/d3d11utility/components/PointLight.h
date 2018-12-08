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
#include  <memory>


namespace  D3D11Utility
{

		class  PointLight :public  Component
		{

				struct  CBufferPointLight
				{
						Vector3  position;
						Vector3  attenuate;
				};

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				PointLight() {}
				PointLight( Vector3  position, Vector3  attenuate, Graphics::Material  material );
				PointLight( Vector3  position, Vector3  attenuate, Graphics::MaterialId  id = 0 );
				~PointLight();


		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------

				static  ComponentId  STATIC_COMPONENT_ID;
				static  const  uint  s_nConstantBufferSlot = eCbufferId::eCBufferPointLight;
				static  ID3D11Buffer  *s_pConstantBuffer;

				std::shared_ptr<Systems::IDirect3DRenderer>  m_pRenderer;
				Transform*  m_transform;
				CBufferPointLight  m_cbuffer;
				Graphics::MaterialId  m_materialId;

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
				void  SetMaterial( Graphics::Material  material );
				void  SetMaterial( Graphics::MaterialId  id );

				/* getter */
				Graphics::Material*  GetMaterial();

				/* derived virtual */
				void  HandleMessage( const  Message&  msg ) {}
				void  Update();

				/* original */
				void  UpdateConstantBuffer();

		};// class  PointLight

}// namespace  D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_POINT_LIGHT_