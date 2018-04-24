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
#include  <GameUtility.h>


namespace  D3D11Utility
{
		struct  EntityId;

		class  Component
		{

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				Component()
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


		private:
				//----------------------------------------------------------------------------------
				// private  functions
				//----------------------------------------------------------------------------------


		public:
				//----------------------------------------------------------------------------------
				// public  functions
				//----------------------------------------------------------------------------------
				template<typename  T>  T*  GetComponent()
				{
						ComponentManager::GetComponent( m_parentsEntityId, T::STATIC_HASH_CODE );
				}
				virtual  void  HandleMessage( const  GameUtility::Message&  msg ) = 0;
				virtual  void  HandleMessage( const  GameUtility::Message&  msg, Value  var ) = 0;
				virtual  void  Update() = 0;


		};// class Component

}// namespace D3D11Utiltiy

#endif // ! _INCLUDED_D3D11_UTILITY_COMPONENT_