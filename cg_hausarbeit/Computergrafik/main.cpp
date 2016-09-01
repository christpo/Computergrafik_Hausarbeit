//
//  main.cpp
//  RealtimeRending
//
//  Created by Philipp Lensing on 22.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//


#include <iostream>
#include <math.h>
#ifdef WIN32
	#include <windows.h>
	#include <GL/glew.h>
	#include <GL/GLUT.h>
	#include <GL/GL.h>
#else
	#include <OpenGL/OpenGL.h>
	#include <GLUT/GLUT.h>
#endif
#include "Camera.h"
#include "texture.h"
#include "PlaneModel.h"
#include "terrain.h"
#include "shaderCompier.h"
#include "tank.h"
#include "cannon.h"
//#include "Tankmanager.h"
#include "Tankwave.h"
#include "collision.h"

using namespace std;


// window x and y size
const unsigned int g_WindowWidth=1024;
const unsigned int g_WindowHeight=768;

// light position (point light)
const Vector g_LightPos = Vector( 0,5,0);

//
Terrain g_TerrainModel;

Collision collision;

//
ShaderCompiler shaderCompiler;

//
Tank tank;

Cannon cannon;

Tankmanager manager;

Tankwave tankwave;

float dir;
float turn;

Camera g_Camera;
int g_MouseButton = 0;
int g_MouseState = 0;

void SetupDefaultGLSettings();
void DrawScene();
void MouseCallback(int Button, int State, int x, int y);
void MouseMoveCallback(int x, int y);
void KeyboardCallback( unsigned char key, int x, int y);

void MousePassiveMoveCallback(int x, int y);
void SpecialKeyboardCallback(int key, int x, int y);
void SpecialKeyboardUpCallback(int key, int x, int y);

float calcDeltaTime();

int main(int argc, char * argv[])
{
    // initialize the glut system and create a window
    glutInitWindowSize(g_WindowWidth, g_WindowHeight);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("CG Praktikum");
#ifdef WIN32
	glewInit();
#endif
    
    SetupDefaultGLSettings();
    
    glutDisplayFunc(DrawScene);
    glutMouseFunc(MouseCallback);
    glutKeyboardFunc(KeyboardCallback);
    glutMotionFunc(MouseMoveCallback);

	glutPassiveMotionFunc(MousePassiveMoveCallback);
	glutSpecialFunc(SpecialKeyboardCallback);
	glutSpecialUpFunc(SpecialKeyboardUpCallback);
    
	
	tank.load("prakt4modelle//tank_bottom.obj", "prakt4modelle//tank_top.obj", Vector(0, 0, 0), 10);
	tankwave.load();
	g_TerrainModel.load("heightmap.bmp", "sand.bmp", "sand.bmp", "mixmap.bmp", 60, 60, 0.2, "texCombinerVertex.glsl", "texCombinerFrag.glsl");
	//tankwave.getTankManager().startReset();

    glutMainLoop();
}


void SetupDefaultGLSettings()
{
    glClearColor(0, 0, 0, 255);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, (double)g_WindowWidth/(double)g_WindowHeight, 0.045f, 1000.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 
    // Setup Light Color
    GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 0.0f };
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    // Setup default material
    float diff[4] = {1,1,1,1};
    float amb[4]  = {0.2f,0.2f,0.2f,1};
    float spec[4] = {0.5f,0.5f,0.5f,1};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMateriali(GL_FRONT, GL_SHININESS, 30);
    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

}

void DrawGroundGrid()
{
    const float GridSize=10.0f;
    const unsigned int GridSegments=20;
    const float GridStep=GridSize/(float)GridSegments;
    const float GridOrigin=-GridSize*0.5f;
    
    glDisable( GL_LIGHTING);
    glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f);
        for( unsigned int i=0; i<GridSegments+1; i++)
        {
            float itpos=GridOrigin + GridStep*(float)i;
            glVertex3f(itpos, 0, GridOrigin);
            glVertex3f(itpos, 0, GridOrigin+GridSize);
        
            glVertex3f(GridOrigin, 0, itpos);
            glVertex3f(GridOrigin+GridSize, 0, itpos);

        }
    glEnd();
    glEnable( GL_LIGHTING);

}

void MouseCallback(int Button, int State, int x, int y)
{
    /*g_MouseButton = Button;
    g_MouseState = State;
    g_Camera.mouseInput(x,y,Button,State);*/
	if (State == 0) {
		
		if (tank.getShootingDelay() > 1.0f) {
			tank.cannonMgr.fireCannon(tank.cononMatrix);
			tank.setShootingDelay(0.0f);
		}
	}
}

void MouseMoveCallback( int x, int y)
{
    //g_Camera.mouseInput(x,y,g_MouseButton,g_MouseState);
}

void KeyboardCallback( unsigned char key, int x, int y)
{    
}

void MousePassiveMoveCallback(int x, int y)
{
	// function is called if the mouse moves without pressing any button.
	tank.aim(x, y, g_Camera);
}

void SpecialKeyboardCallback(int key, int x, int y)
{
	// function is called if a special keyboard button is pressed (e. g. Up-arrow-Key)

	if (key == GLUT_KEY_UP) {
		dir = 1.0f;
	}
	if (key == GLUT_KEY_DOWN) {
		dir = -1.0f;
	}

	if (key == GLUT_KEY_LEFT) {
		turn = 1.0f;
	}

	if (key == GLUT_KEY_RIGHT) {
		turn = -1.0f;
	}

	tank.steer(dir, turn, 1);
	g_Camera.follow(tank.getPosition());
}

void SpecialKeyboardUpCallback(int key, int x, int y)
{
	// function is called if a special keyboard button is released

	if (key == GLUT_KEY_UP) {
		dir = 0.0f;
	}
	if (key == GLUT_KEY_DOWN) {
		dir = 0.0f;
	}

	if (key == GLUT_KEY_LEFT) {
		turn = 0.0f;
	}

	if (key == GLUT_KEY_RIGHT) {
		turn = 0.0f;
	}

	tank.steer(dir, turn, 1);
	g_Camera.follow(tank.getPosition());
}

float calcDeltaTime() {

	//zeit setzen
	static int lastFrameTime = glutGet(GLUT_ELAPSED_TIME);
	//aktuelle zeit holen
	int currentFrameTime = glutGet(GLUT_ELAPSED_TIME);
	//delta bestimmen, in millisec.
	int deltaMs = currentFrameTime - lastFrameTime;
	//delta in sec.
	float delta = deltaMs*0.001f;
	//alte zeit setzten
	lastFrameTime = currentFrameTime;

	return delta;
}

void DrawScene()
{
	float deltatime = calcDeltaTime();
	//cout << "pos: " << g_Camera.getPosition().X << ", " << g_Camera.getPosition().Y << ", " << g_Camera.getPosition().Z << endl;
	//cout << "tar: " << g_Camera.getTarget().X << ", " << g_Camera.getTarget().Y << ", " << g_Camera.getTarget().Z << endl;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//kamera verfolgung
	g_Camera.follow(tank.getPosition());

    glLoadIdentity();
    g_Camera.apply();
    
    //DrawGroundGrid();
    
    GLfloat lpos[4];
    lpos[0]=g_LightPos.X; lpos[1]=g_LightPos.Y; lpos[2]=g_LightPos.Z; lpos[3]=1;
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);


	//kollision feindkugeln - spieler
	for (int i = 0; i < tankwave.getTankManager().getEnemys().size(); i++) {
		Tank* temp = tankwave.getTankManager().getEnemys()[i];

		if (temp->getIsActive()) {
			for (int k = 0; k < temp->cannonMgr.cannons.size(); k++) {
				if (temp->cannonMgr.cannons[k]->doDraw) {
					if (collision.satCollision(tank.cassis.obb, temp->cannonMgr.cannons[k]->cannon.obb)) {
						cout << "hit" << endl;
						
					}
				}
			}
		}
	}

	//kollision spielerkugeln - feinde
	for (int j = 0; j < tankwave.getTankManager().getEnemys().size(); j++) {
		Tank* temp = tankwave.getTankManager().getEnemys()[j];
		
		if (temp->getIsActive()) {
			for (int p = 0; p < tank.cannonMgr.cannons.size(); p++) {
				if (tank.cannonMgr.cannons[p]->doDraw) {
					if (collision.satCollision(temp->cassis.obb, tank.cannonMgr.cannons[p]->cannon.obb)) {
						cout << "hihi" << endl;
						temp->setIsActive(false);
						tank.cannonMgr.cannons[p]->doDraw = false;
					}
				}
			}
		}
	}
	tank.setShootingDelay(tank.getShootingDelay() + deltatime);
	tank.update(deltatime);
	
	tankwave.startWave(deltatime, 60.0f, tank, 5);

	g_TerrainModel.draw();

    glutSwapBuffers();
    glutPostRedisplay();
    
}
