#include "Cube.h"
CCube::CCube()
{}
CCube::~CCube()
{
	Release();
}
void CCube::Create(string filename)
{
	m_texture.Load(filename);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	m_vbo.Create();
	m_vbo.Bind();

	// Write the code to add interleaved vertex attributes to the VBO

	//face 1
	glm::vec3 v0 = glm::vec3(0, 0, 0);
	glm::vec3 v1 = glm::vec3(10, 0, 0);
	glm::vec3 v2 = glm::vec3(10, 1, 0);
	glm::vec3 v3 = glm::vec3(0, 1, 0);
	glm::vec3 n = glm::vec3(0, 0, 1);
	glm::vec2 t0 = glm::vec2(0, 0);
	glm::vec2 t1 = glm::vec2(1, 0);
	glm::vec2 t2 = glm::vec2(1, 1);
	glm::vec2 t3 = glm::vec2(0, 1);

	m_vbo.AddData(&v0, sizeof(glm::vec3));
	m_vbo.AddData(&t0, sizeof(glm::vec2));
	m_vbo.AddData(&n, sizeof(glm::vec3));

	m_vbo.AddData(&v1, sizeof(glm::vec3));
	m_vbo.AddData(&t1, sizeof(glm::vec2));
	m_vbo.AddData(&n, sizeof(glm::vec3));

	m_vbo.AddData(&v3, sizeof(glm::vec3));
	m_vbo.AddData(&t3, sizeof(glm::vec2));
	m_vbo.AddData(&n, sizeof(glm::vec3));

	m_vbo.AddData(&v2, sizeof(glm::vec3));
	m_vbo.AddData(&t2, sizeof(glm::vec2));
	m_vbo.AddData(&n, sizeof(glm::vec3));

	
	

	//face 2
	glm::vec3 v4 = glm::vec3(10, 0, 0);
	glm::vec3 v5 = glm::vec3(10, 0, 0.2f);
	glm::vec3 v6 = glm::vec3(10, 1, 0.2f);
	glm::vec3 v7 = glm::vec3(10, 1, 0);

	glm::vec3 n1 = glm::vec3(1, 0, 0);
	glm::vec2 t4 = glm::vec2(0, 0);
	glm::vec2 t5 = glm::vec2(1, 0);
	glm::vec2 t6 = glm::vec2(1, 1);
	glm::vec2 t7 = glm::vec2(0, 1);

	m_vbo.AddData(&v4, sizeof(glm::vec3));
	m_vbo.AddData(&t4, sizeof(glm::vec2));
	m_vbo.AddData(&n1, sizeof(glm::vec3));

	m_vbo.AddData(&v5, sizeof(glm::vec3));
	m_vbo.AddData(&t5, sizeof(glm::vec2));
	m_vbo.AddData(&n1, sizeof(glm::vec3));

	m_vbo.AddData(&v7, sizeof(glm::vec3));
	m_vbo.AddData(&t7, sizeof(glm::vec2));
	m_vbo.AddData(&n1, sizeof(glm::vec3));

	m_vbo.AddData(&v6, sizeof(glm::vec3));
	m_vbo.AddData(&t6, sizeof(glm::vec2));
	m_vbo.AddData(&n1, sizeof(glm::vec3));

	

	//face 3
	glm::vec3 v8 = glm::vec3(0, 0, 0.2f);
	glm::vec3 v9 = glm::vec3(10, 0, 0.2f);
	glm::vec3 v10 = glm::vec3(10, 1, 0.2f);
	glm::vec3 v11 = glm::vec3(0, 1, 0.2f);
	glm::vec3 n2 = glm::vec3(0, 0, 0.2f);
	glm::vec2 t8 = glm::vec2(0, 0);
	glm::vec2 t9 = glm::vec2(1, 0);
	glm::vec2 t10 = glm::vec2(1, 1);
	glm::vec2 t11 = glm::vec2(0, 1);

	m_vbo.AddData(&v8, sizeof(glm::vec3));
	m_vbo.AddData(&t8, sizeof(glm::vec2));
	m_vbo.AddData(&n2, sizeof(glm::vec3));

	m_vbo.AddData(&v9, sizeof(glm::vec3));
	m_vbo.AddData(&t9, sizeof(glm::vec2));
	m_vbo.AddData(&n2, sizeof(glm::vec3));

	m_vbo.AddData(&v11, sizeof(glm::vec3));
	m_vbo.AddData(&t11, sizeof(glm::vec2));
	m_vbo.AddData(&n2, sizeof(glm::vec3));

	m_vbo.AddData(&v10, sizeof(glm::vec3));
	m_vbo.AddData(&t10, sizeof(glm::vec2));
	m_vbo.AddData(&n2, sizeof(glm::vec3));

	

	//face 4
	glm::vec3 v12 = glm::vec3(0, 0, 0);
	glm::vec3 v13 = glm::vec3(0, 0, 0.2f);
	glm::vec3 v14 = glm::vec3(0, 1, 0.2f);
	glm::vec3 v15 = glm::vec3(0, 1, 0);
	glm::vec3 n3= glm::vec3(0, 0, 0.2f);
	glm::vec2 t12 = glm::vec2(0, 0);
	glm::vec2 t13 = glm::vec2(1, 0);
	glm::vec2 t14 = glm::vec2(1, 1);
	glm::vec2 t15 = glm::vec2(0, 1);

	m_vbo.AddData(&v12, sizeof(glm::vec3));
	m_vbo.AddData(&t12, sizeof(glm::vec2));
	m_vbo.AddData(&n3, sizeof(glm::vec3));

	m_vbo.AddData(&v13, sizeof(glm::vec3));
	m_vbo.AddData(&t13, sizeof(glm::vec2));
	m_vbo.AddData(&n3, sizeof(glm::vec3));

	m_vbo.AddData(&v15, sizeof(glm::vec3));
	m_vbo.AddData(&t15, sizeof(glm::vec2));
	m_vbo.AddData(&n3, sizeof(glm::vec3));

	m_vbo.AddData(&v14, sizeof(glm::vec3));
	m_vbo.AddData(&t14, sizeof(glm::vec2));
	m_vbo.AddData(&n3, sizeof(glm::vec3));

	//face 5
	glm::vec3 v16 = glm::vec3(0, 0, 0.2f);
	glm::vec3 v17 = glm::vec3(10, 0, 0.2f);
	glm::vec3 v18 = glm::vec3(10, 0, 0);
	glm::vec3 v19 = glm::vec3(0, 0, 0);
	glm::vec3 n4 = glm::vec3(0, 1, 0);
	glm::vec2 t16 = glm::vec2(0, 0);
	glm::vec2 t17 = glm::vec2(1, 0);
	glm::vec2 t18 = glm::vec2(1, 1);
	glm::vec2 t19 = glm::vec2(0, 1);

	m_vbo.AddData(&v16, sizeof(glm::vec3));
	m_vbo.AddData(&t16, sizeof(glm::vec2));
	m_vbo.AddData(&n4, sizeof(glm::vec3));

	m_vbo.AddData(&v17, sizeof(glm::vec3));
	m_vbo.AddData(&t17, sizeof(glm::vec2));
	m_vbo.AddData(&n4, sizeof(glm::vec3));

	m_vbo.AddData(&v19, sizeof(glm::vec3));
	m_vbo.AddData(&t19, sizeof(glm::vec2));
	m_vbo.AddData(&n4, sizeof(glm::vec3));

	m_vbo.AddData(&v18, sizeof(glm::vec3));
	m_vbo.AddData(&t18, sizeof(glm::vec2));
	m_vbo.AddData(&n4, sizeof(glm::vec3));

	//face 6
	glm::vec3 v20 = glm::vec3(0, 1, 0.2f);
	glm::vec3 v21 = glm::vec3(10, 1, 0.2f);
	glm::vec3 v22 = glm::vec3(10, 1, 0);
	glm::vec3 v23 = glm::vec3(0, 1, 0);
	glm::vec3 n5 = glm::vec3(0, -1, 0);
	glm::vec2 t20 = glm::vec2(0, 0);
	glm::vec2 t21 = glm::vec2(1, 0);
	glm::vec2 t22 = glm::vec2(1, 1);
	glm::vec2 t23 = glm::vec2(0, 1);

	m_vbo.AddData(&v20, sizeof(glm::vec3));
	m_vbo.AddData(&t20, sizeof(glm::vec2));
	m_vbo.AddData(&n5, sizeof(glm::vec3));

	m_vbo.AddData(&v21, sizeof(glm::vec3));
	m_vbo.AddData(&t21, sizeof(glm::vec2));
	m_vbo.AddData(&n5, sizeof(glm::vec3));

	m_vbo.AddData(&v23, sizeof(glm::vec3));
	m_vbo.AddData(&t23, sizeof(glm::vec2));
	m_vbo.AddData(&n5, sizeof(glm::vec3));

	m_vbo.AddData(&v22, sizeof(glm::vec3));
	m_vbo.AddData(&t22, sizeof(glm::vec2));
	m_vbo.AddData(&n5, sizeof(glm::vec3));

	
	

	

	// Upload data to GPU
	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,
		(void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}
void CCube::Render()
{
	glBindVertexArray(m_vao);
	m_texture.Bind();
	// Call glDrawArrays to render each side
	/*glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);*/
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 0, 40);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 40);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 8, 40);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 12, 40);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 16, 40);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 20, 40);
}
void CCube::Release()
{
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}	