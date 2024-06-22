#include "Renderer.hpp"
#include <GLFW/glfw3.h>
#include <assert.h>
#include <iostream>

#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id,
                            GLenum severity, GLsizei length,
                            const char* message, const void* userParam) {
  // ignore non-significant error/warning codes
  if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
    return;

  std::cout << "---------------" << '\n';
  std::cout << "Debug message (" << id << "): " << message << '\n';

  switch (source) {
  case GL_DEBUG_SOURCE_API:
    std::cout << "Source: API";
    break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    std::cout << "Source: Window System";
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    std::cout << "Source: Shader Compiler";
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    std::cout << "Source: Third Party";
    break;
  case GL_DEBUG_SOURCE_APPLICATION:
    std::cout << "Source: Application";
    break;
  case GL_DEBUG_SOURCE_OTHER:
    std::cout << "Source: Other";
    break;
  }
  std::cout << '\n';

  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
    std::cout << "Type: Error";
    break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    std::cout << "Type: Deprecated Behaviour";
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    std::cout << "Type: Undefined Behaviour";
    break;
  case GL_DEBUG_TYPE_PORTABILITY:
    std::cout << "Type: Portability";
    break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    std::cout << "Type: Performance";
    break;
  case GL_DEBUG_TYPE_MARKER:
    std::cout << "Type: Marker";
    break;
  case GL_DEBUG_TYPE_PUSH_GROUP:
    std::cout << "Type: Push Group";
    break;
  case GL_DEBUG_TYPE_POP_GROUP:
    std::cout << "Type: Pop Group";
    break;
  case GL_DEBUG_TYPE_OTHER:
    std::cout << "Type: Other";
    break;
  }
  std::cout << '\n';

  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    std::cout << "Severity: high";
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    std::cout << "Severity: medium";
    break;
  case GL_DEBUG_SEVERITY_LOW:
    std::cout << "Severity: low";
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    std::cout << "Severity: notification";
    break;
  }
  std::cout << '\n';
  std::cout << '\n';
}

int main(void) {
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Enable gl new error log from 4.3.0 */
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  // sync with monitor refresh rate
  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK) {
    std::cout << "GLEW NOT OK" << '\n';
  }

  std::cout << glGetString(GL_VERSION) << '\n';

  // enable debugging
  int openGlDebugEnableflags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &openGlDebugEnableflags);
  if (openGlDebugEnableflags & GL_CONTEXT_FLAG_DEBUG_BIT) {
    // initialize debug output
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, NULL);
    // for filtering errors
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
                          GL_TRUE);
  }

  // vertex buffer
  float positions[8] = {
      -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,
  };

  VertexBuffer vb{positions, 8 * sizeof(float)};

  VertexArray va{};
  VertexBufferLayout vbLayout{};
  vbLayout.Push<float>(2);
  va.AddBuffer(vb, vbLayout);

  // index buffer
  unsigned int indexBufferObject[] = {0, 1, 2, 2, 3, 0};

  IndexBuffer ib{indexBufferObject, 6};

  Shader shader{"../../Assets/Shaders/Basic.shader"};
  shader.Bind();
  shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

  float r = 0.0f;
  float rIncrement = 0.05f;

  Renderer renderer{};
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    if (glGetError() != GL_NO_ERROR) {
      return EXIT_FAILURE;
    }
    /* Render here */
    renderer.Clear();

    // todo refactor into materials
    shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

    renderer.Draw(va, ib, shader);

    if (r > 1.0f) {
      rIncrement = -0.01f;
    } else if (r < 0.0f) {
      rIncrement = 0.01f;
    }

    r += rIncrement;

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
