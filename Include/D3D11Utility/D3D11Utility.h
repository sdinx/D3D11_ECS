//----------------------------------------------------------------------------------
// file : D3D11Utility.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_
#define  _INCLUDED_D3D11_UTILITY_

//----------------------------------------------------------------------------------
// defines
//----------------------------------------------------------------------------------
#define  SAFE_RELEASE(p)  { if(p != nullptr) { p->Release(); p = nullptr; } }
#define  SAFE_DELETE(p)  { if(p != NULL) { delete p; p = nullptr; } }
#define  SAFE_DELETEARRAY(p)  { if(p != NULL) { delete[] p; p = nullptr; } }

//----------------------------------------------------------------------------------
// pragma
//----------------------------------------------------------------------------------
#pragma  warning(disable:4005)
#pragma  comment(lib,  "d3d11.lib")
#pragma  comment(lib, "d3dcompiler.lib")

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <d3d11.h>
#include  <d3dcompiler.h>
#include  <DirectXMath.h>


namespace  D3D11Utility
{
		//----------------------------------------------------------------------------------
		// using
		//----------------------------------------------------------------------------------
		using  Vector2 = DirectX::XMFLOAT2;
		using  Vector3 = DirectX::XMFLOAT3;
		using  Vector4 = DirectX::XMFLOAT4;
		using  Matrix3x3 = DirectX::XMFLOAT3X3;
		using  Matrix4x4 = DirectX::XMFLOAT4X4;
		using  Value = void*;


		//----------------------------------------------------------------------------------
		// struct
		//----------------------------------------------------------------------------------
		struct  VERTEX
		{
				Vector3  position;
		};// struct VERTEX

		struct  CONSTANTBUFFER
		{
				ID3D11Buffer*  pCB;
				UINT  nCBSlot;
				CONSTANTBUFFER()
				{}
				~CONSTANTBUFFER()
				{
						SAFE_RELEASE( pCB );
				}
		};// struct CONSTANTBUFFER

		//----------------------------------------------------------------------------------
		// enum
		//----------------------------------------------------------------------------------
		enum  PRIMITIVE_TYPE
		{
				PT_CUBE = 0,
				PT_PLANE,
				PT_SPHERE,
		}; // enum POLYGON_TYPE

		//----------------------------------------------------------------------------------
		// const variables
		//----------------------------------------------------------------------------------
		CONST INT  SCREEN_WIDTH = 800;
		CONST INT  SCREEN_HEIGHT = 600;

		//----------------------------------------------------------------------------------
		// extern variables
		//----------------------------------------------------------------------------------
		extern  ID3D11Device*  pd3dDevice;
		extern  ID3D11DeviceContext*  pd3dDeviceContext;
		extern  ID3D11Buffer*  pCBufferObject;

		//----------------------------------------------------------------------------------
		// functions
		//----------------------------------------------------------------------------------
		HRESULT  CompileShaderFromFile( LPCWSTR  szFileName, LPCSTR  szEntryPoint, LPCSTR  szShaderModel, ID3DBlob**  ppBlobOut );
		HRESULT  CreateConstantBuffer(ID3D11Buffer**  ppCB, UINT&  nOutSlot, size_t  byteWidth);
		void  SetD3DDevices( ID3D11Device*  pDevice, ID3D11DeviceContext*  pDeviceContext );
		FLOAT  GetAspectRatio();
		//ID3D11Device*  GetD3DDevice();
		//ID3D11DeviceContext*  GetD3DDeviceContext();

} // namespace  D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_