//----------------------------------------------------------------------------------
// file : Object.h
// desc : abstract class for all of objects.
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_OBJECT_
#define  _INCLUDED_D3D11_UTILITY_OBJECT_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <d3d11utility.h>
#include  <d3d11utility/Interface.h>
#include  <d3d11utility/Timer.h>
#include  <vector>

namespace  D3D11Utility
{
		using  ObjectId = uint;

		class  Object
		{
				friend  class  Systems::DebugSystem;

		protected:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------

				Object() = delete;
				Object( Object&  object ) :
						m_objectId( object.m_objectId ),
						m_ptr( &object )
				{
						m_isAlive = true;
				}
				Object( std::string  name ) :
						m_isAlive( true ),
						m_objectId( s_objectList.size() ),
						m_ptr( this ),
						m_endTime( 0.0f )
				{
						m_className = typeid( *this ).name();

						//s_objectList.push_back( *this );

				}
				~Object()
				{
						s_objectList[m_objectId].m_isAlive = false;
						s_objectList[m_objectId].m_endTime = m_timer.GetElapsed();

						m_className.clear();
						m_className.shrink_to_fit();
				}

		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------
				static  std::vector<Object>  s_objectList;

				bool  m_isAlive;
				const  ObjectId  m_objectId;
				const  void*  m_ptr;
				float  m_endTime;
				Timer  m_timer;
				std::string  m_className;

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