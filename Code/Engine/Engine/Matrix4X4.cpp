#include "Matrix4X4.h"
#include <math.h>
#include <assert.h>
#include <iostream>
#include "DebugLog.h"
#include "Utils.h"

Engine::Matrix4X4::Matrix4X4(void)
{
	//DebugOutput("Default Matrix Constructor Called\n");
}

Engine::Matrix4X4::Matrix4X4(float i_m11, float i_m12, float i_m13, float i_m14,
							 float i_m21, float i_m22, float i_m23, float i_m24,
							 float i_m31, float i_m32, float i_m33, float i_m34,
							 float i_m41, float i_m42, float i_m43, float i_m44)
{
	//DebugOutput("Parametrized Matrix Constructor Called\n");
	m11=i_m11; m12=i_m12; m13=i_m13; m14=i_m14;
	m21=i_m21; m22=i_m22; m23=i_m23; m24=i_m24;
	m31=i_m31; m32=i_m32; m33=i_m33; m34=i_m34;
	m41=i_m41; m42=i_m42; m43=i_m43; m44=i_m44;
}
Engine::Matrix4X4::Matrix4X4(const Engine::Matrix4X4& i_Other)
{
	//DebugOutput("Matrix Copy Constructor Called\n");
	m11=i_Other.m11; m12=i_Other.m12; m13=i_Other.m13; m14=i_Other.m14;
	m21=i_Other.m21; m22=i_Other.m22; m23=i_Other.m23; m24=i_Other.m24;
	m31=i_Other.m31; m32=i_Other.m32; m33=i_Other.m33; m34=i_Other.m34;
	m41=i_Other.m41; m42=i_Other.m42; m43=i_Other.m43; m44=i_Other.m44;
}

Engine::Matrix4X4& Engine::Matrix4X4::operator=(const Matrix4X4& i_Other)
{
	//DebugOutput("Matrix Assignment operator called\n");
	m11=i_Other.m11; m12=i_Other.m12; m13=i_Other.m13; m14=i_Other.m14;
	m21=i_Other.m21; m22=i_Other.m22; m23=i_Other.m23; m24=i_Other.m24;
	m31=i_Other.m31; m32=i_Other.m32; m33=i_Other.m33; m34=i_Other.m34;
	m41=i_Other.m41; m42=i_Other.m42; m43=i_Other.m43; m44=i_Other.m44;
	return *this;
}
void Engine::Matrix4X4::createIdentity(void)
{
	//DebugOutput("Identity Matrix Created\n");
	m11=1;				 m12=0;				   m13=0;				m14=0;
	m21=0;				 m22=1;				   m23=0;				m24=0;
	m31=0;				 m32=0;				   m33=1;				m34=0;
	m41=0;				 m42=0;				   m43=0;				m44=1;
}

void Engine::Matrix4X4::transpose(void)
{
	//DebugOutput("Matrix Transposed, original matrix changed\n");
	Matrix4X4 Temp;
	Temp.m11=m11; Temp.m12=m21; Temp.m13=m31; Temp.m14=m41;
	Temp.m21=m12; Temp.m22=m22; Temp.m23=m32; Temp.m24=m42;
	Temp.m31=m13; Temp.m32=m23; Temp.m33=m33; Temp.m34=m43;
	Temp.m41=m14; Temp.m42=m24; Temp.m43=m34; Temp.m44=m44;
	*this=Temp;
}

Engine::Matrix4X4 Engine::Matrix4X4::getTranspose(void) const
{
	//DebugOutput("Matrix Transposed, Original Matrix Unchanged\n");
	Matrix4X4 Temp;
	Temp.m11=m11; Temp.m12=m21; Temp.m13=m31; Temp.m14=m41;
	Temp.m21=m12; Temp.m22=m22; Temp.m23=m32; Temp.m24=m42;
	Temp.m31=m13; Temp.m32=m23; Temp.m33=m33; Temp.m34=m43;
	Temp.m41=m14; Temp.m42=m24; Temp.m43=m34; Temp.m44=m44;
	return Temp;
}

void Engine::Matrix4X4::invert(void)
{
	//DebugOutput("Matrix Inverted, Original Matrix Changed\n");
	float Determinant=determinant();
	assert(Determinant!=0);
	Matrix4X4 Cofactor;
	Cofactor.m11=     (m22*(m33*m44-m34*m43)-m23*(m32*m44-m34*m42)+m24*(m32*m43-m33*m42));
	Cofactor.m12=(-1)*(m21*(m33*m44-m34*m43)-m23*(m31*m44-m34*m41)+m24*(m31*m43-m33*m41));
	Cofactor.m13=	  (m21*(m32*m44-m34*m42)-m22*(m31*m44-m34*m41)+m24*(m31*m42-m32*m41));
	Cofactor.m14=(-1)*(m21*(m32*m43-m33*m42)-m22*(m31*m43-m33*m41)+m23*(m31*m42-m32*m41));

	Cofactor.m21=(-1)*(m12*(m33*m44-m34*m43)-m13*(m32*m44-m34*m42)+m14*(m32*m43-m33*m42));
	Cofactor.m22=	  (m11*(m33*m44-m34*m43)-m13*(m31*m44-m34*m41)+m14*(m31*m43-m33*m41));
	Cofactor.m23=(-1)*(m11*(m32*m44-m34*m42)-m12*(m31*m44-m34*m41)+m14*(m31*m42-m32*m41));
	Cofactor.m24=	  (m11*(m32*m43-m33*m42)-m12*(m31*m43-m33*m41)+m13*(m31*m42-m32*m41));

	Cofactor.m31=	  (m12*(m23*m44-m24*m43)-m13*(m22*m44-m24*m42)+m14*(m22*m43-m23*m42));
	Cofactor.m32=(-1)*(m11*(m23*m44-m24*m43)-m13*(m21*m44-m24*m41)+m14*(m21*m43-m23*m41));
	Cofactor.m33=	  (m11*(m22*m44-m24*m42)-m12*(m21*m44-m24*m41)+m14*(m21*m42-m22*m41));
	Cofactor.m34=(-1)*(m11*(m22*m43-m23*m42)-m12*(m21*m43-m23*m41)+m13*(m21*m42-m22*m41));

	Cofactor.m41=(-1)*(m12*(m23*m34-m24*m33)-m13*(m22*m34-m24*m32)+m14*(m22*m33-m23*m32));
	Cofactor.m42=	  (m11*(m23*m34-m24*m33)-m13*(m21*m34-m24*m31)+m14*(m21*m43-m23*m31));
	Cofactor.m43=(-1)*(m11*(m22*m34-m24*m32)-m12*(m21*m34-m24*m31)+m14*(m21*m42-m22*m31));
	Cofactor.m44=	  (m11*(m22*m33-m23*m32)-m12*(m21*m33-m23*m31)+m13*(m21*m42-m22*m31));
	*this=Cofactor;
	transpose();
	*this/Determinant;
}

Engine::Matrix4X4 Engine::Matrix4X4::getInverse(void) const
{
	//DebugOutput("Matrix Inverted, Original Matrix UnChanged\n");
	float Determinant=determinant();
	assert(Determinant!=0);
	Matrix4X4 Cofactor;
	Cofactor.m11=     (m22*(m33*m44-m34*m43)-m23*(m32*m44-m34*m42)+m24*(m32*m43-m33*m42));
	Cofactor.m12=(-1)*(m21*(m33*m44-m34*m43)-m23*(m31*m44-m34*m41)+m24*(m31*m43-m33*m41));
	Cofactor.m13=	  (m21*(m32*m44-m34*m42)-m22*(m31*m44-m34*m41)+m24*(m31*m42-m32*m41));
	Cofactor.m14=(-1)*(m21*(m32*m43-m33*m42)-m22*(m31*m43-m33*m41)+m23*(m31*m42-m32*m41));

	Cofactor.m21=(-1)*(m12*(m33*m44-m34*m43)-m13*(m32*m44-m34*m42)+m14*(m32*m43-m33*m42));
	Cofactor.m22=	  (m11*(m33*m44-m34*m43)-m13*(m31*m44-m34*m41)+m14*(m31*m43-m33*m41));
	Cofactor.m23=(-1)*(m11*(m32*m44-m34*m42)-m12*(m31*m44-m34*m41)+m14*(m31*m42-m32*m41));
	Cofactor.m24=	  (m11*(m32*m43-m33*m42)-m12*(m31*m43-m33*m41)+m13*(m31*m42-m32*m41));

	Cofactor.m31=	  (m12*(m23*m44-m24*m43)-m13*(m22*m44-m24*m42)+m14*(m22*m43-m23*m42));
	Cofactor.m32=(-1)*(m11*(m23*m44-m24*m43)-m13*(m21*m44-m24*m41)+m14*(m21*m43-m23*m41));
	Cofactor.m33=	  (m11*(m22*m44-m24*m42)-m12*(m21*m44-m24*m41)+m14*(m21*m42-m22*m41));
	Cofactor.m34=(-1)*(m11*(m22*m43-m23*m42)-m12*(m21*m43-m23*m41)+m13*(m21*m42-m22*m41));

	Cofactor.m41=(-1)*(m12*(m23*m34-m24*m33)-m13*(m22*m34-m24*m32)+m14*(m22*m33-m23*m32));
	Cofactor.m42=	  (m11*(m23*m34-m24*m33)-m13*(m21*m34-m24*m31)+m14*(m21*m33-m23*m31));
	Cofactor.m43=(-1)*(m11*(m22*m34-m24*m32)-m12*(m21*m34-m24*m31)+m14*(m21*m32-m22*m31));
	Cofactor.m44=	  (m11*(m22*m33-m23*m32)-m12*(m21*m33-m23*m31)+m13*(m21*m32-m22*m31));
	Cofactor.transpose();
	return (Cofactor/Determinant);
}
float Engine::Matrix4X4::determinant() const
{
	//DebugOutput("Matrix Determinant Called\n");
	float Minor11=(m22*(m33*m44-m34*m43)-m23*(m32*m44-m34*m42)+m24*(m32*m43-m33*m42));
	float Minor12=(m21*(m33*m44-m34*m43)-m23*(m31*m44-m34*m41)+m24*(m31*m43-m33*m41));
	float Minor13=(m21*(m32*m44-m34*m42)-m22*(m31*m44-m34*m41)+m24*(m31*m42-m32*m41));
	float Minor14=(m21*(m32*m43-m33*m42)-m22*(m31*m43-m33*m41)+m23*(m31*m42-m32*m41));
	float Det=m11*Minor11-m12*Minor12+m13*Minor13-m14*Minor14;
	return Det;
}

void Engine::Matrix4X4::createXRotation(const float i_Rotation)
{
	//DebugOutput("X Rotation Matrix Created\n");
	m11=1;				m12=0;				  m13=0;				m14=0;
	m21=0;				m22=cos(i_Rotation);  m23=-sin(i_Rotation); m24=0;
	m31=0;				m32=sin(i_Rotation);  m33=cos(i_Rotation);  m34=0;
	m41=0;				m42=0;				  m43=0;				m44=1;
}

void Engine::Matrix4X4::createYRotation(const float i_Rotation)
{
	DebugOutput("Y Rotation Matrix Created\n");
	m11=cos(i_Rotation); m12=0;				   m13=sin(i_Rotation); m14=0;
	m21=0;				 m22=1;				   m23=0;			    m24=0;
	m31=-sin(i_Rotation);m32=0;				   m33=cos(i_Rotation); m34=0;
	m41=0;				 m42=0;				   m43=0;				m44=1;
} 

void Engine::Matrix4X4::createZRotation(const float i_Rotation)
{
	//DebugOutput("Z Rotation Matrix Created\n");
	m11=cos(i_Rotation); m12=-sin(i_Rotation); m13=0;				m14=0;
	m21=sin(i_Rotation); m22=cos(i_Rotation);  m23=0;				m24=0;
	m31=0;				 m32=0;				   m33=1;				m34=0;
	m41=0;				 m42=0;				   m43=0;				m44=1;
}

void Engine::Matrix4X4::createTranslation(const float i_Tx, const float i_Ty, const float i_Tz)
{
	//DebugOutput("Translation Matrix Created\n");
	m11=1;				 m12=0;				   m13=0;				m14=i_Tx;
	m21=0;				 m22=1;				   m23=0;				m24=i_Ty;
	m31=0;				 m32=0;				   m33=1;				m34=i_Tz;
	m41=0;				 m42=0;				   m43=0;				m44=1;
}
Engine::Matrix4X4 Engine::Matrix4X4::multiply(const Matrix4X4& i_other)
{
	//DebugOutput("Matrix*Matrix multiplication called\n");
	Matrix4X4 temp;
	temp.m11=m11*i_other.m11+ m12*i_other.m21+ m13*i_other.m31+ m14*i_other.m41;
	temp.m12=m11*i_other.m12+ m12*i_other.m22+ m13*i_other.m32+ m14*i_other.m42;
	temp.m13=m11*i_other.m13+ m12*i_other.m23+ m13*i_other.m33+ m14*i_other.m43;
	temp.m14=m11*i_other.m14+ m12*i_other.m24+ m13*i_other.m34+ m14*i_other.m44;

	temp.m21=m21*i_other.m11+ m22*i_other.m21+ m23*i_other.m31+ m24*i_other.m41;
	temp.m22=m21*i_other.m12+ m22*i_other.m22+ m23*i_other.m32+ m24*i_other.m42;
	temp.m23=m21*i_other.m13+ m22*i_other.m23+ m23*i_other.m33+ m24*i_other.m43;
	temp.m24=m21*i_other.m14+ m22*i_other.m24+ m23*i_other.m34+ m24*i_other.m44;

	temp.m31=m31*i_other.m11+ m32*i_other.m21+ m33*i_other.m31+ m34*i_other.m41;
	temp.m32=m31*i_other.m12+ m32*i_other.m22+ m33*i_other.m32+ m34*i_other.m42;
	temp.m33=m31*i_other.m13+ m32*i_other.m23+ m33*i_other.m33+ m34*i_other.m43;
	temp.m34=m31*i_other.m14+ m32*i_other.m24+ m33*i_other.m34+ m34*i_other.m44;

	temp.m41=m41*i_other.m11+ m42*i_other.m21+ m43*i_other.m31+ m44*i_other.m41;
	temp.m42=m41*i_other.m12+ m42*i_other.m22+ m43*i_other.m32+ m44*i_other.m42;
	temp.m43=m41*i_other.m13+ m42*i_other.m23+ m43*i_other.m33+ m44*i_other.m43;
	temp.m44=m41*i_other.m14+ m42*i_other.m24+ m43*i_other.m34+ m44*i_other.m44;
	return temp;
}

Engine::Matrix4X4& Engine::Matrix4X4::operator/(const float i_Num)
{
	//DebugOutput("Matrix Division operator '/' Called\n");
	assert(i_Num!=0);
	m11=m11/i_Num;				 m12=m12/i_Num;				   m13=m13/i_Num;				m14=m14/i_Num;
	m21=m21/i_Num;				 m22=m22/i_Num;				   m23=m23/i_Num;				m24=m24/i_Num;
	m31=m31/i_Num;				 m32=m32/i_Num;				   m33=m33/i_Num;				m34=m34/i_Num;
	m41=m41/i_Num;				 m42=m42/i_Num;				   m43=m43/i_Num;				m44=m44/i_Num;
	return *this;
}

Engine::Matrix4X4 Engine::Matrix4X4::operator*( const Matrix4X4 & i_Other ) const
{
	//DebugOutput("Matrix Multiplication operator '*' Called\n");
	Matrix4X4 temp;
	temp.m11=m11*i_Other.m11+ m12*i_Other.m21+ m13*i_Other.m31+ m14*i_Other.m41;
	temp.m12=m11*i_Other.m12+ m12*i_Other.m22+ m13*i_Other.m32+ m14*i_Other.m42;
	temp.m13=m11*i_Other.m13+ m12*i_Other.m23+ m13*i_Other.m33+ m14*i_Other.m43;
	temp.m14=m11*i_Other.m14+ m12*i_Other.m24+ m13*i_Other.m34+ m14*i_Other.m44;

	temp.m21=m21*i_Other.m11+ m22*i_Other.m21+ m23*i_Other.m31+ m24*i_Other.m41;
	temp.m22=m21*i_Other.m12+ m22*i_Other.m22+ m23*i_Other.m32+ m24*i_Other.m42;
	temp.m23=m21*i_Other.m13+ m22*i_Other.m23+ m23*i_Other.m33+ m24*i_Other.m43;
	temp.m24=m21*i_Other.m14+ m22*i_Other.m24+ m23*i_Other.m34+ m24*i_Other.m44;

	temp.m31=m31*i_Other.m11+ m32*i_Other.m21+ m33*i_Other.m31+ m34*i_Other.m41;
	temp.m32=m31*i_Other.m12+ m32*i_Other.m22+ m33*i_Other.m32+ m34*i_Other.m42;
	temp.m33=m31*i_Other.m13+ m32*i_Other.m23+ m33*i_Other.m33+ m34*i_Other.m43;
	temp.m34=m31*i_Other.m14+ m32*i_Other.m24+ m33*i_Other.m34+ m34*i_Other.m44;

	temp.m41=m41*i_Other.m11+ m42*i_Other.m21+ m43*i_Other.m31+ m44*i_Other.m41;
	temp.m42=m41*i_Other.m12+ m42*i_Other.m22+ m43*i_Other.m32+ m44*i_Other.m42;
	temp.m43=m41*i_Other.m13+ m42*i_Other.m23+ m43*i_Other.m33+ m44*i_Other.m43;
	temp.m44=m41*i_Other.m14+ m42*i_Other.m24+ m43*i_Other.m34+ m44*i_Other.m44;
	return temp;
}


Engine::Vector4 Engine::Matrix4X4::multiplyRight( const Engine::Vector4 & i_Vector ) const
{
	//DebugOutput("Vector4*Matrix Called\n");
	Vector4 temp;
	temp.setx(i_Vector.getx()*m11+ i_Vector.gety()*m21+ i_Vector.getz()*m31+ i_Vector.getw()*m41);
	temp.sety(i_Vector.getx()*m12+ i_Vector.gety()*m22+ i_Vector.getz()*m32+ i_Vector.getw()*m42);
	temp.setz(i_Vector.getx()*m13+ i_Vector.gety()*m23+ i_Vector.getz()*m33+ i_Vector.getw()*m43);
	temp.setw(i_Vector.getx()*m14+ i_Vector.gety()*m24+ i_Vector.getz()*m34+ i_Vector.getw()*m44);

	return temp;
}

Engine::Vector4 Engine::Matrix4X4::multiplyLeft( const Engine::Vector4 & i_Vector ) const
{
	//DebugOutput("Matrix*Vector4 Called\n");
	Vector4 temp;
	temp.setx(m11*i_Vector.getx()+ m12*i_Vector.gety()+ m13*i_Vector.getz()+ m14*i_Vector.getw());
	temp.sety(m21*i_Vector.getx()+ m22*i_Vector.gety()+ m23*i_Vector.getz()+ m24*i_Vector.getw());
	temp.setz(m31*i_Vector.getx()+ m32*i_Vector.gety()+ m33*i_Vector.getz()+ m34*i_Vector.getw());
	temp.setw(m41*i_Vector.getx()+ m42*i_Vector.gety()+ m43*i_Vector.getz()+ m44*i_Vector.getw());

	return temp;
}

void Engine::Matrix4X4::printMatrix()
{
	//DebugOutput("%f   %f   %f   %f\n%f   %f   %f   %f\n%f   %f   %f   %f\n%f   %f   %f   %f\n",m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,m41,m42,m43,m44);
}


void Engine::Matrix4X4::Matrix4X4UnitTest()
{
	//Default Constructor Test
	Matrix4X4 Test1( 3.0f,0.0f,2.0f,-1.0f,
					1.0f,2.0f,0.0f,-2.0f,
					4.0f,0.0f,6.0f,-3.0f,
					5.0f,0.0f,2.0f,0.0f );
	Test1.printMatrix();

	//Inverse Without Changing original
	Matrix4X4 Test2= Test1.getInverse();
	Test2.printMatrix();
	//Since Original is Unchanged, Test1*Test2=Identity
	Matrix4X4 Test3=Test1*Test2;
	Test3.printMatrix();
	Test1.printMatrix();

	//Inverse Changing the Original
	Test1.invert();
	Test1.printMatrix();

	//Transpose Without Changing the Original
	Test2=Test1.getTranspose();
	Test2.printMatrix();

	//Transpose Changing the Original
	Test1.transpose();
	Test1.printMatrix();

	//Creating Identity Matrices
	Test1.createIdentity();
	Test1.printMatrix();

	//Finding the Determinant of Matrices
	float Determinant=Test1.determinant();
	DebugOutput("Determinant=%f \n",Determinant);

	//Dividing a Matrix by a number
	Test1/10;
	Test1.printMatrix();

	//Rotating A Matrix Around the X Axis
	Test1.createXRotation(Engine::degreesToRadians(90.0f));							   //    1.57==(PI)/2
	Test1.printMatrix();
	Engine::Vector3 TestVec3(0.0f,1.0f,0.0f);                  //Unit Vector along Y Axis
	Engine::Vector4 TestVec4=Test1.multiplyRight(TestVec3);    //multiplyRight expects a Vector4, but converts TestVec3 automatically in
	DebugOutput("%f  %f  %f  %f \n\n",TestVec4.getx(), TestVec4.gety(), TestVec4.getz());

	//Rotating A Matrix Around the Y Axis
	Test1.createYRotation(Engine::degreesToRadians(90.0f));							  
	Test1.printMatrix();
	TestVec3= Vector3(1.0f,0.0f,0.0f);						   //Unit Vector along X Axis
	TestVec4=Test1.multiplyRight(TestVec3);					  
	DebugOutput("%f  %f  %f  %f\n\n",TestVec4.getx(), TestVec4.gety(), TestVec4.getz());

	//Rotating A Matrix Around the Z Axis
	Test1.createZRotation(Engine::degreesToRadians(90.0f));
	Test1.printMatrix();
	TestVec3= Vector3(0.0f,1.0f,0.0f);
	TestVec4=Test1.multiplyRight(TestVec3);					   //Unit Vector along X Axis
	DebugOutput("%f  %f  %f  %f\n\n",TestVec4.getx(), TestVec4.gety(), TestVec4.getz());

}