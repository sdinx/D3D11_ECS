#ifndef _INCLUDED_SINGLETON_
#define _INCLUDED_SINGLETON_


#include <memory>


template<typename T>
struct  singleton_allocator
{
		using  type = std::allocator<T>;
};


template<typename T>  class  _Singleton
{
protected:
		_Singleton() {}

public:
		virtual  ~_Singleton() {}

		_Singleton( _Singleton const& ) = delete;
		_Singleton&  operator=( _Singleton const& ) = delete;

private:
		using  allocator = typename singleton_allocator<T>::type;
		static  std::weak_ptr<T> _instance;

public:
		static  std::shared_ptr<T> GetInstance()
		{
				auto retPtr = _instance.lock();
				if ( !retPtr ) {
						retPtr = std::shared_ptr<T>( new T{} );
						_instance = std::weak_ptr<T>( retPtr );
						return retPtr;
				}

				return _instance.lock();
		}
};// class _Singleton


template<typename T>  std::weak_ptr<T>  _Singleton<T>::_instance;


#endif // !_INCLUDED_SINGLETON_