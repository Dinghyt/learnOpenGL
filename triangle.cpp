#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

//当用户改变窗口的大小的时候，视口也应该被调整。我们可以对窗口注册一个回调函数(Callback Function)，它会在每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//输入控制,需要一个窗口以及一个按键作为输入。这个函数将会返回这个按键是否正在被按下
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 顶点着色器源码
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main() {\n"
	"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\n\0";

// 片段着色器源码
const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
	"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

// 片段着色器源码
const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"FragColor = vec4(0.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main() {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// -------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Fail to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Fail to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	// ------------------------------------
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

	// fragment shader
	unsigned int fragmentShader2;
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);

	// check for shader compile errors
	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
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

	// link shaders
	unsigned int shaderProgram2;
	shaderProgram2 = glCreateProgram();// 创建一个程序对象
	glAttachShader(shaderProgram2, vertexShader);// 把之前编译的着色器附加到程序对象上
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);// 链接它们

								 // check for linking errors
	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::LINKING_FAILED\n" << std::endl;
	}


	// 把着色器对象链接到程序对象以后，记得删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader2);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		0.0f, 0.0f, 0.0f,   // 右上角
		0.5f, 0.0f, 0.0f,  // 右下角
		0.25f, 0.5f, 0.0f,  // 左上角  
	};

	float vertices2[] = {
		0.0f, 0.0f, 0.0f,  // 右下角
		-0.5f, 0.0f, 0.0f, // 左下角
		-0.25f, 0.5f, 0.0f,   // 左上角
		0.0f, 0.0f, 0.0f,
		-0.25f, -0.5f, 0.0f,
		0.35f, -0.5f, 0.0f
	};
	/*
	顶点数组对象：Vertex Array Object，VAO
	顶点缓冲对象：Vertex Buffer Object，VBO
	索引缓冲对象：Element Buffer Object，EBO或Index Buffer Object，IBO
	*/

	// OpenGL的核心模式要求我们使用VAO
	unsigned int VBOs[2], VAOs[2];
	glGenBuffers(2, VBOs);
	glGenVertexArrays(2, VAOs);

	glBindVertexArray(VAOs[0]);

	// 复制顶点数组到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindVertexArray(VAOs[1]);

	// 复制顶点数组到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	// 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);




	float vertices3[] = {
		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f   // 左上角
	};

	unsigned int indices3[] = { // 注意索引从0开始! 
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};



	unsigned int VAO3, VBO3, EBO;
	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3), indices3, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window)) {
		// input
		// -----
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shaderProgram);// 激活这个程序对象
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgram2);// 激活这个程序对象
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// 利用索引缓冲
		glUseProgram(shaderProgram);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(VAO3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(2, VAOs);
	glDeleteVertexArrays(1, VAO3);
	glDeleteBuffers(2, VBOs);
	glDeleteBuffers(1, VBO3);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgram2);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}



