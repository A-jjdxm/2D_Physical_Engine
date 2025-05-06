#include "Windows.h"

Mywindows::Mywindows()
{
    mode = new ViewMode();
}
void Mywindows::initgrid()
{

}
Mywindows::~Mywindows()
{
    delete mode;
    glfwDestroyWindow(window);
    glfwTerminate();
}
void Mywindows::start()
{
    if (!glfwInit()) {
        cout << "Failed to initialize GLFW" << endl;
        return;
    }

    // 配置 GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // 设置主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // 设置次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口
    window = glfwCreateWindow(WIDTH, HEIGHT, "physical engine", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
    }
    glfwSetWindowPos(window, 600, 200);
    glfwMakeContextCurrent(window);

    // 初始化 GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
    }
    std::cout << "The version of OpenGL:" << glGetString(GL_VERSION) << std::endl;

    //设置鼠标键盘回调
    render *re=new render();
    glfwSetWindowUserPointer(window, re);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window)) {
        // 处理输入事件
        glfwPollEvents();
        // 渲染代码
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // 设置清屏颜色
        glClear(GL_COLOR_BUFFER_BIT);  // 清除颜色缓冲区
        glEnable(GL_PROGRAM_POINT_SIZE);

        re->DrawRectrangle(WIDTH,HEIGHT);
        re->DrawCircle(WIDTH, HEIGHT);
        tick(window,re);
        // 交换缓冲区
        glfwSwapBuffers(window);
    }
}
void Mywindows::Addshape(shape* p)
{
    p->id = shapes.size();
    shapes.push_back(p);
    cout << "图形数量为" << shapes.size() << endl;
    Addgirddata(p);
}
void Mywindows::Addgirddata(shape* p)
{
    

}
void Mywindows::Clearshapes()
{
    shapes.clear();
}
void Mywindows::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    render* re = static_cast<render*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        Circle* circle = new Circle(window);
        circle->draw(window, re);
        Addshape(circle);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        Rectangle* rectriangle = new Rectangle(window);
        rectriangle->draw(window,re);
        Addshape(rectriangle);
    }
}
void Mywindows::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    render* re = static_cast<render*>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        re->Clearindicesdata();
        re->Clearvertexdata();
        re->Clearcircledata();
        Clearshapes();
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        Circle* circle = new Circle(window);
        circle->draw(window, re);
        Addshape(circle);
    }
}
void Mywindows::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    WIDTH = width;
    HEIGHT = height;
}

void Mywindows::switchtoEditmode()
{
    delete mode;
    mode = new EditMode();
}

void Mywindows::switchtoViewmode()
{
    delete mode;
    mode = new ViewMode();
}

void Mywindows::tick(GLFWwindow* window,render *re)
{
    float interval=0.05f;
    flush(window,re,interval);
    
}

void Mywindows::flush(GLFWwindow* window,render *re ,float interval)
{
    if (shapes.size()) {
        for (int i = 0; i < shapes.size(); i++) {
            shapes[i]->process(window, re, interval);
        }
        for (int i = 0; i < shapes.size(); i++) {
            shapes[i]->applygravity(interval);
        }
        for (int i = 0; i < shapes.size(); i++) {
            shapes[i]->isout(window, re, interval);
        }
        vector<pair<shape*, shape*>> potentialcols = quickaabb(shapes, re);
        for (int i = 0;i< potentialcols.size();i++) {
            if (test->TestCollision(window, re, potentialcols[i].first, potentialcols[i].second)){}
        }
        if (test->constraints.size()) {
            //cout << test->constraints[0]->p.x << "||" << test->constraints[0]->p.y << "||" <<test->constraints[0]->normal.x<<"||" << test->constraints[0]->normal.y << "||" << test->constraints[0]->penetration << endl;
            for (int i = 0; i < test->constraints.size(); i++) {
                test->constraints[i]->ProcessVelocity();
                test->constraints[i]->ProcessPosition(interval);
            }
            for (int i = 0; i < test->constraints.size(); i++) delete (test->constraints[i]);
            test->clear();
        }
 
        re->flushrender();
        re->flushcirclerender();
    }
}
vector<pair<shape*, shape*>> quickaabb(vector<shape*> shapes, render* re)
{

    int n = shapes.size();
    if (n <= 1)return vector<pair<shape*, shape*>>();
    AABB * aabbs = (AABB *)malloc(sizeof(AABB)*n);
    vector<pair<shape*, shape*> > ret;
    for (int i = 0; i < n; i++) {
        aabbs[i] = shapes[i]->Getaabb(re);
        aabbs[i].Enlarge(0.001f);
    }
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (aabbs[i].Overlap(aabbs[j])) {
                ret.push_back(make_pair(shapes[i], shapes[j]));
            }
        }
    }
    free(aabbs);
    return ret;
}

