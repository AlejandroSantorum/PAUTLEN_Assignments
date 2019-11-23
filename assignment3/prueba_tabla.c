#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol_table.h"

#define BUFF_SZ 256


int main(int argc, char **argv){
    FILE *fin=NULL, *fout=NULL;
    char buff[BUFF_SZ];

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

    while(fgets(buff, BUFF_SZ, fin)){
        printf("%s", buff);
        memset(buff, 0, BUFF_SZ);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}
