#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string> 
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


std::string get_file_contents(const char* filename);

class Shader 
{
public:
    Shader(const char* vertexFile, const char* fragmentFile);

    void Activate();
    void Delete();

    void setVec3Loc();

    void setMat4(const char* uniform, const glm::mat4 matrix);
    void setVec3(const glm::vec3& pos);

    GLuint ID;

private:
    GLuint location = -1;
    GLuint chunkOffsetLoc = -1;

    void compileErrors(unsigned int shader, const char* type);
};
#endif