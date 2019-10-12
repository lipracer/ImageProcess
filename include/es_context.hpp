#ifndef __ES_CONTEXT_H__
#define __ES_CONTEXT_H__

#include <memory>
#include "esUtil.h"

const char vShaderStr[] =
   "#version 300 es                          \n"
   "layout(location = 0) in vec4 vPosition;  \n"
   "void main()                              \n"
   "{                                        \n"
   "   gl_Position = vPosition;              \n"
   "}                                        \n";

const char fShaderStr[] =
   "#version 300 es                              \n"
   "precision mediump float;                     \n"
   "out vec4 fragColor;                          \n"
   "void main()                                  \n"
   "{                                            \n"
   "   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
   "}                                            \n";

typedef struct
{
   // Handle to a program object
   GLuint programObject;

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
    }
    virtual ~es_context()
    {
        printf("es_context destruct\n");
        delete m_user_data;
    }
    
    void draw()
    {
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
    
    void shutdown ()
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
