#include "texture.h"
#include "SOIL.h"
#include <stdio.h>
#include <stdlib.h>


#include "defines.h"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>


using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;


extern int installShaders();
extern int  makeIndices();
extern int  makeSphere(float, float, float, float);
extern void bindBuffers();
extern void drawSphere();
extern GLint getUniLoc(GLuint, const char *);
extern GLuint prog;
GLuint tex_2d;   //the texture handle
GLuint norm_2d;
GLuint spec_2d;

void
loadTexture()
{
   tex_2d = SOIL_load_OGL_texture("brick1.jpg", SOIL_LOAD_AUTO,
                       SOIL_CREATE_NEW_ID,
                       SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y |\
                       SOIL_FLAG_NTSC_SAFE_RGB |\
                       SOIL_FLAG_COMPRESS_TO_DXT);
  if (tex_2d == 0){
    fprintf(stderr, "Cannot load texture...exiting\n");
    exit(1);
  }
  
  
  
    norm_2d = SOIL_load_OGL_texture("brickNormalMap.jpg", SOIL_LOAD_AUTO,
                       SOIL_CREATE_NEW_ID,
                       SOIL_FLAG_MIPMAPS |\
                       SOIL_FLAG_NTSC_SAFE_RGB |\
                       SOIL_FLAG_COMPRESS_TO_DXT);
  if (norm_2d  == 0){
    fprintf(stderr, "Cannot load normal map...exiting\n");
    exit(1);
  }
  
    spec_2d = SOIL_load_OGL_texture("brickSpecularMap.jpg", SOIL_LOAD_AUTO,
                       SOIL_CREATE_NEW_ID,
                       SOIL_FLAG_MIPMAPS |\
                       SOIL_FLAG_NTSC_SAFE_RGB |\
                       SOIL_FLAG_COMPRESS_TO_DXT);
  if (spec_2d  == 0){
    fprintf(stderr, "Cannot load normal map...exiting\n");
    exit(1);
  }
  
  
}


void init()
{

    glEnable(GL_DEPTH_TEST);
    prog = installShaders();
    view = glm::lookAt(vec3(0.0f,2.0f,-4.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = mat4(1.0f);
    angle = 0.0;
	 makeIndices();
	 int number = makeSphere(1.5,0.0,0.0,0.0);
	 bindBuffers();
 model = mat4(1.0f);
    // Load texture file
    loadTexture();


    // Copy file to OpenGL
    glActiveTexture(GL_TEXTURE0);
    glBindTexture  (GL_TEXTURE_2D, tex_2d);
    glUniform1i    (getUniLoc(prog, "DiffuseTextureSampler"), 0);

    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture  (GL_TEXTURE_2D, norm_2d);
    glUniform1i    (getUniLoc(prog, "NormalTextureSampler"),  1);
    
        
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture  (GL_TEXTURE_2D, spec_2d);
    glUniform1i    (getUniLoc(prog, "SpecularTextureSampler"),  2);
    
        
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void update( float t )
{
    angle += 0.01f;
    if( angle > TWOPI) angle -= TWOPI;
    
}


void setMatrices()
{
    mv =  view * model;
    mvp = projection * mv;
    mat3 normalMatrix = mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]));
    
    glUniformMatrix4fv(getUniLoc(prog, "V"),    1, GL_FALSE, &mv[0][0]);
    glUniformMatrix4fv(getUniLoc(prog, "M"),    1, GL_FALSE, &model[0][0]);
    glUniformMatrix3fv(getUniLoc(prog, "MV3x3"),1, GL_FALSE, &normalMatrix[0][0]);
    glUniformMatrix4fv(getUniLoc(prog, "MVP"),  1, GL_FALSE, &mvp[0][0]);

}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform3f(getUniLoc(prog, "LightPosition"), 2.0f,1.0f,-1.0f );
    glUniform3f(getUniLoc(prog, "LightColor" ), 1.0f, 1.0f, 1.0f);
    glUniform1f(getUniLoc(prog, "LightPower" ), 2.0f);

    model *=  glm::rotate(3.0f,vec3(0.0f,1.0f,0.0f));
    setMatrices();
    drawSphere();
}


void resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(60.0f*3.14159f/180.0f, (float)w/h, 0.3f, 100.0f);
}
