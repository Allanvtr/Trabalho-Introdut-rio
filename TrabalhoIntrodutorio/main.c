#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"



// 14712657 - Allan Vitor de Souza Silva



int main(){
    char nomeArquivoCSV[30], nomeArquivoBinario[30];
    t_header cabecalho;
    t_data registro;
    char funcao;
    FILE *CSV;
    FILE *binario;


    scanf("%c", &funcao);

    // Create Table
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


    // Select From
    else if (funcao == '2'){
        scanf("%s", nomeArquivoBinario);

        FILE *binario = fopen(nomeArquivoBinario, "rb");
        if (!binario){
            printf("Falha no processamento do arquivo.");
            return 0;
        }

        selectFrom(binario, &cabecalho, &registro);
        fclose(binario);
    }


    // Select from where
    else if (funcao == '3') {
        int quant;
        scanf("%s %d", nomeArquivoBinario, &quant);
        FILE *binario = fopen(nomeArquivoBinario, "rb");
        if (!binario) {
            printf("Falha no processamento do arquivo.");
            return 0;
        }

        consultasWhere(binario, quant, &cabecalho, &registro);
        fclose(binario);
    }



    return 0;
}
