//----------------------------------------------------------------------------------
// file : Entity.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_ENTITY_
#define  _INCLUDED_D3D11_UTILITY_ENTITY_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <Component.h>
#include  <D3D11Utility.h>
#include  <EntityId.h>
#include  <string>


namespace  D3D11Utility
{

		class  Entity
		{

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				Entity();
				Entity( std::string  name );
				Entity( std::string  name, Component*  components );
				~Entity();


		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------
				BOOL  m_isActive;
				std::string  m_tag;
				EntityId  entityId;


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
				inline  BOOL  GetActive() { return  m_isActive; }
				inline  std::string  GetTag() { return  m_tag; }
				inline  void  SetTag( std::string  tag ) { m_tag = tag; }
				inline  void  SetActive( BOOL  isActive ) { m_isActive = isActive; }
				inline  UINT  GetEntityID() { return  entityId.entityId; }
				inline  BOOL  CompareTag( std::string  tag ) { return  ( m_tag == tag ) ? true : false; }

				void  CreateVertexBuffer( PRIMITIVE_TYPE  primitiveType );
				void  Release();


		public:
				//----------------------------------------------------------------------------------
				// operator
				//----------------------------------------------------------------------------------
				inline  BOOL  operator==( BOOL  isActive ) {
						return  ( m_isActive == isActive ) ? true : false;
				}
				inline  BOOL  operator!=( BOOL  isActive ) {
						return  ( m_isActive != isActive ) ? true : false;
				}

				inline  BOOL  operator==( Entity  entity ) {
						return  ( GetEntityID() == entity.GetEntityID() ) ? true : false;
				}
				inline  BOOL  operator!=( Entity  entity ) {
						return  ( GetEntityID() != entity.GetEntityID() ) ? true : false;
				}

				inline  BOOL  operator==( const  D3D11Utility::Scene*  scene ) {
						return  ( *m_pParentsScene == *scene ) ? true : false;
				}
				inline  BOOL  operator!=( const  D3D11Utility::Scene*  scene ) {
						return  ( *m_pParentsScene != *scene ) ? true : false;
				}

		};// class Entity

}// namespace D3D11Utiltiy

#endif // ! _INCLUDED_D3D11_UTILITY_ENTITY_