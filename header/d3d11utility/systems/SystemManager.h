//----------------------------------------------------------------------------------
// file : SystemManager.h
// desc : 
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_SYSTEM_MANAGER_
#define  _INCLUDED_D3D11_UTILITY_SYSTEM_MANAGER_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Interface.h>
#include  <D3D11Utility\Systems\ISystem.h>
#include  <list>


namespace  D3D11Utility
{
		namespace  Systems
		{

				class  SystemManager
				{

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						SystemManager( ComponentManager*  pComponentManagerInstance, EntityManager*  pEntityManager );
						virtual  ~SystemManager();

				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------

						ComponentManager*  m_pComponentManager;
						EntityManager*  m_pEntityManager;
						std::list<ISystem*>  m_systemList;

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

						//----------------------------------------------------------------------------------
						// func: AddSystem( P&&... ) : void, template<T, P>
						// brief: 登録システムが重複しないように
						// param: (param) 追加するシステムのコンストラクタ引数を指定.
						//----------------------------------------------------------------------------------
						template<class  T, typename  ...P>
						T*  AddSystem( P&&...  param )
						{
								for ( auto system : m_systemList )
										if ( system->GetSystemId() == T::GetStaticSystemId() )
										{
												return  nullptr;
										}// end if

								T::SetStaticSystemId( m_systemList.size() );
								m_systemList.push_back( new  T( std::forward<P>( param )... ) );
								T*  system = dynamic_cast< T* >( m_systemList.back() );
								system->m_pComponentManager = m_pComponentManager;
								system->m_pEntityManager = m_pEntityManager;

								return  system;
						}
						void  Update( float  ms );
						void  Release();

				};// class SystemManager

		}// namespace Systems
}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_SYSTEM_MANAGER_