/*	Grua

	C.A.D. 						Curso 2008-2009
 	
	Codigo base para la realización de las practicas de CAD

	modulo grua.c
	Programa principal
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
#include "grua.h"



/*=================================================*/ 

/**	int main( int argc, char *argv[] )	
	Programa principal.
	
	Inicializa glut y openGL
	Crea la ventana X
	Inicializa las variables globales
	Crea la interfaz de usuario
	lanza los gestores de eventos	
**/

int main( int argc, char *argv[] )
{
// Inicializa glu y openGL
   glutInit( &argc, argv );

// Crea una ventana X para la salida grafica en la posicion 0,0 con tamaño 600x600, con colores RGB-alfa, con doble buffer, y 
// buffer de profundidad
   glutInitWindowPosition( 0, 0 );
   glutInitWindowSize( 600, 600 );
   glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
   gluiCreateWindow("CAD. Curso 2010/11");

// Inicializa las variables globales
   initModel();

// Crea la interfaz de usuario
   creaIU();

// Inicializa las funciones de dibujo y cambio de tamanyo de la ventana X
   glutDisplayFunc( Dibuja );
   glutReshapeFunc( inicializaVentana );

// FUNCIONES DE INTERACCION
   glutKeyboardFunc(letra);
   glutSpecialFunc(especial);
   
   glutMouseFunc( clickRaton );
   glutMotionFunc( RatonMovido );

// Funcion de fondo
   glutTimerFunc(200,idle,0);

// Crea el menu pop-up
   CreaMenu();

// Inicializa parametros de openGL
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glEnable( GL_LIGHTING );
   glEnable( GL_LIGHT0 );

// Lanza el gestor de eventos de glut
   glutMainLoop();
   return 0;
}
