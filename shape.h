#pragma once
#include"render.h"
#include"math.h"
class shape
{
public:
	int id;
	float mass;
	float inv_mass;
	float inertia;
	float inv_inertia;
	Aix2D speed;
	float ang_speed;
	Aix2D core;
	Aix2D force;
	int indice;
	bool active;
	bool awake;
	float angle;
	shape();
	virtual void gravitymove(GLFWwindow* window, render *re,float t)=0;
	void setindex(float x,float y);
	void switchaix(GLFWwindow* window,Aix2D aix,float &ndcX,float &ndcY);
	void clear();
	void move(vector<Vertex*> data, Aix2D offset);
	void rotate(vector<Vertex*> data, float angle);
	void process(GLFWwindow* window, render* re, float t);
	Aix2D GetPointVelocity(Aix2D p);
	Aix2D support(GLFWwindow* window, render* re, Aix2D direction);
	void applygravity(float t);
	void applyImpulse(Aix2D r,Aix2D impulse);
	void ApplyTorque(float torque);
	virtual void draw(GLFWwindow* window, render* re) = 0; // 绘制图形
	virtual void isout(GLFWwindow* window, render* re,float t) = 0; // 检查边界
	virtual vector<Vertex *> getvertexdata(render* re) = 0;//得到图形数据
	virtual int getclasstype() = 0; // 得到类型
	virtual AABB Getaabb(render* re)=0;
};
class Triangle :public shape
{
public:
	static const int ShapeType = 3;
	float edge = 0.1f;
	Triangle(GLFWwindow* window);
	void draw(GLFWwindow* window, render* re) override;
	vector<Vertex*> getvertexdata( render* re);
	void gravitymove(GLFWwindow* window, render *re, float t);
	void isout(GLFWwindow* window, render* re, float t);
	int getclasstype();
	AABB Getaabb(render* re);
};
class Rectangle :public shape
{
public:
	static const int ShapeType = 1;
	const float rectWidth = 60.0f;
	const float rectHeight = 30.0f;
	int npoints;
	Rectangle(GLFWwindow* window);
	void draw(GLFWwindow* window, render* re) override;
	vector<Vertex*> getvertexdata( render* re);
	void gravitymove(GLFWwindow* window, render *re, float t);
	void isout(GLFWwindow* window, render* re,float t);
	int getclasstype();
	AABB Getaabb(render* re);
	void GetProjection(render* re, Aix2D n, float& minI, float& maxI);
};
class Circle :public shape
{
public:
	static const int ShapeType = 2;
	const float redius = 30.0f;
	Circle(GLFWwindow* window);
	void draw(GLFWwindow* window, render* re) override;
	vector<Vertex*> getvertexdata( render* re);
	void gravitymove(GLFWwindow* window, render* re, float t);
	void isout(GLFWwindow* window, render* re, float t);
	int getclasstype();
	AABB Getaabb(render* re);
	void GetProjection(render* re, Aix2D n, float& minI, float& maxI);
	bool IsPointInside(Aix2D p);
};


