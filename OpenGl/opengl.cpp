#define _USE_MATH_DEFINES
#include <cmath>

#include <glut.h>

#include <complex>

typedef std::complex<double> Complex;

Complex sqr(const Complex& v)
{
	return pow(v, 2);
}

using namespace std;

//class Complex
//{
//public:
//	double re;
//	double im;
//	Complex(double are, double aim) : re(are), im(aim) {}
//	Complex operator+(const Complex& v)
//	{
//		return Complex(re + v.re, im + v.im); // So operator "+" now with 2 complex values will do z[1] + z[2] = x[1] + x[2] and y[1] + y[2].
//	}
//};

//double abs(const Complex &v)
//{
//	return sqrt(v.re * v.re + v.im * v.im); // Finding square root from comlex value that will be R value. It names a mudule by complex value. "sqrt(xx + yy)".
//}

//Complex sqr(const Complex &v)
//{
//	return Complex(v.re * v.re - v.im * v.im, 2 * v.re * v.im); // Multiplication a complex value by himself (z^2). "z * z = (xx-yy) + i(2xy), z is complex value".
//}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	//glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(0.00, 0.75);
	glVertex2f(0.00, -0.75);
	glVertex2f(0.75, 0.00);
	glVertex2f(-0.75, 0.00);
	glColor3f(0.7, 0.1, 0.7);
	for (float a = 0; a < 2 * M_PI; a += 0.01)
	{
		glVertex2f(cos(a), sin(a)); //Polar system coordinates
	}
	for (float a = 0; a < 2 * M_PI; a += 0.01)
	{
		glVertex2f(cos(a+1), sin(a));
	}
	glColor3f(0., 0.5, 0.9);
	for (float a = 0; a < 2 * M_PI; a += 0.01)
	{
		glVertex2f(cos(a), sin(a*2));
	}
	glColor3f(0.2, 0.9, 0.2);
	for (float a = 0; a < 2 * M_PI; a += 0.01)
	{
		float r = 0.2 * (1 + sin(a)) * (1 + 0.9 * cos(8 * a)) * (1 + 0.1 * cos(24 * a));
		glVertex2f(r * cos(a), r * sin(a));
	}
	glBegin(GL_POINTS);
	glColor3f(0.9, 0.5, 0.1);
	for (int y = 0; y < 200; ++y)
		for (int x = 0; x < 200; ++x)
		{
			Complex z(0, 0);
			int i = 0;
			while (i < 1000 && abs(z) < 2)
			{
				z = sqr(z) + Complex(1.0 * (x - 100) / 70, // Z is complex value. Equation if z = z^2 + c, there C is a variable of complex values.
					1.0 * (y - 100) / 70);
				++i;
			}
			if (abs(z) >= 2)
			{
				float col = (50.0 - i) / 50.0;
				glColor3f(col, col, col);

				// Modulus of complex value that R value is equel or higher then 2 so print a point.
				// +40 by x axis becouse center of fractal displaysed by -1/5 of width by x.
				glVertex2f((x+40), y);
			}
		}
	glEnd();
	glutSwapBuffers();
	
	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(1800/7,0);
	glutCreateWindow("Grafic");
	glClearColor(0.00, 0.00, 0.00, 0.00);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// glOrtho(-1.0, 1.0, -1.0, 1.0,-1.0, 1.0); // Diapasonic coordinates
	glOrtho(-1, 200, 200, -1, -1, 1);
	glutDisplayFunc(display);
	glutMainLoop();

}