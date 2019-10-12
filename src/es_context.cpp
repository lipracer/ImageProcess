#include "es_context.hpp"

GLuint LoadShader ( GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;

   // Create the shader object
   shader = glCreateShader ( type );

   if ( shader == 0 )
   {
      return 0;
   }

   // Load the shader source
   glShaderSource ( shader, 1, &shaderSrc, NULL );

   // Compile the shader
   glCompileShader ( shader );

   // Check the compile status
   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

   if ( !compiled )
   {
      GLint infoLen = 0;

      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

      if ( infoLen > 1 )
      {
         char *infoLog = new char[infoLen];

         glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
         esLogMessage ( "Error compiling shader:\n%s\n", infoLog );

          delete [] infoLog;
      }

      glDeleteShader ( shader );
      return 0;
   }

   return shader;

}

int get_shader_obj(GLuint *pObj, const char *str_v_shader, const char *str_f_shader)
{
    int error_code = 0;
    
    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;
    GLuint programObject = 0;
    GLint linked = 0;
    
    vertexShader = LoadShader(GL_VERTEX_SHADER, str_v_shader);
    fragmentShader = LoadShader(GL_FRAGMENT_SHADER, str_f_shader);
    
    programObject = glCreateProgram();
    
    if(programObject == 0)
    {
        return 0;
    }

    glAttachShader ( programObject, vertexShader);
    glAttachShader ( programObject, fragmentShader);

      // Link the program
    glLinkProgram ( programObject );

      // Check the link status
    glGetProgramiv ( programObject, GL_LINK_STATUS, &linked);

    if (!linked)
    {
         GLint infoLen = 0;

         glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen);

         if(infoLen > 1)
         {
             char *infoLog = new char[infoLen];
             
             glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            
             esLogMessage ( "Error linking program:\n%s\n", infoLog );
            
             delete [] infoLog;
         }

         glDeleteProgram ( programObject );
         return FALSE;
    }
    
    *pObj = programObject;

    glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
    
    return error_code;
}
