//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <d3d11utility\Timer.h>


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


void  Timer::Reset()
{
		m_tpStart = std::chrono::system_clock::now();
}


Timer::TimePoint  Timer::Now()
{
		return  std::chrono::system_clock::now();
}