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
   int aux=0;
   float pluma_x=0, pluma_y=0, pluma_z=0, distancia=0;
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
               vCajones[ultCajon].Rx= ((x/anchoVentana-0.5)*ventanaMundoParalela) + origenXVentanaMundoParalelo;
               vCajones[ultCajon].Rz= ((y/altoVentana-0.5)*ventanaMundoParalela)* altoVentana /anchoVentana- origenYVentanaMundoParalelo;
            }else{
               leftPuls=0;
            }

            glutPostRedisplay();
         }
         break;

      // Pintar la que se seleccione con el raton
      case pintandoCajas:
         if(status == GLUT_DOWN){
            aux=pick(x,y);
            if(aux>-1 && aux<MaxCajones)
               vCajones[aux].color=colorActivo;
         }
         break;

      // Enganchar la caja que se seleccione con el raton
      case enganchando:
         if(status == GLUT_DOWN && cajonGrua.id==-1){
            cajaSeleccionada=pick(x,y);

            if(cajaSeleccionada>-1 && cajaSeleccionada<MaxCajones)
            {
               radY=(grua.angY*M_PI/180);
               pluma_x=abs((grua.longBrazo + grua.xCabina/2)*cos(radY));
               pluma_z=abs((grua.longBrazo + grua.xCabina/2)*sin(radY));
               radZ=(grua.angZ*M_PI/180);
               pluma_y=abs(grua.longBrazo*sin(radZ));
               pluma_y += grua.lPata+grua.yBase+grua.yBaseGiro-grua.lCuerda-grua.lGancho;

               distancia=sqrt( (pluma_x-vCajones[cajaSeleccionada].Rx)*(pluma_x-vCajones[cajaSeleccionada].Rx)
                             + (pluma_y-vCajones[cajaSeleccionada].Ry)*(pluma_y-vCajones[cajaSeleccionada].Ry)
                             + (pluma_z-vCajones[cajaSeleccionada].Rz)*(pluma_z-vCajones[cajaSeleccionada].Rz));

               if(distancia <= distPermitida)
               {
                  cajonGrua.color=vCajones[cajaSeleccionada].color;
                  cajonGrua.id=vCajones[cajaSeleccionada].id;
                  vCajones[cajaSeleccionada].id=-1;
               }
            }
         }
         break;

      case soltando:
         /* Desenganchar la caja si se encuentra alguan enganchada
         En entradaMenu se llama a sotarCaja(), ya que despues de seleccionar
         la opción no es necesario clicar en el escenario. */
         break;

      } //End switch interno

      break; // End case GLUT_LEFT_BUTTON:
   }
}

/**	void RatonMovido( int x, int y )
Procedimiento para gestionar los eventos de movimiento del raton.
Argumentos: x,y: Posicion, en coordenadas de pantalla, en que se encuantra el cursor.
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
