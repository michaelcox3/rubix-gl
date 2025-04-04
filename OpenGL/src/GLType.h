#pragma once

#include <GL/glew.h>

// Helper template to map C++ types to OpenGL types.
template<typename T>
struct GLType;

template<>
struct GLType<float> {
    // static: means the function belongs to the class (or struct in this case). Call it w/o creating an object of the type
    // constexpr: tells the compiler that the function should be evaluated at compile time if possible
    static constexpr GLenum Get() { return GL_FLOAT; }
};

template<>
struct GLType<unsigned int> {
    static constexpr GLenum Get() { return GL_UNSIGNED_INT; }
};

template<>
struct GLType<unsigned char> {
    static constexpr GLenum Get() { return GL_UNSIGNED_BYTE; }
};
