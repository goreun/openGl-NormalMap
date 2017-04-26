#include <GL/glew.h>
//#include "sphere.h"
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;
using glm::vec4;
using glm::mat3;
#include <glm/gtc/matrix_transform.hpp>

#define GL_STATIC_ARRAY_BUFFER 0x8892
//#define GL_STATIC_DRAW 0x88E4;

extern void resize(int, int);
extern void init();
extern void render();
extern int makeCube();
extern void drawCube();
extern int installShaders();
extern GLuint vaoHandle;
extern mat4 projection;


GLuint programHandle;



char WINDOW_NAME[] = "Graphics Window";
char ICON_NAME[] = "Icon";
Display *display;
int screen;
Window main_window;
unsigned long foreground, background;
GLfloat Angle = 0.0;
static int snglBuf[] = {GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, GLX_DEPTH_SIZE, 12, None};
static int dblBuf[] = {GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, GLX_DEPTH_SIZE, 12,GLX_DOUBLEBUFFER, None};
GLuint *buffers;
GLuint bufObjects[2];
//float Vertices[2400];
//float Normals[2400];
//GLuint bufferID[2];

Bool doubleBuffer = True;
XVisualInfo *vi;
GLXContext cx;
Colormap cmap;
XSetWindowAttributes swa;
Bool recalcModelView = True;
int dummy;
Bool needRedraw = True;

void connectX()
{
   display = XOpenDisplay(NULL);
   if (display == NULL) {fprintf(stderr, "Cannot connect to X\n");
                         exit(1);}
   screen = DefaultScreen(display);
   if (!glXQueryExtension(display, &dummy, &dummy)){
         fprintf(stderr, "X server has no OpenGL GLX Extension\n");
         exit(1);}
   vi = glXChooseVisual(display, screen, dblBuf);
   if (vi == NULL){
      fprintf(stderr, "Double Buffering not available\n");
      vi = glXChooseVisual(display, screen, snglBuf);
      if (vi == NULL) fprintf(stderr, "No RGB Visual with depth buffer\n");
      doubleBuffer = False;
   }
  //if (doubleBuffer == True) fprintf(stderr, "We have double buffering\n");
  /*if (vi->class != TrueColor){
            fprintf(stderr, "Need a TrueColor visual\n");
            exit(1);
          }*/
  cx = glXCreateContext(display, vi , None, True);
  if (cx == NULL){
       fprintf(stderr, "No rendering context\n");
       exit(1);
      }
  cmap = XCreateColormap(display, RootWindow(display, vi->screen),
                         vi->visual, AllocNone);
  /*if (cmap == NULL){
      fprintf(stderr, "Color map is not available\n");
      exit(1);
     }*/
  swa.colormap = cmap;
  swa.border_pixel = 0;
  swa.event_mask = ExposureMask | ButtonPressMask | StructureNotifyMask |
                   KeyPressMask;
}

Window openWindow(int x, int y, int width, int height,
                  int border_width, int argc, char** argv)
{
    XSizeHints size_hints;
    Window new_window;
    new_window = XCreateWindow(display, RootWindow(display, vi->screen),
                 x,y, width, height, border_width, vi->depth, InputOutput,
                 vi->visual, CWBorderPixel | CWColormap | CWEventMask,
                 &swa); 
   size_hints.x = x;
   size_hints.y = y;
   size_hints.width = width;
   size_hints.height = height;
   size_hints.flags = PPosition | PSize;
   XSetStandardProperties(display, new_window, WINDOW_NAME, ICON_NAME,
                          None, argv, argc, &size_hints);
   XSelectInput(display, new_window, (ButtonPressMask | KeyPressMask |
                                       StructureNotifyMask | ExposureMask));
   return (new_window);
}


void disconnectX()
{
   XCloseDisplay(display);
   exit(0);
}

void printMatrix4(mat4 theThing)
{
int i,j;
for(i = 0; i < 4; i++){
  for (j=0; j < 4; j++)
     fprintf(stderr, "%3.2f  ", theThing[i][j]);
     fprintf(stderr, "\n");
   }
 fprintf(stderr, "\n");
}

void printMatrix3(mat3 theThing)
{
int i,j;
for(i = 0; i < 3; i++){
  for (j=0; j < 3; j++)
     fprintf(stderr, "%3.2f  ", theThing[i][j]);
     fprintf(stderr, "\n");
   }
 fprintf(stderr, "\n");
}




void doKeyPressEvent(XKeyEvent *pEvent)
{
 int key_buffer_size = 10;
 char key_buffer[9];
 XComposeStatus compose_status;
 KeySym key_sym;
 XLookupString(pEvent, key_buffer, key_buffer_size,
               &key_sym, &compose_status);
 if (key_buffer[0] == 'q') disconnectX();
 else fprintf(stderr, "You pressed %c\n", key_buffer[0]);
}

 void draw()
{
}


/*void resize(int w, int h)
{
     glViewport(0,0,w,h);
     ///glm::ortho(-1.5,1.5,-1.5,-1.5,0,1.5);
     projection = glm::frustum(-0.8,0.8,-0.8,0.8,2.0,-5.0); //redraw();
     render();
}*/

void doButtonPressEvent(XButtonEvent pEvent)
{
   disconnectX();
}

int main (int argc, char** argv){
  XEvent event;
  GLboolean needRedraw = GL_FALSE, recalcModelView = GL_TRUE;
  const unsigned char *version;
  GLint *params;
  int success;
  connectX();
  main_window = openWindow(10,20,500,500,5, argc, argv);
  glXMakeCurrent(display, main_window, cx);
  XMapWindow(display, main_window);
glewExperimental = GL_TRUE;
  glewInit(); 

 glClear(GL_COLOR_BUFFER_BIT);
 /* glMatrixMode(GM(GL_PROJECTION);
		  go_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.5,1.5,-1.5, 1.5, 0.0,1.5);*/
  init();
 
 while(1){
    do {
    XNextEvent(display, &event);
    switch (event.type){
      case ButtonPress:
        //recalcModelView = GL_TRUE;
        //Angle += 0.5;
        //fprintf(stderr, "Rotating %3.4f degrees\n", Angle);
        //rot = glm::rotate(rot, Angle, glm::vec3(0.0f, 1.0f,0.0f));
       //redraw();
        break;  //passed to Vertex Shader
      case KeyPress:
        doKeyPressEvent((XKeyEvent *)&event);
        break;
      case Expose:
        fprintf(stderr, "In Expose\n");
        render(); 
        break;
      case ConfigureNotify:
        //glMatrixMode(GL_PROJECTION);
        resize(event.xconfigure.width, event.xconfigure.height);
        //glViewport(0, 0, event.xconfigure.width, event.xconfigure.height);
        //glLoadIdentity();
        //glOrtho(-1.5,1.5,-1.5,1.5,0.0,1.5);
        //glMatrixMode(GL_MODELVIEW);
if (doubleBuffer) glXSwapBuffers(display,main_window); else glFlush();
  glBindVertexArray(0);
         fprintf(stderr, "In ConfigureNotify\n");
        break;
    } /*switch*/
  }  while (XPending(display));
    if (recalcModelView){
    /*  glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(Angle, 0.0,0.0,1.0);*/
    recalcModelView =  GL_FALSE;
    needRedraw = GL_TRUE;
    } /*if*/
   /* if (needRedraw){
      redraw();
      needRedraw = GL_FALSE;
    }*/
}
}
