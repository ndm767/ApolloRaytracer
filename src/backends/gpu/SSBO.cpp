#include "SSBO.hpp"

template <typename T>
SSBO<T>::SSBO(std::vector<T> data, int _binding) {
    binding = _binding;
    size = data.size();

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(T), data.data(),
                 GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

template <typename T>
SSBO<T>::~SSBO() {
    glDeleteBuffers(1, &buffer);
}

template <typename T>
std::vector<T> SSBO<T>::read() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);

    std::vector<T> ret(size);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size * sizeof(T),
                       ret.data());

    return ret;
}