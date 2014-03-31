#include "module.h"

const MATRIX4X4 IMAT_4X4 = {1,0,0,0, 
                            0,1,0,0, 
                            0,0,1,0, 
                            0,0,0,1};
#define MAT_COPY_4X4(src_mat, dest_mat) {memcpy((void *)(dest_mat), (void *)(src_mat), sizeof(MATRIX4X4) ); }
#define MAT_IDENTITY_4X4(m) {memcpy((void *)(m), (void *)&IMAT_4X4, sizeof(MATRIX4X4));}

void MUL_VECTOR4D_MATRIX4X4(POINT3D *va, MATRIX4X4 * mb, VECTOR4D * prod)
{
    //функция перемножает 2 матрицы
	    for (int col=0; col < 4; col++)
        {

        float sum = 0; 


             sum+=(va->x * mb->M[0][col]);
             sum+=(va->y * mb->M[1][col]);
             sum+=(va->z * mb->M[2][col]);
             sum+=(va->w * mb->M[3][col]);



		switch(col)
		{
		case 0:
		   prod->x = sum;
		   break;
		case 1:
			prod->y=sum;
			break;
		case 2:
			prod->z=sum;
			break;
		case 3:
			prod->w=sum;
			break;
		}

        } 
}
void VECTOR4D_COPY(POINT3D * vdst, VECTOR4D * vsrc) 
{//функция копирует один вектор в другой
    (vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;  
(vdst)->z = (vsrc)->z; (vdst)->w = (vsrc)->w;  }

void Mat_Init_4X4(MATRIX4X4 * ma, 
                 float m00, float m01, float m02, float m03,
                 float m10, float m11, float m12, float m13,
                 float m20, float m21, float m22, float m23,
                 float m30, float m31, float m32, float m33)

{
    //функция инициализирует матрицу 4х4

ma->M00 = m00; ma->M01 = m01; ma->M02 = m02; ma->M03 = m03;
ma->M10 = m10; ma->M11 = m11; ma->M12 = m12; ma->M13 = m13;
ma->M20 = m20; ma->M21 = m21; ma->M22 = m22; ma->M23 = m23;
ma->M30 = m30; ma->M31 = m31; ma->M32 = m32; ma->M33 = m33;

} 


void Mat_Mul_4X4(MATRIX4X4 * ma, 
                 MATRIX4X4 * mb,
                 MATRIX4X4 * mprod)
{
    //функция перемножает матрицы 4х4 
for (int row=0; row<4; row++)
    {
    for (int col=0; col<4; col++)
        {

        float sum = 0; 

        for (int index=0; index<4; index++)
             {

             sum+=(ma->M[row][index]*mb->M[index][col]);

             }

        mprod->M[row][col] = sum;

        } 

    } 

}



void Build_XYZ_Rotation_MATRIX4X4(float theta_x, // углы эйлера
                                  float theta_y, 
                                  float theta_z,
                                  MATRIX4X4_PTR mrot)
{

//функция строит матрицу поворота по углам
MATRIX4X4 mx, my, mz, mtmp;      
float sin_theta=0, cos_theta=0;   
int rot_seq = 0;                  


MAT_IDENTITY_4X4(mrot);

if (fabs(theta_x) > 0) // x
   rot_seq = rot_seq | 1;

if (fabs(theta_y) > 0) // y
   rot_seq = rot_seq | 2;

if (fabs(theta_z) > 0) // z
   rot_seq = rot_seq | 4;


switch(rot_seq)
      {
      case 0: 
      {
      return;
      } break;

      case 1: 
      {

      cos_theta = cos(rad*theta_x);
      sin_theta = sin(rad*theta_x);
 
      Mat_Init_4X4(&mx, 1,    0,          0,         0,
                        0,    cos_theta,  sin_theta, 0,
                        0,   -sin_theta, cos_theta, 0,
                        0,    0,          0,         1);

      MAT_COPY_4X4(&mx, mrot);
      return;

      } break;

      case 2: 
      {

      cos_theta = cos(rad*theta_y);
      sin_theta = sin(rad*theta_y);

      Mat_Init_4X4(&my,cos_theta, 0, -sin_theta, 0,  
                       0,         1,  0,         0,
                       sin_theta, 0, cos_theta,  0,
                       0,         0, 0,          1);


      MAT_COPY_4X4(&my, mrot);
      return;

      } break;

      case 3: 
      {

      cos_theta = cos(rad*theta_x);
      sin_theta = sin(rad*theta_x);

      Mat_Init_4X4(&mx, 1,    0,          0,         0,
                        0,    cos_theta,  sin_theta, 0,
                        0,   -sin_theta, cos_theta, 0,
                        0,    0,          0,         1);

      cos_theta = cos(rad*theta_y);
      sin_theta = sin(rad*theta_y);
 
      Mat_Init_4X4(&my,cos_theta, 0, -sin_theta, 0,  
                       0,         1,  0,         0,
                       sin_theta, 0, cos_theta,  0,
                       0,         0, 0,          1);

      Mat_Mul_4X4(&mx, &my, mrot);
      return;

      } break;

      case 4: 
      {

      cos_theta = cos(rad*theta_z);
      sin_theta = sin(rad*theta_z);

      Mat_Init_4X4(&mz, cos_theta, sin_theta, 0, 0,  
                       -sin_theta, cos_theta, 0, 0,
                        0,         0,         1, 0,
                        0,         0,         0, 1);


      MAT_COPY_4X4(&mz, mrot);
      return;

      } break;

      case 5:
      {

      cos_theta = cos(rad*theta_x);
      sin_theta = sin(rad*theta_x);

      Mat_Init_4X4(&mx, 1,    0,          0,         0,
                        0,    cos_theta,  sin_theta, 0,
                        0,   -sin_theta, cos_theta, 0,
                        0,    0,          0,         1);

      cos_theta = cos(rad*theta_z);
      sin_theta = sin(rad*theta_z);

      Mat_Init_4X4(&mz, cos_theta, sin_theta, 0, 0,  
                       -sin_theta, cos_theta, 0, 0,
                        0,         0,         1, 0,
                        0,         0,         0, 1);

      Mat_Mul_4X4(&mx, &mz, mrot);
      return;

      } break;

      case 6:
      {

      cos_theta = cos(rad*theta_y);
      sin_theta = sin(rad*theta_y);

      Mat_Init_4X4(&my,cos_theta, 0, -sin_theta, 0,  
                       0,         1,  0,         0,
                       sin_theta, 0, cos_theta,  0,
                       0,         0, 0,          1);

      cos_theta = cos(rad*theta_z);
      sin_theta = sin(rad*theta_z);

      Mat_Init_4X4(&mz, cos_theta, sin_theta, 0, 0,  
                       -sin_theta, cos_theta, 0, 0,
                        0,         0,         1, 0,
                        0,         0,         0, 1);

      Mat_Mul_4X4(&my, &mz, mrot);
      return;

      } break;

      case 7: 
      {

      cos_theta = cos(rad*theta_x);
      sin_theta = sin(rad*theta_x);

      Mat_Init_4X4(&mx, 1,    0,         0,         0,
                        0,    cos_theta, sin_theta, 0,
                        0,   -sin_theta, cos_theta, 0,
                        0,    0,         0,         1);

      cos_theta = cos(rad*theta_y);
      sin_theta = sin(rad*theta_y);

      Mat_Init_4X4(&my,cos_theta, 0, -sin_theta, 0,  
                       0,         1,  0,         0,
                       sin_theta, 0,  cos_theta,  0,
                       0,         0,  0,          1);

      cos_theta = cos(rad*theta_z);
      sin_theta = sin(rad*theta_z);

      Mat_Init_4X4(&mz, cos_theta, sin_theta, 0, 0,  
                       -sin_theta, cos_theta, 0, 0,
                        0,         0,         1, 0,
                        0,         0,         0, 1);

      Mat_Mul_4X4(&mx, &my, &mtmp);
      Mat_Mul_4X4(&mtmp, &mz, mrot);

      } break;

      default: break;
      
      } 
	  }

void Transform_OBJECT4DV1(POINT3D * obj, 
                          MATRIX4X4_PTR mt,  
                          int verticles_number,   
                          int transform_basis) 

{
        //функция поворачивает объект по заданной матрице поворота
      for (int vertex=0; vertex < verticles_number; vertex++)
          {
          POINT3D_ presult;

          MUL_VECTOR4D_MATRIX4X4(&obj[vertex], mt, &presult);

          VECTOR4D_COPY(&obj[vertex], &presult); 
          } 
}


	void VECTOR4D_Build(VECTOR4D * init, VECTOR4D * term, VECTOR4D * result)

	{
        //функция строит вектор по заданным координатам вершин
		result->x = term->x - init->x;
		result->y = term->y - init->y;
		result->z = term->z - init->z;
		result->w = 1;
	}

	void VECTOR4D_Build(POINT3D * init, POINT3D * term, VECTOR4D * result)

	{
        //функция строит вектор по заданным координатам вершин (перегружена)
		result->x = term->x - init->x;
		result->y = term->y - init->y;
		result->z = term->z - init->z;
		result->w = 1;
	}

	void VECTOR4D_Build(VECTOR4D * init, POINT3D * term, VECTOR4D * result)

	{
        //функция строит вектор по заданным координатам вершин (перегружена)
		result->x = term->x - init->x;
		result->y = term->y - init->y;
		result->z = term->z - init->z;
		result->w = 1;
	}

	void VECTOR4D_Cross(VECTOR4D * va, 
						VECTOR4D * vb,
						VECTOR4D * vn)
	{

        //функция считает векторное произведение
	vn->x =  ( (va->y * vb->z) - (va->z * vb->y) );
	vn->y = -( (va->x * vb->z) - (va->z * vb->x) );
	vn->z =  ( (va->x * vb->y) - (va->y * vb->x) ); 
	vn->w = 1;

	} 

	float VECTOR4D_Dot(VECTOR4D * va, VECTOR4D * vb)
	{
        //функция вычисляет скалярное произведение векторов
	return( (va->x * vb->x) + (va->y * vb->y) + (va->z * vb->z) );
	} 


	int Load_SMT_from_file(PYRAMID_PTR pyramid)
{
    //функция загружает данные первичной матрицы из файла

	FILE *SOURCE;
	if(!(SOURCE=fopen("C:\\Temp\\pyram.txt", "r")))
	{
		printf("FILE NOT FOUND");
		return 0;
	}

	while(getc(SOURCE)!='0')
	{
		if(feof(SOURCE))
		{
			printf("Error reading");
			return 0;
		}
	}
	getc(SOURCE);

    for(int i=0; i<4; i++)
    {
        float tempx = 0;
        fscanf(SOURCE, "%f", &pyramid->verts[i].x);			//кол-во вершин
	    fscanf(SOURCE, "%f", &pyramid->verts[i].y);
	    fscanf(SOURCE, "%f", &pyramid->verts[i].z);
    }

    if(DEBUG)
    {
        for(int i=0; i<4; i++)
        {
    	    printf("%f %f %f\n", pyramid->verts[i].x, pyramid->verts[i].y, pyramid->verts[i].z);
        }
    }
	
	fclose(SOURCE);

	return 0;
}




