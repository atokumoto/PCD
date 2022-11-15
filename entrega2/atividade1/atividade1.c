#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NT 2//numero de threads
int soma;
int req = 0;
int res = 0;

void secao_critica(int n) {//procedimento de acesso a seção critica
    int local = soma;
    sleep(rand()%2);
    soma = local + 1;

    printf("(%d) SOMA: %d\n", n, soma);
}

void *cliente(void *n) {

    int t = (int)n;

    while (1 == 1) {

        while (res != t) 
                req = t;

        secao_critica(t);

        res = 0;
    }
}

void *servidor() {
    while (1 == 1) {
        while (req == 0);
        res = req;

        while (res != 0);
        req = 0;
    }
}

int main(){

    int i,k;
    pthread_t tc[NT],ts[NT];

    for(i=0;i<NT;i++){
        pthread_create(&ts[i], NULL, &servidor, NULL);
    }

    for(i=0;i<NT;i++){
        k = i+1;
        pthread_create(&tc[i], NULL, &cliente, (void *)k);
    }

    for(i=0;i<NT;i++){
        pthread_join(ts[i], NULL);
    }

    for(i=0;i<NT;i++){
        pthread_join(tc[i], NULL);
    }

 return 0;
}