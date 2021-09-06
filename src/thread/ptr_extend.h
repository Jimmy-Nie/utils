#pragma once

#include <memory>
#if defined(__cplusplus) && __cplusplus >= 201103L &&__cplusplus < 201402L
namespace std {

template<typename T, typename ...Args>
std::unique_ptr<T> make_unique(Args &&...args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}
#endif

#if defined(__cplusplus) && __cplusplus >= 201103L
#define ALLOW_MAKE_SHARED(x) friend void __gnu_cxx::new_allocator<x>::construct<x>(x*);
#elif defined(_WIN32) || defined(WIN32)
#if defined(_MSC_VER) && _MSC_VER >= 1800
#define ALLOW_MAKE_SHARED(x) friend class std::_Ref_count_obj<x>;
#else
#error msc version does not suport c++11
#endif
#else
#error implement for platform
#endif