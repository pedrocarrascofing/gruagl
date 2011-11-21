/*	Grua 

	C.A.D. 						Curso 2008-2009
 	
	Codigo base para la realización de las practicas de CAD

	modulo mouse.c
	Gestion de eventos de raton
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


/**	 void clickRaton( int boton, int estado, int x, int y )

Procedimiento para gestionar los eventos de pulsacion de los botones del raton.

Argumentos:

boton: Codigo glut del boton que ha manipulado el usuario.

estado: Estado en el que esta el boton que se ha pulsado.

x,y: Posicion, en coordenadas de pantalla, en que se encuantra el cursor.

**/

 void clickRaton( int boton, int status, int x, int y )
{
    int aux;
    switch ( boton )
    {
        case GLUT_LEFT_BUTTON:
            switch ( estado )
            {
             //Poner nueva caja
            case ponerCajas:
                if(ultCajon<MaxCajones){
                    if(status == GLUT_DOWN){
                        leftPuls=1;
                        ultCajon++;
                        vCajones[ultCajon].id=ultCajon;
                    }else{
                        leftPuls=0;
                    }
                    vCajones[ultCajon].Rx= ((x/anchoVentana-0.5)*ventanaMundoParalela) + origenXVentanaMundoParalelo;
                    vCajones[ultCajon].Rz= ((y/altoVentana-0.5)*ventanaMundoParalela)* altoVentana /anchoVentana- origenYVentanaMundoParalelo;                    
                    glutPostRedisplay();
                }
            break;

            // Pintar la que se seleccione con el raton
            case pintandoCajas:
                aux=pick(x,y);
                if(aux>-1 && aux<MaxCajones)
                    vCajones[aux].color=colorActivo;
            break;

            // Enganchar la caja que se seleccione con el raton
            case enganchando:
            break;

            // Desenganchar la caja si se encuentra alguan enganchada
            case soltando:
            break;

            //Seleccionar Cajas
                /*
            else if(status == GLUT_DOWN && estado != ponerCajas)
            {
                aux=pick(x,y);
                if(aux>-1 && aux<MaxCajones)
                    vCajones[aux].id=-1;
            }
            */

            } //End switch interno

        break; // End case GLUT_LEFT_BUTTON:
    }
}

/**	void RatonMovido( int x, int y )

Procedimiento para gestionar los eventos de movimiento del raton.

Argumentos:

x,y: Posicion, en coordenadas de pantalla, en que se encuantra el cursor.

**/

void RatonMovido( int x, int y )
{
    switch (estado)
    {
    case ponerCajas:
        if(leftPuls){
            vCajones[ultCajon].Rx= ((x/anchoVentana-0.5)*ventanaMundoParalela) + origenXVentanaMundoParalelo;
            vCajones[ultCajon].Rz= ((y/altoVentana-0.5)*ventanaMundoParalela)* altoVentana /anchoVentana- origenYVentanaMundoParalelo;
            glutPostRedisplay();
        }
    break;
    }


}
