#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol_table.h"

#define BUFF_SZ 256

int print_succ_insert(FILE *fout, char *id_str);

int print_fail_insert(FILE *fout, char *id_str);

int print_succ_search(FILE *fout, char *id_str, int value);

int print_fail_search(FILE *fout, char *id_str);


int main(int argc, char **argv){
    FILE *fin=NULL, *fout=NULL;
    char buff[BUFF_SZ];
    char id_str[BUFF_SZ];
    int num, value;
    symbol_tb *st=NULL;

    if(argc < 3){
        printf("Insufficient input parameters:\n\tUsage: ./%s input_file_name output_file_name\n", __FILE__);
        return -1;
    }

    fin = fopen(argv[1], "r");
    if(!fin){
        printf("File Error: Unable to open %s file\n", argv[1]);
        return -1;
    }

    fout = fopen(argv[2], "w");
    if(!fout){
        printf("File Error: Unable to open %s file\n", argv[2]);
        fclose(fin);
        return -1;
    }

    st = symb_tb_create();
    if(!st){
        printf("Symbol table Error: Unable to allocate memory for symbol table\n");
        return -1;
    }

    while(fgets(buff, BUFF_SZ, fin)){
        memset(id_str, 0, BUFF_SZ);
        if(strchr(buff, ' ')){
            sscanf(buff, "%s %d\n", id_str, &num);
            if(symb_tb_insert(st, id_str, num) == 0){
                print_succ_insert(fout, id_str);
            }
            else{
                print_fail_insert(fout, id_str);
            }
        }
        else{
            sscanf(buff, "%s\n", id_str);
            if(symb_tb_search(st, id_str, &value) == 0){
                print_succ_search(fout, id_str, value);
            }
            else{
                print_fail_search(fout, id_str);
            }
        }
        memset(buff, 0, BUFF_SZ);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}


int print_succ_insert(FILE *fout, char *id_str){
    if(!fout || !id_str) return -1;
    return fprintf(fout, "%s\n", id_str);
}


int print_fail_insert(FILE *fout, char *id_str){
    if(!fout || !id_str) return -1;
    return fprintf(fout, "-1 %s\n", id_str);
}


int print_succ_search(FILE *fout, char *id_str, int value){
    if(!fout || !id_str) return -1;
    return fprintf(fout, "%s %d\n", id_str, value);
}


int print_fail_search(FILE *fout, char *id_str){
    if(!fout || !id_str) return -1;
    return fprintf(fout, "%s -1\n", id_str);
}
