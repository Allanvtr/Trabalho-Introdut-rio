#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"


//---------------- Funções para criação do arquivo binário ---------------------


void createTable(FILE *binario, FILE *CSV){
    t_header cabecalho;
    t_data dado;

    // Leitura do arquivo e escrita do cabeçalho
    char linha[300];
    fscanf(CSV, " %[^\n]", linha);


    // Leitura do arquivo CSV (Separa as colunas em tokens)
    char **tokens = scan_csv(linha, 7);


    // Inicialização e escrita do cabeçalho
    inicializarCabecalho(&cabecalho, tokens);
    escreveCabecalho(binario, &cabecalho);
    free(tokens);


    // Inicialização e escrita dos registros
    lerRegistroCSV(binario, CSV, &cabecalho, &dado);

    fclose(CSV);

    // Atualiza o cabeçalho com nro de Registros e ProxByteOffSet
    fseek(binario, 0, SEEK_END);
    cabecalho.proxByteOffset = ftell(binario);
    fseek(binario, 0, SEEK_SET);
    escreveCabecalho(binario, &cabecalho);
    fclose(binario);

    return;
}



void lerRegistroCSV(FILE *binario, FILE *CSV, t_header *cabecalho, t_data *dado){
    char linha[500];
    char **tokens;

    // Ignora a linha do cabeçalho
    fgets(linha, sizeof(linha), CSV);

    // Laço para leitura e escrita do registro
    while (fgets(linha, sizeof(linha), CSV) != NULL) {
        cabecalho->nroRegArq++;
        linha[strcspn(linha, "\n")] = '\0';

        tokens = scan_csv(linha, 7);

        // Cria os dados
        inicializarDado(dado, tokens);

        // Escreve no arquivo
        escreveRegistro(binario, cabecalho, dado);

        // Liberar memória alocada
        freeRegistro(dado);
        free(tokens);
    }

    return;
}


void escreveRegistro(FILE *binario, t_header *cabecalho, t_data *dado){
    char separador = '|';

    fwrite(&dado->removido, sizeof(char), 1, binario);
    fwrite(&dado->tamanhoRegistro, sizeof(int), 1, binario);
    fwrite(&dado->prox, sizeof(long long int), 1, binario);
    fwrite(&dado->idAttack, sizeof(int), 1, binario);
    fwrite(&dado->year, sizeof(int), 1, binario);
    fwrite(&dado->financialLoss, sizeof(float), 1, binario);

    if (strlen(dado->country) > 0) {
        fwrite(&cabecalho->codDescreveContry, sizeof(char), 1, binario);
        fwrite(dado->country, sizeof(char), strlen(dado->country), binario);
        fwrite(&separador, sizeof(char), 1, binario);
    }

    if (strlen(dado->attackType) > 0) {
        fwrite(&cabecalho->codDescreveType, sizeof(char), 1, binario);
        fwrite(dado->attackType, sizeof(char), strlen(dado->attackType), binario);
        fwrite(&separador, sizeof(char), 1, binario);
    }

    if (strlen(dado->targetIndustry) > 0) {
        fwrite(&cabecalho->codDescreveTargetIndustry, sizeof(char), 1, binario);
        fwrite(dado->targetIndustry, sizeof(char), strlen(dado->targetIndustry), binario);
        fwrite(&separador, sizeof(char), 1, binario);
    }

    if (strlen(dado->defenseMechanism) > 0) {
        fwrite(&cabecalho->codDescreveDefense, sizeof(char), 1, binario);
        fwrite(dado->defenseMechanism, sizeof(char), strlen(dado->defenseMechanism), binario);
        fwrite(&separador, sizeof(char), 1, binario);
    }

    return;
}



void escreveCabecalho(FILE *binario, t_header *cabecalho){
    fwrite(&cabecalho->status, sizeof(char), 1, binario);
    fwrite(&cabecalho->topo, sizeof(long long int), 1, binario);
    fwrite(&cabecalho->proxByteOffset, sizeof(long long int), 1, binario);
    fwrite(&cabecalho->nroRegArq, sizeof(int), 1, binario);
    fwrite(&cabecalho->nroRegRem, sizeof(int), 1, binario);
    fwrite(&cabecalho->descreveIdentificador, sizeof(char), strlen(cabecalho->descreveIdentificador), binario);
    fwrite(&cabecalho->descreveYear, sizeof(char), strlen(cabecalho->descreveYear), binario);
    fwrite(&cabecalho->descreveFinancialLoss, sizeof(char), strlen(cabecalho->descreveFinancialLoss), binario);
    fwrite(&cabecalho->codDescreveContry, sizeof(char), 1, binario);
    fwrite(&cabecalho->descreveCountry, sizeof(char), strlen(cabecalho->descreveCountry), binario);
    fwrite(&cabecalho->codDescreveType, sizeof(char), 1, binario);
    fwrite(&cabecalho->descreveType, sizeof(char), strlen(cabecalho->descreveType), binario);
    fwrite(&cabecalho->codDescreveTargetIndustry, sizeof(char), 1, binario);
    fwrite(&cabecalho->descreveTargetIndustry, sizeof(char), strlen(cabecalho->descreveTargetIndustry), binario);
    fwrite(&cabecalho->codDescreveDefense, sizeof(char), 1, binario);
    fwrite(&cabecalho->descreveDefense, sizeof(char), strlen(cabecalho->descreveDefense), binario);

    return;
}















//---------------------------- Funções para o select -----------------------------

void selectSQL(FILE *binario, t_header *cabecalho, t_data *registro){
    lerCabecalhoBIN(binario, cabecalho);

    for(int i = 0; i < cabecalho->nroRegArq; i++){
        lerRegistroBIN(binario, registro);
        printRegistro(cabecalho, registro);

        freeRegistro(registro);
    }
}



void freeRegistro(t_data *registro){
    free(registro->country);
    free(registro->attackType);
    free(registro->defenseMechanism);
    free(registro->targetIndustry);

    return;
}




void lerRegistroBIN(FILE *binario, t_data *registro) {
    // Lendo campos de tamanho fixo
    fread(&registro->removido, sizeof(char), 1, binario);
    fread(&registro->tamanhoRegistro, sizeof(int), 1, binario);
    fread(&registro->prox, sizeof(long long int), 1, binario);
    fread(&registro->idAttack, sizeof(int), 1, binario);
    fread(&registro->year, sizeof(int), 1, binario);
    fread(&registro->financialLoss, sizeof(float), 1, binario);

    //Inicializando campos variados
    registro->country = NULL;
    registro->defenseMechanism = NULL;
    registro->targetIndustry = NULL;
    registro->attackType = NULL;

    // Lendo campo de tamanho variável
    char cod;
    int tam;

    // Laço para leitura dos campos
    for (int i = 0; i < 4; i++){
        fread(&cod, sizeof(char), 1, binario); // código do campo
        switch (cod) {
            case '1': {
                tam = calcTamanhoCampo(binario);
                fseek(binario, -(tam + 1), SEEK_CUR); // volta pro início da string

                // Alocação e leitura
                registro->country = (char*)malloc((tam + 1) * sizeof(char));
                fread(registro->country, sizeof(char), tam, binario);
                registro->country[tam] = '\0';

                // Prepara o ponteiro para a próxima leitura
                fseek(binario, 1, SEEK_CUR);

                break;
            }

            case '2':
                tam = calcTamanhoCampo(binario);
                fseek(binario, -(tam + 1), SEEK_CUR);

                registro->attackType = (char*)malloc((tam + 1) * sizeof(char));
                fread(registro->attackType, sizeof(char), tam, binario);
                registro->attackType[tam] = '\0';
                fseek(binario, 1, SEEK_CUR);

                break;

            case '3':
                tam = calcTamanhoCampo(binario);
                fseek(binario, -(tam + 1), SEEK_CUR);

                registro->targetIndustry = (char*)malloc((tam + 1) * sizeof(char));
                fread(registro->targetIndustry, sizeof(char), tam, binario);
                registro->targetIndustry[tam] = '\0';
                fseek(binario, 1, SEEK_CUR);

                break;

            case '4':
                tam = calcTamanhoCampo(binario);
                fseek(binario, -(tam + 1), SEEK_CUR);

                registro->defenseMechanism = (char*)malloc((tam + 1) * sizeof(char));
                fread(registro->defenseMechanism, sizeof(char), tam, binario);
                registro->defenseMechanism[tam] = '\0';
                fseek(binario, 1, SEEK_CUR);

                break;

            default:
                // Se chegou em default, significa que tem algum campo vazio e estamos no final do registro
                // Encerramos o loop e voltamos o ponteiro para a leitura do próximo reg
                i = 4;
                fseek(binario, -1, SEEK_CUR);
        }
    }


    return;
}

void lerCabecalhoBIN(FILE *binario, t_header *cabecalho) {

    // Leitura dos campos fixos
    fread(&cabecalho->status, sizeof(char), 1, binario);
    fread(&cabecalho->topo, sizeof(long long int), 1, binario);
    fread(&cabecalho->proxByteOffset, sizeof(long long int), 1, binario);
    fread(&cabecalho->nroRegArq, sizeof(int), 1, binario);
    fread(&cabecalho->nroRegRem, sizeof(int), 1, binario);

    // Leitura dos campos de string (você pode ajustar o tamanho conforme seus dados)
    fread(cabecalho->descreveIdentificador, sizeof(char), 23, binario);
    cabecalho->descreveIdentificador[23] = '\0';

    fread(cabecalho->descreveYear, sizeof(char), 27, binario);
    cabecalho->descreveYear[27] = '\0';

    fread(cabecalho->descreveFinancialLoss, sizeof(char), 28, binario);
    cabecalho->descreveFinancialLoss[28] = '\0';

    fread(&cabecalho->codDescreveContry, sizeof(char), 1, binario);
    fread(cabecalho->descreveCountry, sizeof(char), 26, binario);
    cabecalho->descreveCountry[26] = '\0';

    fread(&cabecalho->codDescreveType, sizeof(char), 1, binario);
    fread(cabecalho->descreveType, sizeof(char), 38, binario);
    cabecalho->descreveType[38] = '\0';

    fread(&cabecalho->codDescreveTargetIndustry, sizeof(char), 1, binario);
    fread(cabecalho->descreveTargetIndustry, sizeof(char), 38, binario);
    cabecalho->descreveTargetIndustry[38] = '\0';

    fread(&cabecalho->codDescreveDefense, sizeof(char), 1, binario);
    fread(cabecalho->descreveDefense, sizeof(char), 67, binario);
    cabecalho->descreveDefense[67] = '\0';

    // Print dos campos
    /*printf("Status: %c\n", cabecalho->status);
    printf("Topo: %lld\n", cabecalho->topo);
    printf("ProxByteOffset: %lld\n", cabecalho->proxByteOffset);
    printf("NroRegArq: %d\n", cabecalho->nroRegArq);
    printf("NroRegRem: %d\n", cabecalho->nroRegRem);

    printf("Descreve Identificador: %s\n", cabecalho->descreveIdentificador);
    printf("Descreve Year: %s\n", cabecalho->descreveYear);
    printf("Descreve Financial Loss: %s\n", cabecalho->descreveFinancialLoss);
    printf("Cod Country: %c\n", cabecalho->codDescreveContry);
    printf("Descreve Country: %s\n", cabecalho->descreveCountry);
    printf("Cod Type: %c\n", cabecalho->codDescreveType);
    printf("Descreve Type: %s\n", cabecalho->descreveType);
    printf("Cod Target Industry: %c\n", cabecalho->codDescreveTargetIndustry);
    printf("Descreve Target Industry: %s\n", cabecalho->descreveTargetIndustry);
    printf("Cod Defense: %c\n", cabecalho->codDescreveDefense);
    printf("Descreve Defense: %s\n", cabecalho->descreveDefense);*/

    return;
}



void printRegistro(t_header *cabecalho, t_data *registro){

    printf("%s: ", cabecalho->descreveIdentificador);
    printf("%d\n", registro->idAttack);

    printf("%s: ", cabecalho->descreveYear);
    if (registro->year == -1) printf("NADA CONSTA\n");
    else printf("%d\n", registro->year);

    printf("%s: ", cabecalho->descreveCountry);
    if (registro->country == NULL) printf("NADA CONSTA\n");
    else printf("%s\n", registro->country);

    printf("%s: ", cabecalho->descreveTargetIndustry);
    if (registro->targetIndustry == NULL) printf("NADA CONSTA\n");
    else printf("%s\n", registro->targetIndustry);

    printf("%s: ", cabecalho->descreveType);
    if (registro->attackType == NULL) printf("NADA CONSTA\n");
    else printf("%s\n", registro->attackType);

    printf("%s: ", cabecalho->descreveFinancialLoss);
    if (registro->financialLoss == -1) printf("NADA CONSTA\n");
    else printf("%.2f\n", registro->financialLoss);

    printf("%s: ", cabecalho->descreveDefense);
    if (registro->defenseMechanism == NULL) printf("NADA CONSTA\n");
    else printf("%s\n", registro->defenseMechanism);

    printf("\n");

    return;
}


















//---------------- Funções de inicialização dos dados ---------------------

void inicializarCabecalho (t_header *cabecalho, char **tokens){
    cabecalho->status = '1';
    cabecalho->topo = -1;
    cabecalho->proxByteOffset = 0;//recebe o byte offset o final do arquivo
    cabecalho->nroRegArq = 0;
    cabecalho->nroRegRem = 0;
    cabecalho->codDescreveContry = '1';
    cabecalho->codDescreveType = '2';
    cabecalho->codDescreveTargetIndustry = '3';
    cabecalho->codDescreveDefense = '4';

    strcpy(cabecalho->descreveIdentificador, tokens[0]);
    strcpy(cabecalho->descreveYear, tokens[1]);
    strcpy(cabecalho->descreveFinancialLoss, tokens[2]);
    strcpy(cabecalho->descreveCountry, tokens[3]);
    strcpy(cabecalho->descreveType, tokens[4]);
    strcpy(cabecalho->descreveTargetIndustry, tokens[5]);
    strcpy(cabecalho->descreveDefense, tokens[6]);

    return;
}

void inicializarDado(t_data *dado, char **tokens){
    dado->tamanhoRegistro = 0;

    // Tamanho dos campos fixos
    dado->idAttack = atoi(tokens[0]);
    dado->tamanhoRegistro += sizeof(dado->idAttack);

    if (atoi(tokens[1]) > 0) dado->year = atoi(tokens[1]);
    else dado->year = -1;
    dado->tamanhoRegistro += sizeof(dado->year);

    if (strtof(tokens[2], NULL) > 0) dado->financialLoss = strtof(tokens[2], NULL);
    else dado->financialLoss = -1;
    dado->tamanhoRegistro += sizeof(dado->financialLoss);

    dado->prox = -1;
    dado->tamanhoRegistro += sizeof(dado->prox);


    // Campos variáveis
    dado->country = (char*)malloc((strlen(tokens[3]) + 1) * sizeof(char));
    strcpy(dado->country, tokens[3]);
    if (strlen(dado->country) > 0) {
        dado->tamanhoRegistro += 1; // indicador
        dado->tamanhoRegistro += strlen(dado->country);
        dado->tamanhoRegistro += 1; // separador
    }

    dado->attackType = (char*)malloc((strlen(tokens[4]) + 1) * sizeof(char));
    strcpy(dado->attackType, tokens[4]);
    if (strlen(dado->attackType) > 0) {
        dado->tamanhoRegistro += 1;
        dado->tamanhoRegistro += strlen(dado->attackType);
        dado->tamanhoRegistro += 1;
    }

    dado->targetIndustry = (char*)malloc((strlen(tokens[5]) + 1) * sizeof(char));
    strcpy(dado->targetIndustry, tokens[5]);
    if (strlen(dado->targetIndustry) > 0) {
        dado->tamanhoRegistro += 1;
        dado->tamanhoRegistro += strlen(dado->targetIndustry);
        dado->tamanhoRegistro += 1;
    }

    dado->defenseMechanism = (char*)malloc((strlen(tokens[6]) + 1) * sizeof(char));
    strcpy(dado->defenseMechanism, tokens[6]);
    if (strlen(dado->defenseMechanism) > 0) {
        dado->tamanhoRegistro += 1;
        dado->tamanhoRegistro += strlen(dado->defenseMechanism);
        dado->tamanhoRegistro += 1;
    }

    dado->removido = '0';

    return;
}



















//---------------- Funções auxiliares ---------------------


void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}




char **scan_csv(char *linha, int tam) {

    // Aloca um vetor de ponteiros para string
    char **tokens = (char **)malloc(tam * sizeof(char *));
    if (!tokens) {
        printf("Erro ao alocar memória.\n");
        return NULL;
    }

    // Separa as strings pela vírgula
    int i = 0;
    char *ptr = linha;
    char *inicio = linha;
    while (*ptr) {
        if (*ptr == ',') {
            *ptr = '\0';
            tokens[i++] = inicio;
            inicio = ptr + 1;
        }
        ptr++;
    }
    tokens[i++] = inicio;

    return tokens;
}





int calcTamanhoCampo(FILE *binario) {
    int tam = 0;
    char aux;

    while (1) {
        fread(&aux, sizeof(char), 1, binario);
        if (aux == '|') break;
        tam++;
    }

    return tam;
}




//------------- Funções para debug (apagar depois)--------------


// Função que compara byte a byte o arquivo gerado e o arquivo teste
void compararArquivosHex(const char *arq1, const char *arq2) {
    FILE *f1 = fopen(arq1, "rb");
    FILE *f2 = fopen(arq2, "rb");

    if (!f1 || !f2) {
        perror("Erro ao abrir arquivos");
        return;
    }

    unsigned char buffer1[16], buffer2[16];
    size_t bytesLidos1, bytesLidos2;
    long pos = 0;
    int diferencas = 0;

    while (1) {
        bytesLidos1 = fread(buffer1, 1, 16, f1);
        bytesLidos2 = fread(buffer2, 1, 16, f2);

        if (bytesLidos1 == 0 && bytesLidos2 == 0)
            break;

        int diferente = 0;
        for (size_t i = 0; i < 16; i++) {
            if (i >= bytesLidos1 || i >= bytesLidos2) break;
            if (buffer1[i] != buffer2[i]) {
                diferente = 1;
                break;
            }
        }

        if (diferente) {
            printf("%08lX\n", pos);
            diferencas++;
        }

        pos += 16;
    }

    if (diferencas == 0) {
        printf("Arquivos são idênticos!\n");
    } else {
        printf("Total de blocos diferentes: %d\n", diferencas);
    }

    fclose(f1);
    fclose(f2);
}


