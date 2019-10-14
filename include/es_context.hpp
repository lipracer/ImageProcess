#ifndef __ES_CONTEXT_H__
#define __ES_CONTEXT_H__

#include <memory>
#include "esUtil.h"

const char vShaderStr[] =
   "#version 300 es                            \n"
   "layout(location = 0) in vec4 a_position;   \n"
   "layout(location = 1) in vec2 a_texCoord;   \n"
   "out vec2 v_texCoord;                       \n"
   "void main()                                \n"
   "{                                          \n"
   "   gl_Position = a_position;               \n"
   "   v_texCoord = a_texCoord;                \n"
   "}                                          \n";

const char fShaderStr[] =
   "#version 300 es                                     \n"
   "precision mediump float;                            \n"
   "in vec2 v_texCoord;                                 \n"
   "layout(location = 0) out vec4 outColor;             \n"
   "uniform sampler2D s_texture;                        \n"
   "void main()                                         \n"
   "{                                                   \n"
   "  outColor = texture( s_texture, v_texCoord );      \n"
   "}                                                   \n";

typedef struct
{
   // Handle to a program object
   GLuint programObject;
    
    // Sampler location
    GLint samplerLoc;

    // Texture handle
    GLuint textureId;

} UserData;

int get_shader_obj(GLuint *pObj, const char *str_v_shader, const char *str_f_shader);

template<typename T>
class es_context : public ESContext
{
public:
    es_context()
    {
        printf("es_context construct\n");
        m_user_data = new T();
#ifdef __APPLE__
        esCreateWindow(this, "", 0, 0, 0); //other platform later support
#endif

        get_shader_obj(&(m_user_data->programObject), vShaderStr, fShaderStr);
        
        m_user_data->samplerLoc = glGetUniformLocation(m_user_data->programObject, "s_texture");
//        CreateSimpleTexture2D();
    }
    virtual ~es_context()
    {
        printf("es_context destruct\n");
        delete m_user_data;
    }
    
    void draw()
    {
        return draw(nullptr, 0, 0, 0);
        GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f,
                                 -0.5f, -0.5f, 0.0f,
                                 0.5f, -0.5f, 0.0f
                              };

        // Set the viewport
        glViewport(0, 0, this->width, this->height);

        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the program object
        glUseProgram(m_user_data->programObject);

        // Load the vertex data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
        glEnableVertexAttribArray(0);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    
    void CreateSimpleTexture2D(char *psrc, int width, int height)
    {
       // 2x2 Image, 3 bytes per pixel (R, G, B)
//       GLubyte pixels[4 * 3] =
//       {
//          255,   0,   0, // Red
//            0, 255,   0, // Green
//            0,   0, 255, // Blue
//          255, 255,   0  // Yellow
//       };

       // Use tightly packed data
       glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

       // Generate a texture object
       glGenTextures(1, &(m_user_data->textureId));

       // Bind the texture object
       glBindTexture(GL_TEXTURE_2D, m_user_data->textureId);

       // Load the texture
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, psrc);

       // Set the filtering mode
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    }
    
    void draw(char *pSrc, int width, int height, int channel)
    {

        GLfloat vVertices[] = { -0.5f,  0.5f, 0.0f,  // Position 0
                                 0.0f,  0.0f,        // TexCoord 0
            
                                -0.5f, -0.5f, 0.0f,  // Position 1
                                 0.0f,  1.0f,        // TexCoord 1
            
                                 0.5f, -0.5f, 0.0f,  // Position 2
                                 1.0f,  1.0f,        // TexCoord 2
            
                                 0.5f,  0.5f, 0.0f,  // Position 3
                                 1.0f,  0.0f         // TexCoord 3
                              };
        GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

        // Set the viewport
        glViewport(0, 0, this->width, this->height);

        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the program object
        glUseProgram(m_user_data->programObject);

        // Load the vertex position
        glVertexAttribPointer(0, 3, GL_FLOAT,
                                GL_FALSE, 5 * sizeof(GLfloat), vVertices);
        // Load the texture coordinate
        glVertexAttribPointer(1, 2, GL_FLOAT,
                                GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3]);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        // Bind the texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_user_data->textureId);

        // Set the sampler texture unit to 0
        glUniform1i(m_user_data->samplerLoc, 0);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
    }
    
    void shutdown()
    {
        glDeleteProgram (m_user_data->programObject);
    }
    
private:
public:
private:
    T *m_user_data;
    
};
typedef es_context<UserData> LESContext;
typedef std::shared_ptr<LESContext> ALESContext;

#endif
