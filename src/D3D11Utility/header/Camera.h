//----------------------------------------------------------------------------------
// File : Camera.h
// Desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_CAMERA_
#define  _INCLUDED_D3D11_UTILITY_CAMERA_

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <D3D11Utility.h>

//----------------------------------------------------------------------------------
// namespace  D3D11Utility  class
//----------------------------------------------------------------------------------
namespace  D3D11Utility
{

		class  Camera
		{

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				Camera();
				Camera(Vector3  eyePosition, Vector3  focusPosition, Vector3  upDirection, float FovAngleY, float AspectHByW, float NearZ, float FarZ );
				~Camera();


		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------
				static  CONSTANTBUFFER*  s_pCBuffer;
				BOOL  m_isEnable = FALSE;

				DirectX::XMMATRIX  m_view;
				DirectX::XMMATRIX  m_projection;
				DirectX::XMVECTOR  m_eyePosition;
				DirectX::XMVECTOR  m_focusTarget;
				DirectX::XMVECTOR  m_upDirection;


		public:
				//----------------------------------------------------------------------------------
				// public variables
				//----------------------------------------------------------------------------------


		private:
				//----------------------------------------------------------------------------------
				// private methods
				//----------------------------------------------------------------------------------


		public:
				//----------------------------------------------------------------------------------
				// public methods
				//----------------------------------------------------------------------------------
				static  VOID  SetConstantBuffer();
				VOID  UpdateView();
				VOID  UpdateProjection( float fovAngleY, float aspectHByW, float nearZ, float farZ );
				VOID  UpdateConstantBuffer();
				VOID  SetPosition( Vector3  eyePosition );
				VOID  SetTarget( Vector3  focusPosition );
				VOID  SetUp( Vector3  upDirection );
				DirectX::XMMATRIX  GetMatrixProjection() { return  m_projection; }
				DirectX::XMMATRIX  GetMatrixView() {return  m_view; }
				Matrix4x4  GetMatrix4x4Projection();
				Matrix4x4  GetMatrix4x4View();
				VOID  Release();


		};// class  Camera

}// namespace  D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_CAMERA_