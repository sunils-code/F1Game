#include "CatmullRom.h"
#define _USE_MATH_DEFINES
#include <math.h>



CCatmullRom::CCatmullRom()
{
	m_vertexCount = 0;
}

CCatmullRom::~CCatmullRom()
{}

// Perform Catmull Rom spline interpolation between four points, interpolating the space between p1 and p2
glm::vec3 CCatmullRom::Interpolate(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, float t)
{
	float t2 = t * t;
	float t3 = t2 * t;

	glm::vec3 a = p1;
	glm::vec3 b = 0.5f * (-p0 + p2);
	glm::vec3 c = 0.5f * (2.0f*p0 - 5.0f*p1 + 4.0f*p2 - p3);
	glm::vec3 d = 0.5f * (-p0 + 3.0f*p1 - 3.0f*p2 + p3);

	return a + b * t + c * t2 + d * t3;

}


void CCatmullRom::SetControlPoints()
{
	// Set control points (m_controlPoints) here, or load from disk
	m_controlPoints.push_back(glm::vec3(300, 1, 600));
	m_controlPoints.push_back(glm::vec3(-100, 1, 600));
	m_controlPoints.push_back(glm::vec3(-150, 1, 450));
	m_controlPoints.push_back(glm::vec3(-400, 1, 355));
	m_controlPoints.push_back(glm::vec3(-660, 1, 310));
	m_controlPoints.push_back(glm::vec3(-710, 1, 240));
	m_controlPoints.push_back(glm::vec3(-630, 1, 150));
	m_controlPoints.push_back(glm::vec3(-630, 1, 70));
	m_controlPoints.push_back(glm::vec3(-750, 1, -100));
	m_controlPoints.push_back(glm::vec3(-630, 1, -250));
	m_controlPoints.push_back(glm::vec3(-370, 1, -350));
	m_controlPoints.push_back(glm::vec3(-240, 1, -250));
	m_controlPoints.push_back(glm::vec3(-50, 1, -250));
	m_controlPoints.push_back(glm::vec3(70, 1, -150));
	m_controlPoints.push_back(glm::vec3(130, 1, 150));
	m_controlPoints.push_back(glm::vec3(430, 1, 240));
	m_controlPoints.push_back(glm::vec3(550, 1, 160));
	m_controlPoints.push_back(glm::vec3(743, 1, 160));
	m_controlPoints.push_back(glm::vec3(930, 1, 335));
	m_controlPoints.push_back(glm::vec3(820, 1, 540));
	m_controlPoints.push_back(glm::vec3(650, 1, 450));
	m_controlPoints.push_back(glm::vec3(600, 1, 600));
	
	

	
	
	// Optionally, set upvectors (m_controlUpVectors, one for each control point as well)
	
}


// Determine lengths along the control points, which is the set of control points forming the closed curve
void CCatmullRom::ComputeLengthsAlongControlPoints()
{
	int M = (int)m_controlPoints.size();

	float fAccumulatedLength = 0.0f;
	m_distances.push_back(fAccumulatedLength);
	for (int i = 1; i < M; i++) {
		fAccumulatedLength += glm::distance(m_controlPoints[i - 1], m_controlPoints[i]);
		m_distances.push_back(fAccumulatedLength);
	}

	// Get the distance from the last point to the first
	fAccumulatedLength += glm::distance(m_controlPoints[M - 1], m_controlPoints[0]);
	m_distances.push_back(fAccumulatedLength);
}


// Return the point (and upvector, if control upvectors provided) based on a distance d along the control polygon
bool CCatmullRom::Sample(float d, glm::vec3 &p, glm::vec3 &up)
{
	if (d < 0)
		return false;

	int M = (int)m_controlPoints.size();
	if (M == 0)
		return false;
	

	float fTotalLength = m_distances[m_distances.size() - 1];

	// The the current length along the control polygon; handle the case where we've looped around the track
	float fLength = d - (int)(d / fTotalLength) * fTotalLength;

	// Find the current segment
	int j = -1;
	for (int i = 0; i < (int)m_distances.size(); i++) {
		if (fLength >= m_distances[i] && fLength < m_distances[i + 1]) {
			j = i; // found it!
			break;
		}
	}

	if (j == -1)
		return false;

	// Interpolate on current segment -- get t
	float fSegmentLength = m_distances[j + 1] - m_distances[j];
	float t = (fLength - m_distances[j]) / fSegmentLength;

	// Get the indices of the four points along the control polygon for the current segment
	int iPrev = ((j - 1) + M) % M;
	int iCur = j;
	int iNext = (j + 1) % M;
	int iNextNext = (j + 2) % M;

	// Interpolate to get the point (and upvector)
	p = Interpolate(m_controlPoints[iPrev], m_controlPoints[iCur], m_controlPoints[iNext], m_controlPoints[iNextNext], t);
	if (m_controlUpVectors.size() == m_controlPoints.size())
	{
		up = glm::normalize(Interpolate(m_controlUpVectors[iPrev], m_controlUpVectors[iCur], m_controlUpVectors[iNext], m_controlUpVectors[iNextNext], t));
	}

	return true;
}



// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void CCatmullRom::UniformlySampleControlPoints(int numSamples)
{
	glm::vec3 p, up;

	// Compute the lengths of each segment along the control polygon, and the total length
	ComputeLengthsAlongControlPoints();
	float fTotalLength = m_distances[m_distances.size() - 1];

	// The spacing will be based on the control polygon
	float fSpacing = fTotalLength / numSamples;

	// Call PointAt to sample the spline, to generate the points
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);

	}


	// Repeat once more for truly equidistant points
	m_controlPoints = m_centrelinePoints;
	m_controlUpVectors = m_centrelineUpVectors;
	m_centrelinePoints.clear();
	m_centrelineUpVectors.clear();
	m_distances.clear();
	ComputeLengthsAlongControlPoints();
	fTotalLength = m_distances[m_distances.size() - 1];
	fSpacing = fTotalLength / numSamples;
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);
	}


}



void CCatmullRom::CreateCentreline()
{
	// Call Set Control Points
	SetControlPoints();
	// Call UniformlySampleControlPoints with the number of samples required
	UniformlySampleControlPoints(500);
	// Create a VAO called m_vaoCentreline and a VBO to get the points onto the graphics card
	
	//Generate VAO and bind it
	glGenVertexArrays(1, &m_vaoCentreline);
	glBindVertexArray(m_vaoCentreline);

	//Create VBO and bind it
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();
	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < m_centrelinePoints.size(); i++) {
		vbo.AddData(&m_centrelinePoints[i], sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
	}
	// Upload the VBO to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER,m_centrelinePoints.size()* sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3), &m_centrelinePoints[0],GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));
}


void CCatmullRom::CreateOffsetCurves()
{
	float m_currentDistance = 0.0f;
	float w = 50.0f;
	// Compute the offset curves, one left, and one right.  Store the points in m_leftOffsetPoints and m_rightOffsetPoints respectively
	for (auto i = 0; i < m_centrelinePoints.size(); i++)
	{
			glm::vec3 p = m_controlPoints[i];
			glm::vec3 pNext;


			if (i == m_centrelinePoints.size() - 1) {

				pNext = m_controlPoints[0];
				
			}
			else {
				pNext = m_controlPoints[i + 1];
			}

			glm::vec3 T = glm::normalize(pNext - p);
			glm::vec3 N = glm::normalize(glm::cross(T, glm::vec3(0, 1, 0)));
			glm::vec3 B = glm::normalize(glm::cross(N, T));
			auto l = p - (w / 2) * N;
			auto r = p + (w / 2) * N;
			m_leftOffsetPoints.push_back(l);
			m_rightOffsetPoints.push_back(r);
		//}
	}
	// Generate two VAOs called m_vaoLeftOffsetCurve and m_vaoRightOffsetCurve, each with a VBO, and get the offset curve points on the graphics card
	// Note it is possible to only use one VAO / VBO with all the points instead.

	//Generate VAO for left offset and bind it
	glGenVertexArrays(1, &m_vaoLeftOffsetCurve);
	glBindVertexArray(m_vaoLeftOffsetCurve);

	//Create VBO for left curve and bind it
	CVertexBufferObject vbo_l;
	vbo_l.Create();
	vbo_l.Bind();
	glm::vec2 l_texCoord(0.0f, 0.0f);
	glm::vec3 l_normal(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < m_leftOffsetPoints.size(); i++) {
		vbo_l.AddData(&m_leftOffsetPoints[i], sizeof(glm::vec3));
		vbo_l.AddData(&l_texCoord, sizeof(glm::vec2));
		vbo_l.AddData(&l_normal, sizeof(glm::vec3));
	}
	// Upload the VBO to the GPU
	vbo_l.UploadDataToGPU(GL_STATIC_DRAW);
	
	GLsizei l_stride = sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, l_stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, l_stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, l_stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));

	//Generate VAO for Right offset and bind it
	glGenVertexArrays(1, &m_vaoRightOffsetCurve);
	glBindVertexArray(m_vaoRightOffsetCurve);

	//Create VBO for right curve and bind it
	CVertexBufferObject vbo_r;
	vbo_r.Create();
	vbo_r.Bind();
	glm::vec2 r_texCoord(0.0f, 0.0f);
	glm::vec3 r_normal(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < m_rightOffsetPoints.size(); i++) {
		
		vbo_r.AddData(&m_rightOffsetPoints[i], sizeof(glm::vec3));
		vbo_r.AddData(&r_texCoord, sizeof(glm::vec2));
		vbo_r.AddData(&r_normal, sizeof(glm::vec3));
	}

	// Upload the VBO to the GPU
	vbo_r.UploadDataToGPU(GL_STATIC_DRAW);

	GLsizei r_stride = sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, r_stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, r_stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, r_stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));

}


void CCatmullRom::CreateTrack(string Directory,string filename)
{
	
	m_texture.Load(Directory + filename);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card
	//Generate a VAO
	//Generate VAO and bind it
	glGenVertexArrays(1, &m_vaoTrack);
	glBindVertexArray(m_vaoTrack);

	//Create VBO and bind it
	CVertexBufferObject vbo_t;
	vbo_t.Create();
	vbo_t.Bind();
	
	
	std::vector<GLuint> indices;
	glm::vec3 normal(0.0f, 1.0f, 0.0f);

	for (unsigned int i = 0; i < m_centrelinePoints.size(); i++) {
		vbo_t.AddData(&m_leftOffsetPoints[i], sizeof(glm::vec3));
		
		
		//Texture Coord
		if (m_xCount % 2 == 0) {

			vbo_t.AddData(&glm::vec2(0.0f, 0.0f), sizeof(glm::vec2));
			vbo_t.AddData(&normal, sizeof(glm::vec3));

		}else {
			vbo_t.AddData(&glm::vec2(0.0f, 1.0f), sizeof(glm::vec2));
			vbo_t.AddData(&normal, sizeof(glm::vec3));
		}
		indices.push_back(m_vertexCount);
		vbo_t.AddData(&m_rightOffsetPoints[i], sizeof(glm::vec3));
		m_vertexCount++;

		//Texture Coord
		if (m_xCount % 2 == 0) {
			
			vbo_t.AddData(&glm::vec2(1.0f, 0.0f), sizeof(glm::vec2));
			vbo_t.AddData(&normal, sizeof(glm::vec3));
		}
		else {
			vbo_t.AddData(&glm::vec2(1.0f, 1.0f), sizeof(glm::vec2));
			vbo_t.AddData(&normal, sizeof(glm::vec3));
		}
		indices.push_back(m_vertexCount);
		m_vertexCount++;
		m_xCount++;
	}
	vbo_t.AddData(&m_leftOffsetPoints[0], sizeof(glm::vec3));
	vbo_t.AddData(&glm::vec2(0.0f, 0.0f), sizeof(glm::vec2));
	vbo_t.AddData(&normal, sizeof(glm::vec3));
	indices.push_back(m_vertexCount);
	m_vertexCount++;
	vbo_t.AddData(&m_rightOffsetPoints[0], sizeof(glm::vec3));
	vbo_t.AddData(&glm::vec2(1.0f, 0.0f), sizeof(glm::vec2));
	vbo_t.AddData(&normal, sizeof(glm::vec3));
	indices.push_back(m_vertexCount);
	m_vertexCount++;
	vbo_t.AddData(&m_leftOffsetPoints[1], sizeof(glm::vec3));
	vbo_t.AddData(&glm::vec2(0.0f, 1.0f), sizeof(glm::vec2));
	vbo_t.AddData(&normal, sizeof(glm::vec3));

	// Upload the VBO to the GPU
	vbo_t.UploadDataToGPU(GL_STATIC_DRAW);

	GLsizei r_stride = sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3);
	
	//Create Buffers for indices and bind it
	glGenBuffers(1, &m_vaoIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vaoIndices);


	//pass indices to GPU
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *sizeof(GLuint), &indices[0],GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, r_stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, r_stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, r_stride, (void*)(sizeof(glm::vec3)+ sizeof(glm::vec2)));

}


void CCatmullRom::RenderCentreline()
{
	// Bind the VAO m_vaoCentreline and render it
	glLineWidth(5.0f);
	glBindVertexArray(m_vaoCentreline);
	glDrawArrays(GL_POINTS, 0, m_centrelinePoints.size());
	glDrawArrays(GL_LINE_LOOP, 0, m_centrelinePoints.size());
	
	
}

void CCatmullRom::RenderOffsetCurves()
{
	// Bind the VAO m_vaoLeftOffsetCurve and render it
	glLineWidth(5.0f);
	glBindVertexArray(m_vaoLeftOffsetCurve);
	glDrawArrays(GL_LINE_STRIP, 0, m_leftOffsetPoints.size());
	glDrawArrays(GL_POINTS, 0, m_leftOffsetPoints.size());
	
	// Bind the VAO m_vaoRightOffsetCurve and render it
	glLineWidth(5.0f);
	glBindVertexArray(m_vaoRightOffsetCurve);
	glDrawArrays(GL_LINE_STRIP, 0, m_rightOffsetPoints.size());
	glDrawArrays(GL_POINTS, 0, m_rightOffsetPoints.size());

}


void CCatmullRom::RenderTrack()
{
	// Bind the VAO m_vaoTrack and render it
	glBindVertexArray(m_vaoTrack);
	m_texture.Bind();
	glDrawElements(GL_TRIANGLE_STRIP, m_vertexCount, GL_UNSIGNED_INT, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

int CCatmullRom::CurrentLap(float d)
{
	
	return (int)(d / m_distances.back());

}