#include "generacion.h"

void escribir_cabecera_bss(FILE* fpasm){
    fprintf(fpasm, "segment .bss\n");
    fprintf(fpasm, "__esp resd 1\n");
}

void escribir_subseccion_data(FILE* fpasm){
    //TODO: Considerar errores de libreria
    fprintf(fpasm, "segment .data\n");
    fprintf(fpasm, "div_zero db \"Dividing by 0\",0\n");
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
    fprintf(fpasm, "mov [__esp], esp\n");
}

void escribir_fin(FILE* fpasm){
    fprintf(fpasm, "div_zero_f:\n");
    fprintf(fpasm, "push dword div_zero\n");
    fprintf(fpasm, "call print_string\n");
    fprintf(fpasm, "add esp, 4\n");
    fprintf(fpasm, "jmp fin_f\n");
    fprintf(fpasm, "fin_f:\n");
    fprintf(fpasm, "mov esp, [__esp]\n");
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
        fprintf(fpasm, "pop dword [_%s] \n", nombre);
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
    fprintf(fpasm, "je div_zero_f\n");
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
    fprintf(fpasm, "pop dword ebx\n");
    if (es_variable) fprintf(fpasm, "mov ebx, [ebx]\n");
    fprintf(fpasm, "cmp ebx, 0\n");
    fprintf(fpasm, "je escribe_true_%d\n", cuantos_no);
    fprintf(fpasm, "push dword 0\n");
    fprintf(fpasm, "jmp fin_%d\n", cuantos_no);
    fprintf(fpasm, "escribe_true_%d:", cuantos_no);
    fprintf(fpasm, "push dword 1\n");
    fprintf(fpasm, "fin_%d:\n", cuantos_no);
}

void igual(FILE* fpasm, int es_variable1, int es_variable2, int etiqueta){
    _pop_valores_reg(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "cmp eax, ebx\n");
    fprintf(fpasm, "je igual_%d\n", etiqueta);
    fprintf(fpasm, "push dword 0\n");
    fprintf(fpasm, "jmp igual_fin_%d", etiqueta);
    fprintf(fpasm, "igual_%d:\n", etiqueta);
    fprintf(fpasm, "push dword 1\n");
    fprintf(fpasm, "igual_fin_%d:", etiqueta);
}

void distinto(FILE* fpasm, int es_variable1, int es_variable2, int etiqueta){
    _pop_valores_reg(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "cmp eax, ebx\n");
    fprintf(fpasm, "jne distinto_%d\n", etiqueta);
    fprintf(fpasm, "push dword 0\n");
    fprintf(fpasm, "jmp distinto_fin_%d", etiqueta);
    fprintf(fpasm, "distinto_%d:\n", etiqueta);
    fprintf(fpasm, "push dword 1\n");
    fprintf(fpasm, "distinto_fin_%d:", etiqueta);
}

void menor_igual(FILE* fpasm, int es_variable1, int es_variable2, int etiqueta){
    _pop_valores_reg(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "cmp eax, ebx\n");
    fprintf(fpasm, "jle menor_igual_%d\n", etiqueta);
    fprintf(fpasm, "push dword 0\n");
    fprintf(fpasm, "jmp menor_igual_fin_%d", etiqueta);
    fprintf(fpasm, "menor_igual_%d:\n", etiqueta);
    fprintf(fpasm, "push dword 1\n");
    fprintf(fpasm, "menor_igual_fin_%d:", etiqueta);
}

void mayor_igual(FILE* fpasm, int es_variable1, int es_variable2, int etiqueta){
    _pop_valores_reg(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "cmp eax, ebx\n");
    fprintf(fpasm, "jge mayor_igual_%d\n", etiqueta);
    fprintf(fpasm, "push dword 0\n");
    fprintf(fpasm, "jmp mayor_igual_fin_%d", etiqueta);
    fprintf(fpasm, "mayor_igual_%d:\n", etiqueta);
    fprintf(fpasm, "push dword 1\n");
    fprintf(fpasm, "mayor_igual_fin_%d:", etiqueta);
}

void menor(FILE* fpasm, int es_variable1, int es_variable2, int etiqueta){
    _pop_valores_reg(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "cmp eax, ebx\n");
    fprintf(fpasm, "jl menor_%d\n", etiqueta);
    fprintf(fpasm, "push dword 0\n");
    fprintf(fpasm, "jmp menor_fin_%d", etiqueta);
    fprintf(fpasm, "menor_%d:\n", etiqueta);
    fprintf(fpasm, "push dword 1\n");
    fprintf(fpasm, "menor_fin_%d:", etiqueta);
}

void mayor(FILE* fpasm, int es_variable1, int es_variable2, int etiqueta){
    _pop_valores_reg(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "cmp eax, ebx\n");
    fprintf(fpasm, "jg mayor_%d\n", etiqueta);
    fprintf(fpasm, "push dword 0\n");
    fprintf(fpasm, "jmp mayor_fin_%d", etiqueta);
    fprintf(fpasm, "mayor_%d:\n", etiqueta);
    fprintf(fpasm, "push dword 1\n");
    fprintf(fpasm, "mayor_fin_%d:", etiqueta);
}

void leer(FILE* fpasm, char* nombre, int tipo){
    //TODO: saltar a error de lectura
    fprintf(fpasm, "push dword _%s\n", nombre);
    switch (tipo) {
        case ENTERO:    fprintf(fpasm, "call scan_int\n");
                        break;
        case BOOLEANO:  fprintf(fpasm, "call scan_boolean\n");
                        break;
        default:        break;
    }
    fprintf(fpasm, "add esp, 4\n");
}

void escribir(FILE* fpasm, int es_variable, int tipo){
    fprintf(fpasm, "pop dword eax\n");
    if (es_variable) fprintf(fpasm, "mov eax, [eax]\n");
    fprintf(fpasm, "push dword eax\n");
    switch (tipo) {
        case ENTERO:    fprintf(fpasm, "call print_int\n");
                        break;
        case BOOLEANO:  fprintf(fpasm, "call print_boolean\n");
                        break;
        default:        break;
    }
    fprintf(fpasm, "add esp, 4\n"); //TODO: Preguntar si esto está bien.
}
