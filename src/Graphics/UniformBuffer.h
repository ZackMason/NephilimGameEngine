#pragma once
#include <glad/glad.h>
#include <string>

#include "Bindable.h"

class UniformBufferObject : Bindable
{
public:
    UniformBufferObject(const std::string& name) : m_Name(name)
    {
    }
    ~UniformBufferObject() = default;

public:
    void Bind(Graphics& gfx) override
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    }
    void Unbind(Graphics& gfx) override
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    template <typename T>
    void Init(int binding, GLenum type = GL_STATIC_DRAW)
    {
        glGenBuffers(1, &m_ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(T), 0, type);

        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    template <typename T>
    void SetBuffer(T& data)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

    }
public:
    unsigned int m_ubo;
    std::string m_Name;
};
