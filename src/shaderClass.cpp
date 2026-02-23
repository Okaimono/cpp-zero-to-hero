#include "shaderClass.h"


std::string get_file_contents(const char* filename) 
{
    std::ifstream in(filename, std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // attach vertex shader source to the vertex shader object
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    // compile the vertex shader into machine code
    glCompileShader(vertexShader);
    // Checks if the vertex shader has compiled correctly
    compileErrors(vertexShader, "VERTEX");

    // create fragment shader object and get its reference
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // attach fragment shader source to the fragment shader object
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    // compile the fragment shader into machine code
    glCompileShader(fragmentShader);
    // Checks if the fragment shader has compiled correctly
    compileErrors(fragmentShader, "FRAGMENT");

    // create shader program object and get its reference
    ID = glCreateProgram();
    // attach the vertex and fragment shaders to the shader program 
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    // wrap up/link all shaders together into the shader program
    glLinkProgram(ID);
    // Checks if Shader compiled successfully
    compileErrors(ID, "PROGRAM");

    // deletes now useless vertex and fragment shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    location = glGetUniformLocation(ID, "camMatrix");
}

void Shader::Activate() 
{
    glUseProgram(ID);
}

void Shader::Delete() 
{
    glDeleteProgram(ID);
}

void Shader::setVec3Loc()
{
    chunkOffsetLoc = glGetUniformLocation(ID, "chunkOffset");
}

void Shader::setVec3(const glm::vec3& pos)
{
    glUniform3fv(chunkOffsetLoc, 1, glm::value_ptr(pos));
}

void Shader::setMat4(const char* uniform, const glm::mat4 matrix) {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
} 

void Shader::compileErrors(unsigned int shader, const char* type)
{
    GLint hasCompiled;
    char infoLog[1024];
    if (type != "PROGRAM") 
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE) 
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE) 
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
        }
    }
}