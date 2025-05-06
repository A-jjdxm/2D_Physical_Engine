#pragma once
#include<thread>
#include"render.h"
#include"shape.h"
#include"constraint.h"
class WindowMode {
public:
    virtual void draw() = 0; // ���ƴ�������
    virtual void handleInput() = 0; // �����û�����
    virtual ~WindowMode() {}
};

// �鿴ģʽ�ľ���ʵ��
class ViewMode : public WindowMode {
public:
    void draw() override {
        // ʵ�ֲ鿴ģʽ�µĻ����߼�
    }

    void handleInput() override {
        // ʵ�ֲ鿴ģʽ�µ����봦���߼�
    }
};

// �༭ģʽ�ľ���ʵ��
class EditMode : public WindowMode {
public:
    void draw() override {
        // ʵ�ֱ༭ģʽ�µĻ����߼�
    }

    void handleInput() override {
        // ʵ�ֱ༭ģʽ�µ����봦���߼�
    }
};
static vector<shape*> shapes;
static vector<unsigned int>gird[10][10];
static float WIDTH = 800;
static float HEIGHT = 800;
class Mywindows
{
public:
    Mywindows();
	void initgrid();
	~Mywindows();
	void start();
    static void Addshape(shape * p);
    static void Addgirddata(shape* p);
    static void Clearshapes();
private:
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void switchtoEditmode();
    void switchtoViewmode();
    void tick(GLFWwindow* window,render *re);
    void flush(GLFWwindow* window,render *re,float interval);
	GLFWwindow* window;
    WindowMode* mode;
    collision* test = new collision();
};
vector<pair<shape*, shape*> > quickaabb(vector<shape*> shapes, render* re);

