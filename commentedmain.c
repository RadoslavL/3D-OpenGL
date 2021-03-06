#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include<stdio.h>
#include<stdlib.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<stdbool.h>
#include<string.h>
#include<stb/stb_image.h>
#include<cglm/cglm.h>

const unsigned int width = 1054;
const unsigned int height = 1057;
GLint uniColor;
GLint xploc;
GLint yploc;
void key_callback();

int main(){
   if(!glfwInit()){
      printf("Failed to initialize glfw!\n");
      return 1;
   }else{
      printf("Successfully initialized glfw!\n");
   }
   //Set up GLFW
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
   GLFWwindow* window = glfwCreateWindow(width, height, "Title", NULL, NULL);
   glfwSetKeyCallback(window, key_callback);
   if(!window){
      printf("Window creation failed!\n");
      return 1;
   }else{
      printf("Window created successfully!\n");
   }
   glfwMakeContextCurrent(window);
   //Initialize GLEW
   glewExperimental = GL_TRUE;
   if(glewInit() != GLEW_OK){
      printf("Failed to initialize glew!\n");
      return 1;
   }else{
      printf("Successfully initialized glew!\n");
   } 
   //Set the swap interval to 1 millisecond
   glfwSwapInterval(1);
   //Set the vertices of the square
   float Vertices[] = {
     -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
      0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
      0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f
   };
   
   //Set indices to make the square work
   GLuint indices[] = {
      0, 2, 1,
      0, 3, 2
   };
   
   //Ignore this - it was the original square, but then I realized how I actually needed to make it work
   float VerticesSquare[] = {
      -0.5f, 0.5f, 0.0f,
       0.5f, 0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
      -0.5f, -0.5f, 0.0f
   };
   printf("GLfloat variable called successfully\n"); 
   //Create, compile and link the shaders to the shader program
   GLuint ShaderProgram = glCreateProgram();
   GLuint ShaderObjv = glCreateShader(GL_VERTEX_SHADER);
   GLuint ShaderObjf = glCreateShader(GL_FRAGMENT_SHADER);
   const GLchar* pvs[1];
   const GLchar* pfs[1];
   //Set the shader code
   const char* pVertexShaderText = "#version 330 core\n"
   "layout(location = 0) in vec3 Position;\n"
   "layout(location = 1) in vec3 incolor;\n"
   "layout(location = 2) in vec2 Tex;\n"
   "out vec3 outcolor;\n"
   "out vec2 texcoords;\n"
   "uniform uint xp;\n"
   "uniform uint yp;\n"
   "uniform mat4 model;\n"
   "uniform mat4 view;\n"
   "uniform mat4 proj;\n"
   "void main(){\n"
   "   gl_Position = proj * view * model * vec4(Position, 1.0f);\n"
   "outcolor = incolor;\n"
   "texcoords = Tex;\n"
   "}";
   const char* pFragmentShaderText = "#version 330 core\n"
   "uniform vec3 color;\n"
   "uniform sampler2D tex0;\n"
   "in vec3 outcolor;\n"
   "in vec2 texcoords;\n"
   "out vec4 FragColor;\n"
   "void main(){\n"
   "   FragColor = vec4(outcolor, 1.0f);\n"
   "}";
   pvs[0] = pVertexShaderText;
   pfs[0] = pFragmentShaderText;
   GLint LenghtsVertex[1];
   GLint LenghtsFragment[1];
   LenghtsVertex[0] = strlen(pVertexShaderText);
   LenghtsFragment[0] = strlen(pFragmentShaderText);
   glShaderSource(ShaderObjv, 1, pvs, LenghtsVertex);
   glShaderSource(ShaderObjf, 1, pfs, LenghtsFragment);
   glCompileShader(ShaderObjv);
   glCompileShader(ShaderObjf);
   //Shader error handling
   GLint successvs;
   glGetShaderiv(ShaderObjv, GL_COMPILE_STATUS, &successvs);
   if(!successvs){
       GLchar InfoLog1[1024];
       glGetShaderInfoLog(ShaderObjv, sizeof(InfoLog1), NULL, InfoLog1);
       fprintf(stderr, "Error compiling shader type %d: '%s'\n", GL_VERTEX_SHADER, InfoLog1);
       return 1;
   }
   GLint successfs;
   glGetShaderiv(ShaderObjf, GL_COMPILE_STATUS, &successfs);
   if(!successfs){
       GLchar InfoLog2[1024];
       glGetShaderInfoLog(ShaderObjf, sizeof(InfoLog2), NULL, InfoLog2);
       fprintf(stderr, "Error compiling shader type %d: '%s'\n", GL_FRAGMENT_SHADER, InfoLog2);
       return 1;
   }
   glAttachShader(ShaderProgram, ShaderObjv);
   glAttachShader(ShaderProgram, ShaderObjf);
   glLinkProgram(ShaderProgram);
   GLint success;
   glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
   if(!success){
      GLchar InfoLog3[1024];
      glGetProgramInfoLog(ShaderProgram, sizeof(InfoLog3), NULL, InfoLog3);
      fprintf(stderr, "Error linking shader program: '%s'\n", InfoLog3);
      return 1;
   }
   glValidateProgram(ShaderProgram);
   GLint successl;
   glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &successl);
   if(!successl){
      GLchar InfoLog4[1024];
      glGetProgramInfoLog(ShaderProgram, sizeof(InfoLog4), NULL, InfoLog4);
      fprintf(stderr, "Error validating shader program: '%s'\n", InfoLog4);
      return 1;
   }
   glUseProgram(ShaderProgram);
   //Set the vectors for the camera location
   vec3 camerapos = {0.0f, 0.0f, -4.0f};
   vec3 center = {0.0f, 0.0f, 0.0f};
   vec3 yup = {0.0f, 1.0f, 0.0f};
   //Create the matrices and make them be identity matrices
   mat4 model;
   mat4 view;
   mat4 proj;
   glm_mat4_identity(model);
   glm_mat4_identity(view);
   glm_mat4_identity(proj);
   //Create the camera
   glm_lookat(camerapos, center, yup, view);
   glm_perspective(glm_rad(45.0f), (float)(width/height), 0.1f, 100.0f, proj);
   //Set the uniform locations
   GLint uniColor = glGetUniformLocation(ShaderProgram, "color");
   GLint tex0uni = glGetUniformLocation(ShaderProgram, "tex0");
   GLint xploc = glGetUniformLocation(ShaderProgram, "xp");
   GLint yploc = glGetUniformLocation(ShaderProgram, "yp");
   GLint modelloc = glGetUniformLocation(ShaderProgram, "model");
   GLint viewloc = glGetUniformLocation(ShaderProgram, "view");
   GLint projloc = glGetUniformLocation(ShaderProgram, "proj");
   //Set the uniforms
   glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);
   glUniform1i(tex0uni, 0);
   glUniform1ui(xploc, 1);
   glUniform1ui(yploc, 1);
   glUniformMatrix4fv(modelloc, 1, GL_FALSE, &model[0][0]);
   glUniformMatrix4fv(viewloc, 1, GL_FALSE, &view[0][0]);
   glUniformMatrix4fv(projloc, 1, GL_FALSE, &proj[0][0]);
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   //Create the buffers
   GLuint VBO;
   GLuint VAO = 0;
   GLuint EBO;
   glGenBuffers(1, &VBO);
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &EBO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBindVertexArray(VAO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   printf("Buffer created successfully\n");
   glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
   printf("Buffer data set up successfully\n");
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   glEnableVertexAttribArray(2);
   printf("Enabling buffer successfull\n");
   //Set the texture (I won't be using it for now)
   int texwidth, texheight, texnum;
   stbi_set_flip_vertically_on_load(true);
   unsigned char* bytes = stbi_load("image.png", &texwidth, &texheight, &texnum, 0);
   GLuint texture;
   glGenTextures(1, &texture);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   float flatcolor[] = {1.0f, 1.0f, 1.0f, 1.0f};
   glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatcolor);
   printf("Set parameters successfully\n");
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texwidth, texheight, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
   glGenerateMipmap(GL_TEXTURE_2D);
   printf("Texture set successfully\n");
   //Set the VertexAttribPointers at locations 0, 1 and 2
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
   printf("First pointer succeeded\n");
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
   printf("Second pointer succeeded\n");
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
   printf("Third pointer succeeded\n");
   printf("Setting up VectexAttribPointer successfull\n");

   while(!glfwWindowShouldClose(window)){
      glfwPollEvents();  
      glClear(GL_COLOR_BUFFER_BIT);
      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      //glDrawArrays(GL_TRIANGLES, 0, 3); 
      //Draw the elements
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
      GLenum err;
      if((err = glGetError()) != GL_NO_ERROR){
         printf("OpenGL error: %d\n", err);
      }
      glfwSwapBuffers(window);
   } 
   return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if(key == GLFW_KEY_R && action == GLFW_PRESS){
      glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);
   }else if(key == GLFW_KEY_G && action == GLFW_PRESS){
      glUniform3f(uniColor, 0.0f, 1.0f, 0.0f);
   }else if(key == GLFW_KEY_B && action == GLFW_PRESS){
      glUniform3f(uniColor, 0.0f, 0.0f, 1.0f);
   }else if(key == GLFW_KEY_D && action == GLFW_PRESS){
      glUniform1ui(xploc, 2);
   }else if(key == GLFW_KEY_A && action == GLFW_PRESS){
      glUniform1ui(xploc, 1);
   }
}
