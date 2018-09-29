//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <D3D11Utility\InputMove.h>
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <DIKeyboard.h>


//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  DirectX;


//----------------------------------------------------------------------------------
// static  variables
//----------------------------------------------------------------------------------
ComponentId  InputMove::STATIC_COMPONENT_ID = STATIC_ID_INVALID;


InputMove::InputMove( Transform*  target ) :
		m_target( target )
{

}


void  InputMove::Update()
{

}