/***********************************************************************
 * Description: Thread pool
 * Autor: Jimmy
 * Email: nieshihua@126.com
 * Create: 2021/3/4.
 * Modify:
 **********************************************************************/
#pragma once

#include "thread_pool.h"

namespace jm {

class SingletonThreadPool : public ThreadPool {
public:
    static SingletonThreadPool *getInstance() {
        static SingletonThreadPool instance;
        return &instance;
    }

private:
    SingletonThreadPool() : ThreadPool(4) ///please modify this size
    {
    }
};

}
