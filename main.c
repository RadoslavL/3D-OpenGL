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
const float sensitivity = 0.1f;
float degreesx = 1.0f;
float degreesy = 0.5f;
float lastx = (float)width/2;
float lasty = (float)height/2;
float xoffset;
float yoffset;
float yaw = -90.0f;
float pitch = 0.0f;
GLuint checkfront = 0;
GLuint checkback = 0;
GLuint checkleft = 0;
GLuint checkright = 0;
GLuint uniColor;
GLuint xploc;
GLuint yploc;
GLuint tex0uni;
vec3 direction;
vec3 front;
vec3 camerapos;
vec3 location;
void key_callback();
//void mouse_callback();

int main(int argc, char *argv[]){
   if(!glfwInit()){
      printf("Failed to initialize glfw!\n");
      return 1;
   }else{
      printf("Successfully initialized glfw!\n");
   }
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
   //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
   GLFWwindow* window = glfwCreateWindow(width, height, "Title", NULL, NULL);
   //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   if(!window){
      printf("Window creation failed!\n");
      return 1;
   }else{
      printf("Window created successfully!\n");
   }
   glfwMakeContextCurrent(window);
   glfwSetKeyCallback(window, key_callback);
   //glfwSetCursorPosCallback(window, mouse_callback);
   glewExperimental = GL_TRUE;
   if(glewInit() != GLEW_OK){
      printf("Failed to initialize glew!\n");
      return 1;
   }else{
      printf("Successfully initialized glew!\n");
   } 
   glfwSwapInterval(1);
   float Vertices[] = {
    //Front
     -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f, //0
     -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f, //1
      0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f, //2
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f, //3
     
    //Back
      0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f, //4
      0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f, //5
     -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f, //6
     -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  0.0f,  0.0f,  1.0f, //7

    //Right side
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f, //8
      0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f, //9
      0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f, //10
      0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f, //11

    //Left side
     -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f, //12
     -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f, //13
     -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f, //14
     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f, //15

    //Top
     -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f, //16
     -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f, //17
      0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f, //18
      0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f, //19

    //Bottom
      0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f, //20
     -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, //21
     -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f, //22
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f, //23
   };

   float floor[] = {
    //Front
     -2.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f, //0
     -2.0f, -0.5f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f, //1
      2.0f, -0.5f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f, //2
      2.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f, //3
      
    //Back
      2.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f, //4
      2.0f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f, //5
     -2.0f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f, //6
     -2.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  0.0f,  0.0f,  1.0f, //7

    //Right side
      2.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f, //8
      2.0f, -0.5f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f, //9
      2.0f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f, //10
      2.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f, //11

    //Left side
     -2.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f, //12
     -2.0f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f, //13
     -2.0f, -0.5f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f, //14
     -2.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f, //15

    //Top
     -2.0f, -0.5f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f, //16
     -2.0f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f, //17
      2.0f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f, //18
      2.0f, -0.5f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f, //19

    //Bottom
      2.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f, //20
     -2.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, //21
     -2.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f, //22
      2.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f, //23
   };

   GLuint indices[] = {
    //Front
      0, 1, 2,
      0, 2, 3,
    //Back
      4, 5, 6,
      4, 6, 7,
    //Right side
      8, 9, 10,
      8, 10, 11,
    //Left side
      12, 13, 14,
      12, 14, 15,
    //Top
      16, 17, 18,
      16, 18, 19,
    //Bottom
      20, 21, 22,
      20, 22, 23
   };

   printf("GLfloat variable called successfully\n"); 
   GLuint ShaderProgram = glCreateProgram();
   GLuint ShaderObjv = glCreateShader(GL_VERTEX_SHADER);
   GLuint ShaderObjf = glCreateShader(GL_FRAGMENT_SHADER);
   const GLchar* pvs[1];
   const GLchar* pfs[1];
   const char* pVertexShaderText = "#version 110\n"
   "attribute vec3 Position;\n"
   "attribute vec3 incolor;\n"
   "attribute vec2 Tex;\n"
   "attribute vec3 innormal;\n"
   "varying vec3 outcolor;\n"
   "varying vec2 texcoords;\n"
   "varying vec3 outnormal;\n"
   "varying vec3 FragPos;\n"
   "uniform mat4 model;\n"
   "uniform mat4 view;\n"
   "uniform mat4 proj;\n"
   "void main(){\n"
   "   gl_Position = proj * view * model * vec4(Position, 1.0);\n"
   "   FragPos = vec3(model * vec4(Position, 1.0));\n"
   "   outcolor = incolor;\n"
   "   texcoords = Tex;\n"
   "   outnormal = innormal;\n"
   "}";
   const char* pFragmentShaderText = "#version 110\n"
   "uniform vec3 color;\n"
   "uniform sampler2D tex0;\n"
   "uniform vec3 lightPos;\n"
   "varying vec3 outcolor;\n"
   "varying vec2 texcoords;\n"
   "varying vec3 outnormal;\n"
   "varying vec3 FragPos;\n"
   "void main(){\n"
   "   float ambientstrength = 0.4;\n"
   "   vec3 ambient = ambientstrength * vec3(1.0, 1.0, 1.0);\n"
   "   vec3 norm = normalize(outnormal);\n"
   "   vec3 lightDir = normalize(lightPos - FragPos);\n"
   "   float diff = max(dot(norm, lightDir), 0.0);\n"
   "   vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);\n"
   "   vec4 result = vec4(ambient + diffuse, 1.0) * vec4(outcolor, 1.0);\n"
   "   gl_FragColor = result;\n"
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
   vec3 camerapos = {0.0f, 0.0f, -7.0f};
   vec3 center = {0.0f, 0.0f, 0.0f};
   vec3 target = {0.0f, 0.0f, 0.0f};
   vec3 front = {0.0f, 0.0f, 1.0f};
   vec3 yup = {0.0f, 1.0f, 0.0f};
   vec3 speed = {2.5f, 2.5f, 2.5f};
   vec3 location;
   vec3 output;
   vec3 output2;
   vec3 output3; 
   vec3 camspeed;
   float deltatime = 0.0f;
   float lastframe = 0.0f;
   float currentframe = glfwGetTime();
   /*
   direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
   direction[1] = sin(glm_rad(pitch));
   direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
   glm_normalize(direction);
   glm_vec3_copy(direction, front);
   */
   glm_vec3_add(camerapos, front, location);
   GLenum err;
   mat4 model = GLM_MAT4_IDENTITY_INIT;
   mat4 view = GLM_MAT4_IDENTITY_INIT;
   mat4 proj = GLM_MAT4_IDENTITY_INIT;
   mat4 floormodel = GLM_MAT4_IDENTITY_INIT;
   glm_lookat(camerapos, center, yup, view);
   glm_perspective(glm_rad(45.0f), (float)width/(float)height, 0.1f, 100.0f, proj);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glFrontFace(GL_CCW);
   GLint uniColor = glGetUniformLocation(ShaderProgram, "color");
   GLint tex0uni = glGetUniformLocation(ShaderProgram, "tex0");
   GLint modelloc = glGetUniformLocation(ShaderProgram, "model");
   GLint viewloc = glGetUniformLocation(ShaderProgram, "view");
   GLint projloc = glGetUniformLocation(ShaderProgram, "proj");
   GLint lightPos = glGetUniformLocation(ShaderProgram, "lightPos");
   glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);
   glUniform1i(tex0uni, 0);  
   glUniform3f(lightPos, -2.0f, 2.0f, 2.0f);
   glUniformMatrix4fv(modelloc, 1, GL_FALSE, &model[0][0]);
   glUniformMatrix4fv(viewloc, 1, GL_FALSE, &view[0][0]);
   glUniformMatrix4fv(projloc, 1, GL_FALSE, &proj[0][0]);
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
   glEnableVertexAttribArray(3);
   printf("Enabling buffer successfull\n");
   int texwidth, texheight, texnum;
   stbi_set_flip_vertically_on_load(true);
   unsigned char* bytes = stbi_load("crate.jpg", &texwidth, &texheight, &texnum, 0);
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
   GLuint positionattriblocation = glGetAttribLocation(ShaderProgram, "Position");
   GLuint colorattriblocation = glGetAttribLocation(ShaderProgram, "incolor");
   GLuint textureattriblocation = glGetAttribLocation(ShaderProgram, "Tex");
   GLuint normalattriblocation = glGetAttribLocation(ShaderProgram, "innormal");
   printf("Texture set successfully\n");
   glVertexAttribPointer(positionattriblocation, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
   printf("First pointer succeeded\n");
   glVertexAttribPointer(colorattriblocation, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
   printf("Second pointer succeeded\n");
   glVertexAttribPointer(textureattriblocation, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
   printf("Third pointer succeeded\n");
   glVertexAttribPointer(normalattriblocation, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
   printf("Forth pointer succeeded\n");
   printf("Setting up VectexAttribPointer successfull\n");

   while(!glfwWindowShouldClose(window)){
      glfwPollEvents();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      if((err = glGetError()) != GL_NO_ERROR){
         printf("OpenGL error: %d\n", err);
      }
      currentframe = glfwGetTime();
      deltatime = currentframe - lastframe;
      lastframe = currentframe;
      glm_vec3_scale(speed, deltatime, camspeed);
      //glm_rotate(model, glm_rad(degreesx), (vec3){0, 1, 0});
      //glm_rotate(model, glm_rad(degreesy), (vec3){1, 0, 0});
      glm_rotate(floormodel, glm_rad(-degreesy * deltatime * 20), (vec3){0, 1, 0});
      if(checkfront == 1){
         glm_vec3_mul(camspeed, front, output);
         glm_vec3_add(camerapos, output, camerapos);
	 glm_vec3_add(camerapos, front, location);
      }
      if(checkback == 1){
         glm_vec3_mul(camspeed, front, output);
         glm_vec3_sub(camerapos, output, camerapos);
	 glm_vec3_add(camerapos, front, location);
      }
      if(checkleft == 1){
         glm_vec3_crossn(front, yup, output2);
	 glm_vec3_mul(output2, camspeed, output3);
	 glm_vec3_sub(camerapos, output3, camerapos);
	 glm_vec3_add(camerapos, front, location);
      }
      if(checkright == 1){
         glm_vec3_crossn(front, yup, output2);
	 glm_vec3_mul(output2, camspeed, output3);
	 glm_vec3_add(camerapos, output3, camerapos);
	 glm_vec3_add(camerapos, front, location);
      } 
      glm_lookat(camerapos, location, yup, view);
      glUniformMatrix4fv(viewloc, 1, GL_FALSE, &view[0][0]);
      glUniformMatrix4fv(modelloc, 1, GL_FALSE, &model[0][0]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
      glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
      glUniformMatrix4fv(modelloc, 1, GL_FALSE, &floormodel[0][0]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
      glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
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
   }else if(key == GLFW_KEY_W && action == GLFW_PRESS){
      //degreesx = 1.0f;
      //degreesy = 0.5f;
      checkfront = 1;
   }else if(key == GLFW_KEY_S && action == GLFW_PRESS){
      //degreesx = -1.0f;
      //degreesy = -0.5f;
      checkback = 1;
   }else if(key == GLFW_KEY_W && action == GLFW_RELEASE){
      checkfront = 0;
   }else if(key == GLFW_KEY_S && action == GLFW_RELEASE){
      checkback = 0;
   }else if(key == GLFW_KEY_A && action == GLFW_PRESS){
      checkleft = 1;
   }else if(key == GLFW_KEY_A && action == GLFW_RELEASE){
      checkleft = 0;
   }else if(key == GLFW_KEY_D && action == GLFW_PRESS){
      checkright = 1;
   }else if(key == GLFW_KEY_D && action == GLFW_RELEASE){
      checkright = 0;
   }
}
/*
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
   xoffset = xpos - lastx;
   yoffset = lasty - ypos;
   lastx = xpos;
   lasty = ypos;
   xoffset *= sensitivity;
   yoffset *= sensitivity;
   yaw += xoffset;
   pitch += yoffset;
   if(pitch > 89.0f){
      pitch = 89.0f;
   }
   if(pitch < -89.0f){
      pitch = -89.0f;
   }
   direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
   direction[1] = sin(glm_rad(pitch));
   direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
   glm_normalize(direction);
   glm_vec3_negate(direction);
   glm_vec3_copy(direction, front);
   glm_vec3_add(camerapos, front, location);
}
*/
