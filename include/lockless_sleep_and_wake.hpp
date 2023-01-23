#pragma once
///@file lockless_wait_and_wake.hpp
///@brief Piggyback on system-level futex functionality to provide (best-case) lockless thread waiting and waking without relying on c++20
/*!
 * \example example.cpp
 */
#include <atomic>
/*!
 * @brief Sends the current thread to sleep and won't return until woken up. Spurious wake-ups are dealt with internally
 * @param[in] flag                  The memory address that has to change for the thread to wake
 * @param[in] expected              The value flag has to take before the thread wakes, barring timeout
 * @param[in] timeout_milliseconds  Whatever happens the thread will wake after this amount of time in milliseconds (it's not real time, so scheduling and CPU minimum time quantum will affect this, it's ensured it won't return  **before** the timeout is expired).  
 * 0, the default, means no timeout
 */
void sleep(std::atomic_uint32_t &flag, uint32_t expected, uint32_t timeout_milliseconds=0);
/*!
 * @brief Wakes one thread waiting on flag
 * @param[in] flag      The memory address the thread you're waking is waiting on
 * @param[in] desired   The value flag has to take to wake the thread, set in a corresponding [sleep](#sleep) call
 */
void wake_one(std::atomic_uint32_t &flag, uint32_t desired);
/*!
 * @brief Wakes all threads waiting on flag
 * @param[in] flag      The memory address the threads you're waking is waiting on
 * @param[in] desired   The value flag has to take to wake the threads, set in a corresponding #sleep call
 */
void wake_all(std::atomic_uint32_t &flag, uint32_t desired);
