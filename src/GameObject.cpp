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
// コンストラクタで設定したオブジェクト名は不変にする
//----------------------------------------------------------------------------------
GameObject::GameObject( std::string  objectName, std::string  objectTag ) :Mesh( objectName )
{
		SetTag( objectTag );
}

//----------------------------------------------------------------------------------
// class GameUtility::constractor(Scene*, string, string = 0)
// シーンからのオブジェクト生成を行うときのコンストラクタ
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