#pragma once
#include <HAL/HAL.h>

#include "GL/glew.h"

inline void clearErrors()
{
    while (glGetError() != GL_NO_ERROR);
}

inline void getErrors(const std::string_view str)
{
    using namespace std::string_literals;

    GLenum error = glGetError(); 
    if(error != GL_NO_ERROR)
    {
        HAL_ERROR("Opengl Error in expression:\n{}\nErrors: ", str);

        HAL_ERROR("'{}'", error, str);

        auto stack = std::stacktrace::current();

        HAL_ERROR("\nStack Trace:\n", str);
        for (auto se : stack | std::views::drop(1))
            HAL_ERROR(
                (se.source_file() + "("s + std::to_string(se.source_line()) + ")" + se.description() + "\n").c_str());

        __debugbreak();
    }
}

#ifndef NDEBUG
#define GL_CALL(x)                                                                                                     \
    clearErrors();                                                                                                     \
    x;                                                                                                                 \
    getErrors(#x);
#else
#define GL_CALL(x) x;                                                                                                                 
#endif