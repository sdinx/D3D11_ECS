//----------------------------------------------------------------------------------
// file: Collider.h
// desc: 衝突判定フラグや中心点を持った基底クラス
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
/* NOTHING */

#ifndef  _INCLUDED_D3D11_UTILITY_PHYSICAL_COLLIDER_
#define  _INCLUDED_D3D11_UTILITY_PHYSICAL_COLLIDER_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility.h>


namespace  D3D11Utility
{
		namespace  Physical
		{
				class  BoxCollider;
				class  SphereCollider;

				class  Collider
				{

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						Collider()
						{}
						Collider( Vector3  center, Vector3  radius ) :
								m_center( center ),
								m_radius( radius )
						{}
						virtual  ~Collider()
						{}

				protected:
						//----------------------------------------------------------------------------------
						// protected variables
						//----------------------------------------------------------------------------------

						bool  isHit;// フレーム毎に初期化
						Vector3  m_center;// Transformのpositionを原点とした相対位置
						Vector3  m_radius;

				public:
						//----------------------------------------------------------------------------------
						// public variables
						//----------------------------------------------------------------------------------
						/* NOTHING */

				private:
						//----------------------------------------------------------------------------------
						// private methods
						//----------------------------------------------------------------------------------
						/* NOTHING */

				public:
						//----------------------------------------------------------------------------------
						// public methods
						//----------------------------------------------------------------------------------

						static  bool  IsIntersect( const  BoxCollider  a, const  BoxCollider  b );
						static  bool  IsIntersect( const  SphereCollider  a, const  SphereCollider  b );
						static  bool  IsIntersect( const  BoxCollider  a, const  SphereCollider  b );
						static  bool  IsIntersect( const  SphereCollider  a, const  BoxCollider  b );
						
				};// class Collider

		}// namespace Physical
}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_PHYSICAL_COLLIDER_