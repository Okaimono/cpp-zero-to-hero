#include"EBO.h"
#include <vector>

// Constructor that generates a Elements Buffer Object and links it to indices
EBO::EBO()
{
	glGenBuffers(1, &ID);
}

// Binds the EBO
void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Unbinds the EBO
void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Upload(const void* data, GLsizeiptr size)
{
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

// Deletes the EBO
void EBO::Delete()
{
	glDeleteBuffers(1, &ID);
}