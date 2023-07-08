/**
 * @file Texture.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-06
 *
 *
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>

namespace Visualization
{
    class Texture
    {
    public:
        Texture(const std::string &filepath);
        ~Texture();

        void Bind() const;
        void Unbind() const;
        glm::vec3 Sample(float u, float v) const;

    private:
        unsigned int m_TextureID;
        int m_Width;
        int m_Height;
    };
}

#endif