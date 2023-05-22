#pragma once

class TexturedQuadShader : public GL::AbstractShaderProgram {
public:

    typedef GL::Attribute<0, Vector2> Position;
    typedef GL::Attribute<1, Vector2> TextureCoordinates;

    explicit TexturedQuadShader() {
        MAGNUM_ASSERT_GL_VERSION_SUPPORTED(GL::Version::GL330);

        const Utility::Resource rs{ "texturedquad-data" };

        GL::Shader vert{ GL::Version::GL330, GL::Shader::Type::Vertex };
        GL::Shader frag{ GL::Version::GL330, GL::Shader::Type::Fragment };

        vert.addSource(rs.getString("TexturedQuadShader.vert"));
        frag.addSource(rs.getString("TexturedQuadShader.frag"));

        CORRADE_INTERNAL_ASSERT_OUTPUT(vert.compile() && frag.compile());

        attachShaders({ vert, frag });

        CORRADE_INTERNAL_ASSERT_OUTPUT(link());

        _colorUniform = uniformLocation("color");
        setUniform(uniformLocation("textureData"), TextureUnit);
    }

    TexturedQuadShader& setColor(const Color3& color) {
        setUniform(_colorUniform, color);
        return *this;
    }

    TexturedQuadShader& bindTexture(GL::Texture2D& texture) {
        texture.bind(TextureUnit);
        return *this;
    }

private:
    enum : Int { TextureUnit = 0 };

    Int _colorUniform;
};