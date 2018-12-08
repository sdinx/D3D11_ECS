//----------------------------------------------------------------------------------
// file : DirectionLight.h
// desc : direction light component.
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_DIRECTION_LIGHT_
#define  _INCLUDED_D3D11_UTILITY_DIRECTION_LIGHT_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <d3d11utility/components/Transform.h>
#include  <memory>


namespace  D3D11Utility
{

		class  DirectionLight :public  Component
		{
				struct  CBufferDirectionLight
				{
						Vector3  direction;
				};

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				DirectionLight() {}
				DirectionLight( Vector3  direction, Graphics::Material  material );
				DirectionLight( Vector3  direction, Graphics::MaterialId  id );
				~DirectionLight();


		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------

				static  ComponentId  STATIC_COMPONENT_ID;
				static  const  uint  s_nConstantBufferSlot = eCbufferId::eCBufferDirectionLight;
				static  ID3D11Buffer  *s_pConstantBuffer;

				std::shared_ptr<Systems::IDirect3DRenderer>  m_pRenderer;
				CBufferDirectionLight  m_cbuffer;
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

		};// class  DirectionLight

}// namespace  D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_DIRECTION_LIGHT_