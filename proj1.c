# include <stdio.h>
# include <string.h>
# include <stdlib.h>

# define MAX_NOME 42
# define MAX_BANCOS 1000
# define BOM 1
# define MAU 0


typedef struct {
	char nome[MAX_NOME]; /*maximo 41 letras*/
	short int classificacao; /* assumido bom (=1) ou mau (=0) */
	long ref; /* referencia/codigo do banco (e.g., 2189000) */
} banco;

void novo_banco(char nome[], short int classificacao, long ref);
int indice(long ref);
void rating(long ref, int classificacao);
void emprestimo(long ref1, long ref2, int valor);
void despromover_banco();
void list_0();
void list_1();
void list_1_aux(int b);
void list_2();
void list_x();

int num_bancos = 0;			  /* Numero de bancos criados*/
banco vec_bancos[MAX_BANCOS]; /* Vector que contem todos os bancos criados, por ordem de criacao*/
int matriz_adj[MAX_BANCOS][MAX_BANCOS]; /* O primeiro MAX_BANCOS corresponde as linhas*/

int main()
{
	short int classificacao;
	long ref, ref1, ref2;
	int valor, command;
	char nome[MAX_NOME];

	while (1) {

		command = getchar();

        switch (command) {
        case 'a':
            scanf("%s%hd%ld",nome, &classificacao, &ref);
            novo_banco(nome,classificacao,ref);
            break;
        case 'k':
        	scanf("%ld",&ref);
            rating(ref, MAU);
            break;
        case 'r':
        	scanf("%ld",&ref);
            rating(ref, BOM);
            break;
        case 'e':
            scanf("%ld%ld%d",&ref1, &ref2, &valor);
            emprestimo(ref1, ref2, valor);
            break;
        case 'p':
            scanf("%ld%ld%d",&ref1, &ref2, &valor);
            emprestimo(ref2, ref1, -valor);	/* A amortizacao e considerada como um emprestimo de valor negativo */
            break;   
        case 'l':
        getchar(); /* le o ' ' introduzido pelo utilizador */
        	command = getchar();

	        switch(command) {
	        case '0':
	        	list_0();
	       		break;
	       	case '1':
	       		list_1();
	       		break;
	       	case '2':
	       		list_2();
	       		break;
	       	default:
           		printf("ERRO: Comando desconhecido\n");
	        }	
        	break;
       	case 'K':
       		despromover_banco();
       		break;
        case 'x':
        	list_x();
            return EXIT_SUCCESS; /* Termina o programa com sucesso (ver stdlib.h) */
        default:
            printf("ERRO: Comando desconhecido\n");
        }
        getchar(); /* le o '\n' introduzido pelo utilizador */
    }
	return EXIT_FAILURE; /* se chegou aqui algo correu mal (STDLIB)*/
}


/* Esta funcao adiciona um novo elemento ao vector vec_bancos, sendo esse elemento do tipo definido banco, 
com nome, referencia e classificacao dados como parametros de entrada */
void novo_banco(char nome[], short int classificacao, long ref) 
{
	strcpy(vec_bancos[num_bancos].nome, nome);
	vec_bancos[num_bancos].classificacao = classificacao;
	vec_bancos[num_bancos].ref = ref;
	num_bancos++;
}

/* Esta funcao recebe a referencia de um banco e devolve o indice da posicao do mesmo no vector vec_bancos */
int indice(long ref)
{
	int b;

	for (b = 0; b < num_bancos; b++)
		if (vec_bancos[b].ref == ref)
			return b;
		
	return EXIT_FAILURE;
}

/* Esta funcao recebe a referencia de um banco e uma cassificacao (0-BOM ou 1-MAU) e atribui essa classificacao ao banco dado */
void rating(long ref, int classificacao) 
{
	int b;

	for (b = 0; b < num_bancos; b++)
		if (vec_bancos[b].ref == ref){
			vec_bancos[b].classificacao = classificacao;
			break;
		}
}

/* Esta funcao recebe duas referencias (ref1 e ref2) correspondentes respetivamente a banco 1 e banco 2 e adiciona um valor a matriz, 
   correspondente a um emprestimo feito pelo banco 1 ao banco 2  */
void emprestimo(long ref1, long ref2, int valor)
{
	matriz_adj[indice(ref1)][indice(ref2)] += valor;
}

/* Esta funcao procura e desclassifica o banco BOM com maior exposicaoo a divida de bancos MAUs, ou seja,
 o banco BOM com maior valor emprestado a bancos MAUs. Imprime uma linha de informacao sobre o banco despromovido 
 com o formato " ref nome classificacao inP outP outV outVM inV inVM " (siglas definidas na funcao list_1_aux) 
 e uma linha com o formato " num_bancos bons " (definido na funcao list_x) */
void despromover_banco()
{
	int i, j, emprestado_mau, emprestado_max = 1;
	long ref, ref_max = 0;

	for(i = 0; i < num_bancos; i++) {
		ref = vec_bancos[i].ref;

		/* Primeiro verifica se o banco a analisar e bom. Caso seja, inicializa a variavel do dinheiro emprestado a zeros e 
		percorre todos os restantes bancos. Os que forem maus, a divida corrente ao banco a ser analizado e adicionada.
		No fim do ciclo atualiza o banco com mais divida a bancos maus, caso necessario. */

		if (vec_bancos[i].classificacao == BOM) {
			emprestado_mau = 0;
			for(j = 0; j < num_bancos; j++) 
				if (vec_bancos[j].classificacao == MAU)
					emprestado_mau += matriz_adj[i][j];

			if (emprestado_mau >= emprestado_max) {	/* O ">=" garante que em caso de empate seja despromovido o mais recente */
				ref_max = ref;
				emprestado_max = emprestado_mau;
			}
		}
	}

	if (ref_max != 0) {
		rating(ref_max, MAU);
		printf("*");
		list_1_aux(indice(ref_max));
		list_x();
	}
	else
		list_x();

}

/* Esta funcao imprime a lista de todos os bancos, no formato " ref nome classificacao " */
void list_0() 
{
	int b;

	for (b = 0; b < num_bancos; b++)
		printf("%ld %s %d\n", vec_bancos[b].ref, vec_bancos[b].nome, vec_bancos[b].classificacao);
	
}

/* Esta funcao imprime a lista de todos os bancos, no formato " ref nome classificacao inP outP outV outVM inV inVM " 
(siglas definidas na funcao list_1_aux) */
void list_1() 
{
	int b;

	for (b = 0; b < num_bancos; b++) 	/* percorre o vetor dos bancos e para cada banco imprime a informacao dada pela funcao list_1_aux */
		list_1_aux(b);
}

/* Esta funcao e uma auxiliar para a funcao list_1, reunindo os dados a ser imprimidos pela list_1. Utilizada tambem para a funcao despromover_banco. */
void list_1_aux(int b) 
{
	int i, j, inP = 0, outP = 0, outV = 0, outVM = 0, inV = 0, inVM = 0;
				/* inP: numero total de bancos parceiros a quem o banco tem uma divida;
			    outP: numero total de bancos parceiros a quem o banco tem dinheiro emprestado;
				outV: valor total emprestado pelo banco em questao a outros bancos;
				outVM: valor total emprestado pelo banco em questao a bancos maus;
				inV: valor total emprestado ao banco em questao por outros bancos;
				inVM: valor total emprestado ao banco em questao por bancos maus; */

		for (j = 0; j < num_bancos; j++) 		/* fixamos a linha correspondente ao banco (b) em questao, */
			if (matriz_adj[b][j] != 0) {		/* e percorremo-la pelas colunas (j) para ver os valores que emprestou */
				outP += 1;
				outV += matriz_adj[b][j];
				if (vec_bancos[j].classificacao == MAU)
					outVM += matriz_adj[b][j];
			}

		for (i = 0; i < num_bancos; i++)		/* fixamos a coluna correspondente ao banco (b) em questao, */
			if (matriz_adj[i][b] != 0) {		/* e percorremo-la pelas linhas (i) para ver os valores que deve */
				inP += 1;
				inV += matriz_adj[i][b];
				if (vec_bancos[i].classificacao == MAU)
					inVM += matriz_adj[i][b];
			}

		printf("%ld %s %d %d %d %d %d %d %d\n", vec_bancos[b].ref, vec_bancos[b].nome, vec_bancos[b].classificacao, inP, outP, outV, outVM, inV, inVM);
}

/* Esta funcao imprime uma lista em que cada linha corresponde a um numero de parceiros e ao numero de bancos que tem esse numero de parceiros */
void list_2()
{
	int b, i, nvalor = 0, parceiros[MAX_BANCOS] = {0};

	/*Percorre a matriz*/
	for (b = 0; b < num_bancos; b++){
		for (i = 0; i < num_bancos; i++)
			if ((matriz_adj[b][i] != 0) || (matriz_adj[i][b] != 0)) /* Considera o emprestimo e a divida como apenas 1 parceiro */
				nvalor++;
		parceiros[nvalor]++;
		nvalor = 0;		
	}

	for (b = 0; b < num_bancos; b++) /* Imprime uma lista ignorando elementos nulos*/
		if (parceiros[b] != 0)
			printf("%d %d\n", b, parceiros[b]);
}

/* Esta funcao imprime o número de total de bancos registados, seguido do numero total de bancos bons  */
void list_x() 
{
	int b, bons = 0;

	for (b = 0; b < num_bancos; b++) 
		if (vec_bancos[b].classificacao == BOM)
			bons += 1;

	printf("%d %d\n", num_bancos, bons);
}
