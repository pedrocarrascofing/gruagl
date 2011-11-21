/*	Grua 

	C.A.D. 						Curso 2008-2009
 	
	Codigo base para la realización de las practicas de CAD

	modulo grua.h

=======================================================
	Copyright (c) J.C. Torres 
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

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>


#ifndef __gruaH__ // Para evitar doble inclusion
#define __gruaH__


/* El siguiente bloque comprueba que modulo se esta compilando de forma que las variables
globales solo se inicialicen cuando se compile el modulo "modelo.c"
*/

#ifndef __modelo__ // No esta compilando modelo, las variables globales son extern

#define ambito extern

#else // Esta compilando  modelo, las variables globales no son extern (se les asigna memoria)

#define ambito

#endif

// ====================================== DEFINES ========================================================


/**
Definicion de identificadores de colores. Los valores se corresponden con la
posicion en la matriz color.
**/
#define amarillo 0
#define gris 1
#define rojo 2
#define	marron 3
#define celeste 4
#define naranja 5
#define verde 6
#define colorfeedback 7
#define azul 8
#define violeta 7


#define MAXNAMES 10
#define MAXOBJETOS 10

#define NGruas 20
#define NCajones 50
#define MaxCajones 20


//========================================  ESTRUCTURAS DE DATOS ==========================================

typedef enum {neutro,paseando,vistaArriba,ponerCajas, pintandoCajas, enganchando, soltando} ESTADO;


typedef enum{ SALIR,ENGANCHAR, DESENGANCHAR, ROTARCAJA, MOVERCAJA,
	PINTARCAJA, DESDEARRIBA, OPERARGRUA, PASEAR, PONERCAJAS
	} opciones_menu;  	// Ident. de opciones del menu

typedef struct{
    float angY;     // Ángulo de giro de la base
    float angZ;     // Ángulo de giro del brazo.

    float xCabina;  // Dimensión X de la cabina
    float yCabina;  // Dimensión Y de la cabina
    float zCabina;  // Dimensión Z de la cabina

    float yBase;    // Dimensión Y de la base
    float xBase;    // Dimensión X de la base
    float zBase;    // Dimensión Z de la base

    float yBaseGiro;// Dimensión Y de la base de giro

    float lPata;    // Longitud de las patas de la base
    float lCuerda;  // Longitud de la cuerda
    float lGancho;  // Longitud del gancho.
    float tensorTrasero;        //Longitud del tensor trasero
    float tensorTraseroAng;     //Angulo del tensor trasero en grados
    float tensorDelantero;      //Longitud del tensor delantero
    float tensorDelanteroAng;   //Angulo del tensor delantero en grados
    float longBrazo;            //Longitud del brazo
    float longTorre;            //Longitud de la torre superior
    int brazoSt;    /* Estado del brazo
                        si 0 --> Brazo parado
                        si 1 --> Brazo subiendo
                        si -1 --> Brazo bajando */
    int plumaSt;    /* Estado de la pluma
                        si 0 --> Quieta
                        si 1 --> Bajando
                        si -1 --> Subiendo */
    int cabinaSt;   /* Estado de la cabina
                        si 0 --> Quieta
                        si 1 --> Giro en sentido antihorario
                        si -1 --> Giro en sentido horario */
    float brazoSp;  //Velocidad de giro del brao
    float plumaSp;  //Velocidad de subida/bajada de la pluma
    float cabinaSp; //Velocidad de giro de la cabina

} GRUA;
GRUA grua;
   
typedef struct{
    int id;    //Identificador del cajon. Si -1 --> No dibujado
    float Rx;   //Posición en el eje x en el mundo real
    float Ry;   //Posición en el eje y en el mundo real
    float Rz;   //Posición en el eje z en el mundo real
    float lx;   //Longitud de la caja en x
    float ly;   //Longitud de la caja en x
    float lz;   //Longitud de la caja en x
    int color;  //Color para la caja
} CAJA;

/**
    Vector que contiene los cajones.
    Podrá contener hata MaxCajones.
    ultCajon indica cual fue el último cajon usado de vCajones
**/
CAJA vCajones[MaxCajones];
int ultCajon;

/**
    Caja seleccionad será una variable global que tendrá el indice
    de la última caja seleccionada para el vector vCajones
**/
int cajaSeleccionada;

/**
    Cajon que permanecerá enganchado a la pluma de la grua para
    crear el efecto de que la caja es cogida o no.
**/
CAJA cajonGrua;

/**
    Variable que controla cuando el botón izquierdo del ratón
    ha sido pulsado y mantenido.
    Si 0 --> No es mantenido
    Si 1 --> Es mantenido
**/
int leftPuls;

/**
    colorActivo contendrá el último color seleccionado en la paleta
    de colores. Variable Global.
**/
int colorActivo;



//================================================ VARIABLES  =============================================

ambito float color[9][4];	// Paleta de colores (se inicializa en initModel)	

ambito int estado;		// Estado en el que se encuentra el programa

ambito int menu;     		// Identificador del menu glut


/**

Angulos de rotacion de la camara.

**/

ambito float view_rotx, view_roty, view_rotz;

/**

Indica el tipo de proyeccion que se esta realizando. Si esta a uno 
se hace proyeccion perspectiva, en caso contrario se hace paralela.

**/
ambito int VISTA_PERSPECTIVA;

/**

Tamanyo para la ventana del mundo en proyeccion paralela.

**/
ambito float ventanaMundoParalela;

/**

Coordenadas x,y de la esquina inferior de la ventana del mundo en vista
paralela.
 
**/
ambito float origenXVentanaMundoParalelo;
ambito float origenYVentanaMundoParalelo;

/**

Distancia de la camara al centro de coordenadas del modelo.

**/
ambito float d;

ambito float x_camara,y_camara,z_camara;

ambito float anchoVentana,altoVentana;

// ============================================== FUNCIONES ================================================

// modelo.c

/**
	Funcion de redibujado. Se ejecuta con los eventos postRedisplay
**/
void Dibuja( void );


/**
    Fucion pick
**/
int pick(int x, int y);

/**
	Funcion de fondo
**/
void idle();

/**
	Funcion de inicializacion del modelo y de las variables globales
**/
void initModel();

// mouse.c

/**
	Funcion de captura de eventos de pulsacion de botones del raton
	boton: identificador del boton pulsado
    status: estado del boton
	x: posicion del cursor en coordenadas de pantalla
	y: posicion del cursor en coordenadas de pantalla
**/
void clickRaton( int boton, int status, int x, int y );
	

/**
	Funcion de captura de eventos de desplazamiento de raton
	x: posicion del cursor en coordenadas de pantalla
	y: posicion del cursor en coordenadas de pantalla
**/
void RatonMovido( int x, int y );

// entradaTeclado.c

/**	
	Funcion de captura de eventos pulsacion de tecla correspondiente a caracter alfanumerico
	K: codigo ascii del caracter
	x: posicion del cursor en coordenadas de pantalla cuando se pulso la tecla
	y: posicion del cursor en coordenadas de pantalla cuando se pulso la tecla
**/
void letra (unsigned char k, int x, int y);

/**
	Funcion de captura de eventos pulsacion de caracteres especiales y de control
	K: codigo del control pulsado (se definen como constantes en glut.h)
	x: posicion del cursor en coordenadas de pantalla cuando se pulso la tecla
	y: posicion del cursor en coordenadas de pantalla cuando se pulso la tecla
**/
void especial(int k, int x, int y);


// entradaMenu.c

void seleccionMenu( int opcion );
// Este procedimiento es llamado por el sistema cuando se selecciona una
// opcion del menu glut. El parametro contiene el identificador de la opcion

 
void CreaMenu();
// Este proc. se usa para definir el menu glut. Se llama desde main.


// estructura.c

void caja( float a, float b, float m);
/* Construye un paralelepipedo alineado con los ejes de
dimension a x b x m. El objeto se construye en el semiespacio y>=0, con el origen en el
centro de la base. 
*/


void falsoCilindro(float h, float r);
/*

Crea un paralelepipedo con sombreado como phong (simulando un cilindro)
con centro de las bases en 0,0,0 y 0,h,0, y radio de la base r.

*/

void falsoCono( float h, float r);
/*

Crea una piramide sombreada como un cono con centro de las bases en
0,0,0, cuspide en  0,h,0, y radio de la base r.

*/


void ejes(float a);
/*
Dibuja unos ejes  de tamaño a
*/

// ========================== paralelepipedo ===========================================
/*
void paralelepipedo(float x0, float y0, float z0, 
		    float x1, float y1, float z1, 
		    float a, float b);*/
/*
	Crea un paralelepipedo con centro de las bases en
	x0,y0,z0 y x1,y1,z1, y tamaño de la base axb.
*/


// =========================== creaEstructura ==========================================


void creaEstructura(float h, float a, float b, int n);
/*
Crea un fragmento de estructura de una grua, con centro de las bases en
0,0,0 y 0,h,0. Las secciones de las bases tienen tamanyo axb
n es el numero de niveles que tiene la estructura.

Los perfiles tienen seccion cuadrada, dada por el parametro seccion.
	
Para crear la estructura se utiliza el procedimiento paralelepipedo.
*/

void creaBrazo(float h, float a, int n);
/*
Crea un fragmento del brazo de una grua, con centro una de las aristas 
de los extremos en
0,0,0 y 0,h,0. Los lados de las bases tienen tamanyo a,
n es el numero de tramos.

Los perfiles tienen seccion triangular
	
Para crear la estructura se utiliza el procedimiento paralelepipedo.
*/



void creaTorre(float h, float a, float b, int n);
/*

Crea un fragmento de la terminacion de la estructura de una grua, 
con centro de las bases en
0,0,0 y 0,h,0. Las secciones de las bases tienen tamanyo axb
n es el numero de niveles que tiene la estructura.

Los perfiles tienen seccion cuadrada, dada por el parametro seccion.
	
Para crear la estructura se utiliza el procedimiento paralelepipedo.

*/

void creaGancho( float alto);
/*
Crea un gancho colgando del punto (0,0,0) de tamanyo alto
El gancho siempre esta vertical
*/


// visual.c

void fijaProyeccion();

void transformacionVisualizacion();


void inicializaVentana(GLsizei ancho,GLsizei alto);



void  creaIU();


void drawSeleccionColor();


void DrawPalancas();

void seleccionaPalancas( int boton, int estado, int x, int y);

void muevePalanca( int x, int y);


void seleccionColor( int boton, int estado, int x, int y);


#endif
#undef ambito

