#ifndef ARQUIVO_H
#define ARQUIVO_H


// 14712657 - Allan Vitor de Souza Silva


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
    int tamanhoRegistro;
    long long int prox;
} t_data;


//---------------- Funções para criação do arquivo binário ---------------------
void createTable(FILE *binario, FILE *CSV);
char **scan_csv(char *linha, int tam);
void escreveCabecalho(FILE *binario, t_header *cabecalho);
void lerRegistroCSV(FILE *binario, FILE *CSV, t_header *cabecalho, t_data *dado);
void escreveRegistro(FILE *binario, t_header *cabecalho, t_data *dado);





//----------------------- Funções de inicialização dos dados ---------------------
void inicializarCabecalho(t_header *cabecalho, char **tokens);
void inicializarDado(t_data *dado, char **tokens);




//---------------------------- Funções para o select -----------------------------
void selectFrom(FILE *binario, t_header *cabecalho, t_data *registro);
void lerCabecalhoBIN(FILE *binario, t_header *cabecalho);
void lerRegistroBIN (FILE *binario, t_data *registro);
void printRegistro(t_header *cabecalho, t_data *registro);



// ----------------------------- Funções Select Where ---------------------------
void consultasWhere(FILE *binario, int quant, t_header *cabecalho, t_data *registro);
void selectWhere(FILE *binario, char **tokens, t_header *cabecalho, t_data *registro, int qntTokens);
int comparaCampos(char **tokens, t_data *registro, int qntTokens);
char **scan_campos(char *linha, int *tam);





//---------------- Funções auxiliares ---------------------
int calcTamanhoCampo(FILE * binario);
void freeRegistro(t_data *registro);
void binarioNaTela(char *nomeArquivoBinario);
void remove_aspas(char *str);






#endif
