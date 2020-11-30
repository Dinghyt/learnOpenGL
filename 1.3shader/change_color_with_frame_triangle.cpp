#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// vertexShaderSource
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"gl_Position = vec4(aPos, 1.0);\n"
"}\n\0";

// fragmentShaderSource
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main() {\n"
"FragColor = ourColor;\n"
"}\n\0";

int main()
{
	// glfw: initialize and configure
	// ------------------------------

	//初始化GLFW
	glfwInit();

	//使用glfwWindowHint函数来配置GLFW(https://www.glfw.org/docs/latest/window.html#window_hints)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//设置主版本号为3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//设置次版本号为3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//告诉GLFW我们使用的是核心模式(Core - profile)

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//如果使用的是Mac OS X系统需要加上此行
#endif

	// glfw window creation
	// --------------------

	//创建一个窗口对象
	//glfwCreateWindow函数需要窗口的宽和高作为它的前两个参数。第三个参数表示这个窗口的名称（标题）,最后两个参数我们暂时忽略。返回一个GLFWwindow对象。
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	//创建失败处理逻辑
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//通知GLFW将我们窗口的上下文设置为当前线程的主上下文。
	glfwMakeContextCurrent(window);
	//告诉GLFW我们希望每当窗口调整大小的时候调用
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------

	//GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD
	//我们给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数。GLFW给我们的是glfwGetProcAddress，它根据我们编译的系统定义了正确的函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // 创建一个顶点着色器
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);// 把这个着色器源码附加到着色器对象上
	glCompileShader(vertexShader);// 编译它

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	// link shaders
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();// 创建一个程序对象
	glAttachShader(shaderProgram, vertexShader);// 把之前编译的着色器附加到程序对象上
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);// 链接它们

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::LINKING_FAILED\n" << std::endl;
	}

	// 把着色器对象链接到程序对象以后，记得删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	float vertices[] = {
		// 位置              
		0.5f, -0.5f, 0.0f, 
		-0.5f, -0.5f, 0.0f, 
		0.0f,  0.5f, 0.0f
	};


	unsigned int VAO, VBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	// render loop:它能在我们让GLFW退出前一直保持运行
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);//检测特定的键是否被按下，并在每一帧做出处理。

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //使用一个自定义的颜色清空屏幕
		glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲

		// 记得激活着色器
		glUseProgram(shaderProgram);
		
		// 更新uniform颜色
		float timeValue = glfwGetTime();//通过glfwGetTime()获取运行的秒数
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;//使用sin函数让颜色在0.0到1.0之间改变
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); //查询uniform地址不要求你之前使用过着色器程序
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);//但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的。

		// 绘制三角形
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);




		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
	    // 交换缓冲并查询IO事件
		glfwSwapBuffers(window);//交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
		glfwPollEvents();//检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
	}


	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
//当用户改变窗口的大小的时候，视口也应该被调整。我们可以对窗口注册一个回调函数(Callback Function)，它会在每次窗口大小被调整的时候被调用
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
//当用户改变窗口的大小的时候，视口也应该被调整。我们可以对窗口注册一个回调函数(Callback Function)，它会在每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
