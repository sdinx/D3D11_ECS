//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\System\IDirect3D.h>
#include  <string>


ID3D11Device*  D3D11Utility::pd3dDevice = NULL;
ID3D11DeviceContext*  D3D11Utility::pd3dDeviceContext = NULL;


HRESULT  D3D11Utility::CompileShaderFromFile( LPCWSTR  szFileName, LPCSTR  szEntryPoint, LPCSTR  szShaderModel, ID3DBlob**  ppBlobOut )
{
		HRESULT  hr = S_OK;

		// �R���p�C���t���O
		DWORD  dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
		

#if  defined(DEBUG)  ||  defined(_DEBUG)
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif // defined(DEBUG)  ||  defined(_DEBUG)

#if  defined(NDEBUG)  ||  defined(_NDEBUG)
		dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif // defined(NDEBUG)  ||  defined(_NDEBUG)


		ID3DBlob*  pErrorBlob = NULL;


		// �t�@�C������V�F�[�_���R���p�C��
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
				std::string  directory = "../";
				directory += (LPSTR)szFileName;

				hr = D3DCompileFromFile(
						( LPCWSTR ) directory.c_str(),
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


		SAFE_RELEASE( pErrorBlob );


		return  hr;
}


HRESULT  D3D11Utility::CreateConstantBuffer( ID3D11Buffer**  ppCB, UINT&  nOutSlot, size_t  byteWidth )
{
		static  UINT  maxConstantBuffer = 0;
		HRESULT  hr = S_OK;


		// �萔�o�b�t�@�̐ݒ�
		D3D11_BUFFER_DESC  bd;
		ZeroMemory( &bd, sizeof( D3D11_BUFFER_DESC ) );
		bd.ByteWidth = byteWidth;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;


		// �萔�o�b�t�@�̐ݒ�
		hr = pd3dDevice->CreateBuffer( &bd, NULL, ppCB );
		if ( FAILED( hr ) )
		{
				OutputDebugString( TEXT( "<D3D11Utility> FAILED CreateBuffer (constant buffer) \n" ) );
				return  hr;
		}
		nOutSlot = maxConstantBuffer;
		maxConstantBuffer++;


		return  hr;
}

void  D3D11Utility::SetD3DDevices( ID3D11Device*  pDevice, ID3D11DeviceContext*  pDeviceContext )
{
		D3D11Utility::pd3dDevice = pDevice;
		D3D11Utility::pd3dDeviceContext = pDeviceContext;
}


FLOAT  D3D11Utility::GetAspectRatio()
{
		auto  d3d11 = _Singleton<IDirect3D>::GetInstance();

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