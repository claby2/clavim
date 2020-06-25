#ifndef CROSS_PLATFORM_HPP
#define CROSS_PLATFORM_HPP
#include <string>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#define dirSep "\\" // Directory separator for windows
#define PATH_MAX MAX_PATH

/*
Get exe path, windows
*/
std::string exePath() {
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}
#elif __linux__
#include <unistd.h>
#include <limits.h>
#define _getcwd getcwd
#define dirSep "/" // Directory separator

/*
Get exe path, linux
*/
std::string exePath() {
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    std::string path = std::string(result, (count > 0) ? count : 0);
    path = path.substr(0, path.find_last_of('/'));
    return path;
}
#else 
#if (defined (__APPLE__) && defined (__MACH__))
#include <mach-o/dyld.h>
#include <sys/syslimits.h>
#include <unistd.h>
#include <limits.h>
#define _getcwd getcwd
#define dirSep "/" // Directory separator

/*
Get exe path, apple
*/
std::string exePath(){
    char buf[PATH_MAX];
    uint32_t size = sizeof(buf);
    if(_NSGetExecutablePath(buf, &size) == 0) {
        std::string path = buf;
        path = path.substr(0, path.find_last_of('/'));
        return path;
    }
}
#else
    #error PLATFORM NOT SUPPORTED
#endif
#endif

#endif