//----------------------------------------------------------------------------------
// File : Camera.h
// Desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_CAMERA_
#define  _INCLUDED_D3D11_UTILITY_CAMERA_

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\Component.h>
#include  <GameUtility.h>
#include  <memory>

//----------------------------------------------------------------------------------
// namespace  D3D11Utility  class
//----------------------------------------------------------------------------------
namespace  D3D11Utility
{
		// TODO: カメラ表示未確認てか確認できない
		enum  MSG_CAMERA
		{
				MSG_NONE = 0,
				MSG_UPDATE_VIEW,
				MSG_UPDATE_CBUFFER,
		};// enum MSG_CAMERA

		class  Camera :public  Component
		{

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				Camera();
				Camera(Vector3  eyePosition, Vector3  focusPosition, Vector3  upDirection, FLOAT FovAngleY, FLOAT AspectHByW, FLOAT NearZ, FLOAT FarZ );
				~Camera();


		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------
				static  std::unique_ptr<CONSTANTBUFFER>  s_pCBuffer;
				BOOL  m_isEnable = false;

				Matrix4x4  m_view;
				Matrix4x4  m_projection;
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
				static  void  SetConstantBuffer();

				Matrix4x4  GetMatrix4x4Projection()
				{
						return  m_projection;
				}
				Matrix4x4  GetMatrix4x4View()
				{
						return  m_view;
				}
				DirectX::XMMATRIX  GetMatrixProjection()
				{
						return  DirectX::XMLoadFloat4x4( &m_projection );
				}
				DirectX::XMMATRIX  GetMatrixView()
				{
						return  DirectX::XMLoadFloat4x4( &m_view );
				}
				void  HandleMessage( const  GameUtility::Message&  msg );
				void  HandleMessage( const  GameUtility::Message&  msg, Value  var );
				void  SetPosition( Vector3  eyePosition );
				void  SetTarget( Vector3  focusPosition );
				void  SetUp( Vector3  upDirection );
				void  Update();
				void  UpdateView();
				void  UpdateProjection( FLOAT fovAngleY, FLOAT aspectHByW, FLOAT nearZ, FLOAT farZ );
				void  UpdateConstantBuffer();
				void  Release();


		};// class  Camera

}// namespace  D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_CAMERA_