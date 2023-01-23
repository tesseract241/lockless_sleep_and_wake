#include <atomic>
#include <lockless_wait_and_wake.hpp>
#if defined(__linux__)

#include <linux/futex.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>

void sleep(std::atomic_uint32_t &flag, uint32_t expected, uint32_t timeout_milliseconds){
    timespec timeout;
    timespec *timeout_p;
    if(timeout_milliseconds){
        timeout.tv_sec  = timeout_milliseconds / 1000;
        timeout.tv_nsec = (timeout_milliseconds % 1000) * 1000;
        timeout_p = &timeout;
    } else timeout_p = NULL;
    while(expected==flag.load()){
        syscall(SYS_futex, &flag, FUTEX_WAIT_PRIVATE, &expected, timeout_p, NULL, NULL);
    }
}
void wake_one(std::atomic_uint32_t &flag, uint32_t desired){
    flag.store(desired);
    syscall(SYS_futex, &flag, FUTEX_WAKE_PRIVATE, 1, NULL, NULL, NULL);
}
void wake_all(std::atomic_uint32_t &flag, uint32_t desired){
    flag.store(desired);
    syscall(SYS_futex, &flag, FUTEX_WAKE_PRIVATE, UINT32_MAX, NULL, NULL, NULL);
}

#elif defined(__OpenBSD__)

#include <sys/time.h>
#include <sys/futex.h>

void sleep(std::atomic_uint32_t &flag, uint32_t expected, uint32_t timeout_milliseconds){
    timespec timeout;
    timespec *timeout_p;
    if(timeout_milliseconds){
        timeout.tv_sec  = timeout_milliseconds / 1000;
        timeout.tv_nsec = (timeout_milliseconds % 1000) * 1000;
        timeout_p = &timeout;
    } else timeout_p = NULL;
    while(expected==flag.load()){
        futex((volatile uint32_t*) &flag, FUTEX_WAIT, expected, timeout_p, NULL);
    }
}
void wake_one(std::atomic_uint32_t &flag, uint32_t desired){
    flag.store(desired);
    futex((volatile uint32_t*) &flag, FUTEX_WAKE, 1, NULL, NULL);
}
void wake_all(std::atomic_uint32_t &flag, uint32_t desired){
    flag.store(desired);
    futex((volatile uint32_t*) &flag, FUTEX_WAKE, UINT32_MAX, NULL, NULL);
}

#elif defined(_WIN32)

#include <Windows.h>
#pragma comment(lib, "Synchronization.lib")

void sleep(std::atomic_uint32_t &flag, uint32_t expected, uint32_t timeout_milliseconds){
    uint32_t timeout = timeout_milliseconds ? timeout_milliseconds : INFINITE;
    while(expected==flag.load()){
        WaitOnAddress(&flag, &expected, 4, timeout);
    }
}
void wake_one(std::atomic_uint32_t &flag, uint32_t desired){
    flag.store(desired);
    WakeByAddressSingle(&flag);
}
void wake_all(std::atomic_uint32_t &flag, uint32_t desired){
    flag.store(desired);
    WakeByAddressAll(&flag);
}

#else
#error "lockless_wait_and_wake only supports Linux, Windows and OpenBSD. Sorry."
#endif
