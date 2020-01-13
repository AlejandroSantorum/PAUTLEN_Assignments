#include "generacion.h"

void escribir_cabecera_bss(FILE* fpasm){
    fprintf(fpasm, "segment .bss\n");
    fprintf(fpasm, "__esp resd 1\n");
}

void escribir_subseccion_data(FILE* fpasm){
    //TODO: Considerar errores de libreria
    fprintf(fpasm, "segment .data\n");
    // Byte 10 represents end of line in asm
    fprintf(fpasm, "div_zero db \"Dividing by 0\",10,0\n");
    fprintf(fpasm, "out_of_range db \"Accessing a memory out of range\",10,0\n");
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
    fprintf(fpasm, "fin_f:\n");
    fprintf(fpasm, "mov esp, [__esp]\n");
    fprintf(fpasm, "ret\n");

    fprintf(fpasm, "div_zero_f:\n");
    fprintf(fpasm, "push dword div_zero\n");
    fprintf(fpasm, "call print_string\n");
    fprintf(fpasm, "add esp, 4\n");
    fprintf(fpasm, "jmp fin_f\n");

    fprintf(fpasm, "out_of_range_f:\n");
    fprintf(fpasm, "push dword out_of_range\n");
    fprintf(fpasm, "call print_string\n");
    fprintf(fpasm, "add esp, 4\n");
    fprintf(fpasm, "jmp fin_f\n");
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
    fprintf(fpasm, "pop dword ebx\n");
    fprintf(fpasm, "pop dword eax\n");
    if (es_variable_1) fprintf(fpasm, "mov eax, [eax]\n");
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
    fprintf(fpasm, "cdq\n");
    fprintf(fpasm, "cmp ebx, 0\n");
    fprintf(fpasm, "je div_zero_f\n");
    fprintf(fpasm, "idiv ebx\n");
    fprintf(fpasm, "push dword eax\n");
}

void modulo(FILE* fpasm, int es_variable_1, int es_variable_2){
    _pop_valores_reg(fpasm, es_variable_1, es_variable_2);
    fprintf(fpasm, "cdq\n");
    fprintf(fpasm, "cmp ebx, 0\n");
    fprintf(fpasm, "je div_zero_f\n");
    fprintf(fpasm, "idiv ebx\n");
    fprintf(fpasm, "push dword edx\n");
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
    fprintf(fpasm, "escribe_true_%d:\n", cuantos_no);
    fprintf(fpasm, "push dword 1\n");
    fprintf(fpasm, "fin_%d:\n", cuantos_no);
}

void igual(FILE* fpasm, int es_variable1, int es_variable2, int etiqueta){
    _pop_valores_reg(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "cmp eax, ebx\n");
    fprintf(fpasm, "je igual_%d\n", etiqueta);
    fprintf(fpasm, "push dword 0\n");
    fprintf(fpasm, "jmp igual_fin_%d\n", etiqueta);
    fprintf(fpasm, "igual_%d:\n", etiqueta);
    fprintf(fpasm, "push dword 1\n");
    fprintf(fpasm, "igual_fin_%d:\n", etiqueta);
}

void distinto(FILE* fpasm, int es_variable1, int es_variable2, int etiqueta){
    _pop_valores_reg(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "cmp eax, ebx\n");
    fprintf(fpasm, "jne distinto_%d\n", etiqueta);
    fprintf(fpasm, "push dword 0\n");
    fprintf(fpasm, "jmp distinto_fin_%d\n", etiqueta);
    fprintf(fpasm, "distinto_%d:\n", etiqueta);
    fprintf(fpasm, "push dword 1\n");
    fprintf(fpasm, "distinto_fin_%d:\n", etiqueta);
}

void menor_igual(FILE* fpasm, int es_variable1, int es_variable2, int etiqueta){
    _pop_valores_reg(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "cmp eax, ebx\n");
    fprintf(fpasm, "jle menor_igual_%d\n", etiqueta);
    fprintf(fpasm, "push dword 0\n");
    fprintf(fpasm, "jmp menor_igual_fin_%d\n", etiqueta);
    fprintf(fpasm, "menor_igual_%d:\n", etiqueta);
    fprintf(fpasm, "push dword 1\n");
    fprintf(fpasm, "menor_igual_fin_%d:\n", etiqueta);
}

void mayor_igual(FILE* fpasm, int es_variable1, int es_variable2, int etiqueta){
    _pop_valores_reg(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "cmp eax, ebx\n");
    fprintf(fpasm, "jge mayor_igual_%d\n", etiqueta);
    fprintf(fpasm, "push dword 0\n");
    fprintf(fpasm, "jmp mayor_igual_fin_%d\n", etiqueta);
    fprintf(fpasm, "mayor_igual_%d:\n", etiqueta);
    fprintf(fpasm, "push dword 1\n");
    fprintf(fpasm, "mayor_igual_fin_%d:\n", etiqueta);
}

void menor(FILE* fpasm, int es_variable1, int es_variable2, int etiqueta){
    _pop_valores_reg(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "cmp eax, ebx\n");
    fprintf(fpasm, "jl menor_%d\n", etiqueta);
    fprintf(fpasm, "push dword 0\n");
    fprintf(fpasm, "jmp menor_fin_%d\n", etiqueta);
    fprintf(fpasm, "menor_%d:\n", etiqueta);
    fprintf(fpasm, "push dword 1\n");
    fprintf(fpasm, "menor_fin_%d:\n", etiqueta);
}

void mayor(FILE* fpasm, int es_variable1, int es_variable2, int etiqueta){
    _pop_valores_reg(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "cmp eax, ebx\n");
    fprintf(fpasm, "jg mayor_%d\n", etiqueta);
    fprintf(fpasm, "push dword 0\n");
    fprintf(fpasm, "jmp mayor_fin_%d\n", etiqueta);
    fprintf(fpasm, "mayor_%d:\n", etiqueta);
    fprintf(fpasm, "push dword 1\n");
    fprintf(fpasm, "mayor_fin_%d:\n", etiqueta);
}

void leer(FILE* fpasm, char* nombre, int tipo){
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
    fprintf(fpasm, "call print_endofline\n");
}

void suma_iterativa(FILE *fpasm, char *nombre1, char *nombre2){
    leer(fpasm, nombre1, ENTERO);
    leer(fpasm, nombre2, ENTERO);
    fprintf(fpasm, "mov ebx, [_%s]\n", nombre2);
    fprintf(fpasm, "cmp ebx, 0\n");
    fprintf(fpasm, "jne sigue_sumando\n");
    fprintf(fpasm, "fin_suma_iterativa:\n");
    fprintf(fpasm, "ret\n");
    fprintf(fpasm, "sigue_sumando:\n");
    fprintf(fpasm, "mov eax, [_%s]\n", nombre1);
    fprintf(fpasm, "add ebx, eax\n");
    fprintf(fpasm, "push dword ebx\n");
    fprintf(fpasm, "call print_int\n");
    fprintf(fpasm, "add esp, 4\n");
    fprintf(fpasm, "call print_endofline\n");
    leer(fpasm, nombre1, ENTERO);
    fprintf(fpasm, "mov eax, [_%s]\n", nombre1);
    fprintf(fpasm, "cmp eax, 0\n");
    fprintf(fpasm, "je fin_suma_iterativa\n");
    fprintf(fpasm, "jmp sigue_sumando\n");
}

void ifthenelse_inicio(FILE * fpasm, int exp_es_variable, int etiqueta) {
    fprintf(fpasm, "pop dword eax\n");
    if (exp_es_variable == 1)
        fprintf(fpasm, "mov eax, [eax]\n");
    fprintf(fpasm, "cmp eax, 0\n");
    fprintf(fpasm, "je fin_then_else_%d\n", etiqueta);
}

void ifthen_inicio(FILE * fpasm, int exp_es_variable, int etiqueta) {
    fprintf(fpasm, "pop dword eax\n");
    if (exp_es_variable)
        fprintf(fpasm, "mov eax, [eax]\n");
    fprintf(fpasm, "cmp eax, 0\n");
    fprintf(fpasm, "je fin_then_%d\n", etiqueta);
}

void ifthen_fin(FILE * fpasm, int etiqueta) {
    fprintf(fpasm, "fin_then_%d:\n", etiqueta);
}

void ifthenelse_fin_then( FILE * fpasm, int etiqueta) {
    fprintf(fpasm, "jmp fin_else_%d\n", etiqueta);
    fprintf(fpasm, "fin_then_else_%d:\n", etiqueta);
}

void ifthenelse_fin( FILE * fpasm, int etiqueta) {
    fprintf(fpasm, "fin_else_%d:\n", etiqueta);
}

void while_inicio(FILE * fpasm, int etiqueta){
    fprintf(fpasm, "inicio_while_%d:\n", etiqueta);
}

void while_exp_pila (FILE * fpasm, int exp_es_variable, int etiqueta){
    fprintf(fpasm, "pop dword eax\n");
    if (exp_es_variable)
        fprintf(fpasm, "mov eax, [eax]\n");
    fprintf(fpasm, "cmp eax, 0\n");
    fprintf(fpasm, "je fin_while_%d\n", etiqueta);
}

void while_fin(FILE * fpasm, int etiqueta){
    fprintf(fpasm, "jmp inicio_while_%d\n", etiqueta);
    fprintf(fpasm, "fin_while_%d:\n", etiqueta);
}

void escribir_elemento_vector(FILE * fpasm,char * nombre_vector, int tam_max, int exp_es_direccion){
    fprintf(fpasm, "pop dword eax\n");
    if (exp_es_direccion)
        fprintf(fpasm, "mov eax, [eax]\n");
    fprintf(fpasm, "cmp eax, 0\n");
    fprintf(fpasm, "jl out_of_range_f\n");
    fprintf(fpasm, "cmp eax, %d-1\n", tam_max);
    fprintf(fpasm, "jg out_of_range_f\n");
    fprintf(fpasm, "mov edx, _%s\n", nombre_vector);
    fprintf(fpasm, "lea eax, [edx + eax*4]\n");
    fprintf(fpasm, "push dword eax\n");
}

void declararFuncion(FILE * fd_asm, char * nombre_funcion, int num_var_loc){
    fprintf(fd_asm, "_%s:\n", nombre_funcion);
    fprintf(fd_asm, "push ebp\n");
    fprintf(fd_asm, "mov ebp, esp\n");
    fprintf(fd_asm, "sub esp, 4*%d\n", num_var_loc);
}

void retornarFuncion(FILE * fd_asm, int es_variable){
    fprintf(fd_asm, "pop dword eax\n");
    if (es_variable)
        fprintf(fd_asm, "mov eax, [eax]\n");
    fprintf(fd_asm, "mov esp, ebp\n");
    fprintf(fd_asm, "pop dword ebp\n");
    fprintf(fd_asm, "ret\n");
}

void escribirParametro(FILE* fpasm, int pos_parametro, int num_total_parametros){
    int d_ebp;
    d_ebp = 4*(1 + (num_total_parametros - pos_parametro));
    fprintf(fpasm, "lea eax, [ebp + %d]\n", d_ebp);
    fprintf(fpasm, "push dword eax\n");
}

void escribirVariableLocal(FILE* fpasm, int posicion_variable_local){
    int d_ebp;
    d_ebp = 4*(1+posicion_variable_local);
    fprintf(fpasm, "lea eax, [ebp - %d]\n", d_ebp);
    fprintf(fpasm, "push dword eax\n");
}

void asignarDestinoEnPila(FILE* fpasm, int es_variable){
    fprintf(fpasm, "pop dword ebx\n");
    fprintf(fpasm, "pop dword eax\n");
    if (es_variable)
        fprintf(fpasm, "mov eax, [eax]\n");
    fprintf(fpasm, "mov dword [ebx], eax\n");
}

void operandoEnPilaAArgumento(FILE * fd_asm, int es_variable){
    if (es_variable){
        fprintf(fd_asm, "pop dword eax\n");
        fprintf(fd_asm, "mov eax, [eax]\n");
        fprintf(fd_asm, "push dword eax\n");
    }
}

void llamarFuncion(FILE * fd_asm, char * nombre_funcion, int num_argumentos){
    fprintf(fd_asm, "call _%s\n", nombre_funcion);
    limpiarPila(fd_asm, num_argumentos);
    fprintf(fd_asm, "push dword eax\n");
}

void limpiarPila(FILE * fd_asm, int num_argumentos){
    fprintf(fd_asm, "add esp, 4*%d\n", num_argumentos);
}
