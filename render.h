#pragma once
#include "shader.h" 
#include <vector>
#include <array>
struct Aix2D {
	float x;
	float y;
	Aix2D() : x(0.0f), y(0.0f) {}
	Aix2D(float a, float b)
		:x(a), y(b) {
	}
	Aix2D operator+(const Aix2D& v) const { return { x + v.x, y + v.y }; }
	Aix2D operator-(const Aix2D& v) const { return { x - v.x, y - v.y }; }
	Aix2D operator-() const { return { -x, -y }; }
	Aix2D operator*(float a) const { return { x *a, y *a }; }
	Aix2D operator/(float a) const { return { x / a, y / a }; }
	float operator*(const Aix2D& v) const { return x * v.x+ y * v.y; }
	bool operator==(const Aix2D& v) const { return (x==v.x)&&(y==v.y); }
};

struct Vertex {
	float x, y, z;
	float r, g, b;
	Vertex(float x =0, float y =0, float z =0, float r =0 , float g = 0, float b =0)
		:x(x), y(y), z(z), r(r), g(g), b(b) {}
	Aix2D XY() {
		return Aix2D(x, y);
	}
};
struct Circledata {
	Vertex v;
	float r;
};
class render
{
public:
	std::vector<Vertex> Rectriangle;
	std::vector<Circledata> Circle;
	std::vector<int> indices;
	render();
	~render();
	void DrawRectrangle(float width, float height);
	void DrawCircle(float width, float height);
	void flushrender();
	void flushcirclerender();
	void flushvertexs(Vertex v,int index);
	void Addvertexdata(vector<Vertex*> data);
	void Addindicesdata(unsigned int npoints);
	int Getindicessize();
	int Getcirclesize();
	void Clearvertexdata();
	void Clearindicesdata();
	void Clearcircledata();
	void addcircledata(Circledata xyz);
private:
	Shader *ourShader = new Shader("shader_v.txt", "shader_f.txt");
	Shader *ourShader1 =new Shader("shader1_v.txt", "shader1_f.txt");
	GLuint VAO[2], VBO[2], EBO;
};