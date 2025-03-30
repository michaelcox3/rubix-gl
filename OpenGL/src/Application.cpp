#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>

// forces a breakpoint, stop the debugger
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

// TODO CONVERT ERROR DECIMAL TO HEXADECIMAL THAN TO THE GL ERROR ENUM TEXT
static bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << "): " 
			<< function << " " 
			<< file << ":"
			<< line << std::endl;
		return false;
	}
	return true;
}


static std::string ParseShader(const std::string& filePath) {
	std::ifstream stream(filePath);

	bool isStartOfShaderCode = false;

	std::string line;
	std::stringstream ss;
	while (getline(stream, line)) {
		// npos means substring not found
		if (line.find("#shader") != std::string::npos) {
			isStartOfShaderCode = true;
		}
		else if (isStartOfShaderCode) {
			ss << line << '\n';
		}
	}

	return ss.str();
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str(); // or &source[0] same thing

	// if length = null, it means each string is assumed to be null terminated. (\0)
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

// to provide opengl the shader source code
// compile into shader program, link the two shaders into a single shader program
// return unique id for that shader 
static unsigned int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram(); // create shader program, instead of taking in the ref to the int like in genbuffer, it returns the id
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program); // code uwrite once and forget about it :D
	glValidateProgram(program); // performs validation on the program .. XD

	// now that the shaders had been linked into a program, delete the intermediates
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	// OPEN GL 3.3
	// SET CORE
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);


	// set fps to monitor refresh rate
	glfwSwapInterval(10);

	// This must be called after glfwMakeContextCurrent
	if (glewInit() != GLEW_OK) {
		std::cout << "Error init GLEW" << std::endl;
	}

	std::cout << "Running GL_VERSION: " << glGetString(GL_VERSION) << std::endl;

	// basic buffer. the 3 points of the triangle
	// opengl doesn't know how to interpret these vertices at this point
	//float positions[] = {
	//	-0.5f, -0.5f,
	//	 0.5f, -0.5f,
	//	 0.5f,  0.5f,

	//	 0.5f, 0.5f,
	//	 -0.5f, 0.5f,
	//	 -0.5f,  -0.5f,
	//};
	
	// to be put in the vertex buffer
	float positions[] = {
		-0.5f, -0.5f, // 0
		 0.5f, -0.5f, // 1
		 0.5f,  0.5f, // 2
		 -0.5f, 0.5f, // 3
	};

	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	// Define a buffer
	unsigned int buffer;
	// n: specifies number of buffer object names to be generated
	// buffers: an array in which the generated buffer object names are stored (if only generating 1, it can just be an unsigned int
	GLCall(glGenBuffers(1, &buffer)); // this creates the buffer, returns the id

	// Because OpenGL is a 'state machine' , we must call glBindBuffer to specificy which buffer we are refering to for subsequent operations
	// Selecting is known as binding in OpenGL (im about to work on this buffer)
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));

	// glBufferData: load the data into the buffer (from cpu into gpu)
	// target: the array buffer
	// size: the size of the data
	// data: the data
	// usage: GL_STATIC_DRAW is a usage param to define how this buffer will be used, specifically that is will be written to once, but used many times for drawing
	GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));


	GLCall(glEnableVertexAttribArray(0));

	// glVertexAttribPointer: defines the structure of the data. tells opengl how to interpret the data
	// ex. the number of components per vertex, the data type, and stride 
	// 
	// index = 0 because its the first attribute
	// size = component count in the attribute
	// type = float
	// GL_FALSE because no already in floats (already normalized)
	// stride = # of bytes (2 floats = 8 bytes) (the byte offset between consecutive vertex attributes; here, the vertices are tightly packed with two floats each).
	// pointer = offset of the 1st component of the first generic vertex attribute (offset in buffer to where the data begins)
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));





	// index buffer, we do this so we don't repeat storing the same positional data in memory
	// unsigned ints is required for index buffer 
	unsigned int indices[] = { // for drawing a square :D
		0, 1, 2,
		2, 3, 0
	};

	unsigned int indexBufferObject;
	glGenBuffers(1, &indexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);


	// relative to the project directory, defined in debugging
	std::string vertexShader = ParseShader("res/shaders/Vertex.shader");
	std::string fragShader = ParseShader("res/shaders/Frag.shader");

	std::cout << vertexShader << std::endl;
	std::cout << fragShader << std::endl;
	

	unsigned int shader = CreateShaderProgram(vertexShader, fragShader);
	glUseProgram(shader);

	// to grab the location for the u_Color uniform variable
	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	if (location == -1) {
		std::cout << "Couldn't find the u_Color uniform!" << std::endl;
	}
	GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f)); // setting the color here!



	

	// unbind everything D:
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	float r = 0.0f;
	float increment = 0.05f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		GLCall(glUseProgram(shader));
		GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f)); // setting the color here!

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
		

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
		
		
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject));
		




		// HAS TO BE UNSIGNED INT
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); //for index buffers


		// animate the color!
		if (r > 1.0f) {
			increment = -0.05f;
		} else if (r < 0.0f) {
			increment = 0.05f;
		}

		r += increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}