#include <GLApp.h>
#include <OBJFile.h>
#include <Mat4.h>
#include <iostream>

#define SPACEBAR 32
#define BUNNY_OBJ "bunny.obj"
#define CUBE_OBJ "cube.obj"

class MyGLApp : public GLApp {
public:
  double angle{0.0};
  std::vector<float> data;
  bool bunny{true};

  MyGLApp() : GLApp{640,480,1,"Shared vertices to explicit representation demo"} {}

  virtual void drawFromFile(const std::string &filename, bool verbose = false)
  {
    const OBJFile m{filename, true};
    const size_t n = m.indices.size();
    for (size_t i = 0; i < n; ++i)
    {
      const auto &index = m.indices[i];
      if (verbose)
      {
        std::cout << "Triangle " << i << ": "
                  << index[0] << ", "
                  << index[1] << ", "
                  << index[2] << std::endl;
      }
      // for each index entry (a triangle), 3 vertices are stored
      for (size_t j = 0; j < 3; ++j)
      {
        const auto &vertex = m.vertices[index[j]];
        const auto &normal = m.normals[index[j]];
        if (verbose)
        {
          std::cout << "Vertex " << j << ": "
                    << vertex[0] << ", "
                    << vertex[1] << ", "
                    << vertex[2] << std::endl;
          
          std::cout << "Normal " << j << ": "
                    << normal[0] << ", "
                    << normal[1] << ", "
                    << normal[2] << std::endl;
        }

        data.push_back(vertex[0]);
        data.push_back(vertex[1]);
        data.push_back(vertex[2]);

        data.push_back(0.7f + vertex[0]); // color
        data.push_back(0.7f + vertex[1]); // color
        data.push_back(1.0f + vertex[2]); // color
        data.push_back(1.0f);             // color

        data.push_back(normal[0]);
        data.push_back(normal[1]);
        data.push_back(normal[2]);
      }
    }
  }

  virtual void init() override {
    GL(glDisable(GL_CULL_FACE));
    GL(glEnable(GL_DEPTH_TEST));
    GL(glClearColor(0,0,0,0));

    // TODO:
    // Replace this example block of code  by your code to
    // convert the shared vertex representation in the object m
    // by an explicit representation, indices are stored in
    // the stl-vector m.indices the vertex positions are stored
    // in m.vertices and the normals are stored in m.normals.
    // As color you can choose whatever you like
    data.clear();
    if (bunny)
    {
      drawFromFile(BUNNY_OBJ);
    }
    else
    {
      drawFromFile(CUBE_OBJ);
    }
    // example block end
  }
  
  virtual void animate(double animationTime) override {
    angle = animationTime*30;
  }
  
  virtual void draw() override {
    GL(glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT));
    setDrawProjection(Mat4::perspective(45, glEnv.getFramebufferSize().aspect(), 0.0001f, 100.0f));
    setDrawTransform(Mat4::lookAt({0,0,2},{0,0,0},{0,1,0}) * Mat4::rotationY(float(angle)));
    drawTriangles(data, TrisDrawType::LIST, false, true);
  }
  
  virtual void keyboardChar(unsigned int key) override {
    // TODO check for keystrokes here
    if (key == SPACEBAR)
    {
      bunny = !bunny;
      init();
    }
  }


} myApp;

int main(int argc, char ** argv) {
  myApp.run();
  return EXIT_SUCCESS;
}
