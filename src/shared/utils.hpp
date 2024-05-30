#pragma once
#ifndef H___PROJID___SRC_UTILS
#define H___PROJID___SRC_UTILS 1

#include <string>
#include <chrono>
#include <cstdint> // For explicit int types
#include <iostream>

#ifndef _MSVC_LANG
    #include <float.h> // MSVC doesn't like this
#endif


namespace utils
{
    // Give neater names to the forced-width int data types
    // least type defines smallest possible type that is larger than or equal to x (i.e. in 9-bit systems, a 64 would be 72 bits wide)
    // Scoped as to not interfere with other sources
    typedef uint_least64_t    tulong;
    typedef uint_least32_t    tuint;
    typedef uint_least16_t    tushort;
    typedef uint_least8_t     tuchar;

    typedef int_least64_t     tslong;
    typedef int_least32_t     tsint;
    typedef int_least16_t     tsshort;
    typedef int_least8_t      tschar;

    // Give names to forced-width floating point types
    // Let the compiler decide what type to use here, since msvc doesn't support 128-bit floating-point
    //     Line is only included for consistency
    typedef long double       tlongdouble;
#ifdef _MSVC_LANG
    typedef double            tdouble;
    typedef float             tfloat;
#else
    typedef _Float64          tdouble;
    typedef _Float32          tfloat;
#endif

    // Static function utils

    /**
     * @brief Fetches the current time in UTC. Uses the std::chrono::system_clock clock.
     * 
     * @note This needs to be re-written after common implementation of std::chrono::utc_clock
     * 
     * @return std::tm The current time in the UTC timezone
     */
    static inline std::tm fetchUTCTime()
    {
        // Fetch the current local time from the system clock
        // This processing is made redundant by C++ 20 where std::chrono::utc_clock exists
        time_t ttime = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
        
        // Convert the time to UTC and return it
        return *std::gmtime(&ttime);
    }

    /**
     * @brief Formats time in the form of `<dayOfWeek> <monthOfYear> <dayOfMonth> <hourOfDay>:<min>:<sec> <year> <timezone>`
     */
    static inline std::string formatTime(const std::tm &time)
    {
        // Get a pointer to the location on the stack, send it to asctime for formatting
        // Then convert the buffer into a string for easy appending
        std::string s = std::string(std::asctime(&time));

        // Remove the newline character
        s.pop_back();
        
        // Append the timezone
#ifdef _MSVC_LANG
        // It should be UTC, I don't see a workaround for Windows
        return s + " UTC";
#else
        return s + " " + time.tm_zone;
#endif
    }
    
} // End namespace utils

#endif // H___PROJID___SRC_UTILS