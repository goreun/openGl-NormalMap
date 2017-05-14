#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include <GL/glew.h>
#include <GL/gl.h>

const int MajorSegments = 64; 
const int MinorSegments = 32;
float MajorRadius = 1.0f;
float MinorRadius = 0.35f;

float tNormals  [MinorSegments][3*(MajorSegments)];
float tVertices [MinorSegments][3*(MajorSegments)];
float Tangents  [MinorSegments][3*(MajorSegments)];
float BiTangents[MinorSegments][3*(MajorSegments)];

float UVs[MinorSegments][2*(MajorSegments)];  

int tnumberVerts = MajorSegments * MinorSegments; 
GLsizei tsize;
GLuint tIndices[ MinorSegments ][2*( MajorSegments + 1 )];


#define PI_ 3.14159265358979323846

int makeTorus()
{
	fprintf(stderr, "Make Torus \n\n");

	int u, v;
   double x, y, z, TwoPI;

   TwoPI = 2 * PI_;
   
   for (u = 0; u < MinorSegments; u++) {
     
      for (v = 0; v < MajorSegments; v++) {

x = (MajorRadius + MinorRadius * cos( u * TwoPI / MinorSegments )) * cos( v * TwoPI / MajorSegments );
y =  MinorRadius * sin( u * TwoPI / MinorSegments );
z = (MajorRadius + MinorRadius * cos( u * TwoPI / MinorSegments )) * sin( v * TwoPI / MajorSegments );

tVertices[u][v*3 ]     = x;
tVertices[u][v*3 + 1 ] = y;
tVertices[u][v*3 + 2 ] = z;    
             	
	  UVs[u][v*2 ]   = (float)v/((float)MajorSegments-1);
    	  UVs[u][v*2 + 1] = ((float)u+1)/(float)MinorSegments; //first raw in UV's 

tNormals[u][v*3 ]     =  cos( u * TwoPI / MinorSegments ) * cos( v * TwoPI / MajorSegments );
tNormals[u][v*3 + 1 ] =  sin( u * TwoPI / MinorSegments );
tNormals[u][v*3 + 2 ] =  cos( u * TwoPI / MinorSegments ) * sin( v * TwoPI / MajorSegments );   


//computing Tangents and BiTangents by taking the partial derivative with respect to theta.    	  
    	float phi   = u * TwoPI / MinorSegments;
	float theta = v * TwoPI / MajorSegments;

        Tangents[u][v*3 ]   = -cos(phi) * sin(theta);
        Tangents[u][v*3 + 1]=  0;
        Tangents[u][v*3 + 2]=  cos(phi) * cos(theta);
   
        BiTangents[u][v*3 ]   = -sin(phi) * cos(theta);
        BiTangents[u][v*3 + 1]=  cos(phi);
        BiTangents[u][v*3 + 1]= -sin(phi) * sin(theta); 
        			
       /*float absBiTangent = sqrtf(BiTangents[i][count]*BiTangents[i][count]+BiTangents[i][count+1]*BiTangents[i][count+1]+BiTangents[i][count+2]*BiTangents[i][count+2])  ;      

        BiTangents[i][count]    /= absBiTangent;
        BiTangents[i][count + 1]/=absBiTangent;
        BiTangents[i][count + 2]/=absBiTangent; */
                
        /*Normals[i][count] = 	 BiTangents[i][count+1]*Tangents[i][count + 2] - Tangents[i][count + 1] *BiTangents[i][count + 2] ;
        Normals[i][count +1] = BiTangents[i][count+2]*Tangents[i][count] - Tangents[i][count + 2] *BiTangents[i][count] ;
        Normals[i][count+ 2] = BiTangents[i][count]*Tangents[i][count + 1] - Tangents[i][count ] *BiTangents[i][count + 1] ;*/            
      }
   }
	
	return 1;
}


int makeIndices(){
  int u, v;
     for (u = 0; u < MinorSegments; u++) {
        for (v = 0; v < MajorSegments; v++) {
            tIndices[u][2*v]     = ( (u + 1) % MinorSegments * (MajorSegments ) ) + v;
            tIndices[u][2*v + 1] = (  u      % MinorSegments * (MajorSegments ) ) + v;
         }    
	     tIndices[u][2*( MajorSegments + 1 ) - 2] = tIndices[u][0];
		  tIndices[u][2*( MajorSegments + 1 ) - 1] = tIndices[u][1];   
      }
}
      

GLuint vaoHandleS;

void bindBuffers(){
  unsigned int bufferID[6];

  glGenVertexArrays(1,&vaoHandleS);
  glBindVertexArray(vaoHandleS);

  glGenBuffers(6, bufferID);
 
    glBindBuffer(GL_ARRAY_BUFFER, bufferID[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tVertices), tVertices, GL_STATIC_DRAW);
    glVertexAttribPointer( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
    glEnableVertexAttribArray(0);  // Vertex position

    glBindBuffer(GL_ARRAY_BUFFER, bufferID[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tNormals), tNormals, GL_STATIC_DRAW);
    glVertexAttribPointer( (GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
    glEnableVertexAttribArray(1);  // Vertex normal
    
    glBindBuffer(GL_ARRAY_BUFFER, bufferID[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Tangents), Tangents, GL_STATIC_DRAW);
    glVertexAttribPointer( (GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
    glEnableVertexAttribArray(2);  // Vertex tangents
    
     glBindBuffer(GL_ARRAY_BUFFER, bufferID[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BiTangents), BiTangents, GL_STATIC_DRAW);
    glVertexAttribPointer( (GLuint)3, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
    glEnableVertexAttribArray(3);  // Vertex Bitangents

    glBindBuffer(GL_ARRAY_BUFFER, bufferID[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(UVs), UVs, GL_STATIC_DRAW);
    glVertexAttribPointer( (GLuint)4, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
    glEnableVertexAttribArray(4);  // texture coords

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID[5]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tIndices), tIndices, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    
}

void drawObject(){
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 glBindVertexArray(vaoHandleS);

   for (int i = 0; i < MinorSegments ; i++)
     glDrawElements(GL_TRIANGLE_STRIP, 2 * (MajorSegments + 1)  , GL_UNSIGNED_INT,(GLvoid *)((char *)NULL + (i * 8 * (MajorSegments + 1) ) ));
	
	}
