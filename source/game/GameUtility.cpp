//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <game/GameUtility.h>


//----------------------------------------------------------------------------------
// ���O��Ԋ֐� : Random(min, max)
// �����Ɠ����^�̃����_����
// ���l�i�����܂��͐����j��ԋp����
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