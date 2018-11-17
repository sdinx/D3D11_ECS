//----------------------------------------------------------------------------------
// File : Timer.h
// Desc : éûä‘åoâﬂÇä«óùÇ∑ÇÈÉNÉâÉX
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_TIMER_
#define  _INCLUDED_D3D11_UTILITY_TIMER_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <chrono>


namespace  D3D11Utility
{

				class  Timer
				{

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------
						Timer();
						virtual  ~Timer();

						//----------------------------------------------------------------------------------
						// type defined
						//----------------------------------------------------------------------------------
						using  ElapsedMilli = std::chrono::duration<float, std::milli >;
						using  TimePoint = std::chrono::system_clock::time_point;
						using  Nanoseconds = std::chrono::nanoseconds;
						using  Microseconds = std::chrono::microseconds;
						using  Milliseconds = std::chrono::milliseconds;
						using  Seconds = std::chrono::seconds;
						using  Hours = std::chrono::hours;

				private:
						//----------------------------------------------------------------------------------
						// private  variables
						//----------------------------------------------------------------------------------
						TimePoint  m_tpStart;

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

						void  Reset();
						TimePoint  Now();

						template<typename  T = Milliseconds>
						float  GetElapsed()
						{
								return  static_cast< float >( std::chrono::duration_cast< T >( std::chrono::system_clock::now() - m_tpStart ).count() );
						}

				};// class Timer

}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_TIMER_