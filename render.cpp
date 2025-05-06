#include "render.h"
render::render()
{
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);
	//绑定0
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);	// 从显卡中划分一个空间出来
	glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);	// 通道 0 打开
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);	// 通道 1 打开
	glBindVertexArray(0);
	//绑定1
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);	// 从显卡中划分一个空间出来
	glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);	// 通道 0 打开
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);	// 通道 1 打开
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);	// 通道 2 打开
	glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}
render::~render()
{
	glDeleteVertexArrays(2, VAO);			// 释放资源	
	glDeleteBuffers(2, VBO);
}

void render::DrawRectrangle(float width, float height)
{
	ourShader->Use();
	glBindVertexArray(VAO[0]);									// 绑定 VAO
	glUniform2f(glGetUniformLocation(ourShader->Program, "uResolution"), 800.0f, 800.0f);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);	// 画两个三角形 从第0个顶点开始 一共画6次
	glBindVertexArray(0);									// 解绑定 VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void render::DrawCircle(float width,float height)
{
	ourShader1->Use();
	glBindVertexArray(VAO[1]);// 绑定 VAO
	glUniform2f(glGetUniformLocation(ourShader1->Program, "uResolution"), 800.0f, 800.0f);
	glDrawArrays(GL_POINTS, 0,(GLsizei)Circle.size());
	glBindVertexArray(0);									// 解绑定 VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void render::flushrender()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, Rectriangle.size() * sizeof(Vertex), Rectriangle.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(int), indices.data(), GL_DYNAMIC_DRAW);
}
void render::flushcirclerender() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, Circle.size() * sizeof(Circledata), Circle.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void render::flushvertexs(Vertex v,int indice)
{
	Rectriangle[indices[indice]] = v;
}

void render::Addvertexdata(vector<Vertex *> data)
{
	for (auto it : data) {
		Rectriangle.push_back(*it);
	}
}

void render::Addindicesdata(unsigned int npoints)
{
	if (npoints >= 3) {
		for (int i = 1; i < npoints-1; i++) {
			indices.push_back(Rectriangle.size() - npoints);
			indices.push_back(Rectriangle.size() - npoints + i);
			indices.push_back(Rectriangle.size() - npoints + i+1);
		}
	}

}

int render::Getindicessize()
{
	return indices.size();
}

int render::Getcirclesize()
{
	return Circle.size();
}

void render::Clearvertexdata()
{
	Rectriangle.clear();//清空顶点的数据
}

void render::Clearindicesdata()
{
	indices.clear();//清空索引数据
}

void render::Clearcircledata()
{
	Circle.clear();
}

void render::addcircledata(Circledata cir)
{
	Circle.push_back(cir);
}



