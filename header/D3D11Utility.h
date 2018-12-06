//----------------------------------------------------------------------------------
// file : D3D11Utility.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_
#define  _INCLUDED_D3D11_UTILITY_

//----------------------------------------------------------------------------------
// defines
//----------------------------------------------------------------------------------
#define  _CRTDBG_MAP_ALLOC 

//----------------------------------------------------------------------------------
// pragma
//----------------------------------------------------------------------------------
#pragma  warning(disable:4005)
#pragma  comment(lib,  "d3d11.lib")
#pragma  comment(lib, "d3dcompiler.lib")
#pragma  comment(lib, "libfbxsdk-mt.lib")

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include <crtdbg.h>
#include <cstdlib> 
#include  <string>
#include  <d3d11.h>
#include  <d3dcompiler.h>
#include  <D3D11utility\Platform.h>


namespace  D3D11Utility
{

		//----------------------------------------------------------------------------------
		// lambda
		//----------------------------------------------------------------------------------
		static  auto  SafeRelease = [ ]( auto  p )
		{
				if ( p != nullptr )
				{
						p->Release();
						p = nullptr;
				}
		};
		static  auto  SafeDestroy = [ ]( auto  p )
		{
				if ( p != nullptr )
				{
						p->Destroy();
						p = nullptr;
				}
		};
		static  auto  SafeDelete = [ ]( auto  p )
		{
				if ( p != nullptr )
				{
						delete p;
						p = nullptr;
				}
		};
		static  auto  SafeDeleteArray = [ ]( auto  p )
		{
				if ( p != nullptr )
				{
						delete[ ] p;
						p = nullptr;
				}
		};

		//----------------------------------------------------------------------------------
		// struct
		//----------------------------------------------------------------------------------
		struct  VERTEX
		{
				DirectX::XMFLOAT3  position;
				DirectX::XMFLOAT3  normal;
				DirectX::XMFLOAT2  texcoord;
		};// struct VERTEX

		//----------------------------------------------------------------------------------
		// enum
		//----------------------------------------------------------------------------------
		enum  ePrimitiveType
		{
				ePrimitiveCube = 0,
				ePrimitivePlane,
				ePrimitiveSphere,
				ePrimitiveTriangle2d,
				ePrimitiveSquare2d,
				ePrimitiveCircle2d,
				MAX_PRIMITIVE_TYPE,
		};// enum ePrimitiveType

		enum  eCbufferId
		{
				eCBufferCamera = 0,
				eCBufferRenderable,
				eCBufferDirectionLight,
				eCBufferPointLight,
				eCBufferSpotLight,
				MAX_CBUFFER_ID,
		};// enum eCbufferId

		//----------------------------------------------------------------------------------
		// const variables
		//----------------------------------------------------------------------------------
		CONST INT  SCREEN_WIDTH = 800;
		CONST INT  SCREEN_HEIGHT = 600;

		//----------------------------------------------------------------------------------
		// extern variables
		// todo: àÍéûìIÇ»Ç‡ÇÃÇ»ÇÃÇ≈îÒêÑèß
		//----------------------------------------------------------------------------------
		extern  ID3D11Device*  pd3dDevice;
		extern  ID3D11DeviceContext*  pd3dDeviceContext;
		extern  ID3D11Buffer*  pCBufferObject;

		//----------------------------------------------------------------------------------
		// functions
		//----------------------------------------------------------------------------------
		HRESULT  CompileShaderFromFile( LPCWSTR  szFileName, LPCSTR  szEntryPoint, LPCSTR  szShaderModel, ID3DBlob**  ppBlobOut );
		HRESULT  CreateConstantBuffer( ID3D11Buffer*&  ppCB, size_t  byteWidth );
		UINT  CreatePrimitive( ePrimitiveType  primitiveType, Vector3  position, Vector3  size, VERTEX*&  ppVertices );
		UINT  CreatePrimitive( ePrimitiveType  primitiveType, Vector3  position, Vector3  size, VERTEX*&  ppVertices, INT*& pIndices );
		void  SetD3DDevices( ID3D11Device*  pDevice, ID3D11DeviceContext*  pDeviceContext );
		FLOAT  GetAspectRatio();
		// ID3D11Device*  GetD3DDevice();
		// ID3D11DeviceContext*  GetD3DDeviceContext();

} // namespace  D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_