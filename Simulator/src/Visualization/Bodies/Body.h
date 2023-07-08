/**
 * @file Body.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief applies a texture to a sphere
 * @version 0.1
 * @date 2023-07-08
 * 
 * 
 */

#ifndef Body_H
#define Body_H

#include "Sphere.h"
#include "../Texture.h"

namespace Visualization
{
    class Body : public Sphere
    {
    public:
        enum class BodyType
        {
            Planet,
            Star,
            Moon
        };

        Body(double radius, int subdivisionLevel, const std::string &texturePath, const std::string &name, BodyType type);
        ~Body();

        const std::vector<glm::vec2> &GetTexCoords() const { return m_texCoords; }
        std::shared_ptr<Texture> GetTexture() { return m_texture; }
    private:
        std::shared_ptr<Texture> m_texture;
        std::vector<glm::vec2> m_texCoords;  // Texture coordinates
        std::string m_name;
        BodyType m_type;

        void computeTextureCoordinates();
    };
}

#endif