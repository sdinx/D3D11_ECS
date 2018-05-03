//----------------------------------------------------------------------------------
// file : Component.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_COMPONENT_
#define  _INCLUDED_D3D11_UTILITY_COMPONENT_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\System\ComponentManager.h>
#include  <D3D11Utility\System\EntityManager.h>
#include  <GameUtility.h>
#include  <list>


namespace  D3D11Utility
{

		//----------------------------------------------------------------------------------
		// ‘O’ñ’è‹`
		//----------------------------------------------------------------------------------
		struct  EntityId;


		class  Component
		{

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				Component()
				{}
				virtual  ~Component()
				{}

		private:
				//----------------------------------------------------------------------------------
				// private  variables
				//----------------------------------------------------------------------------------
				const  EntityId*  m_parentsEntityId;

		public:
				//----------------------------------------------------------------------------------
				// public  variables
				//----------------------------------------------------------------------------------
				/* NOTHING */

		private:
				//----------------------------------------------------------------------------------
				// private  methods
				//----------------------------------------------------------------------------------
				/* NOTHING */

		public:
				//----------------------------------------------------------------------------------
				// public  methods
				//----------------------------------------------------------------------------------
				template<typename  T>
				inline  T*  GetComponent()const
				{
						ComponentManager::GetComponent( *m_parentsEntityId, T::STATIC_COMPONENT_ID );
				}
				virtual  INT  GetStaticId()const = 0;
				virtual  void  SetStaticId( const  UINT  id ) = 0;
				virtual  void  HandleMessage( const  GameUtility::Message&  msg ) = 0;
				virtual  void  HandleMessage( const  GameUtility::Message&  msg, Value  var ) = 0;
				virtual  void  Update() = 0;


		};// class Component

}// namespace D3D11Utiltiy

#endif // ! _INCLUDED_D3D11_UTILITY_COMPONENT_