#pragma once
#include <cassert>
#include"render.h"
class AABB {
public:
	float x0, y0, x1, y1;
	AABB(float x0, float y0, float x1, float y1) : x0(x0), y0(y0), x1(x1), y1(y1) {}
	AABB() {}
	void Enlarge(float w ) {
		x0 -= w; y0 -= w;
		x1 += w; y1 += w;
	}
	AABB& operator += (const AABB& other) {
		x0 = min(x0, other.x0);
		y0 = min(y0, other.y0);
		x1 = max(x1, other.x1);
		y1 = max(y1, other.y1);

	}
	AABB& operator += (const Aix2D& vec) {
		x0 = min(x0, vec.x);
		y0 = min(y0, vec.y);
		x1 = max(x1, vec.x);
		y1 = max(y1, vec.y);
	}
	bool Overlap(const AABB& other) {
		return (!(x1 < other.x0 || other.x1 < x0)) && (!(y1 < other.y0 || other.y1 < y0));
	}
};
int dcmp(double x);

bool OnSegment(Aix2D P1, Aix2D P2, Aix2D Q);

float vertexcross(Aix2D a, Aix2D b);

float vertexinner(Aix2D a, Aix2D b);

Aix2D floatinner(float a, Aix2D b);

Aix2D floatcross(float a, Aix2D b);
Aix2D vertexreverse(Aix2D a);
float mod(Aix2D a);
Aix2D verticalvertex(Aix2D a);
Aix2D vertexnormal(Aix2D a);
float sqr(float a);
bool isinpolygon(vector<Aix2D> &simplex,Aix2D &dir);

Aix2D getPerpendicularToOrigin(Aix2D dot, Aix2D a, Aix2D b);
