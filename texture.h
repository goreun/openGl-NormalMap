#ifndef TEXTURE_H
#define TEXTURE_H

#include "cube.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
using glm::mat4;
using glm::mat3;

//    GLSLProgram prog;

    int width, height;
    //Cube *cube;
    mat4 mv;
    mat4 mvp;
    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;


#endif // SCENEPERFRAGMENT_H
