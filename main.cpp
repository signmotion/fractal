#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <math.h>
#include "module.h"
#include <cairo.h>


#pragma comment(lib,"gdk-win32-2.0.lib")
#pragma comment(lib,"gtk-win32-2.0.lib")
#pragma comment(lib,"glib-2.0.lib")
#pragma comment(lib,"cairo.lib")
#pragma comment(lib,"pangowin32-1.0.lib")
#pragma comment(lib,"libpng.lib")
#pragma comment(lib,"zdll.lib")
#pragma comment(lib,"pango-1.0.lib")
#pragma comment(lib,"intl.lib")
#pragma comment(lib,"gthread-2.0.lib")
#pragma comment(lib,"gobject-2.0.lib")
#define PYRAMID_MAX_NUM 5000 //������������ ���������� �������

//================================================
    //��������� �������
    void  Draw_Pyramid(PYRAMID pyr);
    void Build_Pyramid_Tree(int dimension);
    void Build_Sierpinski_Triangle();
    void draw_object_v1();
    int checkbackfaces(POINT3D p1, POINT3D p2, POINT3D p3);
    int Compare_AvgZ(const void *arg1, const void *arg2);
    void Sort_Pyramids();

    //���������� ����������
    int dim;
    int angle;
    int cam;
    int draw_graphics;
    int zvalue;
    int Build_Pyramid;
    int pyramids_num;
    float scale;
    GtkWidget *window;
    VECTOR4D pov = {0,50,-500,1};
    PYRAMID pyramids[PYRAMID_MAX_NUM];
    PYRAMID tempPyramids[PYRAMID_MAX_NUM];

//=================================================


void drawfilledquadrangle(float x1, float y1, float x2, float y2, float x3, float y3,  float x4, float y4)
{

    //������� �������������� ��������������� � ��������� ��� ������ ������

	cairo_t *cr;
    cr = gdk_cairo_create (window->window); // �������� ������
    cairo_set_source_rgb(cr, 0, 0, 0); //���� - ������
    cairo_set_line_width(cr, 1); //������� �����
	
    //������������� ���������������� �� �����������
	cairo_line_to(cr, x1, y1);
	cairo_line_to(cr, x2, y2);
	cairo_line_to(cr, x3, y3);
	cairo_line_to(cr, x4, y4);

    //���������� ����������������
    cairo_fill(cr);

}


void drawfilledtriangle(POINT3D p1, POINT3D p2, POINT3D p3)
{
    //������� �������������� �����������, ������������ ��� � ������ ��� �����

    if(!checkbackfaces(p1, p2, p3) && draw_graphics) // ��������� ���������� ������������
    {
        return; //� ������ ���� ����������� �������� ����������, ������ �� �����������
    }

	cairo_t *cr;
    cr = gdk_cairo_create (window->window); //������������� ���������
    cairo_set_line_width(cr, 3); //��������� ������� �����
    
    //������������� ������������ �� �����������
    cairo_move_to(cr, p1.x+200, p1.y+200);
	cairo_line_to(cr, p2.x+200, p2.y+200);
	cairo_line_to(cr, p3.x+200, p3.y+200);

    //��������� �����
    cairo_set_source_rgb(cr, 255, 255, 0);

    //����� �������� ����� ���������� cairo, � ���� ������ ������� ����������������� ��� ������
    //cairo_stroke_preserve(cr);

    GdkGC * gc = window->style->fg_gc[GTK_WIDGET_STATE (window)];
    GdkColor * clr = new GdkColor();
    gdk_gc_set_line_attributes(gc, 4, GDK_LINE_SOLID,       //��������� ����� - ������� 4, ����� �������
                                GDK_CAP_NOT_LAST, GDK_JOIN_BEVEL);
    gdk_color_parse("yellow",clr); //���� ������
    gdk_gc_set_rgb_fg_color( gc, clr ); 

    //��������� ����� ���������� GTK
    gdk_draw_line(window->window, gc, p1.x+200, p1.y+200, p2.x+200, p2.y+200);
    gdk_draw_line(window->window, gc, p2.x+200, p2.y+200, p3.x+200, p3.y+200);
    gdk_draw_line(window->window, gc, p3.x+200, p3.y+200, p1.x+200, p1.y+200);

    cairo_set_source_rgb(cr, 0, 1, 0); //���� - �������
    

    if(draw_graphics)
    cairo_fill(cr); //���������� ������������ 
}



void Fill_Pyramid(PYRAMID pyr)
{
    //������� �������� ���������� ������������� � ��������, ������� ����� ����������

    drawfilledtriangle(pyr.pverts[3], pyr.pverts[1], pyr.pverts[0]);
    drawfilledtriangle(pyr.pverts[0], pyr.pverts[2], pyr.pverts[3]);
    drawfilledtriangle(pyr.pverts[3], pyr.pverts[2], pyr.pverts[1]);
    drawfilledtriangle(pyr.pverts[0], pyr.pverts[1], pyr.pverts[2]);

    //������ �����
    Draw_Pyramid(pyr);
}


int checkbackfaces(POINT3D p1, POINT3D p2, POINT3D p3)
{
    //������� ���������, ������� �� ����������� � ������� ���������

    float scal=0;
    VECTOR4D res;
    VECTOR4D res2;
    VECTOR4D res3;
    VECTOR4D cros;

    VECTOR4D_Build(&p1, &p2, &res); //������ ���������� ������
	VECTOR4D_Build(&p1, &p3, &res2); //������ ���������� ������
	VECTOR4D_Cross(&res, &res2, &cros); //������������� 1-�� � 2-�� ��������
	VECTOR4D_Build(&pov, &p1, &res3); //������ �� ����� ������ � ������� ������������
	scal = VECTOR4D_Dot(&res3, &cros); //��������� ������������ ������� ������ � ��������������
	if(scal<=0) //���� ������ <=0, �� ����������� ���������
	{
		return 0;
	}
		return 1;
}


gboolean on_key_press (GtkWidget * window,
			  GdkEventKey*	pKey,
			  gpointer userdata){
			  
   //������� ����������� ������� ������� ������� � ���������� ��� ���������

	MATRIX4X4 mrot; 

   if (pKey->type == GDK_KEY_PRESS){

          switch (pKey->keyval) //����� ������� ���� ������?
		{
          //���������� ����������� ����� ������
          case 65463: //Num 7
              pov.x++;
              break;
          case 65457: //Num 1
              pov.x--;
              break;
          case 65464: //Num 8
              pov.y++;
              break;
          case 65458: //Num 2
              pov.y--;
              break;
          case 65465: //Num 9
              pov.z++;
              break;
          case 65459: //Num 6
              pov.z--;
              break;

            //����� �� ��������� �� ���������� �������
            case 98: // B
              Build_Pyramid = !Build_Pyramid;
            break;

            //  ������ �������� �� 1 ����������� ������
            case 65506: //Shift
              dim++;
              if(dim>5)
                  break;
              Build_Sierpinski_Triangle();
            break;
            //����������/���������� ������� �������
			case 99 : //C
			  cam+=1;
			  break;
			case 120 :
			  cam-=1;
			  break;
            //�����������/��������� ������� �� z
			case 122 :
			  zvalue+=1;
			  break;
			case 118 :
			  zvalue-=1;
			  break;
          //����������� �������������
			case 65451 :
			  scale+=0.1; //Num+
			  break;
			case 65453 :
			  scale-=0.1; //Num-
			  break;

             //������� �� ��� y �����
			case 65361 :

                Build_XYZ_Rotation_MATRIX4X4(0, 5, 0, &mrot);

                for(int i=0; i<pyramids_num; i++)
                {
                    Transform_OBJECT4DV1(pyramids[i].verts, &mrot, 5,1);
                }
            
			break;

             //������� �� ��� x �����
			case 65362: 

		    	Build_XYZ_Rotation_MATRIX4X4(5, 0, 0, &mrot);

                for(int i=0; i<pyramids_num; i++)
                {
                    Transform_OBJECT4DV1(pyramids[i].verts, &mrot, 5,1);
                }

			break;

            //������� �� ��� y ����
			case 65363:

	            Build_XYZ_Rotation_MATRIX4X4(0, -5, 0, &mrot);

                for(int i=0; i<pyramids_num; i++)
                {
                    Transform_OBJECT4DV1(pyramids[i].verts, &mrot, 5,1);
                }

			break;
            
            //������� �� ��� x ����
			case 65364:

			    Build_XYZ_Rotation_MATRIX4X4(-5, 0, 0, &mrot);

                for(int i=0; i<pyramids_num; i++)
                {
                    Transform_OBJECT4DV1(pyramids[i].verts, &mrot, 5,1);
                }

			break;

            //����� �� �������� ��������
			case 103: //G
			draw_graphics=!draw_graphics;
			break;

			default:break;
			
		}
        //��� ������� ����� ������� ���������� ���������� �������
          draw_object_v1();

	}

	return TRUE;
}

void OBJECT_TO_PERSPECTIVE(PYRAMID * pyr)
{
    //�������������� ��������� � ������ ����������� �������������
	for(int i=0; i<4; i++)
	{
        pyr->pverts[i].x = cam*(pyr->verts[i].x / (pyr->verts[i].z*scale+zvalue));
        pyr->pverts[i].y = cam*(pyr->verts[i].y / (pyr->verts[i].z*scale+zvalue));
        pyr->pverts[i].z = pyr->verts[i].z;
	}
}

void draw_object_v1()
{
    //���������� �������, ����������� �������� ��������� �������

    //����������� ������� ������
    drawfilledquadrangle(0, 0, 0, 400, 600, 600, 600, 0);

    if(Build_Pyramid)
    {
        //��������� �������� �� z
        Sort_Pyramids();
        for(int i=0; i<pyramids_num; i++)
        {
            //���������� � ����� ����������� ��������
	        OBJECT_TO_PERSPECTIVE(&pyramids[i]);
            Fill_Pyramid(pyramids[i]);

        }
    }
}

void Build_Pyramid_Tree(PYRAMID pyram, int dimension)
{
    //������� ��������� �������� �� 4 �����������

    POINT3D foot[3];
    POINT3D side[3];
    PYRAMID tmPyramid[PYRAMID_MAX_NUM];
    int num=0;
    bool isVisible;

    //���������� ������ ���� ������� ������
    foot[0] = pyram.verts[1] + pyram.verts[0];
    foot[1] = pyram.verts[2] + pyram.verts[1];
    foot[2] = pyram.verts[2] + pyram.verts[0]; 

    //���������� ������ ���� ������ � ���������
    side[0] = pyram.verts[3] + pyram.verts[0];
    side[1] = pyram.verts[3] + pyram.verts[1];
    side[2] = pyram.verts[3] + pyram.verts[2];



        for(int n=0; n<3; n++)
        {
            foot[n].x/=2;
            foot[n].y/=2;
            foot[n].z/=2;
            side[n].x/=2;
            side[n].y/=2;
            side[n].z/=2;
        }

        //1 ��������
        tmPyramid[num].verts[0] = pyram.verts[0];
        tmPyramid[num].verts[1] = foot[0];
        tmPyramid[num].verts[2] = foot[2];
        tmPyramid[num].verts[3] = side[0];
        num++;

        //2 ��������
        tmPyramid[num].verts[0] = foot[0];
        tmPyramid[num].verts[1] = pyram.verts[1];
        tmPyramid[num].verts[2] = foot[1];
        tmPyramid[num].verts[3] = side[1];
        num++;

        //3 ��������
        tmPyramid[num].verts[0] = foot[1];
        tmPyramid[num].verts[1] = pyram.verts[2];
        tmPyramid[num].verts[2] = foot[2];
        tmPyramid[num].verts[3] = side[2];
        num++;

        //4 ��������
        tmPyramid[num].verts[0] = side[0];
        tmPyramid[num].verts[1] = side[1];
        tmPyramid[num].verts[2] = side[2];
        tmPyramid[num].verts[3] = pyram.verts[3];
        dimension--;

        //���� ����������� ��� �� ����������� ���������� ������� ����������� ��������
        if(dimension>0)
        {
            for(int p=0; p<4; p++)
            {
                Build_Pyramid_Tree(tmPyramid[p], dimension);
            }
        }

        //������� ��� �������� �� ��������� ������
        for(int k=0; k<=num; k++)
        {
            for(int l=0; l<4; l++)
            {
                tempPyramids[pyramids_num].verts[l] = tmPyramid[k].verts[l];
            }
            pyramids_num++;
        }
}


void Build_Sierpinski_Triangle()
{
        //������� ������ ����������� ����������� �� ��������� ������� �������

        int p_num = pyramids_num;
        pyramids_num = 0;

        //����� ������� ����������
        for(int i=0; i<p_num; i++)
            Build_Pyramid_Tree(pyramids[i], 1);


        //��������� ������ �� ���������� ��������� � ����� ��� ������������
        for(int k=0; k<=pyramids_num; k++)
        {
            for(int l=0; l<4; l++)
            {
                pyramids[k].verts[l] = tempPyramids[k].verts[l];
            }
        }
}


void Draw_Pyramid(PYRAMID pyr)
{
    //������� ������ ����� �������� ��������

   GdkGC * gc = window->style->fg_gc[GTK_WIDGET_STATE (window)];
   GdkColor * clr = new GdkColor();
   gdk_color_parse("red",clr); //���� - �������
   gdk_gc_set_rgb_fg_color( gc, clr ); 
   gdk_gc_set_line_attributes(gc, 1, GDK_LINE_SOLID,
                        GDK_CAP_NOT_LAST, GDK_JOIN_BEVEL);

   //������ ���������� ������ �����
   gdk_draw_line(window->window, gc, 450+(int)pyr.pverts[0].x, 200+(int)pyr.pverts[0].y, 450+(int)pyr.pverts[1].x, 200+(int)pyr.pverts[1].y);
   gdk_draw_line(window->window, gc, 450+(int)pyr.pverts[1].x, 200+(int)pyr.pverts[1].y, 450+(int)pyr.pverts[2].x, 200+(int)pyr.pverts[2].y);
   gdk_draw_line(window->window, gc, 450+(int)pyr.pverts[0].x, 200+(int)pyr.pverts[0].y, 450+(int)pyr.pverts[2].x, 200+(int)pyr.pverts[2].y);
   gdk_draw_line(window->window, gc, 450+(int)pyr.pverts[2].x, 200+(int)pyr.pverts[2].y, 450+(int)pyr.pverts[3].x, 200+(int)pyr.pverts[3].y);
   gdk_draw_line(window->window, gc, 450+(int)pyr.pverts[0].x, 200+(int)pyr.pverts[0].y, 450+(int)pyr.pverts[3].x, 200+(int)pyr.pverts[3].y);
   gdk_draw_line(window->window, gc, 450+(int)pyr.pverts[1].x, 200+(int)pyr.pverts[1].y, 450+(int)pyr.pverts[3].x, 200+(int)pyr.pverts[3].y);
}

int main(int argc, char ** argv )

{
    //�������-������� ����� ����������

    //������������� ����������
	scale=0;
	angle = 0;
	cam = 100;
	zvalue = 35;
	draw_graphics=1;
    dim = 0;
    pyramids_num = 1;
    Build_Pyramid = 0;
    //�������� ������ � ��������� ��������
	Load_SMT_from_file(&pyramids[0]);
    //������� �������� �� 180 ��������
    MATRIX4X4 mrot;
	Build_XYZ_Rotation_MATRIX4X4(180, 0, 0, &mrot);
    Transform_OBJECT4DV1(pyramids[0].verts, &mrot, 5,1);

    //������������� ������������� ��������� � ����������� ������� ����������
	gtk_init( &argc, &argv );
	window =  gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request (window, 600, 400);
	g_signal_connect (window,"key-press-event",G_CALLBACK (on_key_press),NULL);
	g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (gtk_main_quit), NULL);
	gtk_widget_show_all(window);
    
    //������ ��������� �����
	gtk_main();
	return 0;


}


    void Sort_Pyramids()
{
    //������� �������� ���������� ������� �� z
    qsort(pyramids, pyramids_num, sizeof(PYRAMID), Compare_AvgZ);
}



int Compare_AvgZ(const void *arg1, const void *arg2)
{

float z1, z2;

PYRAMID * poly_1;
PYRAMID * poly_2;

poly_1 = ((PYRAMID *)(arg1));
poly_2 = ((PYRAMID *)(arg2));

// ������� ������� �������� ������ ��������
z1 = (float)0.33333*(poly_1->verts[0].z + poly_1->verts[1].z + poly_1->verts[2].z);

// ������� ������� �������� ������ ��������
z2 = (float)0.33333*(poly_2->verts[0].z + poly_2->verts[1].z + poly_2->verts[2].z);

// ���������� �������� z1 � z2 � � ���������� �������� �����
if (z1 < z2)
   return(-1);
else
if (z1 > z2)
   return(1);
else
   return(0);

}