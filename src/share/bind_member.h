/***********************************************************************
 * Description: Bind class member function and arguments
 * Autor: Jimmy
 * Email: nieshihua@126.com
 * Create: 2021/8/4.
 * Modify:
 **********************************************************************/
#include <functional>

#pragma once
namespace jm
{

//---------------------------------------------------------------------
// bind_member
//---------------------------------------------------------------------
template<class Return, class Type, class... Args>
std::function<Return(Args...)> bind_member(Type *instance, Return(Type::*method)(Args...))
{
    return [=](Args &&... args) -> Return {
        return (instance->*method)(std::forward<Args>(args)...);
    };
}

}
