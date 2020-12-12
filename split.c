#include <stdio.h>
#include <stdlib.h>

struct TipoLista {
	int ID;
  	struct TipoLista *prox;
};
typedef struct TipoLista TipoLista;

void Inserir_Aresta(TipoLista *lista, int a, int b) {
	if(lista != NULL) {
    	TipoLista *aux;
    	TipoLista *tmp;
   
    	aux = (TipoLista*) malloc((int)sizeof(TipoLista));    // retorno de um ponteiro genérico.
    	aux->ID = b;
    	aux->prox = NULL;
    
   	 	lista[a].ID++;
    	if(lista[a].prox == NULL)	// Caso a lista estiver vazia - Insere.	
     		lista[a].prox = aux;
    	else {
     		tmp = lista[a].prox;
     		if (tmp->ID > b) {	// insere como primeiro da lista
     			aux->prox = tmp;
     			lista[a].prox = aux;
     		} 		          
     		else if (tmp->prox == NULL) {	//insere os vértices ordenados
     			aux->prox = tmp->prox;
     			tmp->prox = aux;  
     		}
     		else {
     			while((tmp->prox != NULL) &&(tmp->prox->ID < b))
  		    		tmp = tmp->prox;
    	 		aux->prox = tmp->prox;
    	 		tmp->prox = aux;  
      		}
    	}
  	}
}

void Imprimir(TipoLista *lista, int vertices) {
	int i;	
  	TipoLista *tmp;

  	for(i = 0; i < vertices; i++) {
    	tmp = lista[i].prox;
    	printf("%2d: (%d) ==>", i, lista[i].ID);
    	while (tmp != NULL) {
    		printf("%d  ", tmp->ID);
      		tmp = tmp->prox;
    	}
    	printf("\n");
  	}
}

void Split(TipoLista *lista, int vertices) {
	TipoLista *tmp;
	int *v, clique[vertices], i, j, maior, aux, k = 0, flag = 0, iteracoes = 0;
	FILE *fp2;

	v = (int *)malloc(sizeof(int) * vertices);
	
	for(i = 0; i < vertices; i++) {
		v[i] = lista[i].ID;	// salva a quantidade total de arestas de cada vertice no vetor
		clique[i] = -1;	// inicializa vetor com -1			
	}
		
	while(iteracoes < vertices) {
		maior = -1000;
		
		for(i = 0; i < vertices; i++) {	// laço para encontrar o vertice com maior aresta no vetor
			if(clique[i] == -1 && v[i] > maior && v[i]!= 0) {	// v[i] != 0 faz o vertice 1 sair dos cliques 
				maior = v[i];
				aux = i;	// salva a posicao do maior
			}
		}				
		clique[aux] = aux;	// salva os cliques 
		flag = 0;		
		if(maior <= 0) {	// entra quando nao existe mais clique imprime os resultados
			for(i = 0; i < vertices; i++) {
				if(clique[i] != -1) {	// se o vetor clique é -1 entao siginifica que o vertice eh split
					flag = 1;				  	
    				fp2 = fopen("output.txt", "a");
    				if(fp2 != NULL) 
		  				fprintf(fp2, "S\n");				  	
				  	break;
				}
			}
			if(flag == 1) {	// o grafo eh split
				fprintf(fp2, "Q: ");
				for(i = 0; i < vertices; i++) {
					if(clique[i] != -1)
			 			fprintf(fp2, "%d ", i);
			 	}
			 	fprintf(fp2, "\nS: ");
			 	for(i = 0; i < vertices; i++) {
					if(clique[i] == -1)
			 			fprintf(fp2, "%d ", i);
			 	}
			 	break;
			}
			else {
				fprintf(fp2, "N");
			 		break;
			}
			fclose(fp2);
		}
    	v[aux] = -2;	// apaga o vertice no vetor
    	for(j = 0; j < vertices; j++) {	// percorre a lista 
			tmp = lista[j].prox;
  			while(tmp != NULL) {    			
    			if(tmp->ID == aux) {    				
    				v[j] = v[j] - 1;	// vai diminuido o contador de arestas do vertice  								
					break;    				
    			}
    			tmp = tmp->prox;
    		}
    	}
    	iteracoes++;
    }	
}

int main(void) {
	int vertices, i, j, a, b;
	FILE *pArquivo;
	TipoLista *lista;

	pArquivo = fopen("entrada.txt", "rt");
	if(pArquivo != NULL) {
		fscanf(pArquivo, "%d\n", &vertices);
		lista = (TipoLista *)malloc(sizeof(TipoLista) * vertices);
		for(i = 0; i < vertices; i++) {
		    lista[i].ID = 0;
		    lista[i].prox = NULL;
		}
		fscanf(pArquivo,"%d %d", &a, &b);			
		while(!feof(pArquivo)) {				
			Inserir_Aresta(lista, a, b);
		   	Inserir_Aresta(lista, b, a);
		   	fscanf(pArquivo,"%d %d", &a, &b);	
		}
	}
	fclose(pArquivo);
	Imprimir(lista, vertices);
	Split(lista, vertices);
	return 0;	
}
