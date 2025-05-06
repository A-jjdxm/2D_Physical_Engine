#include "shape.h"
#include<cmath>
#define g 9.18
#define PI 3.1415926



shape::shape()
{
	this->mass = 0;
	this->inv_mass = 0;
	this->inertia = 0;
	this->inv_inertia = 0;
	this->active = true;
	this->awake = true;
	this->angle = 0;
	this->speed = Aix2D();
	this->ang_speed = 0;
	this->core = Aix2D();
	this->force = Aix2D();
}


void shape::setindex(float x, float y)
{
	this->core.x = x;
	this->core.y = y;
}

void shape::switchaix(GLFWwindow* window, Aix2D aix, float &ndcX, float&ndcY)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	//ndcX = ((2.0f * aix.x) / width - 1.0f);
	//ndcY = 1.0f - (2.0f * aix.y) / height;
	ndcX = aix.x - width / 2;
	ndcY = height/2 - aix.y;
}


void shape::clear()
{
}

Triangle::Triangle(GLFWwindow* window)
{
	this->mass = mass;
	this->speed.x = 20;
	double xpos, ypos;
	float ndcX, ndcY;
	glfwGetCursorPos(window, &xpos, &ypos);
	switchaix(window,Aix2D(xpos,ypos), ndcX, ndcY);
	this->core.x = ndcX;
	this->core.y = ndcY;
}

void Triangle::draw(GLFWwindow* window, render* re)
{
	float ndcX, ndcY;
	ndcX = this->core.x;
	ndcY = this->core.y;
	cout << ndcX << ndcY << endl;
	float left = ndcX - edge / 2.0f;
	float right = ndcX + edge / 2;
	float top = ndcY + edge / 2 *sqrt(3);
}

vector<Vertex*> Triangle::getvertexdata( render* re)
{
	vector<Vertex*>data;
	for (int i = 0; i < 3; i++) {
		//data[i] = re->Rectriangle[re->indices[this->indice]+i];
	}
	return data;
}

void Triangle::gravitymove(GLFWwindow* window, render *re, float t)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	float dispoffset_x = this->speed.x *t*2.0f/ width;
	float dispoffset_y = (this->speed.y * t + g * t * t / 2)*2.0f/ height;
	float speedoffset_x = 0;
	float speedoffset_y = g * t;
	this->speed.x += speedoffset_x;
	this->speed.y += speedoffset_y;
	this->core.x = this->core.x + dispoffset_x;
	this->core.y = this->core.y + dispoffset_y;
}

void Triangle::isout(GLFWwindow* window, render* re, float t)
{
	
}

int Triangle::getclasstype()
{
	return ShapeType;
}

AABB Triangle::Getaabb(render* re)
{
	return AABB();
}



Rectangle::Rectangle(GLFWwindow* window)
{
	this->mass = 1;
	double xpos, ypos;
	float ndcX, ndcY;
	glfwGetCursorPos(window, &xpos, &ypos);
	switchaix(window, Aix2D(xpos, ypos), ndcX, ndcY);
	//int width, height;
	//glfwGetWindowSize(window, &width, &height);
	//ndcX = xpos - width / 2;
	//ndcY = ypos - height / 2;
	this->core.x = ndcX;
	this->core.y = ndcY;
	this->inv_mass = 1 / mass;
	this->inertia = this->mass*(sqr(this->rectWidth)+sqr(this->rectHeight))/12;
	this->inv_inertia = 1 / this->inertia;
	this->angle = 0;
	this->ang_speed = 0;
}

void Rectangle::draw(GLFWwindow* window, render* re)
{
	float ndcX, ndcY;
	ndcX = this->core.x;
	ndcY = this->core.y;
	float left = ndcX - rectWidth / 2;
	float right = ndcX + rectWidth / 2;
	float top = ndcY + rectHeight / 2;
	float bottom = ndcY - rectHeight / 2;
	Vertex v1 = Vertex(left, top, 0.0f, 0.0f, 1.0f, 0.0f);
	Vertex v2 = Vertex(left, bottom, 0.0f, 0.0f, 1.0f, 0.0f);
	Vertex v3 = Vertex(right, top, 0.0f, 0.0f, 1.0f, 0.0f);
	Vertex v4 = Vertex(right, bottom, 0.0f, 0.0f, 1.0f, 0.0f);
	vector<Vertex*> data;
	data.push_back(&v1);
	data.push_back(&v3);
	data.push_back(&v4);
	data.push_back(&v2);
	rotate(data, this->angle);
	this->npoints = data.size();
	re->Addvertexdata(data);
	this->indice=re->Getindicessize();
	re->Addindicesdata(npoints);
	re->flushrender();
}

vector<Vertex*> Rectangle::getvertexdata( render* re)
{
	vector<Vertex*> data;
	for (int i = 0; i < npoints; i++) {
		data.push_back(&re->Rectriangle[re->indices[this->indice] + i]);
	}
	return data;
}

void Rectangle::gravitymove(GLFWwindow* window, render *re, float t)
{
	/*int width, height;
	glfwGetWindowSize(window, &width, &height);
	float speedoffset_x = (this->force.x*this->inv_mass)*t;
	float speedoffset_y = (this->force.y * this->inv_mass) * t;
	this->speed.x += speedoffset_x;
	this->speed.y += speedoffset_y;

	isout(re,t);
	float dispoffset_x = this->speed.x*t * 2.0f / width;
	float dispoffset_y = this->speed.y*t * 2.0f / height;
	this->core.x = this->core.x + dispoffset_x;
	this->core.y = this->core.y + dispoffset_y;
	float angle_offset = this->ang_speed * t;
	this->angle += angle_offset;
	for (int i = 0; i < 4; i++) {
		move(&re->Rectriangle[re->indices[this->indice] + i], dispoffset_x, dispoffset_y);
		rotate(&re->Rectriangle[re->indices[this->indice] + i], this->core, angle_offset);
	}*/
}

void Rectangle::isout(GLFWwindow* window, render* re,float t)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	float offset_x = 0;
	float offset_y = 0;
	for (int i = 0; i < 4; i++) {
		Aix2D vdata = Aix2D(re->Rectriangle[re->indices[this->indice] + i].x, re->Rectriangle[re->indices[this->indice] + i].y);
		if (vdata.y <= -height/2 || vdata.y>= height / 2) {
			Aix2D normal = Aix2D(0, 1);
			Aix2D r = vdata - this->core;
			offset_y = (-height / 2)- vdata.y;
			float effectMass = 1/(this->inv_mass + sqr(vertexcross(r, normal)) * this->inv_inertia);
			float constraint = -(( this->speed + floatcross(this->ang_speed, r))*normal)+0.1*max(float(offset_y-0.1),0.0f)/t;
			float impulse =  effectMass * constraint;
			if (impulse > 0) {
				applyImpulse(r, floatinner(impulse, normal));
			}
		}
	}
}

int Rectangle::getclasstype()
{
	return ShapeType;
}

AABB Rectangle::Getaabb(render* re )
{
	AABB ret;
	GetProjection(re, Aix2D(1, 0), ret.x0, ret.x1);
	GetProjection(re, Aix2D(0, 1), ret.y0, ret.y1);
	return ret;
}

void Rectangle::GetProjection(render* re, Aix2D n, float& minI, float& maxI)
{
	vector<Vertex*> data;
	data = getvertexdata(re);
	minI = maxI = Aix2D(data[0]->x, data[0]->y) * n;
	for (int i = 0; i < data.size(); i++) {
		float prod = Aix2D(data[i]->x,data[i]->y) * n;
		if (prod < minI) minI = prod;
		if (prod > maxI) maxI = prod;
	}

}

Circle::Circle(GLFWwindow* window) 
{
	this->id = id;
	this->mass = 1;
	this->inertia = sqr(sqr(redius)) * PI / 2;
	double xpos, ypos;
	float ndcX, ndcY;
	glfwGetCursorPos(window, &xpos, &ypos);
	switchaix(window, Aix2D(xpos, ypos), ndcX, ndcY);
	this->core.x = ndcX;
	this->core.y = ndcY;
	this->inv_mass = 1 / mass;
	this->inv_inertia = 1 / this->inertia;
	this->angle = 0;

}

void Circle::draw(GLFWwindow* window, render* re)
{
	float ndcX, ndcY;
	ndcX = this->core.x;
	ndcY = this->core.y;
	Circledata cir;
	cir.v = { ndcX,ndcY,0.0f,0.0f,0.0f,1.0f };
	cir.r = this->redius;
	this->indice = re->Getcirclesize();
	re->addcircledata(cir);
	re->flushcirclerender();

}

vector<Vertex*> Circle::getvertexdata( render* re)
{
	vector<Vertex*> data;
	data.push_back(&re->Circle[this->indice].v);
	return data;
}

void Circle::gravitymove(GLFWwindow* window, render* re, float t)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	float dispoffset_x = 0.0f * 2.0f / width;
	float dispoffset_y = (this->speed.y * t + g * t * t / 2) * 2.0f / height;
	float speedoffset_x = 0;
	float speedoffset_y = g * t;
	this->speed.x += speedoffset_x;
	this->speed.y += speedoffset_y;
	this->core.x = this->core.x + dispoffset_x;
	this->core.y = this->core.y + dispoffset_y;

}

void Circle::isout(GLFWwindow* window, render* re, float t)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	Aix2D normal = Aix2D(0, 1);
	float beta;
	float offset_x = 0;
	float offset_y = 0;
	if (this->core.y - redius < -height / 2 || this->core.y + redius> height / 2) {
		Aix2D r = Aix2D(this->core.x, this->core.y - redius) - this->core;
		offset_y = (-height / 2)-(this->core.y - redius);
		float effectMass = 1 / (this->inv_mass + sqr(vertexcross(r, normal)) * this->inv_inertia);
		float constraint = -((this->speed + floatcross(this->ang_speed, r)) * normal)+0.5* offset_y/t;
		float impulse = effectMass * constraint;
		applyImpulse(r, floatinner(impulse, normal));
	}
}
int Circle::getclasstype()
{
	return ShapeType;
}
AABB Circle::Getaabb(render* re)
{
	return AABB(this->core.x-redius,this->core.y-redius,this->core.x+redius,this->core.y+redius);
}
void Circle::GetProjection(render* re, Aix2D n, float& minI, float& maxI)
{
	minI = maxI = this->core * n;
	minI -= redius;
	maxI+= redius;
}
bool Circle::IsPointInside(Aix2D p)
{
	if ((p - this->core) * (p - this->core) <= sqr(this->redius)) {
		return true;
	}
	else {
		return false;
	}
}
//
void shape::move(vector<Vertex*> data,Aix2D offset) {
	for (int i = 0; i < data.size(); i++) {
		data[i]->x = data[i]->x + offset.x;
		data[i]->y = data[i]->y + offset.y;
	}
}
void shape::rotate(vector<Vertex *> data, float angle)
{
	float radians = angle ;
	Aix2D center = this->core;
	float cosa = cos(radians);
	float sina = sin(radians);
	for (int i = 0; i < data.size(); i++) {
		Aix2D dot = Aix2D(data[i]->x - center.x, data[i]->y - center.y);
		data[i]->x = dot.x * cosa - dot.y * sina + center.x;
		data[i]->y = dot.x * sina + dot.y * cosa + center.y;
	}

}
void shape::process(GLFWwindow* window, render* re,float t)
{
	Aix2D offset = this->speed * t;
	float angoffset = this->ang_speed * t;
	this->core = this->core + offset;
	this->angle = this->angle + angoffset;
	vector<Vertex*> data = this->getvertexdata(re);
	move(data, offset);
	if (this->getclasstype() == 2)return;
	rotate(data,angoffset);

}
Aix2D shape::GetPointVelocity(Aix2D p)
{
	return this->speed+floatcross(this->ang_speed,(p-this->core));
}
Aix2D shape::support(GLFWwindow* window,render* re,Aix2D direction)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	Aix2D best;
	vector<Vertex *> data;
	data = getvertexdata(re);
	float maxDot = -INFINITY;
	for (int i = 0;i<data.size();i++) {
		Aix2D v = Aix2D(data[i]->x, data[i]->y);
		float dot = vertexinner(v, direction);
		if (dot > maxDot + 1e-6) {
			maxDot = dot;
			best = Aix2D(v.x, v.y);
		}

	}
	return best;

}
void shape::applygravity(float t)
{
	applyImpulse(Aix2D(0,0), Aix2D(0, -g) * mass * t);
}
void shape::applyImpulse(Aix2D r, Aix2D impulse)
{
	this->speed = this->speed + impulse*this->inv_mass;
	ApplyTorque(vertexcross(r,impulse));
}

void shape::ApplyTorque(float torque)
{
	this->ang_speed = this->ang_speed + torque * this->inv_inertia;
}
