EXE = normal_mapping
OBJS =  texture.o shader.o main.o makeCube.o image_DXT.o image_helper.o  stb_image_aug.o  SOIL.o MakeSphereInd.o

FILES = defines.h stb_image_aug.h stbi_DDS_aug_c.h image_helper.h stbi_DDS_aug.h image_DXT.h glutils.h shader.h SOIL.h cube.h texture.h texture.c image_helper.c image_DXT.c stb_image_aug.c MakeSphereInd.c shader.c makeCube.c main.c 

INCLUDES = /usr/X11R6/include -I /usr/include 
LIB = /usr/X11R6/lib
LFLAGS = -lGL -lGLU -lGLEW -lX11 
CC = g++

.SUFFIXES: .c
prog: ${EXE}

#${EXE}: ${OBJS}
#	${CC} -L${LIB} -o $@ ${OBJS} ${LFLAGS}
#
#${OBJS}:${FILES}
#	 ${CC} -I${INCLUDES} -c ${FILES}
#another way to do it

.c.o:
	 ${CC} -I${INCLUDES} -c $<

${EXE}: ${OBJS}
	${CC} -L${LIB} -o $@ ${OBJS} ${LFLAGS} 

clean:
	rm -f ${OBJS} ${EXE}
	make
