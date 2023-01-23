# lockless_wait_and_wake
## Objective
This library aims to provide a best-case lockless wait and wake functionality for multithreaded programs, by exploiting futexes, available in multiple common OSes.  


## Rationale  
While C++20 implements an `std::atomic<T>::wait` that provides similar functonality, it also comes with the baggage of having to use C++20, while this library only requires C++11.

## Usage
One or multiple threads `sleep` on an atomic_uint32 variable until notified by one of `wake_one` or `wake_all` called by other threads, or the optional timeout parameter has elapsed.  
[Spurious wakeups](https://en.wikipedia.org/wiki/Spurious_wakeup) are dealt with internally.  
`sleep` allows one to specify a desired value for the variable to take, which enables selective wake-up by giving threads different desired values (or rather, the other threads wake up but immediately go back to sleep without every returning from the `sleep` call).

## Support
As the library relies on OS support for futexes, it only works on Linux, Windows and OpenBSD.  
Other OSes (MacOS is a notably outlier) don't, so you're probably gonna have to use C++20's std::<T>atomic::wait

