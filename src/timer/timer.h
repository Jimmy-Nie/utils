/***********************************************************************
 * Description: Timer, contains simple timer and Timing trigger
 * Autor: Jimmy
 * Email: nieshihua@126.com
 * Create: 2021/8/27.
 * Modify:
 **********************************************************************/

#pragma once

#include <memory>
#include <functional>
#include <chrono>
#include <thread>

namespace jm
{

class TimerImpl;

/// \brief Timer class that invokes a callback on a specified interval
class Timer
{
public:
    /// \brief Creates a timer object
    Timer();

    /// \brief Returns true if the timer repeats until it is stopped
    bool is_repeating() const;

    /// \brief Returns the current timeout. In milliseconds.
    unsigned int get_timeout() const;

    /// \brief Callback invoked every time the timer interval occurs
    std::function<void()> &func_expired();

    /// \brief Starts the timer. Timeout in milliseconds.
    void start(unsigned int timeout, bool repeat = true);

    /// \brief Stop the timer.
    void stop();

private:
    std::shared_ptr<TimerImpl> impl;
};


/**
 * \brief: the simple timer
 */
class SimpleTimer {
public:
    using clock = std::chrono::steady_clock;
public:
    SimpleTimer() { start(); }

    ///\brief: (re)start the timer
    void start() { m_start_ = clock::now(); }

    ///\brief: elapsed (nano/micro/milli/.)seconds since start
    uint32_t elapsed_s() {
        return static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::seconds>
                (clock::now() - m_start_).count());
    }

    uint32_t elapsed_ms() {
        return static_cast<unsigned int>(std::chrono::duration_cast<std::chrono::milliseconds>
                (clock::now() - m_start_).count());
    }

    uint64_t elapsed_us() {
        return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>
                (clock::now() - m_start_).count());
    }

    uint64_t elapsed_ns() {
        return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>
                (clock::now() - m_start_).count());
    }

    ///\brief: sleep for period duration
    void sleep_s(uint32_t secs) {
        std::this_thread::sleep_for(std::chrono::seconds(secs));
    }

    void sleep_ms(uint32_t msecs) {
        std::this_thread::sleep_for(std::chrono::milliseconds(msecs));
    }

private:
    clock::time_point m_start_;
};

}
