/*
desenvolvido por:
André Filipe Siqueira Tokumoto
Jogo da vida,paralelo(opm) em C
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <assert.h>

#define TAM_TAB 2048
#define NTHREAD 1    //numero maximo de threads 
int **grid;

void imprime_tabuleiro(int**grid){

	int i,j;
	for(i=0;i<50;i++){	
		for(j=0;j<50;j++){
	           printf("%d ",grid[i][j]);
		}
		printf("\n");
	}
}

int **inicia_tabuleiro(){//cria o tabuleiro com as condições iniciais

	int **ngrid;
	int i,j;
	
	ngrid = (int**)malloc(TAM_TAB * sizeof(int*));//cria as colunas 
	
	for(i=0;i<TAM_TAB;i++)
		ngrid[i] = (int*)malloc(TAM_TAB * sizeof(int));//aloca uma linha por coluna, formando o tabuleiro NxN
	
	for(i=0;i<TAM_TAB;i++){//inicia todas as posições com zero
	
		for(j=0;j<TAM_TAB;j++){
		
			ngrid[i][j]=0;
		}
	}
	
	
	//GLIDER
	int lin = 1, col = 1;
	ngrid[lin ][col+1] = 1;
	ngrid[lin+1][col+2] = 1;
	ngrid[lin+2][col ] = 1;
	ngrid[lin+2][col+1] = 1;
	ngrid[lin+2][col+2] = 1;

	//R-pentomino
	lin =10; col = 30;
	ngrid[lin ][col+1] = 1;
	ngrid[lin ][col+2] = 1;
	ngrid[lin+1][col ] = 1;
	ngrid[lin+1][col+1] = 1;
	ngrid[lin+2][col+1] = 1;
	
	
	return ngrid;
}

int getNeighbors( int**grid,int i, int j){ //retorna o numero de vizinhos vivos da celula na posição (i,j)

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


int conta_vivos(int**grid){//conta o numero de celulas vivas no tabuleiro

	int i,j,total=0;
	
    #pragma omp parallel num_threads(NTHREAD) private(j) reduction (+:total)
	{
    #pragma omp for
	    for(i=0;i<TAM_TAB;i++){
		    for(j=0;j<TAM_TAB;j++){
			    total = total + grid[i][j];
		    }

	    }
	} 
	return total;
}

void geracoes(int num_ger){

	int i,j,k,v_vivos;
	int **newgrid,**ax;

	newgrid = (int**)malloc(TAM_TAB * sizeof(int*));//cria as colunas 
	
	for(i=0;i<TAM_TAB;i++)
		newgrid[i] = (int*)malloc(TAM_TAB * sizeof(int));//aloca uma linha por coluna, formando o tabuleiro NxN

	for(k=0;k<num_ger;k++){//passa as gerações

        #pragma omp parallel num_threads(NTHREAD) private(j,v_vivos) shared(newgrid)
		
        #pragma omp for	
		    for(i=0;i<TAM_TAB-1;i++){
			
			    for(j=0;j<TAM_TAB-1;j++){
				
				    v_vivos = getNeighbors(grid,i,j);

				    if(grid[i][j]==1){//celula esta viva
				
					    if(v_vivos < 2)   newgrid[i][j]=0;

					    else if(v_vivos > 3) newgrid[i][j]=0;	

					    else newgrid[i][j]=1;

				    }
				    else{//celula esta morta
					
					    if(v_vivos == 3) newgrid[i][j]= 1;
					    else newgrid[i][j]= 0;
			    	}				
			    }
		    }
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



int main(void){

	int n,vivos;
	double start, end;
    start = omp_get_wtime();//inicio contagem de tempo

	scanf("%d",&n);
	grid = inicia_tabuleiro();
	geracoes(n);
	vivos = conta_vivos(grid);
	printf("celulas vivas: %d\n",vivos);
  	end = omp_get_wtime();
    printf(" tempo %f seconds.\n", end-start);

	return 0;
}
