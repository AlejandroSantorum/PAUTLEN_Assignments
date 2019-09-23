#include "generacion.h"

void escribir_cabecera_bss(FILE* fpasm){
    fprintf(fpasm, "segment .bss\n");
    fprintf(fpasm, "__esp resd 1\n");
}

void escribir_subseccion_data(FILE* fpasm){
    fprintf(fpasm, "segment .data\n");
    fprintf(fpasm, "div-zero db \"Dividing by 0\",0", );
}
