#include"math.h"

int dcmp(double x)
{
	if (fabs(x) < 1e-6) return 0;
	else
		return x < 0 ? -1 : 1;
}
bool OnSegment(Aix2D P1, Aix2D P2, Aix2D Q)
{
	//前一个判断点Q在P1P2直线上 后一个判断在P1P2范围上
	return dcmp(vertexcross((P1 - Q) , (P2 - Q))) == 0 && dcmp(vertexinner((P1 - Q) , (P2 - Q))) <= 0;
}
float vertexcross(Aix2D a, Aix2D b)
{
	return a.x * b.y - a.y * b.x;
}
float vertexinner(Aix2D a, Aix2D b)
{
	return a.x * b.x + a.y * b.y;
}
Aix2D floatinner(float a, Aix2D b)
{
	return Aix2D(b.x * a, b.y * a);
}
Aix2D floatcross(float a, Aix2D b)
{
	return Aix2D(-b.y * a, b.x * a);
}

Aix2D vertexreverse(Aix2D a)
{
	return Aix2D(-a.x,-a.y);
}

float mod(Aix2D a)
{
	return sqrt(a.x*a.x+a.y*a.y);
}

Aix2D verticalvertex(Aix2D a)
{
	return Aix2D();
}

Aix2D vertexnormal(Aix2D a)
{
	float mod = sqrt(a.x * a.x + a.y * a.y);
	return Aix2D(a.x/mod,a.y/mod);
}
float sqr(float a)
{
	return a*a;
}
Aix2D tripleProd(Aix2D a, Aix2D b, Aix2D c)
{
	float out = dcmp(vertexcross(a, b));
	return floatcross(out, c);
}
bool isinpolygon(vector<Aix2D> &simplex,Aix2D & dir)
{
	Aix2D origin = Aix2D(0, 0);
	if (simplex.size() == 2) {
		if (OnSegment(simplex[0],simplex[1],origin)) {
			return true;
		}
		else {
			dir = tripleProd(simplex[1] - simplex[0], origin - simplex[0], simplex[1] - simplex[0]);
			return false;
		}
	}
	if (simplex.size() >= 3) {
		Aix2D CA = simplex[0] - simplex[2];
		Aix2D CB = simplex[1] - simplex[2];
		Aix2D CO = origin - simplex[2];
		Aix2D CAprep = tripleProd(CB, CA, CA);
		Aix2D CBprep = tripleProd(CA, CB, CB);
		if (dcmp(vertexinner(CAprep, CO)) >= 0)
		{
			simplex.erase(simplex.begin() + 1);//移除B
			dir = CAprep;
			return false;
		}
		else if (dcmp(vertexinner(CBprep, CO)) >= 0)
		{
			simplex.erase(simplex.begin());//移除A
			dir = CBprep;
			return false;
		}
		else {
			return true;
		}
	}
}
Aix2D getPerpendicularToOrigin(Aix2D dot, Aix2D a, Aix2D b)
{
	Aix2D ab = b - a;
	double t = vertexinner(dot -a,ab)/ vertexinner(ab, ab);
	//if (t > 1)return b;
	//if (t < 0)return a;
	return Aix2D(a.x + ab.x * t, a.y + ab.y*t);

}

