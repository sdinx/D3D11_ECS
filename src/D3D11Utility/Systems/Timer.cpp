//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Systems\Timer.h>


//----------------------------------------------------------------------------------
// using
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  D3D11Utility::Systems;


Timer::Timer() :
		m_tpStart( std::chrono::system_clock::now() )
{

}


Timer::~Timer()
{

}


Timer::TimePoint  Timer::Now()
{
		return  std::chrono::system_clock::now();
}