//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <VertexShader.h>
#include  <D3D11Utility.h>
#include  <IDirect3D.h>

//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------
#define  DEFAULT_SHADER  L"Simple.fx"
#define  DEFAULT_FUNCTION  "VSFunc"
#define  DEFAULT_VERSION  "vs_5_0"

//----------------------------------------------------------------------------------
// using namespace
//----------------------------------------------------------------------------------
using namespace D3D11Utility;
using namespace Graphics;


CVertexShader::CVertexShader()
{
		CreateVertexShader( DEFAULT_SHADER );
}


CVertexShader::~CVertexShader()
{
		Release();
}


HRESULT  CVertexShader::CreateVertexShader( LPCWSTR  szFileName )
{
		HRESULT  hr = S_OK;

		// ���_�V�F�[�_�̃R���p�C��
		ID3DBlob*  pVSBlob = NULL;

		hr = CompileShaderFromFile( szFileName, DEFAULT_FUNCTION, DEFAULT_VERSION, &pVSBlob );
		if ( FAILED( hr ) )
		{
				OutputDebugString( TEXT( "<CVertexShader> FAILED CompileShaderFromFile \n" ) );
				return  hr;
		}


		// ���_�V�F�[�_�[�̐���
		hr = pd3dDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVertexShader );
		if ( FAILED( hr ) )
		{
				OutputDebugString( TEXT( "<CVertexShader> FAILED CreateVertexShader \n" ) );
				SAFE_RELEASE( pVSBlob );
				return  hr;
		}


		// ���̓��C�A�E�g����
		hr = CreateInputLayout( pVSBlob );
		SAFE_RELEASE( pVSBlob );
		if ( FAILED( hr ) )
		{
				OutputDebugString( TEXT( "<CVertexShader> FAILED CreateInputLayout \n" ) );
				return  hr;
		}


		return  hr;
}


HRESULT  CVertexShader::CreateInputLayout( ID3DBlob* pVSBlob )
{
		HRESULT  hr = S_OK;


		// ���̓��C�A�E�g�̒�`
		D3D11_INPUT_ELEMENT_DESC  layout[ ] = {
				{ "POSITION",  0,  DXGI_FORMAT_R32G32B32A32_FLOAT,  0,  0,  D3D11_INPUT_PER_VERTEX_DATA,  0 },
		};
		UINT  numElements = ARRAYSIZE( layout );


		hr = pd3dDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pInputLayout );



		return  hr;
}


VOID  CVertexShader::UpdateShader()
{
		pd3dDeviceContext->VSSetShader( m_pVertexShader, nullptr, 0 );

		// ���̓A�Z���u���ɓ��̓��C�A�E�g��ݒ�
		pd3dDeviceContext->IASetInputLayout( m_pInputLayout );
}


VOID  CVertexShader::Release()
{
		SAFE_RELEASE( m_pVertexShader );
		SAFE_RELEASE( m_pInputLayout );
}