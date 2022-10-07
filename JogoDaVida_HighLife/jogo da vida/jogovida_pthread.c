/*
desenvolvido por:
André Filipe Siqueira Tokumoto
Jogo da vida,paralelo(pthread) em C
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

#define NTHREAD 4    //numero maximo de threads 
#define TAM_TAB 2048
int num_ger;//numero maximo de gerações
int vivos;//contador de celulas vivas
int **grid;
sem_t mutex;

void imprime_tabuleiro(int** grid){

	int i,j;
	for(i=0;i<50;i++){	
		for(j=0;j<50;j++){
	           printf("%d ",grid[i][j]);
		}
		printf("\n");
	}
}

int **inicia_tabuleiro(){//cria o tabuleiro com as condições iniciais

	int **grid;
	int i,j;
	
	grid = (int**)malloc(TAM_TAB * sizeof(int*));//cria as colunas 
	
	for(i=0;i<TAM_TAB;i++)
		grid[i] = (int*)malloc(TAM_TAB * sizeof(int));//aloca uma linha por coluna, formando o tabuleiro NxN
	
	for(i=0;i<TAM_TAB;i++){//inicia todas as posições com zero
	
		for(j=0;j<TAM_TAB;j++){
		
			grid[i][j]=0;
		}
	}
	
	//GLIDER
	int lin = 1, col = 1;
	grid[lin ][col+1] = 1;
	grid[lin+1][col+2] = 1;
	grid[lin+2][col ] = 1;
	grid[lin+2][col+1] = 1;
	grid[lin+2][col+2] = 1;

	//R-pentomino
	lin =10; col = 30;
	grid[lin ][col+1] = 1;
	grid[lin ][col+2] = 1;
	grid[lin+1][col ] = 1;
	grid[lin+1][col+1] = 1;
	grid[lin+2][col+1] = 1;
	
	printf("tabuleiro inicial\n");
	imprime_tabuleiro(grid);
	
	return grid;
}

int getNeighbors(int i, int j){ //retorna o numero de vizinhos vivos da celula na posição (i,j)

	int vizinhos_vivos = 0;
	
		if(i==0){//celula na borda superior
	
			if(j==0){//celula da ponta superior esquerda
				
				if(grid[TAM_TAB-1][j] == 1) vizinhos_vivos++;//vizinho superior
				if(grid[TAM_TAB-1][j+1] == 1) vizinhos_vivos++;//vizinho diagonal superior direito 
				if(grid[TAM_TAB-1][TAM_TAB-1] == 1) vizinhos_vivos++;//vizinho diagonal superior esquerdo
				if(grid[i][TAM_TAB-1] == 1) vizinhos_vivos++;//vizinho esquerdo
				if(grid[i][j+1] == 1) vizinhos_vivos++;//vizinho direito
				if(grid[i+1][j] == 1) vizinhos_vivos++;//vizinho inferior
				if(grid[i+1][TAM_TAB-1] == 1) vizinhos_vivos++;//vizinho diagonal inferior esquerdo
				if(grid[i+1][j+1] == 1) vizinhos_vivos++;//vizinho diagonal inferior direito
				
				return vizinhos_vivos;			
			}
			else if(j == TAM_TAB-1){//celula da ponta superior direita
			
				if(grid[TAM_TAB-1][j] == 1) vizinhos_vivos++;//vizinho superior
				if(grid[TAM_TAB-1][0] == 1) vizinhos_vivos++;//vizinho diagonal superior direito 
				if(grid[TAM_TAB-1][j-1] == 1) vizinhos_vivos++;//vizinho diagonal superior esquerdo		
				if(grid[i][j-1] == 1) vizinhos_vivos++;//vizinho esquerdo
				if(grid[i][0] == 1) vizinhos_vivos++;//vizinho direito	
				if(grid[i+1][j] == 1) vizinhos_vivos++;//vizinho inferior
				if(grid[i+1][j-1] == 1) vizinhos_vivos++;//vizinho diagonal inferior esquerdo
			if(grid[i+1][0] == 1) vizinhos_vivos++;//vizinho diagonal inferior direito
				
				return vizinhos_vivos;			
			}
			else{//celula não está na ponta
			
				if(grid[TAM_TAB-1][j] == 1) vizinhos_vivos++;//vizinho superior			
				if(grid[TAM_TAB-1][j+1] == 1) vizinhos_vivos++;//vizinho diagonal superior direito 	
				if(grid[TAM_TAB-1][j-1] == 1) vizinhos_vivos++;//vizinho diagonal superior esquerdo	
				if(grid[i][j-1] == 1) vizinhos_vivos++;//vizinho esquerdo
				if(grid[i][j+1] == 1) vizinhos_vivos++;//vizinho direito	
				if(grid[i+1][j] == 1) vizinhos_vivos++;//vizinho inferior
				if(grid[i+1][j-1] == 1) vizinhos_vivos++;//vizinho diagonal inferior esquerdo	
				if(grid[i+1][j+1] == 1) vizinhos_vivos++;//vizinho diagonal inferior direito
				
				return vizinhos_vivos;			
			}
	
		}
		
		else if(i==TAM_TAB-1){//celula na borda inferior
	
			if(j==0){//celula da ponta inferior esquerda
			
				if(grid[i-1][j] == 1) vizinhos_vivos++;//vizinho superior
				if(grid[i-1][j+1] == 1) vizinhos_vivos++;//vizinho diagonal superior direito 
				if(grid[i-1][TAM_TAB-1] == 1) vizinhos_vivos++;//vizinho diagonal superior esquerdo
				if(grid[i][TAM_TAB-1] == 1) vizinhos_vivos++;//vizinho esquerdo
				if(grid[i][j+1] == 1) vizinhos_vivos++;//vizinho direito
				if(grid[0][j] == 1) vizinhos_vivos++;//vizinho inferior
				if(grid[0][TAM_TAB-1] == 1) vizinhos_vivos++;//vizinho diagonal inferior esquerdo
				if(grid[0][j+1] == 1) vizinhos_vivos++;//vizinho diagonal inferior direito
				
				return vizinhos_vivos;			
			}
			else if(j == TAM_TAB-1){//celula da ponta inferior direita    
			
				if(grid[i+1][j] == 1) vizinhos_vivos++;//vizinho superior
				if(grid[i-1][0] == 1) vizinhos_vivos++;//vizinho diagonal superior direito 				
				if(grid[i-1][j-1] == 1) vizinhos_vivos++;//vizinho diagonal superior esquerdo		
				if(grid[i][j-1] == 1) vizinhos_vivos++;//vizinho esquerdo
				if(grid[i][0] == 1) vizinhos_vivos++;//vizinho direito	
				if(grid[0][j] == 1) vizinhos_vivos++;//vizinho inferior				
				if(grid[0][j-1] == 1) vizinhos_vivos++;//vizinho diagonal inferior esquerdo
				if(grid[0][0] == 1) vizinhos_vivos++;//vizinho diagonal inferior direito
				
				return vizinhos_vivos;			
			}
			else{//celula não está na ponta
			
				if(grid[i-1][j] == 1) vizinhos_vivos++;//vizinho superior			
				if(grid[i-1][j+1] == 1) vizinhos_vivos++;//vizinho diagonal superior direito 	
				if(grid[i-1][j-1] == 1) vizinhos_vivos++;//vizinho diagonal superior esquerdo	
				if(grid[i][j-1] == 1) vizinhos_vivos++;//vizinho esquerdo
				if(grid[i][j+1] == 1) vizinhos_vivos++;//vizinho direito		
				if(grid[0][j] == 1) vizinhos_vivos++;//vizinho inferior
				if(grid[0][j-1] == 1) vizinhos_vivos++;//vizinho diagonal inferior esquerdo	
				if(grid[0][j+1] == 1) vizinhos_vivos++;//vizinho diagonal inferior direito
				
				return vizinhos_vivos;			
			}
		}
		
		else if(j==0){//celula na borda lateral esquerda
		
			if(grid[i-1][j] == 1) vizinhos_vivos++;//vizinho superior			
			if(grid[i-1][j+1] == 1) vizinhos_vivos++;//vizinho diagonal superior direito 	
			if(grid[i-1][TAM_TAB-1] == 1) vizinhos_vivos++;//vizinho diagonal superior esquerdo	
			if(grid[i][TAM_TAB-1] == 1) vizinhos_vivos++;//vizinho esquerdo
			if(grid[i][j+1] == 1) vizinhos_vivos++;//vizinho direito		
			if(grid[i+1][j] == 1) vizinhos_vivos++;//vizinho inferior
			if(grid[i+1][TAM_TAB-1] == 1) vizinhos_vivos++;//vizinho diagonal inferior esquerdo	
			if(grid[i+1][j+1] == 1) vizinhos_vivos++;//vizinho diagonal inferior direito
		
			return vizinhos_vivos;	
		}
		
		else if(j==TAM_TAB-1){//celula na borda lateral direita
		
			if(grid[i-1][j] == 1) vizinhos_vivos++;//vizinho superior						
			if(grid[i-1][0] == 1) vizinhos_vivos++;//vizinho diagonal superior direito 	
			if(grid[i-1][j-1] == 1) vizinhos_vivos++;//vizinho diagonal superior esquerdo				
			if(grid[i][j-1] == 1) vizinhos_vivos++;//vizinho esquerdo
			if(grid[i][0] == 1) vizinhos_vivos++;//vizinho direito		
			if(grid[i+1][j] == 1) vizinhos_vivos++;//vizinho inferior
			if(grid[i+1][j-1] == 1) vizinhos_vivos++;//vizinho diagonal inferior esquerdo	
			if(grid[i+1][0] == 1) vizinhos_vivos++;//vizinho diagonal inferior direito
		
			return vizinhos_vivos;	
		}
		
		else{//celula não está na borda
		
			if(grid[i-1][j] == 1) vizinhos_vivos++;//vizinho superior						
			if(grid[i-1][j+1] == 1) vizinhos_vivos++;//vizinho diagonal superior direito 	
			if(grid[i-1][j-1] == 1) vizinhos_vivos++;//vizinho diagonal superior esquerdo				
			if(grid[i][j-1] == 1) vizinhos_vivos++;//vizinho esquerdo
			if(grid[i][j+1] == 1) vizinhos_vivos++;//vizinho direito		
			if(grid[i+1][j] == 1) vizinhos_vivos++;//vizinho inferior
			if(grid[i+1][j-1] == 1) vizinhos_vivos++;//vizinho diagonal inferior esquerdo	
			if(grid[i+1][j+1] == 1) vizinhos_vivos++;//vizinho diagonal inferior direito
		
			return vizinhos_vivos;			
		}

	
	return vizinhos_vivos;
}


int conta_vivos(){//conta o numero de celulas vivas no tabuleiro

	int i,j,total=0;
	
	for(i=0;i<TAM_TAB;i++){
		
		for(j=0;j<TAM_TAB;j++){
	
			total = total + grid[i][j];
		}
	}
	return total;

}

void *percorre(void **newgrid){

	int i,j,v_vivos;
	int **newgrid1;

	newgrid1 = (int**)newgrid;

		for(i=0;i<TAM_TAB/NTHREAD;i++){
			
			sem_wait(&mutex);
			for(j=0;j<TAM_TAB;j++){
				
				
				v_vivos = getNeighbors(i,j);
			
				if(grid[i][j]==1){//celula esta viva

					if(v_vivos < 2) newgrid1[i][j]=0;
					
					else if(v_vivos > 3) newgrid1[i][j]=0;	

					else newgrid1[i][j]=1;
				}
				else{//celula esta morta
					if(v_vivos == 3) newgrid1[i][j]= 1;
					else newgrid1[i][j]= 0;
				}
				
			}
			sem_post(&mutex);
		}
}

void geracoes(){

	int k,th,i;
	int **newgrid,**ax;
	pthread_t t[NTHREAD];//vetor de threads

	newgrid = (int**)malloc(TAM_TAB * sizeof(int*));//cria as colunas 
	
	for(i=0;i<TAM_TAB;i++)
		newgrid[i] = (int*)malloc(TAM_TAB * sizeof(int));//aloca uma linha por coluna, formando o tabuleiro NxN

	for(k=0;k<num_ger;k++){//passa as gerações
	
		for(th=0;th<NTHREAD;th++){
        	pthread_create(&t[th], NULL, &percorre,  (void *) newgrid);
    	}
    	for(th=0; th<NTHREAD; th++) {
    	    pthread_join(t[th],NULL);     
   		}

		ax = grid;
		grid = newgrid;
		newgrid = ax;
		ax = NULL;

		if(k<5) {
			printf("geração %d\n",k+1);
			imprime_tabuleiro(grid);
		}
	}
	free(newgrid);
}


int main(void){

	int th;
    pthread_t t[NTHREAD];//vetor de threads
	
	sem_init(&mutex, 0, 1);
	scanf("%d",&num_ger);
	grid = inicia_tabuleiro();
	geracoes();
	vivos = conta_vivos();
	printf("celulas vivas: %d\n",vivos);

	return 0;
}
