#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
	glfwInit();  // 初始化GLFW
	// 配置GLFW，glfwWindowHint第一项为配置项名称，第二项为对应值
	// 设置OpenGL的主版本号为3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// 设置OpenGL的次版本号为3，即OpenGL3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// 设置OpenGL使用可编程渲染管线（核心模式）
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// 设置窗口对象并设置其宽高以及名称
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// 通知GLFW将我们窗口的上下文设置为当前线程的主上下文
	glfwMakeContextCurrent(window);

	// 初始化GLAD，GLAD是用来管理OpenGL的函数指针的。
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 渲染前需要告诉OpenGL渲染窗口的尺寸大小，即视口(ViewPort)，这样OpenGL才知道如何根据窗口大小显示数据和坐标
	// 通过调用glViewport来设置视口维度（x/y/w/h）
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);  //(0,0)为左下角

	// 设置窗口的大小变化时的callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Render Loop
	while(!glfwWindowShouldClose(window))  // 检查GLFW窗口是否被要求关闭
	{
		// 处理输入
		processInput(window);

		// 渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // OpenGL状态设置函数
		glClear(GL_COLOR_BUFFER_BIT);  // OpenGL状态使用函数

		glfwSwapBuffers(window);  // 交换颜色缓冲
		glfwPollEvents();    // 检查有没有触发事件（键盘输入/鼠标移动）、更新窗口状态，并调用对应的回调函数
	}

	// 渲染结束后，释放之前分配的所有资源
	glfwTerminate();
	return 0;
}


// 当用户改变窗口的大小的时候，视口也应该被调整。此即为该回调
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// 处理输入
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}