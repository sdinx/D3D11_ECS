//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <D3D11Utility\InputMove.h>
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <DIKeyboard.h>


//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  DirectX;


//----------------------------------------------------------------------------------
// static  variables
//----------------------------------------------------------------------------------
ComponentId  InputMove::STATIC_COMPONENT_ID = STATIC_ID_INVALID;


InputMove::InputMove( Transform*  target, Camera*  camera ) :
		m_target( target ),
		m_camera( camera )
{

}


void  InputMove::Update()
{
		Transform  cameraTrans = m_camera->GetComponent<Transform>();
		Vector3&  targetPos = m_target->GetPosition();

		// カメラの前向き（Y軸）ベクトルを取得
		Vector3  cameraForward = cameraTrans.GetForward() * Vector3( 1, 0, 1 ).normalized();

		Vector3  moveForward = cameraForward * Input::KeyVertical() + cameraTrans.GetRight()*Input::KeyHorizontal();
		targetPos += moveForward;
		m_target->GetRotation().set128( moveForward.get128() );
}