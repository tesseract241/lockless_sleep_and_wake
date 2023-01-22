
#pragma once
#include <atomic>
void sleep(std::atomic_uint32_t &flag, uint32_t expected, uint32_t timeout_milliseconds=0);
void wake_one(std::atomic_uint32_t &flag, uint32_t desired);
void wake_all(std::atomic_uint32_t &flag, uint32_t desired);
