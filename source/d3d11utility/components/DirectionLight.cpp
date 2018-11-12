//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <d3d11utility\components/DirectionLight.h>
#include  <d3d11utility\Systems\ComponentManager.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  DirectX;

//----------------------------------------------------------------------------------
// static  variables
//----------------------------------------------------------------------------------
ComponentId  DirectionLight::STATIC_COMPONENT_ID = STATIC_ID_INVALID;
const  UINT  DirectionLight::s_nConstantBufferSlot;
ID3D11Buffer*  DirectionLight::s_pConstantBuffer = nullptr;


void  DirectionLight::SetConstantBuffer()
{
		if ( s_pConstantBuffer == nullptr )
		{
				CreateConstantBuffer( s_pConstantBuffer, sizeof( CBufferDirectionLight ) );
		}
}