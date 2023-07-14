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

        Body(float radius, int subdivisionLevel, const std::string &texturePath, BodyType type);
        ~Body();

        const std::vector<glm::vec2> &GetTexCoords() const { return m_texCoords; }
        std::shared_ptr<Texture> GetTexture() { return m_texture; }

        void changeSubdivisionLevel(int subdivisionLevel);
    private:
        std::shared_ptr<Texture> m_texture;
        std::vector<glm::vec2> m_texCoords;  // Texture coordinates
        BodyType m_type;

        void computeTextureCoordinates();
    };
}

#endif