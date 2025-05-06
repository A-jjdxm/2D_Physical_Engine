#include "constraint.h"
#define DBL_INF std::numeric_limits<double>::max()
void collision::pocess()
{
	if (constraints.size()) return;
	for (int i=0; i < constraints.size(); i++) {

	}
}
bool collision::TestCollision(GLFWwindow* window, render* re, shape* a, shape* b) {
	int typeA = a->getclasstype(), typeB = b->getclasstype();
	if (typeA == Rectangle::ShapeType && typeB == Rectangle::ShapeType)
		return GJK( window,  re,(Rectangle*)a, (Rectangle*)b);
	if (typeA == Circle::ShapeType && typeB == Circle::ShapeType)
		return circlevscircle((Circle*)a, (Circle*)b);
	if (typeA == Circle::ShapeType && typeB == Rectangle::ShapeType)
		return SAT(window, re, (Circle*)a, (Rectangle*)b);
	if (typeA == Rectangle::ShapeType && typeB == Circle::ShapeType)
		return SAT(window, re, (Circle*)b, (Rectangle*)a);
}

void collision::clear()
{
	constraints.clear();
}

bool collision::circlevscircle(Circle *c1, Circle*c2)
{
	Aix2D n = c2->core - c1->core;
	float depth = c1->redius + c2->redius - mod(n);
	if (depth>=0)
	{
		Aix2D normal = vertexnormal(n);
		constraints.push_back(new Contactconstraint(c1, c2, c1->core + normal * c1->redius, normal, depth));
		return true;
	}
	else 
	{
		return false;
	}
}
bool collision::recttorect(Rectangle r1, Rectangle r2)
{
	bool collisionX = (r1.core.x + r1.rectWidth / 2 >= r2.core.x - r2.rectWidth / 2) && (r2.core.x + r2.rectWidth / 2 >= r1.core.x - r1.rectWidth / 2);
	bool collisionY = (r1.core.y + r1.rectHeight / 2 >= r2.core.y - r2.rectHeight / 2) && (r2.core.y + r2.rectHeight / 2 >= r1.core.y - r1.rectHeight / 2);
	return collisionX && collisionY;
}
bool collision::circletorect(Circle c, Rectangle r)
{
	float DeltaX = c.core.x - std::max(r.core.x- r.rectWidth / 2, std::min(c.core.x, r.core.x + r.rectWidth/2));

	float DeltaY = c.core.y - std::max(r.core.y - r.rectHeight / 2, std::min(c.core.y, r.core.y + r.rectHeight / 2));
	return (DeltaX * DeltaX + DeltaY * DeltaY) < (c.redius * c.redius);
}

bool collision::GJK(GLFWwindow* window,render * re, Rectangle* A, Rectangle* B)
{
	vector<Aix2D> simplex;
	int maxiter = 20;
	int iter = 0;
	Aix2D direction = A->core - B->core;
	Aix2D first = support(window,re, A, B, direction);
//	if(dcmp(vertexinner(first,first))==0) return true;
	simplex.push_back(first);
	direction = vertexreverse(direction);
	while (true)
	{
		Aix2D p = support(window,re,A,B,direction);
		if (dcmp(vertexinner(p, direction)) <= 0)
			return false;
		simplex.push_back(p);
		if (isinpolygon(simplex, direction)) {
			epa(window,re,simplex, A, B);
			return true;
		}
		iter = iter + 1;
		if (iter > maxiter) break;
	}
	cout << "没有找到" << endl;
	return false;
}

Aix2D collision::support(GLFWwindow* window,render *re, Rectangle* A, Rectangle* B,Aix2D dir)
{
	Aix2D a = A->support(window,re,dir);
	Aix2D b = B->support(window,re,vertexreverse(dir));
	return a - b;
}
 void collision::epa(GLFWwindow* window,render* re,vector<Aix2D>& initialSimplex, Rectangle* A, Rectangle* B) {
	std::vector<Edge> edges;
	// 将GJK的初始单纯形转换为多边形边
	for (int i = 0; i < initialSimplex.size(); ++i) {
		int j = (i + 1) % initialSimplex.size();
		edges.push_back(Edge(initialSimplex[i], initialSimplex[j]));
	}
	int index;
	for (int iter = 0; iter < 50; ++iter) {
		// 找到离原点最近的边
		Edge* closest = &edges[0];
		Aix2D p, normal;
		double minDistance = DBL_INF;
		index =0;
		for (int i = 0; i < edges.size();i++) {
			Aix2D h=getPerpendicularToOrigin(Aix2D(0,0), edges[i].a, edges[i].b);
			double distance = mod(h);
			if (distance < minDistance) {
				minDistance = distance;
				index = i;
				if (minDistance < 1e-6) {
					Aix2D tempn = floatcross(1.0f,(edges[i].b - edges[i].a));
					Aix2D temp = support(window, re, A, B, normal);
					if ((temp == edges[index].a) || (temp == edges[index].b)) {
						normal = tempn;
					}
					else {
						normal = -tempn;
					}
				}
				else {
					normal = Aix2D(h.x / minDistance, h.y / minDistance);
				}
			}
		}
		// 在法线方向上获取新的支撑点
		Aix2D new_p = support(window,re,A, B, normal);
		//float newDistance = vertexinner(new_p, normal);
		// 检查收敛条件
		if ((new_p==edges[index].a)||(new_p==edges[index].b)) {
			Aix2D Acontactpoint = A->support(window, re, normal);
			Aix2D Bcontactpoint = B->support(window, re, vertexreverse(normal));
			int width, height;
			glfwGetWindowSize(window, &width, &height);
			p.x = (Acontactpoint.x + Bcontactpoint.x) * 0.5f ;
			p.y = (Acontactpoint.y + Bcontactpoint.y) * 0.5f ;
			constraints.push_back(new Contactconstraint(A, B, p, normal, minDistance));
			return;
		}
		// 插入新点并重构多边形边
		edges.push_back(Edge(edges[index].a, new_p));
		edges.push_back(Edge(new_p, edges[index].b));
		edges.erase(edges.begin() + index);
	}
}
 bool collision::SAT(GLFWwindow* window, render* re, Circle* A, Rectangle* B)
 {
 	Aix2D n, p;
 	Edge edge;
 	double minDepth = DBL_INF;
 	vector<Vertex*> Points = B->getvertexdata(re);
 	for (int i = 0; i < B->npoints; i++) {
 		float min0, max0, min1, max1;
 		Edge tempEdge(Points[i]->XY(), Points[(i + 1) % Points.size()]->XY());
 		Aix2D normal = tempEdge.getNormal();
 		B->GetProjection(re,normal, min0, max0);
 		A->GetProjection(re,normal, min1, max1);
		if (max0 < min1 || max1 < min0) {
			return false;
		}
 		Aix2D tempP = getPerpendicularToOrigin(A->core,tempEdge.a,tempEdge.b);
 		if (dcmp(vertexinner((tempEdge.a - tempP), (tempEdge.b - tempP))) <= 0 && max0 < max1 && min1 < max0 && max0 - min1 < minDepth) {
 			minDepth = max0 - min1;
 			n = normal;
 			edge = tempEdge;
 			p = tempP;
 		}
 	}
	if (minDepth < DBL_INF) {
		constraints.push_back(new Contactconstraint(B, A, p, n, minDepth));
		return true;
	}
 	for (int i = 0; i < B->npoints; i++) {
 		Aix2D p = Points[i]->XY();
 		if (A->IsPointInside(p)) {
 			float depth = A->redius - mod(A->core - p);
 			constraints.push_back(new Contactconstraint(A, B, p, vertexnormal(p - A->core), depth));
 			return true;
 		}
 	}
 }
 void Contactconstraint::ProcessVelocity()
 {
	 Aix2D vab = B->GetPointVelocity(p) - A->GetPointVelocity(p);
	 Aix2D ra = p - A->core;
	 Aix2D rb = p - B->core;
	 float va = -(normal * vab);
	 float effectmass = A->inv_mass + B->inv_mass + sqr(vertexcross(ra, normal)) * A->inv_inertia + sqr(vertexcross(rb, normal)) * B->inv_inertia;
	 float J = (1+0.2)*va / effectmass;
	 if (J < 0)return;
	 Aix2D impluse =normal *J;
	 A->applyImpulse(ra, -impluse);
	 B->applyImpulse(rb, impluse);
	 
	 Aix2D tao = Aix2D(-normal.y, normal.x);
	 vab = B->GetPointVelocity(p) - A->GetPointVelocity(p);
	 float j = -(vab * tao) / (A->inv_mass + B->inv_mass + sqr(vertexcross(ra, tao)) * A->inv_inertia + sqr(vertexcross(rb, tao) * B->inv_inertia));
	 j = max(min(j, 0.3f*J ), - 0.3f*J);
	 Aix2D fImpulse =  tao * j;
	 A->applyImpulse(ra, -fImpulse);
	 B->applyImpulse(rb, fImpulse);

 }
 void Contactconstraint::ProcessPosition(float interval) {
	 Aix2D ra = p - A->core;
	 Aix2D rb = p - B->core;
	 double correctiveJ = ( 0.01*max(penetration - 0.1f,0.0f )/ interval) /(A->inv_mass + B->inv_mass + sqr(vertexcross(ra,normal)) * A->inv_inertia + sqr(vertexcross( rb,normal) * B->inv_inertia));
	 if (correctiveJ > 0) {
		 A->applyImpulse(ra,   normal*( - correctiveJ));
		 B->applyImpulse(rb, normal * (correctiveJ));
	 }
 }
