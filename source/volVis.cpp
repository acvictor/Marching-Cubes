#include <bits/stdc++.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <unistd.h>
#include "../include/model.h"

using namespace std;

#define mp make_pair
#define pb push_back
#define inf 1000000
#define length 150
#define width 62
#define depth 62
#define cout(a) cout<<a<<endl

Vertex vertices[length][width][depth];
Cube cubes[length - 1][width - 1][depth - 1];
enum {UP = 1, DOWN = 0};
int flag = 1, oldX = 0, oldY = 0, spin = 0, slices;
int mState = UP;
int h = 0, w = 0;
bool c = false, p = false;
float scale = 1, minval = INT_MAX, maxval = INT_MIN, transx = -1.5, transy = 0;
float rot[3]={0.0, 0.0, 0.0};
float da = 1, db = 0, dc = 0, dd = -4;

void read(char **argv) 
{
	FILE *infile;
	infile = fopen(argv[1], "r");
	int i, j, k;
	float value;

	for(k = 0; k < 248; k++) 
	{ // Z
		for(j = 0; j < 248; j++) 
		{ // Y
			for(i = 0; i < 600; i++) 
			{ // X
				fscanf(infile, "%f ", &value);
				// Sub sampling by taking every fourth z slice
				if(k % 4 != 0) continue;
				minval = min(minval, value);
				maxval = max(maxval, value);
				Vertex v(i, j, k, value);
				vertices[i/4][j/4][k/4] = v;
			}
		}
	}
	fclose(infile);
	cout << minval << " " << maxval << endl;
	
	for(i = 0; i < length - 1; i++) 
	{
		for(j = 0; j < width - 1; j++) 
		{
			for(k = 0; k < depth - 1; k++) 
			{
				cubes[i][j][k].vertices.pb(&vertices[i][j][k + 1]);
				cubes[i][j][k].vertices.pb(&vertices[i + 1][j][k + 1]);
				cubes[i][j][k].vertices.pb(&vertices[i + 1][j][k]);
				cubes[i][j][k].vertices.pb(&vertices[i][j][k]);
				
				cubes[i][j][k].vertices.pb(&vertices[i][j + 1][k + 1]);
				cubes[i][j][k].vertices.pb(&vertices[i + 1][j + 1][k + 1]);
				cubes[i][j][k].vertices.pb(&vertices[i + 1][j + 1][k]);
				cubes[i][j][k].vertices.pb(&vertices[i][j + 1][k]);
				cubes[i][j][k].setEdges();
			}
		}
	}
}

void isoSurflegend()
{
	float r = 0;
	float g = 0;
	float b = 0.25;
	float interval = (maxval - minval) / 5;
	float xval = 0, var = 0.27, val= 0.1;
	char conVal[5];
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	for(val = maxval; val >= minval; val -= interval) 
	{
		g = (val - minval) / (maxval-minval);
		glColor4f(r, g, b, .8);
		glBegin(GL_QUADS);
		glVertex3f(-0.7 + xval, -1, 0);
		glVertex3f(-0.7 + xval, -1.2, 0);
		g = (val - interval - minval) / (maxval - minval);
		glColor4f(r, g, b, .8);
		glVertex3f(-0.7 + var + xval, -1.2, 0);
		glVertex3f(-0.7 + var + xval, -1, 0);
		glEnd();
		
		sprintf(conVal, "%.2f", val);
		glRasterPos3f(-0.7 + xval, -1.3, 0);
		for(int digit=0; digit < 5; digit++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, conVal[digit]);
		}
		
		xval += 0.27;
	}
	sprintf(conVal, "%.2f", minval);
	glRasterPos3f(-0.7 + xval, -1.3, 0);
	for(int digit=0; digit < 5; digit++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, conVal[digit]);
	}
}

void drawIsosurface() 
{
	int *tempArr;
	Vertex *v;
	glPushMatrix(); 
	if(p) 
	{
		spin += 5;
	}

	glTranslatef(0, 0.33, 0);
	glRotatef(spin % 360, 1, 0, 0);
	glTranslatef(0, -0.33, 0);
	glRotatef(rot[0], 1, 0, 0);
	glRotatef(rot[1], 0, 1, 0);
	glTranslatef(transx, transy, 0);
	glScalef(scale, scale, scale);
	for (float isosurf = maxval; isosurf >= minval; isosurf -= (maxval - minval) / slices) 
	{

		float g = (isosurf - minval)/(maxval - minval);
		glColor4f(0, g, 0.25, g);

		for(int i = 0; i < length - 1; i++) 
		{
			for(int j = 0; j < width - 1; j++) 
			{
				for(int k = 0; k < depth - 1; k++) 
				{
					int code = 0;
					if(cubes[i][j][k].vertices[0]->val < isosurf) code |= 1;
					if(cubes[i][j][k].vertices[1]->val < isosurf) code |= 2;
					if(cubes[i][j][k].vertices[2]->val < isosurf) code |= 4;
					if(cubes[i][j][k].vertices[3]->val < isosurf) code |= 8;
					if(cubes[i][j][k].vertices[4]->val < isosurf) code |= 16;
					if(cubes[i][j][k].vertices[5]->val < isosurf) code |= 32;
					if(cubes[i][j][k].vertices[6]->val < isosurf) code |= 64;
					if(cubes[i][j][k].vertices[7]->val < isosurf) code |= 128;
					tempArr = getCodeVal(code);
					int l = 0;
					while (l < 16 && tempArr[l] != -1) 
					{
						glBegin(GL_TRIANGLES);
						v = cubes[i][j][k].edges[tempArr[l]]->interpolatePoint(isosurf);
						glVertex3f(v->x / length - 0.5, v->y /length - 0.33, v-> z / 500 - 0.6);
						
						v = cubes[i][j][k].edges[tempArr[l + 1]]->interpolatePoint(isosurf);
						glVertex3f(v->x / length - 0.5, v->y / length - 0.33, v->z / 500 - 0.6);
						
						v = cubes[i][j][k].edges[tempArr[l+2]]->interpolatePoint(isosurf);
						glVertex3f(v->x / length - 0.5, v->y / length - 0.33, v->z / 500 - 0.6);
						
						l += 3;
						glEnd();
					}	
				}
			}
		}
	}
	glPopMatrix();
	isoSurflegend();
}

void plane() 
{
	char conVal[30];
	sprintf(conVal, "%.1fx + %.1fy + %.1fz + %.1f = 0", da, db, dc, dd);
	glColor3f(1, 1, 1);
	glRasterPos3f(-.2, -1.5, 0);
	for(int digit=0; digit < 30; digit++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, conVal[digit]);
	}
}

void legend()
{
	float r = 0;
	float g = 0;
	float b = 0.25;
	float interval = (maxval - minval) / 5;
	float xval = 0, var = 0.27, val= 0.1;
	char conVal[5];
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	for(val = maxval; val >= minval; val -= interval) 
	{
		g = (val - minval) / (maxval-minval);
		glColor4f(r, g, b, .8);
		glBegin(GL_QUADS);
		glVertex3f(-0.7 + xval, -1, 0);
		glVertex3f(-0.7 + xval, -1.2, 0);
		g = (val - interval - minval) / (maxval - minval);
		glColor4f(r, g, b, .8);
		glVertex3f(-0.7 + var + xval, -1.2, 0);
		glVertex3f(-0.7 + var + xval, -1, 0);
		glEnd();
		
		sprintf(conVal, "%.2f", val);
		glRasterPos3f(-0.7 + xval, -1.3, 0);
		for(int digit=0; digit < 5; digit++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, conVal[digit]);
		}
		xval += 0.27;
	}

	sprintf(conVal, "%.2f", minval);
	glRasterPos3f(-0.7 + xval, -1.3, 0);
	for(int digit=0; digit < 5; digit++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, conVal[digit]);
	}

	plane();
}

void drawSlice() 
{
	int i, j, k, l;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Vertex *v, w;
	glPushMatrix(); 
	if(p) 
	{
		spin += 5;
	}

	glTranslatef(0, 0.33, 0);
	glRotatef(spin % 360, 1, 0, 0);
	glTranslatef(0, -0.33, 0);
	glRotatef(rot[0], 1, 0, 0);
	glRotatef(rot[1], 0, 1, 0);
	glTranslatef(transx, transy, 0);
	glScalef(scale, scale, scale);

	float mod = sqrt(da * da + db * db + dc * dc + dd * dd);
	float a = da, b = db, c = dc, d = dd;
	a /= mod; b /= mod;
	c /= mod; d /= mod;

	int code;
	float val;
	float vertDist[8];
	int *tempArr;
	
	for (i = 0; i < length - 1; i++) 
	{ //X
		for(j = 0; j < width - 1; j++) 
		{ //Y
			for (k = 0; k < depth - 1; k++) 
			{ //Z
				for(l = 0; l < 8; l++) 
				{
					vertDist[l] = cubes[i][j][k].vertices[l]->x * a +
						      	  cubes[i][j][k].vertices[l]->y * b +
						      	  cubes[i][j][k].vertices[l]->z * c + 
								  d;
				}
				code = 0;
				if (vertDist[0] < 0) code |= 1;
				if (vertDist[1] < 0) code |= 2;
				if (vertDist[2] < 0) code |= 4;
				if (vertDist[3] < 0) code |= 8;
				if (vertDist[4] < 0) code |= 16;
				if (vertDist[5] < 0) code |= 32;
				if (vertDist[6] < 0) code |= 64;
				if (vertDist[7] < 0) code |= 128;
				tempArr = getCodeVal(code);
				l = 0;
				while (l < 16 && tempArr[l] != -1) 
				{
					glBegin(GL_TRIANGLES);
					v = cubes[i][j][k].getEdge(tempArr[l]).interpolateValue(a,b,c,d);
					glColor3f(0, (v->val - minval) / (maxval - minval), 0.25);
					glVertex3f(v->x / length - 0.5, v->y / length - 0.33, v->z / 500 - 0.6);
					
					v = cubes[i][j][k].getEdge(tempArr[l + 1]).interpolateValue(a,b,c,d);
					glColor3f(0, (v->val - minval) / (maxval-minval), 0.25);
					glVertex3f(v->x / length - 0.5, v->y / length - 0.33, v->z / 500 - 0.6);
					
					v = cubes[i][j][k].getEdge(tempArr[l+2]).interpolateValue(a,b,c,d);
					glColor3f(0, (v->val - minval) / (maxval - minval), 0.25);
					glVertex3f(v->x / length - 0.5, v->y / length - 0.33, v->z / 500 - 0.6);
					
					l += 3;
					glEnd();
				}
			}
		}
	}
	glPopMatrix();
	legend();
}

void initRendering() 
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();    
	gluPerspective(45.0, w / h, 0.0, 10.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void handleResize(int wt, int ht)
{
	if (ht == 0)
	{
		ht = 1;
	}

	w = wt;
	h = ht;
	glViewport(0, 0, wt, ht);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, wt / ht, 0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) 
{
	if (key == char(27) || key == 'q')  
		exit(-1);

	if (key  == '+') 
		scale += 0.1;
	
	if (key == '-')
		scale -= 0.1;

	if(key == 'm') {
		if(c == true) c = false;
		else c = true;
	}

	if(key == 'p') {
		if(p == true) p = false;
		else p = true;
	}

	if (key == 'A')
	{
		da += 0.5;
	}
	
	if (key == 'B')
	{
		db += 0.5;
	}

	if (key == 'C')
	{
		dc += 0.5;
	}

	if (key == 'D')
	{
		dd += 0.5;
	}

	if (key == 'a')
	{
		da -= 0.5;
	}

	if (key == 'b')
	{
		db -= 0.5;
	}

	if (key == 'c')
	{
		dc -= 0.5;
	}

	if (key == 'd')
	{
		dd -= 0.5;
	}

	if (key == 'i')
	{
		cin >> da >> db >> dc >> dd;
	}
	
}

void glutMotion(int x, int y) 
{
    if (mState == DOWN) 
	{
      	if(oldY - y < 0)
		{
        	rot[0] += .2;      
		}
      	else 
		{
        	rot[0] -= .2;
		}
      
      	if(oldX - x < 0) 
		{
        	rot[1] += .2;    
		}
      	else
		{
        	rot[1] -= .2; 
		}
       
        glutPostRedisplay();
    }
    oldX = x;
    oldY = y;
}

void glutMouse(int button, int state, int x, int y) 
{
    if(state == GLUT_DOWN) 
	{
        switch(button) 
		{
            case GLUT_LEFT_BUTTON:
            case GLUT_RIGHT_BUTTON:
                mState = DOWN;
                oldX = x;
                oldY = y;
            break;
        }
    } 
    else if(state == GLUT_UP)
	{
        mState = UP;
	}
}

void specialKeys(int key, int x, int y) 
{
	if (key == GLUT_KEY_LEFT)
	{
		transx += 0.1;
	}

	if (key == GLUT_KEY_UP)
	{
		transy -= 0.1;
	}

	if (key == GLUT_KEY_RIGHT)
	{
		transx -= 0.1;
	}

	if (key == GLUT_KEY_DOWN)
	{
		transy += 0.1;
	}

	glutPostRedisplay();
}

void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	gluLookAt
	(
		0, 0, 5,
		0, 0, 0,
		0, 1, 0
	);
	if(c)
	{
		drawIsosurface();
	}
	else 
	{
		drawSlice();
	}

	glPopMatrix();
	glViewport(0, 0, w, h);
	glutSwapBuffers();
	usleep(1000);
	glutPostRedisplay();
}

int main(int argc, char **argv) 
{
	if(argc < 3)
	{
		cout << "Usage: ./exec <filename> <number of slices>\n";
	}

	read(argv);
	sscanf(argv[2], "%d", &slices);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1200, 800);
	glutInitWindowPosition(50,50);
	glutCreateWindow("Marching Cubes and Slicing");
	initRendering();
	glutDisplayFunc(display);
	glutReshapeFunc(handleResize);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);
	glutMotionFunc(glutMotion);
	glutMouseFunc(glutMouse);
	glutMainLoop();
	return 0; 
}

