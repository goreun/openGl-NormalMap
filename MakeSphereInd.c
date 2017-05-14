#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "cube.h"
#include "defines.h"
#include <GL/glew.h>
#include <GL/gl.h>

const int M = 20; //M = Number of vertices around one row
const int N = 20;//Number of rows including poles. Each pole and its row is M+1
const int numberVerts = 2*(M+1) + M*(N-3); //382 = 2*21 + 17*20

float Normals[N-2][3*(M+1)];
float Vertices[N-2][3*(M+1)]; //Each vertex is 3 floats

float Tangents[N-2][3*(M+1)];
float BiTangents[N-2][3*(M+1)];

float renderNormals[N-2][6*(M+1)];

float UVs[N-2][2*(M+1)];  
int numberVertices = 0;
int total = 0;
GLsizei size;
GLsizei count[N-1];
GLuint indices[N-1][2*(M+1)];

int makeIndices()
{
/*First make the triangle fan for the South Pole*/
  indices[0][0] = 0;
  for (int i = 1; i <=M; i++){
     indices[0][i] = i;
   }
     indices[0][M+1] = 1;  //close the fan, left rest empty
     count[0] = M+2;
  fprintf(stderr, "Made the first triangle fan with %d vertices\n", M+2);

   int l = 0;
   for(int j = 0; j < 2*M; j +=2){
      indices[1][j] = M+1 + l;      //fan row has M+1 elements
      indices[1][j+1] = indices[0][j/2+1]; // + l;
      l++; 
   }
    count[1] = 2*(M+1);
   /*close the row*/
      indices[1][2*M+1] = indices[1][1];
      indices[1][2*M] = indices[1][0]; 

/*Now do the N-4 rows of triangle strips*/
/*Note that the first row of triangle strips is indexed out of Vertices[0]
  Note that the last row of triangle strips is  indexed out of Vertices[N-3]*/
   for(int i = 2; i<N-3; i++){  
     int k = 0;
     for(int j = 0; j < 2*M; j += 2){
         indices[i][j] = i*(M+1) + k;
         indices[i][j+1] = (i-1)*(M+1) + k;
         k++;
     }
     /*close the row*/
     indices[i][2*M] = indices[i][0]; 
     indices[i][2*M+1] = indices[i][1]; 
     count[i] = 2*(M+1);
   }

/*Make the North Pole triangle fan*/
  indices[N-2][0] = (N-3)*(M+1);   //the North Pole
  fprintf(stderr, "North Pole index is: %d\n", indices[N-2][0]);
  fprintf(stderr, "North Pole is %f,%f,%f\n", Vertices[17][0], Vertices[17][1],
                            Vertices[17][2]);
  for (int i = 1; i <= M; i++){
      indices[N-2][i] = (N-3)*(M+1) + i;
   }
      indices[N-2][M+1] = indices[N-2][0]; //close the fan
      count[N-2] = M+2;
   fprintf(stderr,"Index buffer has %lu size\n", sizeof(indices));
   count[0] = 22;
   count[18]=22;
   for (int i = 1; i < N-2; i++) count[i] = 42;

/*Make the second-to-last row interleave with the NP fan vertices*/
   for (int j = 0; j < 2*M; j +=2){
      indices[N-3][j] = indices[N-4][j]; 
      indices[N-3][j+1] = indices[N-2][j/2+1];
   }
   //close the row
      indices[N-3][2*M] = indices[N-3][0];
      indices[N-3][2*M+1] = indices[N-3][1];
   fprintf(stderr,"Index buffer has %lu size\n", sizeof(indices));
   count[0] = 22;
   count[18]=22;
return 1;
}


int makeSphere(float r, float centerx, float centery, float centerz)
{
int i,j;
float x,y,z;
float phi,theta;
float PioverN2 = M_PI/(N-2); //for phi
float TwopioverM = (2.0*M_PI)/M; //for theta--don't need to close row
float Piovertwo = M_PI/2.0;
fprintf(stderr, "We should have %d Vertices and Normals\n", numberVerts);
int count = 0;
int uvCounter = 0;

if ( (r <= 0.0) || (M <= 1) || (N <= 1)){
      fprintf(stderr, "Wrong data for sphere...exiting\n");
      exit(1);
    }

/*  Vertices = (float *)malloc((M+1)*6*(N-1));
  Normals = (float *)malloc((M+1)*6*(N-1)*sizeof(float));*/
/* Put in South Pole (centerx,centery,centerz) plus first row for fan */
    phi = PioverN2 - Piovertwo;
    x = 0.0;
    y = -1.0;
    z = 0.0;
    Vertices[0][count] = centerx + r*x;
    Vertices[0][count+1] = centery + r*y;
    Vertices[0][count+2] = centerz + r*z;
    Normals[0][count] = x;
    Normals[0][count+1] = y;
    Normals[0][count+2] = z;
    
    UVs[0][uvCounter]   = 0.5;
    UVs[0][uvCounter+1] = 0.0;
    
    Tangents[0][count]=0.0f;
    Tangents[0][count + 1]=0.0f;
    Tangents[0][count + 2]=1.0f;
    
    BiTangents[0][count]    = 1.0f;
    BiTangents[0][count + 1]= 0.0f;
    BiTangents[0][count + 2]= 0.0f;
    
    total++;
    numberVertices++;
    count +=3; 
    uvCounter += 2;
    
    for (j = 0; j < M;j++){
      theta = ((float) j) * TwopioverM;
      
       x = cos(phi) * cos(theta);
       y = sin(phi);
       z = cos(phi) * sin(theta);

       
        
       Vertices[0][count] = centerx + r*x;
       Vertices[0][count+1] = centery + r*y;
       Vertices[0][count+2] = centerz + r*z;
       Normals[0][count] = x;
       Normals[0][count+1] = y;
       Normals[0][count+2] = z;
    
       UVs[0][uvCounter]   = (float)j/((float)M-1);
       UVs[0][uvCounter+1] = 1.0f/(float)N; //first raw in UV's
       
	//computing Tangents and BiTangents by taking the partial derivative with respect to theta.
	       
       Tangents[0][count]    = -cos(phi) * sin(theta);
       Tangents[0][count + 1]=  0;
       Tangents[0][count + 2]= cos(phi) * cos(theta);
    
       BiTangents[0][count]    = - sin(phi) * cos(theta);
       BiTangents[0][count + 1]= cos(phi);
       BiTangents[0][count + 2]= -sin(phi) * sin(theta);

	renderNormals[0][count/3*6]  = Vertices[0][count];
	renderNormals[0][count/3*6+1]= Vertices[0][count] + Tangents[0][count] ; 
	renderNormals[0][count/3*6+2]= Vertices[0][count+1];
	renderNormals[0][count/3*6+3]= Vertices[0][count+1] + Tangents[0][count+1] ; 
	renderNormals[0][count/3*6+4]= Vertices[0][count+2];
	renderNormals[0][count/3*6+5]= Vertices[0][count+2] + Tangents[0][count+2] ; 
    
      count +=3;
      uvCounter += 2;
     
      numberVertices++;
      total++; 
    }
     //First row contains a triangle fan of M vertices
    /*no need to close the row*/
    fprintf(stderr, "First row contains %d Vertices\n", numberVertices);
    numberVertices = 0;
/* Caculate N-4 rows between the 2 fans*/ 
  for (i = 1; i < N-3; i++)
   {
     numberVertices = 0;
     phi = ((float) i)*PioverN2 - Piovertwo;
     count = 0;
     uvCounter = 0;
     for (j = 0; j < M; j++) /* Don't go for M!!*/
     {
        //count = 0;
        theta = ((float) j) * TwopioverM;
        x = cos(phi) * cos(theta);
        y = sin(phi);
        z = cos(phi) * sin(theta);
        Vertices[i][count] = centerx + r * x;
        Vertices[i][count+1] = centery + r * y;
        Vertices[i][count+2] = centerz + r * z;
        Normals[i][count] = x;
        Normals[i][count +2] = z;
        Normals[i][count+1] = y;

		  UVs[i][uvCounter]   = (float)j/((float)M-1);
    	  UVs[i][uvCounter+1] = ((float)i+1)/(float)N; //first raw in UV's 
    	  
   	//computing Tangents and BiTangents by taking the partial derivative with respect to theta.    	  
    	  
        Tangents[i][count]    = -cos(phi) * sin(theta);
        Tangents[i][count + 1]=  0;
        Tangents[i][count + 2]= cos(phi) * cos(theta);

			float absTangent = sqrtf(Tangents[i][count]*Tangents[i][count]+Tangents[i][count+1]*Tangents[i][count+1]+Tangents[i][count+2]*Tangents[i][count+2])  ;      
        
        Tangents[i][count]/=absTangent;
        Tangents[i][count + 1]=  0;
        Tangents[i][count + 2]/= absTangent;
    
        BiTangents[i][count]    = - sin(phi) * cos(theta);
        BiTangents[i][count + 1]=   cos(phi);
        BiTangents[i][count + 2]= - sin(phi) * sin(theta); 
        			
       float absBiTangent = sqrtf(BiTangents[i][count]*BiTangents[i][count]+BiTangents[i][count+1]*BiTangents[i][count+1]+BiTangents[i][count+2]*BiTangents[i][count+2])  ;      

        BiTangents[i][count]    /= absBiTangent;
        BiTangents[i][count + 1]/=absBiTangent;
        BiTangents[i][count + 2]/=absBiTangent; 
                
        /*Normals[i][count] = 	 BiTangents[i][count+1]*Tangents[i][count + 2] - Tangents[i][count + 1] *BiTangents[i][count + 2] ;
        Normals[i][count +1] = BiTangents[i][count+2]*Tangents[i][count] - Tangents[i][count + 2] *BiTangents[i][count] ;
        Normals[i][count+ 2] = BiTangents[i][count]*Tangents[i][count + 1] - Tangents[i][count ] *BiTangents[i][count + 1] ;*/

	renderNormals[i][count/3*6]  = Vertices[i][count];
	renderNormals[i][count/3*6+1]=  Vertices[i][count+1];
	renderNormals[i][count/3*6+2]= Vertices[i][count+2];
	renderNormals[i][count/3*6+3]= Vertices[i][count]   + 0.3f*BiTangents[i][count] ;
	renderNormals[i][count/3*6+4]= Vertices[i][count+1] + 0.3f*BiTangents[i][count+1] ; 
	renderNormals[i][count/3*6+5]= Vertices[i][count+2] + 0.3f*BiTangents[i][count+2] ;  
	

        
        count += 3;
        uvCounter += 2;
         
        numberVertices++;
        total++;
       }
        fprintf(stderr,"Row number %d has %d Vertices and Normals\n",i,numberVertices);
     } //for i

 /* Now put in the North Pole and the last row in that order*/
    x = 0.0;
    y = 1.0;
    z = 0.0;
    count = 0;
    uvCounter = 0;
    numberVertices = 0;
    Vertices[N-3][count] = centerx + r*x;
    Vertices[N-3][count+1] = centery + r*y;
    Vertices[N-3][count+2] = centerz + r*z;
    Normals[N-3][count] = x;
    Normals[N-3][count+1] = y;
    Normals[N-3][count+2] = z;

    UVs[N-3][uvCounter]   = 0.5f;
    UVs[N-3][uvCounter+1] = 1.0f; 
    
    Tangents[N-3][count]   = 0.0f;
    Tangents[N-3][count + 1]=0.0f;
    Tangents[N-3][count + 2]=-1.0f;
    
    BiTangents[N-3][count]    = 1.0f;
    BiTangents[N-3][count + 1]= 0.0f;
    BiTangents[N-3][count + 2]= 0.0f;
    
    total++;

    count +=3;
    uvCounter += 2;
    
    numberVertices++;
    phi = (N-3)*PioverN2 - Piovertwo;
    for (j = 0; j < M; j++)
    {
      theta = ((float) j) * TwopioverM;
      x = cos(phi) * cos(theta);
      y = sin(phi);
      z = cos(phi) * sin(theta);
      Vertices[N-3][count] = centerx + r*x;
      Vertices[N-3][count+1] = centery + r*y;
      Vertices[N-3][count+2] = centerz + r*z;
      Normals[N-3][count] = x;
      Normals[N-3][count+1] = y;
      Normals[N-3][count+2] = z;
      
    	UVs[N-3][uvCounter]   = (float)j/((float)M-1);
    	UVs[N-3][uvCounter+1] = ((float)N-1)/(float)N; 
    	
    	  	//computing Tangents and BiTangents by taking the partial derivative with respect to theta.    	  
    	  
        Tangents[N-3][count]    = -cos(phi) * sin(theta);
        Tangents[N-3][count + 1]=  0;
        Tangents[N-3][count + 2]= cos(phi) * cos(theta);
    
        BiTangents[N-3][count]    = - sin(phi) * cos(theta);
        BiTangents[N-3][count + 1]=   cos(phi);
        BiTangents[N-3][count + 2]= - sin(phi) * sin(theta);   

	renderNormals[N-3][count/3*6]  = Vertices[N-3][count];
	renderNormals[N-3][count/3*6+1]= Vertices[N-3][count]   + Tangents[N-3][count] ; 
	renderNormals[N-3][count/3*6+2]= Vertices[N-3][count+1];
	renderNormals[N-3][count/3*6+3]= Vertices[N-3][count+1] + Tangents[N-3][count+1] ; 
	renderNormals[N-3][count/3*6+4]= Vertices[N-3][count+2];
	renderNormals[N-3][count/3*6+5]= Vertices[N-3][count+2] + Tangents[N-3][count+2] ;  
    		
    		
    
      count +=3;
      uvCounter +=2;
      
      numberVertices++;
      total++;
    }
    count = 0;
    uvCounter = 0;
    
  /*  for(int v = 0 ; v < N-2 ; v++){
		for(int u = 0; u < M+1; u++){
			fprintf(stderr, ", UV (%.2f, %.2f)", UVs[v][u*2], UVs[v][(u*2)+1]); 
		 }    
		 fprintf(stderr, "\n"); 
    }*/
    
    fprintf(stderr, "North Pole has %d Vertices and Normals\n", numberVertices);
    fprintf(stderr, "Your sphere has %d Vertices and Normals\n",total);
    if (makeIndices() != 0) fprintf(stderr, "Made the indices\n");
    else fprintf(stderr, "Problem with indices\n");

    return total;
}

GLuint vaoHandleS;

void bindBuffers(){
  unsigned int bufferID[6];

  glGenVertexArrays(1,&vaoHandleS);
  glBindVertexArray(vaoHandleS);

  glGenBuffers(6, bufferID);
 
    glBindBuffer(GL_ARRAY_BUFFER, bufferID[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
    glEnableVertexAttribArray(0);  // Vertex position

    glBindBuffer(GL_ARRAY_BUFFER, bufferID[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Normals), Normals, GL_STATIC_DRAW);
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    
}

void drawObject(){
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 glBindVertexArray(vaoHandleS);

 glDrawElements(GL_TRIANGLE_FAN, 21, GL_UNSIGNED_INT, (GLvoid*)((char*)NULL));

  for (int i = 1; i < 18; i++){
     glDrawElements(GL_TRIANGLE_STRIP, 42, GL_UNSIGNED_INT,(GLvoid *)((char *)NULL + (i*168)));
}
  glDrawElements(GL_TRIANGLE_FAN, 21, GL_UNSIGNED_INT, (GLvoid*)((char*)NULL+(18*168)));
  
  
  
 /* glBegin(GL_LINES);
  glColor3f(1,0.0,0.0);
  for(int i = 2; i<17 ; i++){
  	for(int j=0; j < 6*(M+1); j+=6){
  	 glVertex3f(renderNormals[i][j],renderNormals[i][j+1],renderNormals[i][j+2]);
  	 glVertex3f(renderNormals[i][j+3],renderNormals[i][j+4],renderNormals[i][j+5]);
   }
  }
  glEnd();*/
}


