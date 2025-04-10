#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"

int main(){
    char nomeArquivoCSV[30], nomeArquivoBinario[30];
    t_header cabecalho;
    t_data registro;
    char funcao;
    FILE *CSV;
    FILE *binario;


    scanf("%c", &funcao);

    if (funcao == '1'){
        scanf ("%s %s", nomeArquivoCSV, nomeArquivoBinario);

        CSV = fopen(nomeArquivoCSV, "r");
        if (!CSV){
            printf("Falha no processamento do arquivo.");
            return 0;
        }

        binario = fopen(nomeArquivoBinario, "wb");
        if (!binario){
            printf("Falha no processamento do arquivo.");
            return 0;
        }
        createTable(binario, CSV);
        binarioNaTela(nomeArquivoBinario);
    }

    else if (funcao == '2'){
        scanf("%s", nomeArquivoBinario);
        FILE *binario = fopen(nomeArquivoBinario, "rb");
        if (!binario){
            printf("Falha no processamento do arquivo.");
            return 0;
        }
        //lerCabecalhoBIN(binario, &cabecalho);
        //lerRegistroBIN(binario, &registro);
        selectSQL(binario, &cabecalho, &registro);
        fclose(binario);
    }

    else if (funcao == '3'){

    }

    //compararArquivosHex(nomeArquivoBinario, "ataque2.bin");



    return 0;
}
