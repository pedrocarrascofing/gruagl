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
   int aux=0, i;
   float pluma_x=0, pluma_y=0, pluma_z=0, distancia=0, aux_x, aux_y, aux_z;
   char encima=0;
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
                  aux_x=((x/anchoVentana-0.5)*ventanaMundoParalela) + origenXVentanaMundoParalelo;
                  aux_z=((y/altoVentana-0.5)*ventanaMundoParalela)* altoVentana /anchoVentana- origenYVentanaMundoParalelo;
                  vCajones[ultCajon].Rx= aux_x;
                  vCajones[ultCajon].Rz= aux_z;
                  for(i=ultCajon-1; i>=0; i--)
                      if ( ((vCajones[i].Rz+vCajones[i].lz)> aux_z) && ((vCajones[i].Rz-vCajones[i].lz)<aux_z) &&
                           ((vCajones[i].Rx+vCajones[i].lx)> aux_x) && ((vCajones[i].Rx-vCajones[i].lx)<aux_x) &&
                           (vCajones[i].Ry>=vCajones[ultCajon].Ry) )
                      {
                            vCajones[ultCajon].Ry=vCajones[ultCajon].ly+vCajones[i].Ry;

                      }

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
          if(status == GLUT_DOWN && cajonGrua.id==-1)
          {
              /* Obtener el indice de la caja seleccionada */
              cajaSeleccionada=pick(x,y);

              if(cajaSeleccionada>-1 && cajaSeleccionada<MaxCajones)
              {
                  /* Comprobar que no hay cajas encima de la caja seleccionada */
                  encima=0;
                  aux_x=vCajones[cajaSeleccionada].Rx;
                  aux_z=vCajones[cajaSeleccionada].Rz;
                  for(i=ultCajon;i>=0 && !encima; i--)
                      if ( i!=cajaSeleccionada &&
                           ((vCajones[i].Rz+vCajones[i].lz)> aux_z) && ((vCajones[i].Rz-vCajones[i].lz)<aux_z) &&
                           ((vCajones[i].Rx+vCajones[i].lx)> aux_x) && ((vCajones[i].Rx-vCajones[i].lx)<aux_x) &&
                           (vCajones[i].Ry>vCajones[cajaSeleccionada].Ry))
                          encima=1;
                  /*Si no hay cajas encima se pasa a calcular la distancia de la caja a la pluma */
                  if(!encima)
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
                          cajonGrua.angY=grua.angY+vCajones[cajaSeleccionada].angY;
                      }
                  }
              }
          }
          break;

      case soltando:
         /* Desenganchar la caja si se encuentra alguan enganchada
         En entradaMenu se llama a sotarCaja(), ya que despues de seleccionar
         la opción no es necesario clicar en el escenario. */
         break;
      case moviendoCaja:
          if(status == GLUT_DOWN){
              moverPermit=1;
              cajaSeleccionada=pick(x,y);
              xRef=x;
              yRef=y;
              aux_x=vCajones[cajaSeleccionada].Rx;
              aux_z=vCajones[cajaSeleccionada].Rz;
              for(i=ultCajon;i>=0 && moverPermit && cajaSeleccionada!=-1; i--)
                  if ( i!=cajaSeleccionada &&
                       ((vCajones[i].Rz+vCajones[i].lz)> aux_z) && ((vCajones[i].Rz-vCajones[i].lz)<aux_z) &&
                       ((vCajones[i].Rx+vCajones[i].lx)> aux_x) && ((vCajones[i].Rx-vCajones[i].lx)<aux_x) &&
                       (vCajones[i].Ry>vCajones[cajaSeleccionada].Ry))
                      moverPermit=0;
          }

          break;
      case rotandoCaja:
          if(status == GLUT_DOWN){
              cajaSeleccionada=pick(x,y);
              xRef=x;
              yRef=y;
              if(cajaSeleccionada!=-1)
              {
                  oldColor=vCajones[cajaSeleccionada].color;
                  vCajones[cajaSeleccionada].color=violeta;
              }
          }
          else if(status == GLUT_UP && cajaSeleccionada!=-1)
          {
              vCajones[cajaSeleccionada].angY+=vCajones[cajaSeleccionada].angAux;
              vCajones[cajaSeleccionada].angAux=0;
              vCajones[cajaSeleccionada].color=oldColor;
          }
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
    int i;
   float aux_x, aux_z, aux_y;
   switch (estado)
   {
   case ponerCajas:
      if(leftPuls){
          aux_x=((x/anchoVentana-0.5)*ventanaMundoParalela) + origenXVentanaMundoParalelo;
          aux_z=((y/altoVentana-0.5)*ventanaMundoParalela)* altoVentana /anchoVentana- origenYVentanaMundoParalelo;
         vCajones[ultCajon].Rx= aux_x;
         vCajones[ultCajon].Rz= aux_z;
         vCajones[ultCajon].Ry=0;
         for(i=ultCajon-1;i>=0; i--)
             if ( ((vCajones[i].Rz+vCajones[i].lz)> aux_z) && ((vCajones[i].Rz-vCajones[i].lz)<aux_z) &&
                  ((vCajones[i].Rx+vCajones[i].lx)> aux_x) && ((vCajones[i].Rx-vCajones[i].lx)<aux_x) &&
                  (vCajones[i].Ry>=vCajones[ultCajon].Ry) )
             {
                 vCajones[ultCajon].Ry=vCajones[ultCajon].ly+vCajones[i].Ry;
             }
      }
      break;
   case moviendoCaja:
       if(cajaSeleccionada!=-1 && moverPermit)
       {
           aux_x=(vCajones[cajaSeleccionada].Rx+=(x-xRef)*0.25); //0.1 Evita que se mueva demasiado rapido
           aux_z=(vCajones[cajaSeleccionada].Rz+=(y-yRef)*0.25);
           vCajones[cajaSeleccionada].Ry=0;
           for(i=ultCajon;i>=0; i--)
               if ( i!=cajaSeleccionada &&
                    ((vCajones[i].Rz+vCajones[i].lz)> aux_z) && ((vCajones[i].Rz-vCajones[i].lz)<aux_z) &&
                    ((vCajones[i].Rx+vCajones[i].lx)> aux_x) && ((vCajones[i].Rx-vCajones[i].lx)<aux_x) &&
                    (vCajones[i].Ry>=vCajones[cajaSeleccionada].Ry) )
               {
                   vCajones[cajaSeleccionada].Ry=vCajones[cajaSeleccionada].ly+vCajones[i].Ry;
               }
           xRef=x;
           yRef=y;
       }
      break;

   case rotandoCaja:
      if(cajaSeleccionada!=-1)
      {
         vCajones[cajaSeleccionada].angAux=-1*((atan2( (y-yRef), (x-xRef)))*180/M_PI);
         glutSetCursor(0);
      }

      break;
   }
   glutPostRedisplay();
}
