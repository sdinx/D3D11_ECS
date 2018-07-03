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
#include  <DirectXMath.h>


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
		// type defined
		//----------------------------------------------------------------------------------
		using  Vector2 = DirectX::XMFLOAT2;
		using  Vector3 = DirectX::XMFLOAT3;
		using  Vector4 = DirectX::XMFLOAT4;
		using  Matrix3x3 = DirectX::XMFLOAT3X3;
		using  Matrix4x4 = DirectX::XMFLOAT4X4;
		using  Value = void*;// Žg—p‹ÖŽ~

		//----------------------------------------------------------------------------------
		// struct
		//----------------------------------------------------------------------------------
		struct  VERTEX
		{
				Vector3  position;
				Vector2  texcoord;
		};// struct VERTEX

		//----------------------------------------------------------------------------------
		// enum
		//----------------------------------------------------------------------------------
		enum  PRIMITIVE_TYPE
		{
				PRMTV_CUBE = 0,
				PRMTV_PLANE,
				PRMTV_SPHERE,
				PRMTV_2D_TRIANGLE,
				PRMTV_2D_SQUARE,
				PRMTV_2D_CIRCLE,
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
		HRESULT  CreateConstantBuffer(ID3D11Buffer*&  ppCB, size_t  byteWidth);
		UINT  CreatePrimitive( PRIMITIVE_TYPE  primitiveType, Vector3  position, Vector3  size, VERTEX*&  ppVertices );
		UINT  CreatePrimitive( PRIMITIVE_TYPE  primitiveType, Vector3  position, Vector3  size, VERTEX*&  ppVertices, INT*& pIndices );
		void  SetD3DDevices( ID3D11Device*  pDevice, ID3D11DeviceContext*  pDeviceContext );
		FLOAT  GetAspectRatio();
		// ID3D11Device*  GetD3DDevice();
		// ID3D11DeviceContext*  GetD3DDeviceContext();

} // namespace  D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_