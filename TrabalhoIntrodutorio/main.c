#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"

int main(){
    char nomeArquivoCSV[30], nomeArquivoBinario[30];

    scanf ("%s", nomeArquivoCSV);
    scanf ("%s", nomeArquivoBinario);

    createTable(nomeArquivoBinario, nomeArquivoCSV);
    binarioNaTela(nomeArquivoBinario);
    //compararArquivosHex(nomeArquivoBinario, "ataque2.bin");

    return 0;
}
