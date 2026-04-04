#ifndef PATH_UTILS_H
#define PATH_UTILS_H

#include <string>
#include <filesystem>
#include <stdexcept>

// Platform specific includes
#ifdef _WIN32
    #include <windows.h>
#elif __APPLE__
    #include <mach-o/dyld.h>
#else
    #include <unistd.h>
    #include <limits.h>
#endif

static constexpr int MAX_PATH_LEN = 1024;


class PathUtils {
    
public:
    static std::string getExectuablePath() {
        char buffer[MAX_PATH_LEN];

        #ifdef _WIN32
        GetModuleFileNameA(NULL, buffer, MAX_PATH_LEN);
        #elif __APPLE__
        uint32_t size = sizeof(buffer);
        if (_NSGetExecutablePath(buffer, &size) != 0) {
            throw std::runtime_error("Buffer too small for executable path");
        }
        #else
        ssize_t count = readlink("/proc/self/exe", buffer, MAX_PATH_LEN);
        if (count == -1) {
            throw std::runtime_error("Failed to read executable path");
        }
        buffer[count] = '\0';
        #endif

        return std::filesystem::absolute(std::filesystem::path(buffer)).parent_path().string();
    }


    static std::string getShaderPath(const std::string& shaderName) {
        return getExectuablePath() + "/src/shaders/" + shaderName;
    }

};

#endif