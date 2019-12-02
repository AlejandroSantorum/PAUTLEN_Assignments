#include <stdio.h>
#include "../src/generacion.h"

#define MAX_ETIQUETAS 255

int main (int argc, char** argv)
{
  FILE * salida;
  int etiqueta = 0;
  int getiqueta = 0;
  int etiquetas[MAX_ETIQUETAS];
  int cima_etiquetas=-1;

  if (argc != 2) {
    fprintf (stdout, "ERROR POCOS ARGUMENTOS\n");
    return -1;
  }
  salida = fopen(argv[1],"w");
  escribir_subseccion_data(salida);
  escribir_cabecera_bss(salida);
  declarar_variable(salida, "b1", BOOLEANO, 1);
  declarar_variable(salida, "x", ENTERO, 1);
  escribir_segmento_codigo(salida);
  escribir_inicio_main(salida);
  /* b1 = true; */
  escribir_operando(salida, "1", 0);
  asignar(salida, "b1", 0);

  /* Incluye aqui el codigo necesario para
  while (b1) {
    scanf x;
    b1 = x > 4;
    if (b1) {
      printf x;
    }
    else {
      printf false;
    }
  }
  */
  // Gestión de etiquetas
  getiqueta++;
  cima_etiquetas++;
  etiquetas[cima_etiquetas]=getiqueta;

  // Escribimos el inicio del bucle while
  etiqueta = etiquetas[cima_etiquetas];
  while_inicio(salida, etiqueta);

  // Condición del bucle, escribimos b1 en la pila
  escribir_operando(salida, "b1", 1);

  //Obtenemos la etiqueta para imprimir la condición del bucle
  etiqueta = etiquetas[cima_etiquetas];
  while_exp_pila(salida, 1, etiqueta);

  // Contenido del bucle while
  // Leemos la variable
  leer(salida,"x",ENTERO);

  // b1 = x > 4
  escribir_operando(salida,"x",1);
  escribir_operando(salida,"4",0);
  mayor(salida,1,0,etiqueta);
  asignar(salida, "b1", 0);

  // Gestión de etiquetas del bloque if-then-else
  getiqueta++;
  cima_etiquetas++;
  etiquetas[cima_etiquetas]=getiqueta;

  // Condición del bloque if, escribimos b1 en la pila
  escribir_operando(salida, "b1", 1);

   //Obtenemos la etiqueta para imprimir la condición del bloque if
  etiqueta = etiquetas[cima_etiquetas];
  ifthenelse_inicio(salida, 1, etiqueta);

  escribir_operando(salida, "x", 1);
  escribir(salida, 1, ENTERO);

  // Empieza la rama else
  etiqueta = etiquetas[cima_etiquetas];
  ifthenelse_fin_then(salida, etiqueta);

  escribir_operando(salida, "0", 0);
  escribir(salida, 0, BOOLEANO);
  // Termina el bloque else
  etiqueta = etiquetas[cima_etiquetas];
  ifthenelse_fin(salida, etiqueta);
  cima_etiquetas--;

  //Obtenemos la etiqueta del bloque para imprimir el final del bucle
  etiqueta = etiquetas[cima_etiquetas];
  while_fin(salida, etiqueta);
  cima_etiquetas--;

  escribir_fin(salida);
  fclose(salida);
  return 0;
}
