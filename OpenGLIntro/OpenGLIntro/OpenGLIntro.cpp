#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// handle keyboard input to update transformation parameters
void processInput(GLFWwindow* window, glm::vec3 &translation, float &rotationAngleZ, float &scaleFactor, float deltaTime)
{
    // close on ESC
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // rotate around Z with Q/E
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        rotationAngleZ += 90.0f * deltaTime; // degrees per second
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        rotationAngleZ -= 90.0f * deltaTime;

    // scale with R/F
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        scaleFactor += 1.0f * deltaTime; // scale units per second
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        scaleFactor -= 1.0f * deltaTime;
        if (scaleFactor < 0.01f) scaleFactor = 0.01f;
    }

    // translate with arrow keys (optional)
    const float moveSpeed = 1.0f; // units per second
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        translation.x -= moveSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        translation.x += moveSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        translation.y += moveSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        translation.y -= moveSpeed * deltaTime;
}

// vertex shader
const char* vs = R"glsl(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aColor;
	uniform mat4 transform;
	out vec3 vertexColor;
	void main() { gl_Position = transform * vec4(aPos, 1.0); vertexColor = aColor; }
)glsl";

// fragment shader
const char* fs = R"glsl(
	#version 330 core
	in vec3 vertexColor;
	out vec4 FragColor;
	void main() { FragColor = vec4(vertexColor, 1.0); }
)glsl";



// (per-frame transformation state will be declared inside main)


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "COMP371 A2", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewInit();
    glEnable(GL_DEPTH_TEST);

    unsigned int v = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v, 1, &vs, NULL);
    glCompileShader(v);
    unsigned int f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f, 1, &fs, NULL);
    glCompileShader(f);
    unsigned int prog = glCreateProgram();
    glAttachShader(prog, v);
    glAttachShader(prog, f);
    glLinkProgram(prog);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
         0.0f,  0.5f,  0.0f,  1.0f, 0.0f, 1.0f
    };
    unsigned int indices[] = {
        0,1,2, 0,2,3, 0,1,4, 1,2,4, 2,3,4, 3,0,4
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // number of indices for drawing
    int indexCount = (int)(sizeof(indices) / sizeof(indices[0]));

    // per-frame transformation state
    glm::vec3 translation(0.0f, 0.0f, 0.0f);
    float rotationAngleZ = 0.0f; // degrees
    float scaleFactor = 1.0f;
    float lastFrame = 0.0f;

    int transformLoc = glGetUniformLocation(prog, "transform");

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, translation, rotationAngleZ, scaleFactor, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(prog);

        // build model, view, projection to visualize the 3D pyramid
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, translation);
        // tilt the model so the pyramid sides are visible
        model = glm::rotate(model, glm::radians(-25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // interactive rotation around Y (use Q/E to change rotationAngleZ)
        model = glm::rotate(model, glm::radians(rotationAngleZ), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scaleFactor));

        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        glm::mat4 t = projection * view * model;

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(t));
        // ensure EBO is bound for this VAO (stored with VAO but be explicit)
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
