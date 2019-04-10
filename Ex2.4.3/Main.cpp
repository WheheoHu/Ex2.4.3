// Pointsz.c
// OpenGL SuperBible, Chapter 4
// Demonstrates OpenGL Primative GL_POINTS with point size
// Program by Richard S. Wright Jr.
#include <cmath>
#include <vector>
#include "glut.h"
template <typename TYPE>
class GeometryVector
{

public:

	GeometryVector(const TYPE x_ = TYPE(), const TYPE y_ = TYPE(), const TYPE z_ = TYPE());

	const TYPE x;
	const TYPE y;
	const TYPE z;

};
template <typename TYPE>
GeometryVector<TYPE>::GeometryVector(const TYPE x_, const TYPE y_, const TYPE z_)
	:
	x(x_),
	y(y_),
	z(z_)
{
}
std::vector<GeometryVector<float> > geometryData_;
std::vector<unsigned short> indexData_;


// Define a constant for the value of PI
#define PI 3.1415f

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

void RenderASphere(float fRadius, int iSlice = 10, int iStack = 10);
// Called to draw scene
void RenderScene(void)
{


	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT);

	// Save matrix state and do the rotation
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);


	RenderASphere(40);
	// Restore matrix state
	glPopMatrix();

	// Flush drawing commands
	glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Set drawing color to green
	glColor3f(0.0f, 1.0f, 0.0f);
}

void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		xRot -= 5.0f;

	if (key == GLUT_KEY_DOWN)
		xRot += 5.0f;

	if (key == GLUT_KEY_LEFT)
		yRot -= 5.0f;

	if (key == GLUT_KEY_RIGHT)
		yRot += 5.0f;

	if (key > 356.0f)
		xRot = 0.0f;

	if (key < -1.0f)
		xRot = 355.0f;

	if (key > 356.0f)
		yRot = 0.0f;

	if (key < -1.0f)
		yRot = 355.0f;

	// Refresh the Window
	glutPostRedisplay();
}


void ChangeSize(int w, int h)
{
	GLfloat nRange = 100.0f;

	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset projection matrix stack
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
	if (w <= h)
		glOrtho(-nRange, nRange, -nRange * h / w, nRange*h / w, -nRange, nRange);
	else
		glOrtho(-nRange * w / h, nRange*w / h, -nRange, nRange, -nRange, nRange);

	// Reset Model view matrix stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Sphere");
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);
	SetupRC();
	glutMainLoop();

	return 0;
}

void RenderASphere(float fRadius, int iSlice, int iStack)
{



	for (unsigned int stackNumber = 0; stackNumber < iStack; stackNumber++)
	{

		for (unsigned int sliceNumber = 0; sliceNumber < iSlice; sliceNumber++)
		{
			float theta = stackNumber * PI / iStack;
			float phi = sliceNumber * 2 * PI / iSlice;
			float sinTheta = std::sin(theta);
			float sinPhi = std::sin(phi);
			float cosTheta = std::cos(theta);
			float cosPhi = std::cos(phi);
			geometryData_.push_back(GeometryVector<float>(fRadius * cosPhi * sinTheta, fRadius * sinPhi * sinTheta, fRadius * cosTheta));
		}
	}
	for (unsigned int stackNumber = 0; stackNumber < iStack; ++stackNumber)
	{
		for (unsigned int sliceNumber = 0; sliceNumber <= iSlice; ++sliceNumber)
		{
			indexData_.push_back((stackNumber * iSlice) + (sliceNumber % iSlice));
			indexData_.push_back(((stackNumber + 1) * iSlice) + (sliceNumber % iSlice));
		}
	}

	glVertexPointer(3, GL_FLOAT, 0, &geometryData_[0]);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawElements(GL_TRIANGLE_STRIP, indexData_.size(), GL_UNSIGNED_SHORT, &indexData_[0]);


}
