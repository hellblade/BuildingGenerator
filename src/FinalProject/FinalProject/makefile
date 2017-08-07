CC = g++
CFLAGS = -g -Wall -Wextra -pedantic -O2
IPATH = -I/usr/X11/include -I/usr/pkg/include -I../../Externals/Include -I../SharedLib -I../BuildingsLib
LPATH = -L/usr/X11/lib -L/usr/pkg/lib -L../../Externals/Lib
LDPATH = -Wl,-R/usr/pkg/lib 
RM = rm
all: FinalProject

FinalProject: ../BuildingsLib/SkyDomeShader.o ../BuildingsLib/SkyDome.o ../BuildingsLib/Frustum.o ../BuildingsLib/QuadTree.o ../BuildingsLib/BuildingTextureInfo.o ../SharedLib/Random.o ../SharedLib/G308_ImageLoader.o ../SharedLib/MathHelper.o ../BuildingsLib/BuildingShader.o ../BuildingsLib/gpc.o ../SharedLib/Buffers.o ../BuildingsLib/BuildingGenerator.o ../BuildingsLib/Building.o ../SharedLib/ColourShader.o ../SharedLib/BitmapLoader.o ../SharedLib/ShaderFile.o ../SharedLib/TextureShader.o Terrain.o GUI.o main.o
	$(CC) -o $@ $^ -lm -lGL -lGLU -lglut -ljpeg -l:libGLEW.a $(LPATH) $(LDPATH)
	
.cpp.o: 
	$(CC) $(CFLAGS) -c -o $@ $^ $(IPATH)
	
clean:
	$(RM) -f *.o *.gch FinalProject
