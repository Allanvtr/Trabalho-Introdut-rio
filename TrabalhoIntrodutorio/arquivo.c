#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"

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
    //if (tokens[1][0] == '\0') printf("sim %d", dado->year + 1);

    if (strtof(tokens[2], NULL) > 0) dado->financialLoss = strtof(tokens[2], NULL);
    else dado->financialLoss = -1;
    dado->tamanhoRegistro += sizeof(dado->financialLoss);
    //if (tokens[2][0] == '\0') printf("sim %f", dado->financialLoss + 1);

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


//---------------- Funções para criação do arquivo binário ---------------------


void createTable(char *nomeArquivoBinario, char *nomeArquivoCSV){
    t_header cabecalho;
    t_data dado;

    FILE *CSV = fopen(nomeArquivoCSV, "r");
    if (!CSV){
        printf("Falha no processamento do arquivo.");
        return;
    }

    FILE *binario = fopen(nomeArquivoBinario, "wb");
    if (!binario){
        printf("Falha no processamento do arquivo.");
        return;
    }

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
    lerRegistro(binario, CSV, &cabecalho, &dado);

    fclose(CSV);
    fclose(binario);

    // Atualiza o cabeçalho com nro de Registros e ProxByteOffSet
    binario = fopen(nomeArquivoBinario, "rb+");
    fseek(binario, 0, SEEK_END);
    cabecalho.proxByteOffset = ftell(binario);
    fseek(binario, 0, SEEK_SET);
    escreveCabecalho(binario, &cabecalho);
    fclose(binario);

    return;
}



void lerRegistro(FILE *binario, FILE *CSV, t_header *cabecalho, t_data *dado){
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
        free(dado->country);
        free(dado->attackType);
        free(dado->defenseMechanism);
        free(dado->targetIndustry);
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





//------------- Funções para debug (apagar depois)--------------

void printRegistro(t_data *dado){

    printf("Removido: %c\n", dado->removido);
    printf("Byte Offset Prox: %d | Tamanho: %lu\n", dado->prox, sizeof(dado->prox));
    printf("ID do ataque: %d | Tamanho: %lu bytes\n", dado->idAttack, sizeof(dado->idAttack));
    printf("Ano: %d | Tamanho: %lu bytes\n", dado->year, sizeof(dado->year));
    printf("Prejuizo financeiro: %.2f | Tamanho: %lu bytes\n", dado->financialLoss, sizeof(dado->financialLoss));

    printf("Pais: %s | Tamanho: %lu bytes\n", dado->country, strlen(dado->country));
    printf("Tipo de ataque: %s | Tamanho: %lu bytes\n", dado->attackType, strlen(dado->attackType));
    printf("Setor-alvo: %s | Tamanho: %lu bytes\n", dado->targetIndustry,strlen(dado->targetIndustry));
    printf("Mecanismo de defesa: %s | Tamanho: %lu bytes\n", dado->defenseMechanism, strlen(dado->defenseMechanism));

    printf("Tamanho total do registro: %lld bytes\n", dado->tamanhoRegistro);
}


void printHeader(t_header cabecalho) {
    printf("status: '%c' | tamanho: %zu byte(s)\n", cabecalho.status, sizeof(cabecalho.status));
    printf("topo: %d | tamanho: %zu byte(s)\n", cabecalho.topo, sizeof(cabecalho.topo));
    printf("proxByteOffset: %lld | tamanho: %zu byte(s)\n", cabecalho.proxByteOffset, sizeof(cabecalho.proxByteOffset));
    printf("nroRegArq: %d | tamanho: %zu byte(s)\n", cabecalho.nroRegArq, sizeof(cabecalho.nroRegArq));
    printf("nroRegRem: %d | tamanho: %zu byte(s)\n", cabecalho.nroRegRem, sizeof(cabecalho.nroRegRem));
    printf("descreveIdentificador: \"%s\" | tamanho: %zu byte(s)\n", cabecalho.descreveIdentificador, strlen(cabecalho.descreveIdentificador));
    printf("descreveYear: \"%s\" | tamanho: %zu byte(s)\n", cabecalho.descreveYear, strlen(cabecalho.descreveYear));
    printf("descreveFinancialLoss: \"%s\" | tamanho: %zu byte(s)\n", cabecalho.descreveFinancialLoss, strlen(cabecalho.descreveFinancialLoss));
    printf("codDescreveContry: '%c' | tamanho: %zu byte(s)\n", cabecalho.codDescreveContry, sizeof(cabecalho.codDescreveContry));
    printf("descreveCountry: \"%s\" | tamanho: %zu byte(s)\n", cabecalho.descreveCountry, strlen(cabecalho.descreveCountry));
    printf("codDescreveType: '%c' | tamanho: %zu byte(s)\n", cabecalho.codDescreveType, sizeof(cabecalho.codDescreveType));
    printf("descreveType: \"%s\" | tamanho: %zu byte(s)\n", cabecalho.descreveType, strlen(cabecalho.descreveType));
    printf("codDescreveTargetIndustry: '%c' | tamanho: %zu byte(s)\n", cabecalho.codDescreveTargetIndustry, sizeof(cabecalho.codDescreveTargetIndustry));
    printf("descreveTargetIndustry: \"%s\" | tamanho: %zu byte(s)\n", cabecalho.descreveTargetIndustry, strlen(cabecalho.descreveTargetIndustry));
    printf("codDescreveDefense: '%c' | tamanho: %zu byte(s)\n", cabecalho.codDescreveDefense, sizeof(cabecalho.codDescreveDefense));
    printf("descreveDefense: \"%s\" | tamanho: %zu byte(s)\n", cabecalho.descreveDefense, strlen(cabecalho.descreveDefense));
}


void lerCabecalho(FILE *binario) {
    char status;
    long long int topo, proxByteOffset;
    int nroRegArq, nroRegRem;

    char descreveIdentificador[100], descreveYear[100], descreveFinancialLoss[100];
    char codDescreveContry, codDescreveType, codDescreveTargetIndustry, codDescreveDefense;
    char descreveCountry[100], descreveType[100], descreveTargetIndustry[100], descreveDefense[100];

    // Leitura dos campos fixos
    fread(&status, sizeof(char), 1, binario);
    fread(&topo, sizeof(long long int), 1, binario);
    fread(&proxByteOffset, sizeof(long long int), 1, binario);
    fread(&nroRegArq, sizeof(int), 1, binario);
    fread(&nroRegRem, sizeof(int), 1, binario);

    // Leitura dos campos de string (você pode ajustar o tamanho conforme seus dados)
    fread(descreveIdentificador, sizeof(char), 23, binario);
    descreveIdentificador[23] = '\0'; // garantir \0
    fread(descreveYear, sizeof(char), 27, binario);
    descreveYear[27] = '\0';
    fread(descreveFinancialLoss, sizeof(char), 28, binario);
    descreveFinancialLoss[28] = '\0';

    fread(&codDescreveContry, sizeof(char), 1, binario);
    fread(descreveCountry, sizeof(char), 26, binario);
    descreveCountry[26] = '\0';

    fread(&codDescreveType, sizeof(char), 1, binario);
    fread(descreveType, sizeof(char), 38, binario);
    descreveType[38] = '\0';

    fread(&codDescreveTargetIndustry, sizeof(char), 1, binario);
    fread(descreveTargetIndustry, sizeof(char), 38, binario);
    descreveTargetIndustry[38] = '\0';

    fread(&codDescreveDefense, sizeof(char), 1, binario);
    fread(descreveDefense, sizeof(char), 67, binario);
    descreveDefense[67] = '\0';

    // Print dos campos
    printf("Status: %c\n", status);
    printf("Topo: %lld\n", topo);
    printf("ProxByteOffset: %lld\n", proxByteOffset);
    printf("NroRegArq: %d\n", nroRegArq);
    printf("NroRegRem: %d\n", nroRegRem);

    printf("Descreve Identificador: %s\n", descreveIdentificador);
    printf("Descreve Year: %s\n", descreveYear);
    printf("Descreve Financial Loss: %s\n", descreveFinancialLoss);
    printf("Cod Country: %c\n", codDescreveContry);
    printf("Descreve Country: %s\n", descreveCountry);
    printf("Cod Type: %c\n", codDescreveType);
    printf("Descreve Type: %s\n", descreveType);
    printf("Cod Target Industry: %c\n", codDescreveTargetIndustry);
    printf("Descreve Target Industry: %s\n", descreveTargetIndustry);
    printf("Cod Defense: %c\n", codDescreveDefense);
    printf("Descreve Defense: %s\n", descreveDefense);
}



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


