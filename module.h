#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <math.h>
#include <iostream>
using namespace std;
#define rad 3.14/180
#define DEBUG 0

typedef struct POINT3D_
{

         float x,y,z,w;
		 bool visibility;



}POINT3D_, VECTOR4D;

class POINT3D
{


public:
         float x,y,z,w;

         POINT3D & POINT3D::operator+( const POINT3D & point )
        {
            float x = this->x;
            float y = this->y;

            POINT3D * result = new POINT3D();
            result->x = point.x + x;
            result->y = point.y + y;
            result->z = point.z + z;
            return *result;

        }

        POINT3D & POINT3D::operator=( const POINT3D & point )
        {
            this->x = point.x;
            this->y = point.y;
            this->z = point.z;
            return *this;
        }
};



typedef struct PYRAMID
{

    POINT3D verts[4];
    POINT3D pverts[4];

}PYRAMID, *PYRAMID_PTR;


typedef struct MATRIX4X4
{
union
    {
    float M[4][4];

    struct
         {
         float M00, M01, M02, M03;
         float M10, M11, M12, M13;
         float M20, M21, M22, M23;
         float M30, M31, M32, M33;
         }; 

    }; 

}MATRIX4X4, *MATRIX4X4_PTR;

void MUL_VECTOR4D_MATRIX4X4(VECTOR4D *va, MATRIX4X4 * mb, VECTOR4D * prod);
void VECTOR4D_COPY(VECTOR4D * vdst, VECTOR4D * vsrc);
void Mat_Init_4X4(MATRIX4X4 * ma, 
                 float m00, float m01, float m02, float m03,
                 float m10, float m11, float m12, float m13,
                 float m20, float m21, float m22, float m23,
                 float m30, float m31, float m32, float m33);
void Mat_Mul_4X4(MATRIX4X4 * ma, 
                 MATRIX4X4 * mb,
                 MATRIX4X4 * mprod);
void Build_XYZ_Rotation_MATRIX4X4(float theta_x, // euler angles
                                  float theta_y, 
                                  float theta_z,
                                  MATRIX4X4_PTR mrot);
void Transform_OBJECT4DV1(POINT3D * obj, 
                          MATRIX4X4_PTR mt,  
                          int verticles_number,   
                          int transform_basis);


	float VECTOR4D_Dot(VECTOR4D * va, VECTOR4D * vb);
	
	void VECTOR4D_Cross(VECTOR4D * va, 
						VECTOR4D * vb,
						VECTOR4D * vn);
	void VECTOR4D_Build(VECTOR4D * init, VECTOR4D * term, VECTOR4D * result);
    void VECTOR4D_Build(POINT3D * init, POINT3D * term, VECTOR4D * result);
    void VECTOR4D_Build(VECTOR4D * init, POINT3D * term, VECTOR4D * result);
	int Load_SMT_from_file(PYRAMID_PTR pyramid);
	void drawfilledquadrangle(float x1, float y1, float x2, float y2, float x3, float y3,  float x4, float y4);