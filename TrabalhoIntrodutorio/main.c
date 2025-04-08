#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"

int main(){
    char nomeArquivoCSV[30], nomeArquivoBinario[30];

    scanf ("%s", nomeArquivoCSV);
    scanf ("%s", nomeArquivoBinario);

    createTable(nomeArquivoBinario, nomeArquivoCSV);

    //Abertura do arquivo
    /*FILE *fp = fopen(nomeArquivo, "r");
    if (!fp){
        printf("Falha no processamento do arquivo.");
        return 0;
    }


    //leitura do arquivo
    char linha[300];
    fscanf(fp, " %[^\n]", linha);


    char **tokens = scan_csv(linha, 7);*/
    /*for (int j = 0; j < 7; j++){
        printf("%s\n", tokens[j]);
    }*/


    // Inicialização
   /* inicializarCabecalho(&cabecalho, tokens);

    //printHeader(cabecalho);
    free(tokens);



    fgets(linha, sizeof(linha), fp);
   // while (fgets(linha, sizeof(linha), fp) != NULL) {
        // Remove o '\n' do final, se houver
        linha[strcspn(linha, "\n")] = '\0';

        tokens = scan_csv(linha, 7);

        inicializarDado(&dado, tokens);
        printRegistro(&dado);
        printf("\n\n");

        free(dado.country);
        free(dado.attackType);
        free(dado.defenseMechanism);
        free(dado.targetIndustry);
        free(tokens);
    //}



    fclose(fp);*/



    return 0;
}
