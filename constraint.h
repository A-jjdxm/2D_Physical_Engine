#pragma once
#include <algorithm>
#include"shape.h"
#include "collision.h"
struct Edge
{
	Aix2D a;
	Aix2D b;
	Edge(Aix2D a, Aix2D b) :a(a), b(b){}
	Edge(){}
	Aix2D getNormal() {
		return vertexnormal(floatcross(1, b-a)) ;
	}
};
class Contactconstraint
{
public:
	shape* A;
	shape* B;
	float penetration;
	Aix2D normal, p;
	Contactconstraint(shape* a, shape* b, Aix2D p, Aix2D normal, float penetration) : A(a), B(b), p(p), normal(normal), penetration(penetration) {}
	void ProcessVelocity();
	void ProcessPosition(float interval);
};
class collision
{
public:
	vector<Contactconstraint*> constraints;

	void pocess();
	bool TestCollision(GLFWwindow* window, render* re, shape* a, shape* b);
	void clear();

	bool circlevscircle(Circle* c1, Circle* c2);

	bool recttorect(Rectangle r1, Rectangle r2);

	bool circletorect(Circle c, Rectangle r);

	bool GJK(GLFWwindow* window, render* re, Rectangle* A, Rectangle* B);
	bool SAT(GLFWwindow* window, render* re, Circle* A, Rectangle* B);


	Aix2D support(GLFWwindow* window, render* re, Rectangle* A, Rectangle* B, Aix2D dir);

	void epa(GLFWwindow* window, render* re, vector<Aix2D>& initialSimplex, Rectangle* A, Rectangle* B);


};
