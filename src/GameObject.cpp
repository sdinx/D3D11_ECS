//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <GameUtility.h>
#include  <GameObject.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  GameUtility;

//----------------------------------------------------------------------------------
// class GameUtility::constractor(string, string = 0)
// �R���X�g���N�^�Őݒ肵���I�u�W�F�N�g���͕s�ςɂ���
//----------------------------------------------------------------------------------
GameObject::GameObject( std::string  objectName, std::string  objectTag ) :Mesh( objectName )
{
		SetTag( objectTag );
}

//----------------------------------------------------------------------------------
// class GameUtility::constractor(Scene*, string, string = 0)
// �V�[������̃I�u�W�F�N�g�������s���Ƃ��̃R���X�g���N�^
//----------------------------------------------------------------------------------
GameObject::GameObject( D3D11Utility::Scene*  pParentsScene, std::string  objectName, std::string  objectTag ) :Mesh( objectName )
{
		SetTag( objectTag );
		m_pParentsScene = pParentsScene;
}


GameObject::~GameObject()
{
		Release();
}


VOID  GameObject::Release()
{

}