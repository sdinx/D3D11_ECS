//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <game/GameUtility.h>


//----------------------------------------------------------------------------------
// 名前空間関数 : Random(min, max)
// 引数と同じ型のランダムな
// 数値（実数または整数）を返却する
//----------------------------------------------------------------------------------
template<typename  T>
T  GameUtility::Random( T  min, T  max )
{
		T  ret = 0;

		static  std::random_device  rnd;
		static  std::mt19937  mt( rnd() );


		if ( typeid( min ) == typeid( double ) || typeid( min ) == typeid( float ) )
		{
				std::uniform_real_distribution<T>  random( min, max );
				ret = random( mt );
				random.reset();
		}
		else // signed or unsigned
		{
				std::uniform_int_distribution<T>  random( min, max );
				ret = random( mt );
				random.reset();
		}


		return  ret;
}