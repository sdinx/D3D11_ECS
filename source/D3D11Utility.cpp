//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility.h>
#include  <d3d11utility\Systems\IDirect3D.h>
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


UINT  D3D11Utility::CreatePrimitive( ePrimitiveType  primitiveType, Vector3  position, Vector3  size, VERTEX*&  ppVertices )
{
		FLOAT  sx = size.m_floats[0] / 2.0f;
		FLOAT  sy = size.m_floats[1] / 2.0f;
		FLOAT  sz = size.m_floats[2] / 2.0f;
		UINT  numVertices = 0;

		switch ( primitiveType )
		{
		case  ePrimitiveCube:
				{

				}// end case ePrimitiveCube
				break;
		case ePrimitivePlane:
				{

				}// end case ePrimitivePlane
				break;
		case  ePrimitiveSphere:
				{

				}// end case ePrimitiveSphere
		case ePrimitiveTriangle2d:
				{
						numVertices = 3;
						ppVertices = new  VERTEX[numVertices];
						ppVertices[0].position = DirectX::XMFLOAT3( position.m_floats[0], position.m_floats[1] + sy, position.m_floats[2] );
						ppVertices[1].position = DirectX::XMFLOAT3( position.m_floats[0] + sx, position.m_floats[1] - sy, position.m_floats[2] );
						ppVertices[2].position = DirectX::XMFLOAT3( position.m_floats[0] - sx, position.m_floats[1] - sy, position.m_floats[2] );

				}// end case ePrimitiveTriangle2d
				break;
		case  ePrimitiveSquare2d:
				{
						numVertices = 4;
						ppVertices = new  VERTEX[numVertices];
						ppVertices[0].position = DirectX::XMFLOAT3( position.m_floats[0] - sx, position.m_floats[1] + sy, position.m_floats[2] );
						ppVertices[1].position = DirectX::XMFLOAT3( position.m_floats[0] + sx, position.m_floats[1] + sy, position.m_floats[2] );
						ppVertices[2].position = DirectX::XMFLOAT3( position.m_floats[0] + sx, position.m_floats[1] - sy, position.m_floats[2] );
						ppVertices[3].position = DirectX::XMFLOAT3( position.m_floats[0] - sx, position.m_floats[1] - sy, position.m_floats[2] );


						ppVertices[0].texcoord = DirectX::XMFLOAT2( 0, 0 );
						ppVertices[1].texcoord = DirectX::XMFLOAT2( 1, 0 );
						ppVertices[2].texcoord = DirectX::XMFLOAT2( 1, 1 );
						ppVertices[3].texcoord = DirectX::XMFLOAT2( 0, 1 );

				}// end case ePrimitiveSquare2d
				break;
		case  ePrimitiveCircle2d:
				{

				}// end case ePrimitiveCircle2d
				break;
		}// end switch

		return  numVertices;
}


UINT  D3D11Utility::CreatePrimitive( ePrimitiveType  primitiveType, Vector3  position, Vector3  size, VERTEX*&  ppVertices, INT*& pIndices )
{
		FLOAT  sx = size.m_floats[0] / 2.0f;
		FLOAT  sy = size.m_floats[1] / 2.0f;
		FLOAT  sz = size.m_floats[2] / 2.0f;
		UINT  numVertices = 0;

		switch ( primitiveType )
		{
		case  ePrimitiveCube:
				{
						numVertices = 24;
						ppVertices = new  VERTEX[numVertices];
						pIndices = new INT[numVertices * 6];

						// 頂点設定
						{
								// front face
								ppVertices[0].position = DirectX::XMFLOAT3( position.m_floats[0] - sx, position.m_floats[1] + sy, position.m_floats[2] - sz );
								ppVertices[1].position = DirectX::XMFLOAT3( position.m_floats[0] + sx, position.m_floats[1] + sy, position.m_floats[2] - sz );
								ppVertices[2].position = DirectX::XMFLOAT3( position.m_floats[0] + sx, position.m_floats[1] - sy, position.m_floats[2] - sz );
								ppVertices[3].position = DirectX::XMFLOAT3( position.m_floats[0] - sx, position.m_floats[1] - sy, position.m_floats[2] - sz );

								// back face
								ppVertices[4].position = DirectX::XMFLOAT3( position.m_floats[0] + sx, position.m_floats[1] + sy, position.m_floats[2] + sz );
								ppVertices[5].position = DirectX::XMFLOAT3( position.m_floats[0] - sx, position.m_floats[1] + sy, position.m_floats[2] + sz );
								ppVertices[6].position = DirectX::XMFLOAT3( position.m_floats[0] - sx, position.m_floats[1] - sy, position.m_floats[2] + sz );
								ppVertices[7].position = DirectX::XMFLOAT3( position.m_floats[0] + sx, position.m_floats[1] - sy, position.m_floats[2] + sz );

								// left face
								ppVertices[8].position = DirectX::XMFLOAT3( position.m_floats[0] - sx, position.m_floats[1] + sy, position.m_floats[2] + sz );
								ppVertices[9].position = DirectX::XMFLOAT3( position.m_floats[0] - sx, position.m_floats[1] + sy, position.m_floats[2] - sz );
								ppVertices[10].position = DirectX::XMFLOAT3( position.m_floats[0] - sx, position.m_floats[1] - sy, position.m_floats[2] - sz );
								ppVertices[11].position = DirectX::XMFLOAT3( position.m_floats[0] - sx, position.m_floats[1] - sy, position.m_floats[2] + sz );

								// right face
								ppVertices[12].position = DirectX::XMFLOAT3( position.m_floats[0] + sx, position.m_floats[1] + sy, position.m_floats[2] - sz );
								ppVertices[13].position = DirectX::XMFLOAT3( position.m_floats[0] + sx, position.m_floats[1] + sy, position.m_floats[2] + sz );
								ppVertices[14].position = DirectX::XMFLOAT3( position.m_floats[0] + sx, position.m_floats[1] - sy, position.m_floats[2] + sz );
								ppVertices[15].position = DirectX::XMFLOAT3( position.m_floats[0] + sx, position.m_floats[1] - sy, position.m_floats[2] - sz );

								// top face
								ppVertices[16].position = DirectX::XMFLOAT3( position.m_floats[0] - sx, position.m_floats[1] + sy, position.m_floats[2] + sz );
								ppVertices[17].position = DirectX::XMFLOAT3( position.m_floats[0] + sx, position.m_floats[1] + sy, position.m_floats[2] + sz );
								ppVertices[18].position = DirectX::XMFLOAT3( position.m_floats[0] + sx, position.m_floats[1] + sy, position.m_floats[2] - sz );
								ppVertices[19].position = DirectX::XMFLOAT3( position.m_floats[0] - sx, position.m_floats[1] + sy, position.m_floats[2] - sz );

								// bottom face
								ppVertices[20].position = DirectX::XMFLOAT3( position.m_floats[0] + sx, position.m_floats[1] - sy, position.m_floats[2] - sz );
								ppVertices[21].position = DirectX::XMFLOAT3( position.m_floats[0] - sx, position.m_floats[1] - sy, position.m_floats[2] - sz );
								ppVertices[22].position = DirectX::XMFLOAT3( position.m_floats[0] - sx, position.m_floats[1] - sy, position.m_floats[2] + sz );
								ppVertices[23].position = DirectX::XMFLOAT3( position.m_floats[0] + sx, position.m_floats[1] - sy, position.m_floats[2] + sz );
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
								pIndices[12] = 8;
								pIndices[13] = 9;
								pIndices[14] = 10;

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
								// front face
								ppVertices[0].texcoord = DirectX::XMFLOAT2( 0, 0 );
								ppVertices[1].texcoord = DirectX::XMFLOAT2( 1, 0 );
								ppVertices[2].texcoord = DirectX::XMFLOAT2( 1, 1 );
								ppVertices[3].texcoord = DirectX::XMFLOAT2( 0, 1 );

								// back face
								ppVertices[4].texcoord = DirectX::XMFLOAT2( 0, 0 );
								ppVertices[5].texcoord = DirectX::XMFLOAT2( 1, 0 );
								ppVertices[6].texcoord = DirectX::XMFLOAT2( 1, 1 );
								ppVertices[7].texcoord = DirectX::XMFLOAT2( 0, 1 );

								// left face
								ppVertices[8].texcoord = DirectX::XMFLOAT2( 0, 0 );
								ppVertices[9].texcoord = DirectX::XMFLOAT2( 1, 0 );
								ppVertices[10].texcoord = DirectX::XMFLOAT2( 1, 1 );
								ppVertices[11].texcoord = DirectX::XMFLOAT2( 0, 1 );

								// right face
								ppVertices[12].texcoord = Vector2( 0, 0 );
								ppVertices[13].texcoord = Vector2( 1, 0 );
								ppVertices[14].texcoord = Vector2( 1, 1 );
								ppVertices[15].texcoord = Vector2( 0, 1 );

								// top face
								ppVertices[16].texcoord = Vector2( 0, 0 );
								ppVertices[17].texcoord = Vector2( 1, 0 );
								ppVertices[18].texcoord = Vector2( 1, 1 );
								ppVertices[19].texcoord = Vector2( 0, 1 );

								// bottom face
								ppVertices[20].texcoord = Vector2( 0, 0 );
								ppVertices[21].texcoord = Vector2( 1, 0 );
								ppVertices[22].texcoord = Vector2( 1, 1 );
								ppVertices[23].texcoord = Vector2( 0, 1 );
						}

				}// end case ePrimitiveCube
				break;
		case ePrimitivePlane:
				{

				}// end case ePrimitivePlane
				break;
		case  ePrimitiveSphere:
				{

				}// end case ePrimitiveSphere
		case ePrimitiveTriangle2d:
				{
						numVertices = 3;
						ppVertices = new  VERTEX[numVertices];
						ppVertices[0].position = DirectX::XMFLOAT3( position.m_floats[0], position.m_floats[1] + sy, position.m_floats[2] );
						ppVertices[1].position = DirectX::XMFLOAT3( position.m_floats[0] + sx, position.m_floats[1] - sy, position.m_floats[2] );
						ppVertices[2].position = DirectX::XMFLOAT3( position.m_floats[0] - sx, position.m_floats[1] - sy, position.m_floats[2] );

				}// end case ePrimitiveTriangle2d
				break;
		case  ePrimitiveSquare2d:
				{
						numVertices = 4;
						ppVertices = new  VERTEX[numVertices];
						ppVertices[0].position = DirectX::XMFLOAT3( position.m_floats[0] - sx, position.m_floats[1] + sy, position.m_floats[2] );
						ppVertices[1].position = DirectX::XMFLOAT3( position.m_floats[0] + sx, position.m_floats[1] + sy, position.m_floats[2] );
						ppVertices[2].position = DirectX::XMFLOAT3( position.m_floats[0] + sx, position.m_floats[1] - sy, position.m_floats[2] );
						ppVertices[3].position = DirectX::XMFLOAT3( position.m_floats[0] - sx, position.m_floats[1] - sy, position.m_floats[2] );

						ppVertices[0].texcoord = Vector2( 0, 0 );
						ppVertices[1].texcoord = Vector2( 1, 0 );
						ppVertices[2].texcoord = Vector2( 1, 1 );
						ppVertices[3].texcoord = Vector2( 0, 1 );

						pIndices = new INT[6];
						pIndices[0] = 0;
						pIndices[1] = 1;
						pIndices[2] = 2;
						pIndices[3] = 2;
						pIndices[4] = 3;
						pIndices[5] = 0;

				}// end case ePrimitiveSquare2d
				break;
		case  ePrimitiveCircle2d:
				{

				}// end case ePrimitiveCircle2d
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