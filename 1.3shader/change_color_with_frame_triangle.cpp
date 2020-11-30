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

	//��ʼ��GLFW
	glfwInit();

	//ʹ��glfwWindowHint����������GLFW(https://www.glfw.org/docs/latest/window.html#window_hints)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//�������汾��Ϊ3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//���ôΰ汾��Ϊ3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//����GLFW����ʹ�õ��Ǻ���ģʽ(Core - profile)

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//���ʹ�õ���Mac OS Xϵͳ��Ҫ���ϴ���
#endif

	// glfw window creation
	// --------------------

	//����һ�����ڶ���
	//glfwCreateWindow������Ҫ���ڵĿ�͸���Ϊ����ǰ����������������������ʾ������ڵ����ƣ����⣩,�����������������ʱ���ԡ�����һ��GLFWwindow����
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	//����ʧ�ܴ����߼�
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳��������ġ�
	glfwMakeContextCurrent(window);
	//����GLFW����ϣ��ÿ�����ڵ�����С��ʱ�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------

	//GLAD����������OpenGL�ĺ���ָ��ģ������ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD
	//���Ǹ�GLAD��������������ϵͳ��ص�OpenGL����ָ���ַ�ĺ�����GLFW�����ǵ���glfwGetProcAddress�����������Ǳ����ϵͳ��������ȷ�ĺ���
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // ����һ��������ɫ��
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);// �������ɫ��Դ�븽�ӵ���ɫ��������
	glCompileShader(vertexShader);// ������

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
	shaderProgram = glCreateProgram();// ����һ���������
	glAttachShader(shaderProgram, vertexShader);// ��֮ǰ�������ɫ�����ӵ����������
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);// ��������

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::LINKING_FAILED\n" << std::endl;
	}

	// ����ɫ���������ӵ���������Ժ󣬼ǵ�ɾ����ɫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	float vertices[] = {
		// λ��              
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
	
	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	// render loop:������������GLFW�˳�ǰһֱ��������
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);//����ض��ļ��Ƿ񱻰��£�����ÿһ֡��������

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //ʹ��һ���Զ������ɫ�����Ļ
		glClear(GL_COLOR_BUFFER_BIT);//�����ɫ����

		// �ǵü�����ɫ��
		glUseProgram(shaderProgram);
		
		// ����uniform��ɫ
		float timeValue = glfwGetTime();//ͨ��glfwGetTime()��ȡ���е�����
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;//ʹ��sin��������ɫ��0.0��1.0֮��ı�
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); //��ѯuniform��ַ��Ҫ����֮ǰʹ�ù���ɫ������
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);//���Ǹ���һ��uniform֮ǰ�������ʹ�ó��򣨵���glUseProgram)����Ϊ�����ڵ�ǰ�������ɫ������������uniform�ġ�

		// ����������
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);




		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
	    // �������岢��ѯIO�¼�
		glfwSwapBuffers(window);//������ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
		glfwPollEvents();//�����û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã���
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
//���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ����������ǿ��ԶԴ���ע��һ���ص�����(Callback Function)��������ÿ�δ��ڴ�С��������ʱ�򱻵���
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
//���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ����������ǿ��ԶԴ���ע��һ���ص�����(Callback Function)��������ÿ�δ��ڴ�С��������ʱ�򱻵���
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
