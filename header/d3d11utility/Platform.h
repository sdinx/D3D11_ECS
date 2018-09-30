//----------------------------------------------------------------------------------
// file : Platform.h
// desc : ŠÂ‹«‚ÉˆË‘¶‚µ‚È‚¢Œ^’è‹`‚È‚Ç
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_PLATFORM_
#define  _INCLUDED_D3D11_UTILITY_PLATFORM_

//----------------------------------------------------------------------------------
// defined
//----------------------------------------------------------------------------------
/* NOTHING */

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <DirectXMath.h>
#include  <btBulletCollisionCommon.h>


namespace  D3D11Utility
{

		
		struct  Vector2 :public  DirectX::XMFLOAT2
		{
				constexpr  Vector2() :DirectX::XMFLOAT2()
				{
						x = 0.0f;
						y = 0.0f;
				}
				constexpr  Vector2( float  _x, float  _y ) :DirectX::XMFLOAT2( _x, _y ) {}
				explicit  Vector2( _In_reads_( 2 )  const  float*  pArray ) : DirectX::XMFLOAT2( pArray[0], pArray[1] ) {}
				Vector2( DirectX::XMFLOAT2  f2 ) :DirectX::XMFLOAT2( f2 ) {}

				DirectX::XMFLOAT2  operator+( DirectX::XMFLOAT2  f2 )
				{
						DirectX::XMFLOAT2  ret( *this );

						ret.x += f2.x;
						ret.y += f2.y;

						return  ret;
				}
				DirectX::XMFLOAT2  operator-( DirectX::XMFLOAT2  f2 )
				{
						DirectX::XMFLOAT2  ret( *this );

						ret.x -= f2.x;
						ret.y -= f2.y;

						return  ret;
				}
				void  operator=( DirectX::XMFLOAT2  f2 )
				{
						this->x = f2.x;
						this->y = f2.y;
				}
				void  operator+=( DirectX::XMFLOAT2  f2 )
				{
						*this = *this + f2;
				}
				void  operator-=( DirectX::XMFLOAT2  f2 )
				{
						*this = *this - f2;
				}
		};/*
		struct  Vector3 :public  DirectX::XMFLOAT3
		{
				constexpr  Vector3() :DirectX::XMFLOAT3() 
				{
						x = 0.0f;
						y = 0.0f;
						z = 0.0f;
				}
				constexpr  Vector3( DirectX::XMFLOAT3  f3 ) :DirectX::XMFLOAT3( f3 ) {}
				explicit  Vector3( _In_reads_( 3 )  const  float*  pArray ) : DirectX::XMFLOAT3( pArray[0], pArray[1], pArray[2] ) {}
				Vector3( float  x, float  y, float  z ) :DirectX::XMFLOAT3( x, y, z ) {}

				void  Sync( float*  _x, float&  _y, float&  _z )
				{
						_x = &x;
				}
				DirectX::XMFLOAT3  operator+( DirectX::XMFLOAT3  f3 )
				{
						DirectX::XMFLOAT3  ret( *this );

						ret.x += f3.x;
						ret.y += f3.y;
						ret.z += f3.z;

						return  ret;
				}
				DirectX::XMFLOAT3  operator-( DirectX::XMFLOAT3  f3 )
				{
						DirectX::XMFLOAT3  ret( *this );

						ret.x -= f3.x;
						ret.y -= f3.y;
						ret.z -= f3.z;

						return  ret;
				}
				void  operator=( DirectX::XMFLOAT3  f3 )
				{
						this->x = f3.x;
						this->y = f3.y;
						this->z = f3.z;
				}
				void  operator+=( DirectX::XMFLOAT3  f3 )
				{
						*this = *this + f3;
				}
				void  operator-=( DirectX::XMFLOAT3  f3 )
				{
						*this = *this - f3;
				}
		};
		*/

		using  Vector3 = btVector3;
		using  Vector4 = DirectX::XMFLOAT4;
		using  Quaternion = btQuaternion;
		using  Matrix3x3 = btMatrix3x3;
		using  Matrix4x4 = DirectX::XMFLOAT4X4;

		struct  Material
		{
				Vector4  diffuse;
				Vector4  ambient;
				Vector4  emissive;
				Vector4  specular;
				Vector4  shininess;
		};

}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_PLATFORM_