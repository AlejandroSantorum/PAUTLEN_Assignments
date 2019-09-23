#include "generacion.h"

void escribir_cabecera_bss(FILE* fpasm){
    fprintf(fpasm, "segment .bss\n");
    fprintf(fpasm, "__esp resd 1\n");
}

void escribir_subseccion_data(FILE* fpasm){
    fprintf(fpasm, "segment .data\n");
    fprintf(fpasm, "div_zero db \"Dividing by 0\",0", );
}

void declarar_variable(FILE* fpasm, char * nombre,  int tipo,  int tamano){
    //TODO: tendremos que tener en cuenta el tipo
    fprintf(fpasm, "_%s resd %d\n", nombre, tamano);
}

void escribir_segmento_codigo(FILE* fpasm){
    fprintf(fpasm, "segment .text\n");
    fprintf(fpasm, "global main\n");
    fprintf(fpasm, "extern print_int, print_boolean, print_string, print_blank, ");
    fprintf(fpasm, "print_endofline, scan_int, scan_boolean\n");
}

void escribir_inicio_main(FILE* fpasm){
    fprintf(fpasm, "main:\n");
    fprintf(fpasm, "mov __esp esp\n");
}

void escribir_fin(FILE* fpasm){
    fprintf(fpasm, "div_zero_1:\n");
    fprintf(fpasm, "push dword div_zero\n");
    fprintf(fpasm, "call print_string\n");
    fprintf(fpasm, "add esp, 4\n");
    fprintf(fpasm, "jmp fin_2\n", );
    fprintf(fpasm, "fin_2:\n");
    fprintf(fpasm, "mov esp __esp\n");
    fprintf(fpasm, "ret\n");
}

void escribir_operando(FILE* fpasm, char* nombre, int es_variable){
    if (es_variable){
        fprintf(fpasm, "push dword _%s\n", nombre);
    }else{
        fprintf(fpasm, "push dword %s\n", nombre);
    }
}

void asignar(FILE* fpasm, char* nombre, int es_variable){
    if (es_variable) {
        fprintf(fpasm, "pop dword ebx\n");
        fprintf(fpasm, "mov _%s, [ebx]\n", nombre);
    }else{
        fprintf(fpasm, "pop dword _%s \n", nombre);
    }
}

void _pop_valores_reg(FILE* fpasm, int es_variable_1, int es_variable_2){
    fprintf(fpasm, "pop dword eax\n");
    if (es_variable_1) fprintf(fpasm, "mov eax, [eax]\n");
    fprintf(fpasm, "pop dword ebx\n");
    if (es_variable_2) fprintf(fpasm, "mov ebx, [ebx]\n");
}

void sumar(FILE* fpasm, int es_variable_1, int es_variable_2){
    _pop_valores_reg(fpasm, es_variable_1, es_variable_2);
    fprintf(fpasm, "add eax, ebx\n");
    fprintf(fpasm, "push dword eax\n");
}

void restar(FILE* fpasm, int es_variable_1, int es_variable_2){
    _pop_valores_reg(fpasm, es_variable_1, es_variable_2);
    fprintf(fpasm, "sub eax, ebx\n");
    fprintf(fpasm, "push dword eax\n");
}

void multiplicar(FILE* fpasm, int es_variable_1, int es_variable_2){
    _pop_valores_reg(fpasm, es_variable_1, es_variable_2);
    fprintf(fpasm, "imul eax, ebx\n");
    fprintf(fpasm, "push dword eax\n");
}

void dividir(FILE* fpasm, int es_variable_1, int es_variable_2){
    _pop_valores_reg(fpasm, es_variable_1, es_variable_2);
    fprintf(fpasm, "mov edx, 0\n");
    fprintf(fpasm, "cmp ebx, 0\n");
    fprintf(fpasm, "je div_zero_1\n");
    fprintf(fpasm, "idiv ebx\n");
    fprintf(fpasm, "push dword, eax\n");
}

void o(FILE* fpasm, int es_variable_1, int es_variable_2){
    _pop_valores_reg(fpasm, es_variable_1, es_variable_2);
    fprintf(fpasm, "or eax, ebx\n");
    fprintf(fpasm, "push dword eax\n");
}

void y(FILE* fpasm, int es_variable_1, int es_variable_2){
    _pop_valores_reg(fpasm, es_variable_1, es_variable_2);
    fprintf(fpasm, "and eax, ebx\n");
    fprintf(fpasm, "push dword eax\n");
}

void cambiar_signo(FILE* fpasm, int es_variable){
    fprintf(fpasm, "pop dword ebx\n");
    if (es_variable) fprintf(fpasm, "mov ebx, [ebx]\n");
    fprintf(fpasm, "neg ebx\n");
    fprintf(fpasm, "push dword ebx\n");
}

void no(FILE* fpasm, int es_variable, int cuantos_no){
    
}
