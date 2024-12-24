#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>

#define __LOG_STR(s) #s
#define __LOG_STR2(s) __LOG_STR(s)

#ifndef NDEBUG
#define LOG_DEBUG(str, ...) printf("[" __FILE__ ":" __LOG_STR2(__LINE__) "](\e[34m%s()\e[0m): \e[35m" str "\e[0m\n", __FUNCTION__, ##__VA_ARGS__)
#else
#define LOG_DEBUG(str, ...)
#endif
#define LOG_INFO(str, ...) printf("[" __FILE__ ":" __LOG_STR2(__LINE__) "](\e[34m%s()\e[0m): \e[32m" str "\e[0m\n", __FUNCTION__, ##__VA_ARGS__)
#define LOG_WARN(str, ...) printf("[" __FILE__ ":" __LOG_STR2(__LINE__) "](\e[34m%s()\e[0m): \e[33m" str "\e[0m\n", __FUNCTION__, ##__VA_ARGS__)
#define LOG_ERROR(str, ...) printf("[" __FILE__ ":" __LOG_STR2(__LINE__) "](\e[34m%s()\e[0m): \e[31m" str "\e[0m\n", __FUNCTION__, ##__VA_ARGS__)

#endif
