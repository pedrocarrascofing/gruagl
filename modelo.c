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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>                   // Libreria de utilidades de OpenGL

#define __modelo__			// Hace que se asigne memoria a las variables globales

#include "grua.h"

int COLORGRUA=0;



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
//Cabina
grua.xCabina=7;
grua.zCabina=7;
grua.yCabina=3;
//Base
grua.xBase=7;
grua.yBase=1;
grua.zBase=7;

//Patas
grua.lPata=10;

grua.yBaseGiro=1;

grua.lCuerda=15;
grua.lGancho=1;
grua.longBrazo=20;
grua.longTorre=15;
grua.tensorTrasero= sqrt(grua.longTorre*grua.longTorre + (grua.xCabina/2)*(grua.xCabina/2));
grua.tensorTraseroAng=(asin(grua.longTorre/grua.tensorTrasero))*180/M_PI;
grua.tensorDelantero;
grua.cabinaSt=0;
grua.cabinaSp=1;
grua.brazoSt=0;
grua.brazoSp=0.2;
grua.plumaSt=0;
grua.plumaSp=0.025;

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
   vCajones[a].angY=0;
}
ultCajon=-1;

/**
    Cajón Grua
**/
{
cajonGrua.id=-1;
cajonGrua.Rx=0;
cajonGrua.Ry=0;
cajonGrua.Rz=0;
cajonGrua.lx=2;
cajonGrua.ly=1;
cajonGrua.lz=2;
cajonGrua.color=marron;
cajonGrua.angY=0;
}

/**
    Pulsación no mantenida
**/
leftPuls=0;

cajaSeleccionada=-1;

/**
   Variables trigonometricas
**/
radZ=0;
radY=0;
proy_x=0;
trig_b=0;
trig_c=0;

/**
   Distancia para enganchar
**/
distPermitida=7;
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
        creaEstructura(grua.lPata,1,1,grua.lPata);

        /*Pata2*/
        glTranslatef(5,0,0);
        creaEstructura(grua.lPata,1,1,grua.lPata);

        /*Pata3*/
        glTranslatef(0,0,5);
        creaEstructura(grua.lPata,1,1,grua.lPata);

        /*Pata4*/
        glTranslatef(-5,0,0);
        creaEstructura(grua.lPata,1,1,grua.lPata);
    glPopMatrix();
    }

    /*BaseA + Base Giro*/
    {
    glPushMatrix();
        /*BaseA*/
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[gris]);
        glTranslatef(0,grua.lPata,0);
        caja(grua.xBase,grua.yBase,grua.zBase);

        /*BaseGiro*/
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[verde]);
        glTranslatef(0,grua.yBase,0);
        falsoCilindro(grua.yBaseGiro,2.2);
    glPopMatrix();
    }


    /*2º Parte*/
	{
    glPushMatrix();
        /*Situación*/
        glTranslatef(0,grua.lPata+grua.yBase+grua.yBaseGiro,0);
        glRotatef(grua.angY,0,1,0);

        /*Cabina*/
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[gris]);
        caja(grua.xCabina,grua.yCabina,grua.zCabina);

        /*Torre*/
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[rojo]);
            glPushMatrix();
            glTranslatef(0,grua.yCabina,0);
            creaTorre(grua.longTorre,2,2,grua.longTorre);
        glPopMatrix();

        /*Tensor Trasero*/
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[naranja]);
        glPushMatrix();
            glTranslatef(-grua.xCabina/2,grua.yCabina,0);
            glRotatef(grua.tensorTraseroAng-90,0,0,1);
            falsoCilindro(grua.tensorTrasero,0.1);
        glPopMatrix();

        /*Brazo*/
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[rojo]);
        glPushMatrix();
            radZ=(grua.angZ*M_PI/180);
            proy_x=grua.xCabina/2+grua.longBrazo*cos(radZ);
            trig_b=grua.longBrazo*sin(radZ);
            trig_c=grua.longTorre+grua.yCabina-trig_b;
            glTranslatef(proy_x,trig_b,0);
            glRotatef(180,0,1,0);
            glRotatef(-(90+grua.angZ),0,0,1);
            creaBrazo(grua.longBrazo,1,grua.longBrazo);
        glPopMatrix();

        /*Tensor Delantero*/
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[naranja]);
        glPushMatrix();
            glTranslatef(proy_x,trig_b,0);
            grua.tensorDelantero=sqrt(proy_x*proy_x+trig_c*trig_c);
            grua.tensorDelanteroAng=asin(proy_x/grua.tensorDelantero)*180/M_PI;
            glRotatef(grua.tensorDelanteroAng,0,0,1);
            falsoCilindro(grua.tensorDelantero,0.1);
        glPopMatrix();

        /*Cuerda + gancho + cajonFicticio*/

        glPushMatrix();
            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[violeta]);
            glTranslatef(proy_x,trig_b-grua.lCuerda,0);
            //Cuerda
            falsoCilindro(grua.lCuerda,0.05);
            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[rojo]);
            glRotatef(180,0,1,0);
            //Gancho
            creaGancho(grua.lGancho+0.25); // +0.25 para ajustar un pequeño error.

            //Cajon Ficticio
            if(cajonGrua.id!=-1)
            {                
                glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color[cajonGrua.color]);
                glTranslatef(0,-cajonGrua.ly-grua.lGancho,0);
                caja(cajonGrua.lx,cajonGrua.ly,cajonGrua.lz);
            }
        glPopMatrix();
    glPopMatrix();	//Situación General
	}

    /*Dibujar Cajas */
    {
    int a;
    for(a=0; a<MaxCajones; a++)
        if(vCajones[a].id!=-1){
            glLoadName(a);
            glPushMatrix();
                /*Situación*/
                glTranslatef(vCajones[a].Rx,vCajones[a].Ry,vCajones[a].Rz);
                glRotatef(vCajones[a].angY,0,1,0);

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
    //Subida/Bajada brazo
    if(grua.brazoSt==1 && trig_b<(grua.longTorre+grua.yCabina-1) && grua.angZ < 80)
        grua.angZ+=grua.brazoSp;
    else if(grua.brazoSt==-1 && grua.angZ>0 &&
           (grua.lCuerda+grua.lGancho+cajonGrua.ly)<(trig_b+grua.yBase+grua.yBaseGiro+grua.lPata))
        grua.angZ-=grua.brazoSp;

    //Subida/Bajada Cuerda
    //printf("%f, %f, %f\n\n", trib_c)
    if(grua.plumaSt==1 && (grua.lCuerda+grua.lGancho+cajonGrua.ly)<(trig_b+grua.yBase+grua.yBaseGiro+grua.lPata))
        grua.lCuerda+=grua.plumaSp;
    else if(grua.plumaSt==-1 && grua.lCuerda>1)
        grua.lCuerda-=grua.plumaSp;

    //Giro Cabina
    if(grua.cabinaSt==1)
        grua.angY+=grua.cabinaSp;
    else if(grua.cabinaSt==-1)
        grua.angY-=grua.cabinaSp;

    gluiPostRedisplay();			// Redibuja
    glutTimerFunc(30,idle,0);		// Vuelve a lanzar otro evento de redibujado en 30 ms
}


/**
    Función Pick
    Devuelve el índice que ocupa la caja en el vector
    de cajas vCajones o -1 si no se selecciona ninguna caja
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

    //Hits contiene el nº de objetos que han caido en la seleccion
    hits=glRenderMode(GL_RENDER);

    int i, aux=-1;
    float zmin, zmax, cercana=-1;

    for (i=0; i< hits*4; i+=4)
    {
        zmin=-1*(float)buff[i+1]/0xFFFFFFFF;
        //zmax=-1*(float)buff[i+2]/0xFFFFFFFF;
        //printf("Buffer de %i:  %i -- %f -- %f -- %i\n", i, buff[i], zmin, zmax, buff[i+3]);
        if(buff[i+3]!=-1 && zmin>cercana)
        {
            cercana=zmin;
            aux=buff[i+3];
        }
    }
    //printf ("\n_______________________%i_______________________%f______________________________________\n\n", aux, cercana);
    return aux;
}

void soltarCaja()
{
    if( cajonGrua.id!=-1){
        //radZ=(grua.angZ*M_PI/180);
        //a=grua.xCabina/2+grua.longBrazo*cos(radZ);
        vCajones[cajaSeleccionada].id=cajonGrua.id;
        vCajones[cajaSeleccionada].Rx=proy_x*cos(grua.angY*M_PI/180);
        vCajones[cajaSeleccionada].Rz=-1*proy_x*sin(grua.angY*M_PI/180);
        vCajones[cajaSeleccionada].angY=grua.angY;
        cajonGrua.id=-1;
    }
}

