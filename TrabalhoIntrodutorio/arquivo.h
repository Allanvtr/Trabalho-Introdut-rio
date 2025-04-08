#ifndef ARQUIVO_H
#define ARQUIVO_H

//---------------- Definição das Structs ---------------------
typedef struct header{
    char status;
    long long int topo;
    long long int proxByteOffset;
    int nroRegArq;
    int nroRegRem;
    char descreveIdentificador[24];
    char descreveYear[28];
    char descreveFinancialLoss[29];
    char codDescreveContry;
    char descreveCountry[27];
    char codDescreveType;
    char descreveType[39];
    char codDescreveTargetIndustry;
    char descreveTargetIndustry[39];
    char codDescreveDefense;
    char descreveDefense[68];
} t_header;

typedef struct data{
    int idAttack;
    int year;
    float financialLoss;
    char *country;
    char *attackType;
    char *targetIndustry;
    char *defenseMechanism;
    char removido;
    long long int tamanhoRegistro;
    long long int prox;
} t_data;


//---------------- Funções para criação do arquivo binário ---------------------
void createTable(char *nomeArquivoBinario, char *nomeArquivoCSV);
void escreveCabecalho(FILE *binario, t_header *cabecalho);
void escreveRegistro(FILE *binario, FILE *CSV, t_header *cabecalho, t_data *dado);



//---------------- Funções de inicialização dos dados ---------------------
void inicializarCabecalho(t_header *cabecalho, char **tokens);
void inicializarDado(t_data *dado, char **tokens);


//---------------- Funções auxiliares ---------------------
char **scan_csv(char *linha, int tam);
void binarioNaTela(char *nomeArquivoBinario);
void printHeader(t_header cabecalho);
void printRegistro(t_data *dado);
void lerCabecalho(FILE *binario);


#endif
