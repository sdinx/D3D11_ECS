//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\Systems\IDirect3D.h>
#include  <string>


ID3D11Device*  D3D11Utility::pd3dDevice = NULL;
ID3D11DeviceContext*  D3D11Utility::pd3dDeviceContext = NULL;


HRESULT  D3D11Utility::CompileShaderFromFile( LPCWSTR  szFileName, LPCSTR  szEntryPoint, LPCSTR  szShaderModel, ID3DBlob**  ppBlobOut )
{
		HRESULT  hr = S_OK;

		// コンパイルフラグ
		DWORD  dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
		

#if  defined(DEBUG)  ||  defined(_DEBUG)
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif // defined(DEBUG)  ||  defined(_DEBUG)

#if  defined(NDEBUG)  ||  defined(_NDEBUG)
		dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif // defined(NDEBUG)  ||  defined(_NDEBUG)


		ID3DBlob*  pErrorBlob = NULL;


		// ファイルからシェーダをコンパイル
		hr = D3DCompileFromFile(
				szFileName,
				NULL,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				szEntryPoint,
				szShaderModel,
				dwShaderFlags,
				0,
				ppBlobOut,
				&pErrorBlob
		);


		if ( FAILED( hr ) )
		{
				std::wstring  directory = L"../";
				directory += szFileName;

				hr = D3DCompileFromFile(
						directory.c_str(),
						NULL,
						D3D_COMPILE_STANDARD_FILE_INCLUDE,
						szEntryPoint,
						szShaderModel,
						dwShaderFlags,
						0,
						ppBlobOut,
						&pErrorBlob
				);

				if ( FAILED( hr ) )
						return  hr;
		}


		SafeRelease( pErrorBlob );


		return  hr;
}


HRESULT  D3D11Utility::CreateConstantBuffer( ID3D11Buffer*&  ppCB, size_t  byteWidth )
{
		HRESULT  hr = S_OK;


		// 定数バッファの設定
		D3D11_BUFFER_DESC  bd;
		ZeroMemory( &bd, sizeof( D3D11_BUFFER_DESC ) );
		bd.ByteWidth = byteWidth;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;


		// 定数バッファの設定
		hr = pd3dDevice->CreateBuffer( &bd, NULL, &ppCB );
		if ( FAILED( hr ) )
		{
				OutputDebugString( TEXT( "<D3D11Utility> FAILED CreateBuffer (constant buffer) \n" ) );
				return  hr;
		}

		return  hr;
}

void  D3D11Utility::SetD3DDevices( ID3D11Device*  pDevice, ID3D11DeviceContext*  pDeviceContext )
{
		D3D11Utility::pd3dDevice = pDevice;
		D3D11Utility::pd3dDeviceContext = pDeviceContext;
}


UINT  D3D11Utility::CreatePrimitive( PRIMITIVE_TYPE  primitiveType, Vector3  position, Vector3  size, VERTEX*&  ppVertices )
{
		FLOAT  sx = size.x / 2.0f;
		FLOAT  sy = size.y / 2.0f;
		FLOAT  sz = size.z / 2.0f;
		UINT  numVertices = 0;

		switch ( primitiveType )
		{
		case  PRMTV_CUBE:
				{

				}// end case PRMTV_CUBE
				break;
		case PRMTV_PLANE:
				{

				}// end case PRMTV_PLANE
				break;
		case  PRMTV_SPHERE:
				{

				}// end case PRMTV_SPHERE
		case PRMTV_2D_TRIANGLE:
				{
						numVertices = 3;
						ppVertices = new  VERTEX[numVertices];
						ppVertices[0].position = Vector3( position.x, position.y + sy, position.z );
						ppVertices[1].position = Vector3( position.x + sx, position.y - sy, position.z );
						ppVertices[2].position = Vector3( position.x - sx, position.y - sy, position.z );

				}// end case PRMTV_PLANE
				break;
		case  PRMTV_2D_SQUARE:
				{
						numVertices = 4;
						ppVertices = new  VERTEX[numVertices];
						ppVertices[0].position = Vector3( position.x - sx, position.y + sy, position.z );
						ppVertices[1].position = Vector3( position.x + sx, position.y + sy, position.z );
						ppVertices[2].position = Vector3( position.x + sx, position.y - sy, position.z );
						ppVertices[3].position = Vector3( position.x - sx, position.y - sy, position.z );


						ppVertices[0].texcoord = Vector2( 0, 0 );
						ppVertices[1].texcoord = Vector2( 1, 0 );
						ppVertices[2].texcoord = Vector2( 1, 1 );
						ppVertices[3].texcoord = Vector2( 0, 1 );

				}// end case PRMTV_SPHERE
				break;
		case  PRMTV_2D_CIRCLE:
				{

				}// end case PRMTV_CUBE
				break;
		}// end switch

		return  numVertices;
}


UINT  D3D11Utility::CreatePrimitive( PRIMITIVE_TYPE  primitiveType, Vector3  position, Vector3  size, VERTEX*&  ppVertices, INT*& pIndices )
{
		FLOAT  sx = size.x / 2.0f;
		FLOAT  sy = size.y / 2.0f;
		FLOAT  sz = size.z / 2.0f;
		UINT  numVertices = 0;

		switch ( primitiveType )
		{
		case  PRMTV_CUBE:
				{
						numVertices = 8;
						ppVertices = new  VERTEX[numVertices];
						pIndices = new INT[numVertices * 6];

						// 頂点設定
						{
								// front face
								ppVertices[0].position = Vector3( position.x - sx, position.y + sy, position.z - sz );
								ppVertices[1].position = Vector3( position.x + sx, position.y + sy, position.z - sz );
								ppVertices[2].position = Vector3( position.x + sx, position.y - sy, position.z - sz );
								ppVertices[3].position = Vector3( position.x - sx, position.y - sy, position.z - sz );

								// back face
								ppVertices[4].position = Vector3( position.x + sx, position.y + sy, position.z + sz );
								ppVertices[5].position = Vector3( position.x - sx, position.y + sy, position.z + sz );
								ppVertices[6].position = Vector3( position.x - sx, position.y - sy, position.z + sz );
								ppVertices[7].position = Vector3( position.x + sx, position.y - sy, position.z + sz );
						}

						// 頂点インデックス設定
						{
								// front face
								pIndices[0] = 0;
								pIndices[1] = 1;
								pIndices[2] = 2;

								pIndices[3] = 0;
								pIndices[4] = 2;
								pIndices[5] = 3;

								// back face
								pIndices[6] = 4;
								pIndices[7] = 5;
								pIndices[8] = 6;

								pIndices[9] = 4;
								pIndices[10] = 6;
								pIndices[11] = 7;

								// left face
								pIndices[12] = 1;
								pIndices[13] = 4;
								pIndices[14] = 7;

								pIndices[15] = 1;
								pIndices[16] = 7;
								pIndices[17] = 2;

								// right face
								pIndices[18] = 5;
								pIndices[19] = 0;
								pIndices[20] = 3;

								pIndices[21] = 5;
								pIndices[22] = 3;
								pIndices[23] = 6;

								// top face
								pIndices[24] = 5;
								pIndices[25] = 4;
								pIndices[26] = 1;

								pIndices[27] = 5;
								pIndices[28] = 1;
								pIndices[29] = 0;

								// bottom face
								pIndices[30] = 3;
								pIndices[31] = 2;
								pIndices[32] = 6;

								pIndices[33] = 3;
								pIndices[34] = 6;
								pIndices[35] = 5;
						}

						// テクスチャ座標設定
						{
								ppVertices[0].texcoord = Vector2( 0, 0 );
								ppVertices[1].texcoord = Vector2( 1, 0 );
								ppVertices[2].texcoord = Vector2( 1, 1 );
								ppVertices[3].texcoord = Vector2( 0, 1 );

								ppVertices[4].texcoord = Vector2( 0, 0 );
								ppVertices[5].texcoord = Vector2( 1, 0 );
								ppVertices[6].texcoord = Vector2( 1, 1 );
								ppVertices[7].texcoord = Vector2( 0, 1 );
						}

				}// end case PRMTV_CUBE
				break;
		case PRMTV_PLANE:
				{

				}// end case PRMTV_PLANE
				break;
		case  PRMTV_SPHERE:
				{

				}// end case PRMTV_SPHERE
		case PRMTV_2D_TRIANGLE:
				{
						numVertices = 3;
						ppVertices = new  VERTEX[numVertices];
						ppVertices[0].position = Vector3( position.x, position.y + sy, position.z );
						ppVertices[1].position = Vector3( position.x + sx, position.y - sy, position.z );
						ppVertices[2].position = Vector3( position.x - sx, position.y - sy, position.z );

				}// end case PRMTV_PLANE
				break;
		case  PRMTV_2D_SQUARE:
				{
						numVertices = 4;
						ppVertices = new  VERTEX[numVertices];
						ppVertices[0].position = Vector3( position.x - sx, position.y + sy, position.z );
						ppVertices[1].position = Vector3( position.x + sx, position.y + sy, position.z );
						ppVertices[2].position = Vector3( position.x + sx, position.y - sy, position.z );
						ppVertices[3].position = Vector3( position.x - sx, position.y - sy, position.z );


						ppVertices[0].texcoord = Vector2( 0, 0 );
						ppVertices[1].texcoord = Vector2( 1, 0 );
						ppVertices[2].texcoord = Vector2( 1, 1 );
						ppVertices[3].texcoord = Vector2( 0, 1 );

				}// end case PRMTV_SPHERE
				break;
		case  PRMTV_2D_CIRCLE:
				{

				}// end case PRMTV_CUBE
				break;
		}// end switch

		return  numVertices;
}


FLOAT  D3D11Utility::GetAspectRatio()
{
		auto  d3d11 = _Singleton<Systems::IDirect3D>::GetInstance();

		POINT  screen = d3d11->GetScreenSize();

		return  ( float ) screen.x / ( float ) screen.y;
}


ID3D11Device*  GetD3DDevice()
{
		return  NULL;
}


ID3D11DeviceContext*  GetD3DDeviceContext()
{
		return  NULL;
}