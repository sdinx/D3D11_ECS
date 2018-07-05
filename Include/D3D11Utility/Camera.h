//----------------------------------------------------------------------------------
// File : Camera.h
// Desc : 
//----------------------------------------------------------------------------------


#ifndef  _INCLUDED_D3D11_UTILITY_CAMERA_
#define  _INCLUDED_D3D11_UTILITY_CAMERA_

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Component.h>
#include  <D3D11Utility\D3D11Utility.h>
#include  <GameUtility.h>
#include  <memory>

//----------------------------------------------------------------------------------
// namespace  D3D11Utility  class
//----------------------------------------------------------------------------------
namespace  D3D11Utility
{


		class  Camera :public  Component
		{

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------

				Camera();
				Camera(Vector3  eyePosition, Vector3  focusPosition, Vector3  upDirection, FLOAT FovAngleY, FLOAT AspectHByW, FLOAT NearZ, FLOAT FarZ );
				~Camera();

		enum  MSG_CAMERA
		{
				MSG_NONE = 0,
				MSG_UPDATE_VIEW,
				MSG_UPDATE_CBUFFER,
				MSG_UPDATE_ALL,
		};// enum MSG_CAMERA

		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------

				static  ComponentId  STATIC_COMPONENT_ID;
				static  const  UINT  s_nConstantBufferSlot = 0;
				static  ID3D11Buffer  *s_pConstantBuffer;
				BOOL  m_isEnable = false;

				Matrix4x4  m_view;
				Matrix4x4  m_projection;
				Vector3  m_eyePosition;
				Vector3  m_focusTarget;
				Vector3  m_upDirection;


		public:
				//----------------------------------------------------------------------------------
				// public variables
				//----------------------------------------------------------------------------------
				/* NOTHING */

		private:
				//----------------------------------------------------------------------------------
				// private methods
				//----------------------------------------------------------------------------------


		public:
				//----------------------------------------------------------------------------------
				// public methods
				//----------------------------------------------------------------------------------

				static  void  SetConstantBuffer();
				static  ComponentId  GetStaticComponentId()
				{
						return  STATIC_COMPONENT_ID;
				}
				static  void  SetStaticComponentId( ComponentId  id )
				{
						if ( STATIC_COMPONENT_ID == STATIC_ID_INVALID )
						{
								STATIC_COMPONENT_ID = id;
								// TODO: need  to output debug string.
						}
				}

				Matrix4x4&  GetMatrix4x4Projection()
				{
						return  m_projection;
				}
				Matrix4x4&  GetMatrix4x4View()
				{
						return  m_view;
				}
				DirectX::XMMATRIX&  GetMatrixProjection()
				{
						return  DirectX::XMLoadFloat4x4( &m_projection );
				}
				DirectX::XMMATRIX&  GetMatrixView()
				{
						return  DirectX::XMLoadFloat4x4( &m_view );
				}
				void  HandleMessage( const  GameUtility::Message&  msg );
				void  HandleMessage( const  GameUtility::Message&  msg, Value  var );
				Vector3&  GetPosition()
				{
						return  m_eyePosition;
				}
				Vector3&  GetTarget()
				{
						return  m_focusTarget;
				}
				Vector3&  GetUp()
				{
						return  m_upDirection;
				}
				void  SetPosition( Vector3  eyePosition );
				void  SetTarget( Vector3  focusPosition );
				void  SetRotation( float  x, float  y, float  z );
				void  SetUp( Vector3  upDirection );
				void  Update();
				void  UpdateView();
				void  UpdateProjection( FLOAT fovAngleY, FLOAT aspectHByW, FLOAT nearZ, FLOAT farZ );
				void  UpdateConstantBuffer();
				void  Release();


		};// class  Camera

}// namespace  D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_CAMERA_