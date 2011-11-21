/*	Grua 

	C.A.D. 						Curso 2008-2009
 	
	Codigo base para la realización de las practicas de CAD

	modulo modelo.c
	Dibujo del modelo
=======================================================
	J.C. Torres 
	Dpto. Lenguajes y Sistemas Informticos
	(Univ. de Granada, SPAIN)

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details 
 http://www.gnu.org/copyleft/gpl.html

=======================================================
Queda prohibido cobrar canon por la copia de este software

*/

/*
  Commit de prueba
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>                   // Libreria de utilidades de OpenGL

#define __modelo__			// Hace que se asigne memoria a las variables globales

#include "grua.h"

int COLORGRUA=0;
float aux, a, b, c, x;


/**	void initModel()

Inicializa el modelo y de las variables globales

**/

void initModel()
{

/**
    Valores iniciales para la grua
**/
grua.angY=15;
grua.angZ=50;
grua.lCuerda=15;
grua.longBrazo=15;
grua.longTorre=10;
grua.tensorTrasero= sqrt(grua.longTorre*grua.longTorre + 3.5*3.5);
grua.tensorTraseroAng=(asin(10/grua.tensorTrasero))*180/M_PI;
grua.tensorDelantero;
grua.cabinaSt=0;
grua.cabinaSp=1;
grua.brazoSt=0;
grua.brazoSp=0.2;
grua.plumaSt=0;
grua.plumaSp=0.05;

/**
    Vector de cajones y valores iniciales
**/
int a=0;
for(a=0; a<MaxCajones; a++)
{
    vCajones[a].id=-1;
    vCajones[a].Rx=10;
    vCajones[a].Ry=0;
    vCajones[a].Rz=10;
    vCajones[a].lx=2;
    vCajones[a].ly=1;
    vCajones[a].lz=2;
    vCajones[a].color=marron;
}
ultCajon=-1;

/**
    Pulsación no mantenida
**/
leftPuls=0;


/**
	Definicion de los colores usados.
**/
float colores[9][4]={{1,1,0,1.},{0.7,0.7,0.7,1},{1.0,0.3,0.3,1},
   		   {0.7,0.6,0.2,1},{0.2,1.0,1.0,1},{1.0,0.86,0.3,1},
		   {0.4,1,0.4,1.},{1,0.6,1,1.},{0,0,1,1.}};
int i,j;
	for(i=0;i<4;++i)
		for(j=0;j<9;++j)
			color[j][i]=colores[j][i];

	COLORGRUA=0;

/** 

Parametros iniciales de la camara

**/
	view_rotx=-30.0;	// Angulos de rotacion 
	view_roty=-45.0;
	view_rotz=0.0;
	d=100.0;

	x_camara=20;		// Posicion de la camara
	y_camara=2;
	z_camara=20;

	VISTA_PERSPECTIVA=0;	// Flag perspectiva/paralela

	ventanaMundoParalela=200;	// Ventana para proyeccion paralela
	origenXVentanaMundoParalelo=0;	
	origenYVentanaMundoParalelo=0;

}

/**	void Dibuja( void )

Procedimiento de dibujo del modelo. Es llamado por glut cada vez que se debe redibujar.

**/


void Dibuja( void )
{
   static GLfloat pos[4] = {5.0, 5.0, 10.0, 0.0 };		// Posicion de la fuente de luz
   int i;
   
   glInitNames();
   glPushName(-1);

   glPushMatrix();		// Apila la transformacion geometrica actual
   glClearColor(0,0,0.6,1);	// Fija el color de fondo a azul
   glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT );	 // Inicializa el buffer de color
   transformacionVisualizacion();	 // Carga transformacion de visualizacion
   glLightfv( GL_LIGHT0, GL_POSITION, pos );	// Declaracion de luz. Colocada aqui esta fija en la escena
   ejes(3);	// Dibuja los ejes

// Dibuja el suelo

    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[verde]);
    glTranslatef(0,-0.5,0);
    caja(200,0.5,200);
    glTranslatef(0,+0.5,0);
    /**************************************/



    /*Patas*/
    {
    glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[amarillo]);

        /*Pata1*/
        glTranslatef(-2.5,0,-2.5);
        creaEstructura(10,1,1,10);

        /*Pata2*/
        glTranslatef(5,0,0);
        creaEstructura(10,1,1,10);

        /*Pata3*/
        glTranslatef(0,0,5);
        creaEstructura(10,1,1,10);

        /*Pata4*/
        glTranslatef(-5,0,0);
        creaEstructura(10,1,1,10);
    glPopMatrix();
    }

    /*BaseA + Base Giro*/
    {
    glPushMatrix();
        /*BaseA*/
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[gris]);
        glTranslatef(0,10,0);
        caja(7,1,7);

        /*BaseGiro*/
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[verde]);
        glTranslatef(0,1,0);
        falsoCilindro(1,2.2);
    glPopMatrix();
    }


    /*2º Parte*/
	{
    glPushMatrix();
        /*Situación*/
        glTranslatef(0,12,0);
        glRotatef(grua.angY,0,1,0);

        /*Cabina*/
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[gris]);
        caja(7,3,7);

        /*Torre*/
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[rojo]);
            glPushMatrix();
            glTranslatef(0,3,0);
            creaTorre(grua.longTorre,2,2,10);
        glPopMatrix();

        /*Tensor Trasero*/
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[naranja]);
        glPushMatrix();
            glTranslatef(-3.5,3,0);
            glRotatef(grua.tensorTraseroAng-90,0,0,1);
            falsoCilindro(grua.tensorTrasero,0.1);
        glPopMatrix();

        /*Brazo*/
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[rojo]);
        glPushMatrix();
            aux=(grua.angZ*M_PI/180);
            a=3.5+grua.longBrazo*cos(aux);
            b=grua.longBrazo*sin(aux);
            c=13-b;
            glTranslatef(a,b,0);
            glRotatef(180,0,1,0);
            glRotatef(-(90+grua.angZ),0,0,1);
            creaBrazo(grua.longBrazo,1,15);
        glPopMatrix();

        /*Tensor Delantero*/
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[naranja]);
        glPushMatrix();
            glTranslatef(a,b,0);
            grua.tensorDelantero=sqrt(a*a+c*c);
            grua.tensorDelanteroAng=asin(a/grua.tensorDelantero)*180/M_PI;
            glRotatef(grua.tensorDelanteroAng,0,0,1);
            falsoCilindro(grua.tensorDelantero,0.1);
        glPopMatrix();

        /*Cuerda + gancho*/
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[violeta]);
        glPushMatrix();
            glTranslatef(a,b-grua.lCuerda,0);
            falsoCilindro(grua.lCuerda,0.05);
            glRotatef(180,0,1,0);
            creaGancho(1.0);
        glPopMatrix();
    glPopMatrix();	//Situación General
	}

    /*Dibujar Cajas */
    {
    int a;
    for(a=0; a<MaxCajones && vCajones[a].id!=-1; a++){
        glLoadName(a);
        glPushMatrix();
            /*Situación*/
            glTranslatef(vCajones[a].Rx,vCajones[a].Ry,vCajones[a].Rz);
            //glRotatef(ang,0,1,0);

            /*Caja*/
            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[vCajones[a].color]);
            caja(vCajones[a].lx,vCajones[a].ly,vCajones[a].lz);
        glPopMatrix();
    }
    }

    /***************************************/
    glPopMatrix(); 		// Desapila la transformacion geometrica
    glLoadName(-1);
    glutSwapBuffers();	// Intercambia el buffer de dibujo y visualizacion

}


/**	void idle()

Procedimiento de fondo. Es llamado por glut cuando no hay eventos pendientes.

**/
void idle()
{
    if(grua.brazoSt==1 && grua.angZ<45)
        grua.angZ+=grua.brazoSp;
    else if(grua.brazoSt==-1 && grua.angZ>0)
        grua.angZ-=grua.brazoSp;

    if(grua.plumaSt==1 && grua.lCuerda<b+11)
        grua.lCuerda+=grua.plumaSp;
    else if(grua.plumaSt==-1 && grua.lCuerda>1)
        grua.lCuerda-=grua.plumaSp;

    if(grua.cabinaSt==1)
        grua.angY+=grua.cabinaSp;
    else if(grua.cabinaSt==-1)
        grua.angY-=grua.cabinaSp;

    gluiPostRedisplay();			// Redibuja
    glutTimerFunc(30,idle,0);		// Vuelve a lanzar otro evento de redibujado en 30 ms
}


/**
    Función Pick
**/
int pick(int x, int y)
{
    GLint buff[512];
    glSelectBuffer(512, buff); //Indicamos que buff es la variable a usar como buffer de selección
    GLint viewport[4];
    GLint hits;

    glRenderMode(GL_SELECT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glGetIntegerv(GL_VIEWPORT, viewport);

    //Tamaño de ventana de selección de 5x5
    gluPickMatrix(x,viewport[3]-y,5,5, viewport);
    fijaProyeccion();
    Dibuja();

    //Tras todo esto, se deja la matriz de proyección como estaba
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    fijaProyeccion();

    //Hits contiene el nº de objetos que han caido en la seleccion???
    hits=glRenderMode(GL_RENDER);

    printf("Contenido de hits %i\n",hits);
    int i, aux;
    for(i=3; i<hits*4; i+=4)
    {
        printf ("Contenido del buffer[%i]: %i\n",i, buff[i]);
        if(buff[i]!=-1)
            aux=buff[i];
    }
    printf("\n\n");
    return aux;
}

