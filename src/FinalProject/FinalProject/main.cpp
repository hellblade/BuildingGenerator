#define GLM_FORCE_RADIANS
#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <string.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include <BitmapLoader.h>
#include <TextureShader.h>
#include <ColourShader.h>
#include <BuildingShader.h>
#include <Building.h>
#include <BuildingGenerator.h>
#include "define.h"
#include "GUI.h"
#include "Terrain.h"
#include <G308_ImageLoader.h>
#include <BuildingTextureInfo.h>
#include <Frustum.h>
#include <QuadTree.h>
#include <SkyDome.h>
#include <SkyDomeShader.h>
//#include "vld.h"

GLuint g_mainWnd;
GLuint g_nWinWidth = G308_WIN_WIDTH;
GLuint g_nWinHeight = G308_WIN_HEIGHT;

void G308_Reshape(int w, int h);
void G308_display();
void mainMenu(int);
void G308_mouseListener(int button, int state, int x, int y);
void G308_mousePassiveListener(int x, int y);

GUI *myGUI;
GLuint tex;
TextureShader *shader;
ColourShader *colourShader;
BuildingShader *buildingShader;
glm::fmat4x4 persp;
Terrain* ground;
Building* building;
bool lookAbove;

void keypress(unsigned char character, int x, int y);
float rot = 0;
float maxAnisotropy;

GLuint brick;

Vector3 lightDir = Vector3(0, -1, 1);
Vector3 ambient = Vector3(0.6f, 0.6f, 0.6f);
Frustum * frustum;

float x = 0.0f;
float y = 0.0f;
float z = 0.0f;

float x2 = 0.0f;
float z2 = 0.0f;

QuadTree* quadTree;
SkyDomeShader* skyShader;
SkyDome *skyDome;
float timeOfDay = 0.0f;

#define SKYTIME 10

float cameraRotX, cameraRotY;
float cameraTransX = 0.0f, cameraTransY = 0.0f;
float cameraZoom = 0.0f;
int startX, startY;
float initX, initY;
bool mouseDown;
bool mouseRightDown;
bool mouseMiddleDown;

void SkyDomeAnimation(int);

int width = 50;
int labelWidth;
char* currentWidthText = NULL;

int height = 50;
int labelHeight;
char* currentHeightText = NULL;

int depth = 50;
int labelDepth;
char* currentDepthText = NULL;


int main(int argc, char** argv)
{
    srand(time(NULL));
    BitmapData data = LoadBitmap("Grass.bmp");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(g_nWinWidth, g_nWinHeight);
    g_mainWnd = glutCreateWindow("COMP308 Final Project");

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        printf("GLEW failed to start\n");
        return EXIT_FAILURE;
    }

    if (glewIsSupported("GL_VERSION_3_0"))
    {
        printf("OpenGL 3.0 supported.\n");
    }
    else {
        printf("OpenGL 3.0 not supported !\n");
        return EXIT_FAILURE;
    }

    if (glewGetExtension("GL_EXT_texture_filter_anisotropic"))
    {
        printf("Can use anisotropic filtering\n");
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
    }
    else
    {
        maxAnisotropy = 0.0f;
        printf("Cannot use anisotropic filtering\n");
    }

    int textureUnits;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);

    printf("%d Texture Units Supported\n", textureUnits);

    if (textureUnits < 4)
    {
        printf("Require at least 4 texture units\n");
        return EXIT_FAILURE;
    }

    BuildingTextureInfo::Load();

    shader = new TextureShader();
    shader->Initilise();
    
    colourShader = new ColourShader();
    colourShader->Initilise();
    
    buildingShader = new BuildingShader();
    buildingShader->Initilise();

    skyShader = new SkyDomeShader();
    skyShader->Initilise();

    skyDome = new SkyDome();

    quadTree = new QuadTree(Rectangle3D(-1000.0f, -1000.0f, 2000.0f, 2000.0f, 100.0f));
    
    BuildingInfo info = BuildingGenerator::CreateInfo(-25.0f, -25.0f, 50, 50, 50, true);    
    building = BuildingGenerator::GenerateBuilding(info);

    quadTree->Add(building);

    tex = MakeGLTexture(data, maxAnisotropy);
    ground = new Terrain();
    ground->Create(600, 600, 50, 50);
    
    glutDisplayFunc(G308_display);
    glutReshapeFunc(G308_Reshape);
    glutKeyboardFunc(keypress);
    glutMotionFunc(G308_mousePassiveListener);
    glutPassiveMotionFunc(G308_mousePassiveListener);
    glutMouseFunc(G308_mouseListener);
    glutTimerFunc(SKYTIME, SkyDomeAnimation, 1);

    myGUI = new GUI(mainMenu);    
    labelWidth = myGUI->AddLabel(0, 0, "Width = 50");
    labelHeight = myGUI->AddLabel(0, 100, "Height = 50");
    labelDepth = myGUI->AddLabel(0, 200, "Depth = 50");
    
    frustum = new Frustum();

    glutMainLoop();
    
    shader->Shutdown();
    colourShader->Shutdown();
    buildingShader->Shutdown();

    BuildingTextureInfo::Unload();

    delete shader;
    delete colourShader;
    delete buildingShader;

    return EXIT_SUCCESS;
}

void G308_display()
{

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);      

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glm::fmat4x4 trans, trans2, identity;

    identity = glm::mat4(1.0f);

   // glTranslatef(cameraTransX, cameraTransY, cameraZoom);
 //   glRotatef(cameraRotX, 1, 0, 0);
  //  glRotatef(cameraRotY + extraRotY, 0, 1, 0);
    
    if (lookAbove)
    {
        trans = glm::lookAt(glm::vec3(x2, 100.0f, z2), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    else
    {
	    trans = glm::lookAt(glm::vec3(25, 2, 100), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));            
    }                
    
    trans2 = glm::rotate(identity, cameraRotX, glm::vec3(1.0f, 0.0f, 0.0f));
    trans2 = glm::rotate(trans2, cameraRotY, glm::vec3(0.0f, 1.0f, 0.0f));    
    trans2 = glm::translate(trans2, glm::vec3(25, 2, 100));        
    trans2 = glm::scale(trans2, glm::vec3(150, 150, 150));
    

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);    

    skyShader->SetShader();
    skyShader->SetParam(trans2, trans, persp, timeOfDay);

    skyDome->Render();

    glEnable(GL_DEPTH_TEST);

    trans = glm::translate(trans, glm::vec3(cameraTransX, cameraTransY, cameraZoom));
    trans = glm::rotate(trans, cameraRotX, glm::vec3(1.0f, 0.0f, 0.0f));
    trans = glm::rotate(trans, cameraRotY, glm::vec3(0.0f, 1.0f, 0.0f));

    shader->SetShader();
    shader->SetParam(persp * trans);    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    ground->Draw();
    glBindTexture(GL_TEXTURE_2D, 0);
       

    frustum->SetPlanes(persp * trans);
   // printf("Building in frustum: %s...%f\n", frustum->InFrustum(building->bounds) ? "Yes" : "No", x);

    //colourShader->SetShader();
    //colourShader->SetParam(persp * trans);    

    buildingShader->SetShader();
    buildingShader->SetParam(trans, glm::mat4(1.0f), persp, lightDir, ambient);

    //quadTree->DrawAll(frustum);
    building->Draw();
    
    glUseProgram(0);
    
    /*glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        printf("%s\n", gluErrorString(err));
    }
    
    

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);*/

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, g_nWinWidth, g_nWinHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    myGUI->Draw();

    glutSwapBuffers();
}

void keypress(unsigned char character, int xx, int y)
{
    bool changedW, changedH, changedD;
    changedW = changedH = changedD = false;

    if (character == 'b')
    {
        BuildingInfo info = BuildingGenerator::CreateInfo(-width * 0.5f, -depth * 0.5f, width, depth, height, true);
        delete building;
        building = NULL;        
        building = BuildingGenerator::GenerateBuilding(info);        
    }
    else if (character == 'q')
    {
        width = Max(width - 1, 5);
        changedW = true;
    }
    else if (character == 'w')
    {
        width += 1;
        changedW = true;
    }
    else if (character == 'a')
    {
        height = Max(height - 1, 5);
        changedH = true;
    }
    else if (character == 's')
    {
        height += 1;
        changedH = true;
    }
    else if (character == 'z')
    {
        depth = Max(depth - 1, 5);
        changedD = true;
    }
    else if (character == 'x')
    {
        depth += 1;
        changedD = true;
    }

    if (changedW)
    {
        using namespace std;
        stringstream strs;
        strs << "Width = ";
        strs << width;

        //using namespace std;
        string temp_str = strs.str();
        const char* info = temp_str.c_str();

        int newLength = strlen(info) + 1;

        if (currentWidthText != NULL)
        {
            delete[] currentWidthText;
        }

        currentWidthText = new char[newLength];
        strcpy(currentWidthText, info);

        myGUI->SetLabelText(labelWidth, currentWidthText);
    }
    else if (changedH)
    {
        using namespace std;
        stringstream strs;
        strs << "Height = ";
        strs << height;

        //using namespace std;
        string temp_str = strs.str();
        const char* info = temp_str.c_str();

        int newLength = strlen(info) + 1;

        if (currentHeightText != NULL)
        {
            delete[] currentHeightText;
        }

        currentHeightText = new char[newLength];
        strcpy(currentHeightText, info);

        myGUI->SetLabelText(labelHeight, currentHeightText);
    }
    else if (changedD)
    {
        using namespace std;
        stringstream strs;
        strs << "Depth = ";
        strs << depth;

        //using namespace std;
        string temp_str = strs.str();
        const char* info = temp_str.c_str();

        int newLength = strlen(info) + 1;

        if (currentDepthText != NULL)
        {
            delete[] currentDepthText;
        }

        currentDepthText = new char[newLength];
        strcpy(currentDepthText, info);

        myGUI->SetLabelText(labelDepth, currentDepthText);
    }

    glutPostRedisplay();
}

// Reshape function
void G308_Reshape(int w, int h)
{
    if (h == 0)
        h = 1;

    g_nWinWidth = w;
    g_nWinHeight = h;

    myGUI->width = g_nWinWidth;
    myGUI->height = g_nWinHeight;
    glViewport(0, 0, g_nWinWidth, g_nWinHeight);
    persp = glm::perspectiveFov((float)(M_PI / 2), (float)w, (float)h, 1.0f, 1000.0f);
   // persp = glm::ortho(0, 1, 0, 1);
}

void mainMenu(int item)
{

}

void SkyDomeAnimation(int i)
{
    timeOfDay += 0.0001f;
    glutPostRedisplay();
    glutTimerFunc(SKYTIME, SkyDomeAnimation, 1);
}

void G308_mouseListener(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        initX = x;
        initY = y;
    }

    if (button == GLUT_LEFT_BUTTON)
    {
        mouseDown = state == GLUT_DOWN;
    }
    else if (button == GLUT_RIGHT_BUTTON)
    {
        mouseRightDown = (state == GLUT_DOWN);
    }
    else if (button == GLUT_MIDDLE_BUTTON)
    {
        mouseMiddleDown = (state == GLUT_DOWN);
    }

    glutPostRedisplay();
}

void G308_mousePassiveListener(int x, int y)
{
    if (mouseDown)
    {
        if (initX == x && initY == y)
            return;

        int diffX = x - initX;
        int diffY = y - initY;

        cameraRotY += 0.5f * diffX * 0.0174532925f;
        cameraRotX = Clamp(0.0174532925f * 0.5f * diffY + cameraRotX, -(float)M_PI_2, (float)M_PI_2);

        initX = x;
        initY = y;
    }
    else if (mouseRightDown)
    {
        cameraTransX += 0.25f * (initX - x);
        cameraTransY -= 0.25f * (initY - y);

        initX = x;
        initY = y;
    }
    else if (mouseMiddleDown)
    {
        cameraZoom += 0.25f * (initY - y);
        initY = y;
    }

    glutPostRedisplay();
}