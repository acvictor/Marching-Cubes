#include <bits/stdc++.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <unistd.h>

using namespace std;

int* getCodeVal(int i);

class Vertex 
{
public:
	float x, y, z;
	float val;
	Vertex() 
	{
		x = y = z = 0.0;
		val = 0.0;
	}

	Vertex(float xCoor, float yCoor, float zCoor, float v) 
	{
		x = xCoor;
		y = yCoor;
		z = zCoor;
		val = v;
	}
};

class Edge 
{
public:
	Vertex *a, *b;
	Edge() 
	{
		a = b = NULL;
	}

	Edge(Vertex va, Vertex vb) 
	{
		a = new Vertex(va.x, va.y, va.z, va.val);
		b = new Vertex(vb.x, vb.y, vb.z, vb.val);
	}
	Vertex* interpolatePoint(float);
	Vertex* interpolateValue(float, float, float, float);
};

class Cube 
{
public:
	float val;
	int code, caseNo;
	vector<Edge*> edges;
	vector<Vertex*> vertices;
	Cube() 
	{
		code = caseNo = -1;
		val = -1;
	}

	Cube(vector<Vertex*> v, vector<Edge*> e) 
	{
		vertices = v;
		edges = e;
		Cube();
	}
	
	void setEdges();
	Edge getEdge(int);
};