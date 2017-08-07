#include "Buffers.h"

namespace Buffers
{
    GLuint GenVertexBuffer(int sizeInBytes, void* data)
    {
	GLuint result;
	// TODO: CHECK FOR GEN BUFFER FAIL?
	glGenBuffers(1, &result);        
	glBindBuffer(GL_ARRAY_BUFFER, result);
	glBufferData(GL_ARRAY_BUFFER, sizeInBytes, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return result;
    }
    
    GLuint GenIndexBuffer(int sizeInBytes, void* data)
    {
      GLuint result;
      // TODO: CHECK FOR GEN BUFFER FAIL?
      glGenBuffers(1, &result);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, data, GL_STATIC_DRAW);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      return result;
    }
}
 
