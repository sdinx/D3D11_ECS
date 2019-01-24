//----------------------------------------------------------------------------------
// file : Object.h
// desc : abstract class for all of objects.
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_OBJECT_
#define  _INCLUDED_D3D11_UTILITY_OBJECT_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <d3d11utility.h>
#include  <d3d11utility/Interface.h>
#include  <d3d11utility/Timer.h>
#include  <d3d11utility/systems/IDirect3D.h>
#include  <unordered_map>

//----------------------------------------------------------------------------------
// defines
//----------------------------------------------------------------------------------


namespace  D3D11Utility
{
		using  ObjectId = unsigned;

		class  Object
		{
				friend  class  Systems::DebugSystem;

		protected:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------

				Object() :
						m_isAlive( true ),
						m_objectId( std::hash<Object*>()( this ) ),
						m_ptr( this )
				{
						//m_className = nullptr;
						auto  d3dInterface = _Singleton<Systems::IDirect3D>::GetInstance();
						pd3dDevice = d3dInterface->GetDevice();
						pd3dDeviceContext = d3dInterface->GetDeviceContext();
				}
				virtual  ~Object()
				{
						m_className.clear();
						m_className.shrink_to_fit();
				}

		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------
				bool  m_isAlive;
				const  ObjectId  m_objectId;
				const  void*  m_ptr;
				float  m_endTime;
				Timer  m_timer;
				std::string  m_className;

		protected:
				//----------------------------------------------------------------------------------
				// protected variables
				//----------------------------------------------------------------------------------
				ID3D11Device*  pd3dDevice;
				ID3D11DeviceContext*  pd3dDeviceContext;

		private:
				//----------------------------------------------------------------------------------
				// private methods
				//----------------------------------------------------------------------------------
				/* NOTHING */

		public:
				//----------------------------------------------------------------------------------
				// public variables
				//----------------------------------------------------------------------------------
				/* NOTHING */

		public:
				//----------------------------------------------------------------------------------
				// public methods
				//----------------------------------------------------------------------------------



		};// class Object

}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_OBJECT_