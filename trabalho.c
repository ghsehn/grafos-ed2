/**
 * Trabalho pratico de grafos
 * Aluno: Guilherme Henrique Sehn
 * Disciplina: Estrutura de Dados II
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/**
 * Estruturas de dados utilizadas no programa
 */
struct nodoLista
{
	struct vertice *vertice;
	struct nodoLista *prox;
};

struct vertice
{
	char valor;
	int visitado;
	struct nodoLista *arestas;
	struct vertice *prox;
};

typedef struct nodoLista aresta;
typedef struct nodoLista nodoLista;
typedef struct vertice vertice;

/**
 * Definicoes das funcoes utilitarias
 */
int criarVertice(char valor, vertice **listaVertices);
vertice *verticeComValor(char valor, vertice *listaVertices);
void excluirVertice(vertice *v, vertice **listaVertices);
void inserirArestaLista(vertice *origem, vertice *destino);

int criarAresta(vertice *v1, vertice *v2);
int removerArestaLista(vertice *v, vertice *remover);
int excluirAresta(vertice *v1, vertice *v2);
int existeAresta(vertice *v1, vertice *v2);
void ordenarArestas(vertice *v);

void imprimirLista(nodoLista *lista);
void apagarInfosVisitado(vertice *listaVertices);
void buscaAmplitude(vertice *vInicial, vertice *listaVertices);
void buscaProfundidade(vertice *vInicial, vertice *listaVertices);

void criarDadosPadrao(vertice **listaVertices);

/**
 * Definicoes das funcoes de menu
 */
void menuCriarVertice(vertice **listaVertices);
void menuImprimirVertices(vertice *listaVertices);
void menuConsultarVertice(vertice *listaVertices);
void menuPrincipal();
void menuCriarAresta(vertice **listaVertices);
void menuExcluirAresta(vertice **listaVertices);
void menuBusca(char tipo, vertice *listaVertices);
void menuCriarDadosPadrao(vertice **listaVertices);
void menuAviso(char *mensagem, int _limparTela);
void limparTela();
void limparBuffer();

/**
 * Cria um vertice
 */
int criarVertice(char valor, vertice **listaVertices)
{
	vertice *ptAux = *listaVertices;

	// Checa se o vertice ja existe, se sim, retorna falso
	while (ptAux != NULL)
	{
		if (ptAux->valor == valor)
			return 0;

		ptAux = ptAux->prox;
	}

	// Aloca o vertice
	vertice *ptNovo = (vertice *)malloc(sizeof(vertice));
	ptNovo->valor = valor;
	ptNovo->visitado = 0;
	ptNovo->arestas = NULL;
	ptNovo->prox = NULL;

	// Caso a lista de vertices esteja vazia, faz o ponteiro da lista de vertices
	// apontar para este novo vertice
	if (*listaVertices == NULL)
	{
		*listaVertices = ptNovo;
		return 1;
	}

	// Caso a lista de vertices ja tenha registros, insere no final da lista
	ptAux = *listaVertices;

	while (ptAux->prox != NULL)
		ptAux = ptAux->prox;

	ptAux->prox = ptNovo;

	return 1;
}

/**
 * Excluir um vertice
 */
void excluirVertice(vertice *v, vertice **listaVertices)
{
	// Exclui todas as arestas do vertice
	while (v->arestas != NULL)
		excluirAresta(v, v->arestas->vertice);

	// Reorganiza lista de vertices
	if (*listaVertices == v)
	{
		*listaVertices = (*listaVertices)->prox;
		free(v);
	}
	else
	{
		vertice *ptAuxVertice = *listaVertices,
		        *ptAnt = NULL;

		while (ptAuxVertice->prox != NULL)
		{
			if (ptAuxVertice == v)
			{
				ptAnt->prox = ptAuxVertice->prox;
				free(ptAuxVertice);
				break;
			}

			ptAnt = ptAuxVertice;
			ptAuxVertice = ptAuxVertice->prox;
		}
	}
}

/**
 * Busca vertice com determinado valor e retorna um ponteiro para ele
 * caso exista
 */
vertice *verticeComValor(char valor, vertice *listaVertices)
{
	vertice *ptVerticeEncontrado = NULL;
	vertice *ptAux = listaVertices;

	while (ptAux != NULL)
	{
		if (ptAux->valor == valor)
		{
			ptVerticeEncontrado = ptAux;
			break;
		}

		ptAux = ptAux->prox;
	}

	return ptVerticeEncontrado;
}

/**
 * Insere aresta na lista de arestas de um vertice
 */
void inserirArestaLista(vertice *origem, vertice *destino)
{
	aresta *ptAresta = (aresta *)malloc(sizeof(aresta));
	ptAresta->vertice = destino;
	ptAresta->prox = NULL;

	aresta *ptAux = origem->arestas;

	if (ptAux == NULL)
	{
		origem->arestas = ptAresta;
	}
	else
	{
		while (ptAux->prox != NULL)
			ptAux = ptAux->prox;

		ptAux->prox = ptAresta;
	}
}

/**
 * Criar aresta
 */
int criarAresta(vertice *v1, vertice *v2)
{
	if (v1 == NULL || v2 == NULL)
		return 0;

	inserirArestaLista(v1, v2);
	ordenarArestas(v1);

	if (v1 != v2)
	{
		inserirArestaLista(v2, v1);
		ordenarArestas(v2);
	}

	return 1;
}

/**
 * Exclui aresta com determinado vertice na lista de arestas de um vertice
 */
int removerArestaLista(vertice *v, vertice *remover)
{
	if (v->arestas == NULL)
		return 0;

	if (v->arestas->vertice == remover)
	{
		aresta *prox = v->arestas->prox;
		free(v->arestas);
		v->arestas = prox;
		return 1;
	}

	int sucesso = 0;

	aresta *ptAux = v->arestas,
	       *ptAnt = NULL;

	while (ptAux != NULL)
	{
		if (ptAux->vertice == remover)
		{
			sucesso = 1;
			ptAnt->prox = ptAux->prox;
			free(ptAux);
			break;
		}

		ptAnt = ptAux;
		ptAux = ptAux->prox;
	}

	return sucesso;
}

/**
 * Excluir aresta
 */
int excluirAresta(vertice *v1, vertice *v2)
{
	int sucesso = removerArestaLista(v1, v2);

	if (v1 == v2)
		return sucesso;
	else
		return sucesso && removerArestaLista(v2, v1);
}

/**
 * Checa se aresta entre dois vertices existe
 */
int existeAresta(vertice *v1, vertice *v2)
{
	if (v1->arestas == NULL || v2->arestas == NULL)
		return 0;

	int existe = 0;
	aresta *ptAux = v1->arestas;

	while (ptAux != NULL)
	{
		if (ptAux->vertice == v2)
		{
			existe = 1;
			break;
		}

		ptAux = ptAux->prox;
	}

	return existe;
}

/**
 * Ordenacao de arestas
 */
void ordenarArestas(vertice *v)
{
	aresta *ptAux = v->arestas;
	vertice *ptTransicao = NULL;

	int houveTroca = 1;

	while (houveTroca)
	{
		houveTroca = 0;

		while (ptAux != NULL)
		{
			if (ptAux->prox == NULL)
			{
				ptAux = ptAux->prox;
				continue;
			}

			if (ptAux->vertice->valor > ptAux->prox->vertice->valor)
			{
				ptTransicao = ptAux->vertice;
				ptAux->vertice = ptAux->prox->vertice;
				ptAux->prox->vertice = ptTransicao;

				houveTroca = 1;
			}

			ptAux = ptAux->prox;
		}
	}
}

/**
 * Imprime uma lista
 */
void imprimirLista(nodoLista *lista)
{
	if (lista == NULL)
	{
		printf("(vazia)");
	}
	else
	{
		nodoLista *ptAux = lista;

		while (ptAux != NULL)
		{
			printf("%c", ptAux->vertice->valor);

			if (ptAux->prox != NULL)
				printf(", ");

			ptAux = ptAux->prox;
		}
	}
}

/**
 * Limpa dados "visitado" dos vertices
 */
void apagarInfosVisitado(vertice *listaVertices)
{
	vertice *ptAux = listaVertices;

	while (ptAux != NULL)
	{
		ptAux->visitado = 0;
		ptAux = ptAux->prox;
	}
}

/**
 * Busca em amplitude
 */
void buscaAmplitude(vertice *vInicial, vertice *listaVertices)
{
	apagarInfosVisitado(listaVertices);

	int encontrou = 0;

	vertice *ptAuxVertice = vInicial;
	aresta *ptAuxAresta = NULL;

	nodoLista *ptFila = NULL,
	          *ptAuxFila = NULL,
	          *ptNovoNodo = NULL;

	while (ptAuxVertice != NULL)
	{
		printf("%c", ptAuxVertice->valor);
		ptAuxVertice->visitado = 1;

		if (ptAuxVertice->arestas != NULL)
		{
			ptAuxAresta = ptAuxVertice->arestas;

			while (ptAuxAresta != NULL)
			{
				// Nao insere na fila se ja foi acessado
				if (ptAuxAresta->vertice->visitado)
				{
					ptAuxAresta = ptAuxAresta->prox;
					continue;
				}

				// Nao insere na fila se ja se encontra nela
				encontrou = 0;
				ptAuxFila = ptFila;

				while (ptAuxFila != NULL)
				{
					if (ptAuxFila->vertice == ptAuxAresta->vertice)
						encontrou = 1;

					ptAuxFila = ptAuxFila->prox;
				}

				if (encontrou)
				{
					ptAuxAresta = ptAuxAresta->prox;
					continue;
				}

				// Insere vertice na fila
				ptNovoNodo = (nodoLista *)malloc(sizeof(nodoLista));
				ptNovoNodo->vertice = ptAuxAresta->vertice;
				ptNovoNodo->prox = NULL;

				if (ptFila == NULL)
				{
					ptFila = ptNovoNodo;
				}
				else
				{
					ptAuxFila = ptFila;

					while (ptAuxFila->prox != NULL)
						ptAuxFila = ptAuxFila->prox;

					ptAuxFila->prox = ptNovoNodo;
				}

				ptAuxAresta = ptAuxAresta->prox;
			}
		}

		printf("\t\tFila: ");
		imprimirLista(ptFila);

		// Remove o primeiro da fila e consulta-o
		if (ptFila != NULL)
		{
			ptAuxVertice = ptFila->vertice;
			ptAuxFila = ptFila->prox;
			free(ptFila);
			ptFila = ptAuxFila;
		}
		else
		{
			ptAuxVertice = NULL;
		}

		printf("\n");
	}

	printf("\n");
}


/**
 * Busca em profundidade
 */
void buscaProfundidade(vertice *vInicial, vertice *listaVertices)
{
	apagarInfosVisitado(listaVertices);

	vertice *ptAuxVertice = vInicial;
	aresta *ptAuxAresta = NULL;

	nodoLista *ptPilha = NULL,
	          *ptAuxPilha = NULL,
	          *ptNovoNodo = NULL;

	while (ptAuxVertice != NULL)
	{
		if (ptAuxVertice->visitado == 2)
		{
			ptAuxAresta = NULL;
		}
		else
		{
			ptAuxAresta = ptAuxVertice->arestas;

			while (ptAuxAresta != NULL)
			{
				if (ptAuxAresta->vertice->visitado == 0)
					break;

				ptAuxAresta = ptAuxAresta->prox;
			}
		}

		printf("%c visitado", ptAuxVertice->valor);

		if (ptAuxAresta == NULL)
		{
			printf(" (verde -> vermelho)");

			ptAuxVertice->visitado = 2;
			
			// Mostra a pilha
			printf("\t\tPilha: ");
			imprimirLista(ptPilha);

			if (ptPilha != NULL)
			{
				ptAuxVertice = ptPilha->vertice;

				ptAuxPilha = ptPilha->prox;
				free(ptPilha);
				ptPilha = ptAuxPilha;
			}
			else
			{
				ptAuxVertice = NULL;
			}
		}
		else
		{
			printf(" (branco -> verde)");

			ptNovoNodo = (nodoLista *)malloc(sizeof(nodoLista));
			ptNovoNodo->vertice = ptAuxVertice;
			ptNovoNodo->prox = ptPilha;
			ptPilha = ptNovoNodo;

			ptAuxVertice->visitado = 1;
			ptAuxVertice = ptAuxAresta->vertice;

			// Mostra a pilha
			printf("\t\tPilha: ");
			imprimirLista(ptPilha);
		}

		printf("\n");
	}

	printf("\n");
}

/**
 * Cria dados para testar o programa
 */
void criarDadosPadrao(vertice **listaVertices)
{
	// Exclui todos os vértices
	while (*listaVertices != NULL)
		excluirVertice(*listaVertices, listaVertices);

	// Cria os vertices
	char c;
	for (c = 'A'; c <= 'J'; c++)
		criarVertice(c, listaVertices);

	// Cria as arestas
	char arestas[][2] = {
		{'A', 'B'}, {'A', 'C'},
		{'B', 'C'}, {'B', 'D'},
		{'C', 'D'}, {'C', 'E'},
		{'D', 'E'}, {'D', 'F'}, {'D', 'G'},
		{'E', 'F'}, {'E', 'H'},
		{'F', 'I'}, {'F', 'G'},
		{'H', 'I'},
		{'I', 'J'}
	};

	int i;
	for (i = 0; i < 15; i++)
		criarAresta(verticeComValor(arestas[i][0], *listaVertices), verticeComValor(arestas[i][1], *listaVertices));
}

/**
 * Consultar vertice
 */
void menuConsultarVertice(vertice *listaVertices)
{
	char valor;

	printf("Digite o nome do vertice (0 para cancelar): ");
	scanf(" %c", &valor);
	limparBuffer();

	if (valor == '0')
	{
		menuAviso("Operacao cancelada.", 1);
		return;
	}

	vertice *vertice = verticeComValor(valor, listaVertices);

	if (vertice == NULL)
	{
		menuAviso("Vertice nao encontrado.", 0);
		return;	
	}

	if (vertice->arestas == NULL)
	{
		limparTela();
		printf("O vertice '%c' nao tem arestas.\n", vertice->valor);
		menuAviso(NULL, 0);
		return;		
	}

	printf("Vertices ligados ao '%c':\n", vertice->valor);

	aresta *ptAux = vertice->arestas;
	while (ptAux != NULL)
	{
		printf(" - %c\n", ptAux->vertice->valor);
		ptAux = ptAux->prox;
	}

	printf("\n");
	menuAviso(NULL, 0);
}

/**
 * Menu para criar vertice
 */
void menuCriarVertice(vertice **listaVertices)
{
	char valor;

	printf("Digite um caractere para representar o vertice (0 para cancelar): ");
	scanf(" %c", &valor);
	limparBuffer();

	if (valor == '0')
	{
		menuAviso("Operacao cancelada.", 1);
		return;
	}

	if (criarVertice(valor, listaVertices))
	{
		menuAviso("Vertice inserido com sucesso.", 1);
	}
	else
	{
		printf("Ja existe um vertice com este valor. Escolha outro.\n");
		menuCriarVertice(listaVertices);
	}
}

/**
 * Menu que imprime lista de vertices
 */
void menuImprimirVertices(vertice *listaVertices)
{
	printf("Lista de vertices\n-----------------\n\n");

	if (listaVertices == NULL)
	{
		menuAviso("Nao ha vertices.", 1);
	}

	vertice *ptAux = listaVertices;

	while (ptAux != NULL)
	{
		printf("%c ", ptAux->valor);

		if (ptAux->arestas == NULL)
		{
			printf("(nao tem arestas)");
		}
		else
		{
			printf("(adjacente a: ");
			imprimirLista(ptAux->arestas);
			printf(")");
		}

		printf("\n");

		ptAux = ptAux->prox;
	}

	printf("\n");
	menuAviso(NULL, 0);
}

/**
 * Menu para excluir determinado vertice
 */
void menuExcluirVertice(vertice **listaVertices)
{
	char val;
	vertice *v;
	int digitou = 0;

	do
	{
		if (digitou)
			printf("Vertice inexistente.\n");

		printf("Digite o valor do vertice para excluir (0 para cancelar): ");
		scanf(" %c", &val);
		limparBuffer();

		if (val == '0')
		{
			menuAviso("Operacao cancelada.", 1);
			return;
		}

		v = verticeComValor(val, *listaVertices);
	}
	while (v == NULL);

	excluirVertice(v, listaVertices);
	menuAviso("Vertice excluido com sucesso.", 1);
}

/**
 * Menu para criar aresta
 */
void menuCriarAresta(vertice **listaVertices)
{
	char val;
	int preencheu1 = 0, preencheu2 = 0;
	vertice *v1 = NULL, *v2 = NULL;

	printf("Criar aresta entre vertices\n");
	printf("---------------------------\n");
	printf("Insira o valor dos vertices a seguir para a criacao de uma aresta os ligando.\n");
	printf("Para cancelar a operacao, digite 0.\n\n");

	do
	{
		if (preencheu1)
			printf("Vertice nao encontrado. Digite um outro valor ou 0 para cancelar.\n");

		printf("Vertice 1: ");
		scanf(" %c", &val);
		limparBuffer();

		if (val == '0')
		{
			menuAviso("Operacao cancelada.", 1);
			return;
		}

		v1 = verticeComValor(val, *listaVertices);
		preencheu1 = 1;
	}
	while (v1 == NULL);

	do
	{
		if (preencheu2)
			printf("Vertice nao encontrado. Digite um outro valor ou 0 para cancelar.\n");

		printf("Vertice 2: ");
		scanf(" %c", &val);
		limparBuffer();

		if (val == '0')
		{
			menuAviso("Operacao cancelada.", 1);
			return;
		}

		v2 = verticeComValor(val, *listaVertices);
		preencheu2 = 1;
	}
	while (v2 == NULL);

	if (existeAresta(v1, v2))
	{
		menuAviso("Ja existe uma aresta ligando estes dois vertices.", 1);
	}
	else if (criarAresta(v1, v2))
	{
		menuAviso("Aresta criada com sucesso.", 1);
	}
	else
	{
		menuAviso("Nao foi possivel criar a aresta pois ao menos um dos vertices especificados nao existe.\n", 1);
	}
}

/**
 * Menu para excluir vertice
 */
void menuExcluirAresta(vertice **listaVertices)
{
	char val;
	int preencheu1 = 0, preencheu2 = 0;
	vertice *v1 = NULL, *v2 = NULL;

	printf("Excluir aresta entre vertices\n");
	printf("---------------------------\n");
	printf("Insira o valor dos vertices a seguir para a exclusao da aresta os ligando.\n");
	printf("Para cancelar a operacao, digite 0.\n\n");

	do
	{
		if (preencheu1)
			printf("Vertice nao encontrado. Digite um outro valor ou 0 para cancelar.\n");

		printf("Vertice 1: ");
		scanf(" %c", &val);
		limparBuffer();

		if (val == '0')
		{
			menuAviso("Operacao cancelada.", 1);
			return;
		}

		v1 = verticeComValor(val, *listaVertices);
		preencheu1 = 1;
	}
	while (v1 == NULL);

	do
	{
		if (preencheu2)
			printf("Vertice nao encontrado. Digite um outro valor ou 0 para cancelar.\n");

		printf("Vertice 2: ");
		scanf(" %c", &val);
		limparBuffer();

		if (val == '0')
		{
			menuAviso("Operacao cancelada.", 1);
			return;
		}

		v2 = verticeComValor(val, *listaVertices);
		preencheu2 = 1;
	}
	while (v2 == NULL);

	if (excluirAresta(v1, v2))
	{
		menuAviso("Aresta excluida com sucesso", 1);
	}
	else
	{
		menuAviso("Nao foi possivel excluir a aresta pois nao ha ligacao entre os vertices mencionados.", 1);
	}
}

/**
 * Realiza busca
 */
void menuBusca(char tipo, vertice *listaVertices)
{
	char val;
	int preencheu = 0;
	vertice *v;

	printf("Digite o valor do vertice para inicio da busca em %s (0 para cancelar).\n\n", (tipo == 'p' ? "profundidade" : "amplitude"));

	do
	{
		if (preencheu)
			printf("Vertice nao encontrado. Digite um outro valor ou 0 para cancelar.\n");

		printf("Valor: ");
		scanf(" %c", &val);
		limparBuffer();

		if (val == '0')
		{
			menuAviso("Operacao cancelada.", 1);
			return;
		}

		v = verticeComValor(val, listaVertices);
		preencheu = 1;
	}
	while (v == NULL);

	printf("\n");

	if (tipo == 'p')
		buscaProfundidade(v, listaVertices);
	else
		buscaAmplitude(v, listaVertices);

	menuAviso(NULL, 0);
}

/**
 * Menu para criar os dados padrao
 */
void menuCriarDadosPadrao(vertice **listaVertices)
{
	if (*listaVertices != NULL)
	{
		char c;

		printf("Tem certeza que deseja utilizar o grafo padrao?\n");
		printf("Isto fara com que todos os vertices e arestas do seu grafo atual sejam excluidas.\n");

		do
		{
			printf("Opcao (S/N): ");
			scanf(" %c", &c);
			limparBuffer();
			c = toupper(c);
		}
		while (c != 'S' && c != 'N');

		if (c == 'N')
		{
			menuAviso("Operacao cancelada.", 1);
			return;
		}
	}

	criarDadosPadrao(listaVertices);
	menuAviso("Grafo padrao criado com sucesso.", 1);
}

/**
 * Menu de Aviso
 */
void menuAviso(char *mensagem, int _limparTela)
{
	if (_limparTela)
		limparTela();

	if (mensagem != NULL)
		printf("%s\n", mensagem);

	printf("Tecle enter para voltar ao menu.\n");

	char c;
	while ((c = getchar()) != '\n' && c != EOF);
	limparBuffer();

	limparTela();
	menuPrincipal();
}

/**
 * Menu principal
 */
void menuPrincipal()
{
	static vertice *primeiroVertice = NULL;
	static vertice **listaVertices = &primeiroVertice;

	limparTela();

	printf("Trabalho pratico de grafos\n");
	printf("Aluno: Guilherme Henrique Sehn\n");
	printf("Disciplina: Estrutura de Dados II\n");
	printf("---------------------------------\n\n");

	printf("O que deseja fazer agora?\n\n");
	printf("1 - Criar vertice\n");
	printf("2 - Listar vertices\n");
	printf("3 - Consultar vertice\n");
	printf("4 - Excluir vertice\n");
	printf("5 - Criar aresta\n");
	printf("6 - Excluir aresta\n");
	printf("7 - Executar busca em profundidade\n");
	printf("8 - Executar busca em amplitude\n");
	printf("9 - Criar grafo padrao\n");
	printf("0 - Encerrar programa");
	printf("\n\nDigite a opcao: ");

	int opcao;
	scanf(" %d", &opcao);

	limparBuffer();
	limparTela();

	switch (opcao)
	{
		case 1: menuCriarVertice(listaVertices); break;
		case 2: menuImprimirVertices(*listaVertices); break;
		case 3: menuConsultarVertice(*listaVertices); break;
		case 4: menuExcluirVertice(listaVertices); break;
		case 5: menuCriarAresta(listaVertices); break;
		case 6: menuExcluirAresta(listaVertices); break;
		case 7: menuBusca('p', *listaVertices); break;
		case 8: menuBusca('a', *listaVertices); break;
		case 9: menuCriarDadosPadrao(listaVertices); break;
		case 0: printf("Tchau! ^_^\n\n"); break;
		default: menuAviso("Codigo inexistente.", 0); break;
	}
}

/**
 * Funcao para limpar a tela do usuario
 * Utiliza a funcao correta de acordo com o sistema operacional
 */
void limparTela()
{
	#if defined(_WIN32) || defined(_WIN64)
		system("cls");
	#elif defined(__APPLE__) || defined(__linux) || defined(__unix)
		system("clear");
	#endif
}

/**
 * Limpa buffer para evitar que chamadas a funcao scanf sejam ignoradas
 */
void limparBuffer()
{
	fflush(stdin);

	#if defined(__APPLE__) || defined(__linux) || defined(__unix)
		fpurge(stdin);
	#endif
}

/**
 * Funcao principal
 */
int main()
{
	menuPrincipal();
	return 0;
}