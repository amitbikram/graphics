#include <GLFW/glfw3.h>

#include <iostream>

#define ASSERT(x) if(!(x)) __builtin_trap();
#define GLCall(x) GLClearError();\
    x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__));


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error](" << error << ")" << function << " "<<file<<":"<<line <<std::endl;
        return false;
    }
    return true;
}

const GLint WIDTH = 800, HEIGHT = 600;

// Draw primative(s)
void draw() {

    GLenum mode = GL_TRIANGLES;
    GLint first = 0;
    GLsizei count = 6;
    
    GLCall(glDrawElements(mode, 12, GL_UNSIGNED_INT, nullptr));

    // GLCall(glDrawElem(mode, first, count));

}

// Create and compile shaders
static GLuint CompileShader(const std::string& source, GLuint shaderType) {

    // Create shader object
    GLuint shaderID = glCreateShader(shaderType);
    const char* src = source.c_str();

    // Attach source code to shader object
    glShaderSource(shaderID, 1, &src, nullptr);

    // Compile shader
    std::cout << "Compiling shader..." << std::endl;
    glCompileShader(shaderID);

    // Return ID of compiled shader
    return shaderID;
}

// Create program object
static GLuint CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {

    // Compile vertex shader
    std::cout << "***** Compiling Vertex Shader *****" << std::endl;
    GLuint vertexShaderComp = CompileShader(vertexShader, GL_VERTEX_SHADER);

    // Compile fragment shader
    std::cout << "***** Compiling Fragment Shader *****" << std::endl;
    GLuint fragmentShaderComp = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    // Create program object
    std::cout << "***** Create program object *****" << std::endl;
    GLuint shaderProgram = glCreateProgram();

    // Attach vertex and fragment shaders to program object
    glAttachShader(shaderProgram, vertexShaderComp);
    glAttachShader(shaderProgram, fragmentShaderComp);
    std::cout << "***** Attached both Shaders *****" << std::endl;

    // Link shaders to create executable
    glLinkProgram(shaderProgram);

    // Delete compiled shaders
    glDeleteShader(vertexShaderComp);
    glDeleteShader(fragmentShaderComp);

    // Return shaderProgram
    return shaderProgram;

}


int main() {

    glfwInit();  // Initialize the glfw library

    // Setup properties for the window
    // THESE OPTIONS CAUSED THE TRIANGLE TO FAIL RENDERING; NOT SURE WHY
    /*glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE);*/

    // Create instance of the window
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Kevin Tooley", nullptr, nullptr);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    // Handle the case that the window was not initialized
    if ( nullptr == window ) {
        std::cout << "Failed to create OpenGL Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent( window );  // Make the window active


    // Parameters used to display the window in relation to my screen
    glViewport( 0, 0, screenWidth, screenHeight );

//    GLfloat vertices[] = {
//
//        // Triangle 1
//        0.0, 0.0, 0.0, // vert 0
//        1.0, 0.0, 0.0, // Red
//
//        -0.5, 0.0, 0.0, // vert 1
//        0.0, 1.0, 0.0, // Green
//
//        -0.5, 0.5, 0.0, // vert2
//        0.0, 0.0, 1.0, // Blue
//
//        // Triangle 2
//        0.0, 0.0, 0.0, // vert 0
//        1.0, 1.0, 0.0, // Red
//
//        0.5, 0.0, 0.0, // vert 1
//        0.0, 1.0, 1.0, // Green
//
//        0.5, -0.5, 0.0, // vert2
//        1.0, 0.0, 1.0 // Blue
//
//    };
    
    GLfloat vertices[] = {

        // Triangle 1
        0.0, 0.0, 0.0, // vert 0
        1.0, 0.0, 0.0, // Red

        -0.5, 0.0, 0.0, // vert 1
        0.0, 1.0, 0.0, // Green

        -0.5, 0.5, 0.0, // vert2
        0.0, 0.0, 1.0, // Blue

        // Triangle 2
        0.5, 0.0, 0.0, // vert 1
        0.5, -0.5, 0.0 // vert2
    };
    
    unsigned int indeces[] = {
        0, 1, 2, 3, 4, 5,
        0, 1, 6, 3, 7, 5
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);  // Create VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // Select buffer ( VBO )
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // Load vertex attributes

    // Specify location and layout to GPU
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    
    GLuint ibo;
    glGenBuffers(1, &ibo);  // Create VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);  // Select buffer ( VBO )
    glBufferData(GL_ARRAY_BUFFER, 12* sizeof(unsigned int), indeces, GL_STATIC_DRAW);
    
    
    // Vertex Shader source code
    std::string vertexShaderSource =
    "#version 330 core\n"
    "layout(location = 0) in vec4 aPosition;\n"
    "layout(location = 1) in vec4 aColor;\n"
    "out vec4 oColor;\n"
    "void main()\n"
    "{\n"
    "gl_Position = aPosition;\n"
    "oColor = aColor;\n"
    "}\n";

    // Fragment shader source code
    std::string fragmentShaderSource =
    "#version 330 core\n"
    "in vec4 oColor;\n"
    "out vec4 fragColor;\n"
    "void main()\n"
    "{\n"
    "fragColor = oColor;\n"
    "}\n";


    // Create shader program
    GLuint shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

    // Use shader program
    glUseProgram(shaderProgram);

    // Loop to process while window is open
    while ( !glfwWindowShouldClose( window ) ) {

        glfwPollEvents();

        glClear( GL_COLOR_BUFFER_BIT );

        // Draw primative
        draw();

        glfwSwapBuffers( window );

        glfwPollEvents();

    }

    // The window has been closed, so terminate glfw
    glfwTerminate();

    return 0;

}
