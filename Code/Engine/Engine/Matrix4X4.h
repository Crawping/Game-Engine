#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "Vector4.h"
namespace Engine
{
	class Matrix4X4
	{
		float m11,m12,m13,m14,
			  m21,m22,m23,m24,
			  m31,m32,m33,m34,
			  m41,m42,m43,m44;
	public:
		Matrix4X4(void);
		Matrix4X4(float,float,float,float,
				  float,float,float,float,
				  float,float,float,float,
				  float,float,float,float);
		Matrix4X4(const Matrix4X4& i_Other);
		Matrix4X4& operator=(const Matrix4X4& i_Other);
		void createIdentity(void);
		void invert(void);
		Matrix4X4 getInverse( void ) const;
		float determinant() const;
		void createXRotation(const float i_Rotation);
		void createYRotation(const float i_Rotation);
		void createZRotation(const float i_Rotation);
		void createTranslation(const float, const float, const float);
		Matrix4X4 multiply(const Matrix4X4& i_other);
		Matrix4X4& operator/(const float);
		Matrix4X4 operator*( const Matrix4X4 & i_Other ) const;
		void transpose( void );
		Matrix4X4 getTranspose( void ) const;
		Vector4 multiplyLeft( const Vector4 & i_Vector ) const;
		Vector4 multiplyRight( const Vector4 & i_Vector ) const;
		void printMatrix();
		static void Matrix4X4UnitTest();
	};
	inline Vector4 operator*( const Matrix4X4 & i_Matrix, const Vector4 & i_Vector4 )
	{
		return i_Matrix.multiplyLeft( i_Vector4 );
	}

	inline Vector4 operator*( const Vector4 & i_Vector4, const Matrix4X4 & i_Matrix )
	{
		return i_Matrix.multiplyRight( i_Vector4 );
	}
}
#endif