/***********************************************************************
 * Description: Thread
 * Autor: Jimmy
 * Email: nieshihua@126.com
 * Create: 2021/8/4.
 * Modify:
 **********************************************************************/

#pragma once

#include "signals/signals.h"
#include "share/ptr_extend.h"

#include <future>
#include <thread>

namespace jm
{

class Thread : public Connections {
public:
    Thread() : stop_(true) {}

    virtual ~Thread() {
        stop();
        wait();
    }

    /// \brief Create and start the thread
    void start() {
        if (!isFinished()) {
            return;
        }
        auto task = std::make_shared<std::packaged_task<void()>>(std::bind(&Thread::exec, this));
        future_   = task->get_future();
        thread_   = std::make_unique<std::thread>([task]() { (*task)(); });
    }

    /// \brief If use 'isRunning' to determine whether the loop body ends,
    /// Need to call this function to end the loop body
    void stop() {
        stop_ = true;
    }

    /// \brief Wait for the thread to end,
    /// Maybe first call the 'stop' function to end loop body if you have
    void wait(uint32_t wait_ms = MAX_WAIT_TIME) {
        if (!thread_) {
            return;
        }

        if (wait_ms == MAX_WAIT_TIME && thread_->joinable()) {
            thread_->join();
            return;
        }

        int elapsed_ms = 0;
        while (!isFinished() && ++elapsed_ms < wait_ms) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    /// \brief Whether the main function is running
    bool isRunning() { return !stop_; }

    /// \brief Whether the thread ends
    /// waiting for the thread end itself, can't force abort it
    bool isFinished() {
        if (!thread_) {
            return true;
        }
        return future_.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready;
    }

    ///\brief Get the thread id
    std::thread::native_handle_type getID() { return thread_->native_handle(); }

public:
    Signal<void()> started;
    Signal<void()> finished;

    const static uint32_t MAX_WAIT_TIME = std::numeric_limits<uint32_t>::max();

protected:
    ///\brief The derived class should relealize this function
    virtual void run() = 0;

private:
    void exec() {
        stop_ = false;
        started();
        run();
        finished();
        stop_ = true;
    }

private:
    std::unique_ptr<std::thread> thread_;

    std::future<void> future_;
    std::atomic<bool> stop_;
};

} // namespace jm