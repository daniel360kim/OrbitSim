/**
 * @file Platform.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-07
 * 
 * 
 */

//Macros to set the platform
#if defined(_WIN32) || defined(_WIN64)
    #define WL_PLATFORM_WINDOWS
#else
    #error Walnut only supports Windows!
#endif