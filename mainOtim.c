#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
extern void aceleracaofast(float *particula1, float *particula2, float *constante, float *auxvector);
extern void velocidadefast(float *particula1, float *particula2, float *ms, float *auxvector);
extern void posicaofast(float *particula1, float *particula2, float *ms, float *auxvector);
clock_t start_t, end_t;
int main(int argc, char *argv[]){
    unsigned int numinter;
    float ms;
    float constante = 0.00000000006674;
    float **particula = malloc(400*sizeof(float*));
    float *auxvector = malloc(4*sizeof(float)); 
    char buffer[100];
    if (fgets(buffer, 100, stdin) == NULL) {
        return 1;
    }
    if (sscanf(buffer, "%u %f", &numinter, &ms) != 2) {
        return 1;
    }   
    for(int i = 0;i<400; i++){
        *(particula + i) = malloc(7 * sizeof(float));
        if (fgets(buffer, 100, stdin) == NULL){
            *(particula + i) = NULL;
            break;
        }
        if (sscanf(buffer, "%f %f %f %f %f", &particula[i][0], &particula[i][1], &particula[i][2], &particula[i][3], &particula[i][4]) != 5) {
            *(particula + i) = NULL;
            break;
        }
        particula[i][5] = 0;
        particula[i][6] = 0;
    }
    float ant = 0;
    for(int k=0; k<numinter;k++){
        start_t = clock();
        int j=1;
        for(int i=0; particula[i]!=NULL;i++){
            particula[i][5]=0;
            particula[i][6]=0;
        }
        for(int i=0; particula[i] != NULL; i++){
            for(;particula[j]!= NULL; j++){
                if(i!=j){
                    aceleracaofast(particula[i],particula[j],&constante, auxvector);
            }
            }
            j=i+1;
        }
        for(int i=0; particula[i] != NULL; i+=2){ 
            if(particula[i+1]==NULL){
                float *placebo = malloc(7*sizeof(float));
                placebo[0] = 10;
                placebo[1] = 1;
                placebo[2] = 1;
                placebo[3] = 0;
                placebo[4] = 0;
                placebo[5] = 0;
                placebo[6] = 0;
                posicaofast(particula[i], placebo,&ms,auxvector);
                velocidadefast(particula[i], placebo,&ms,auxvector);
            }
            else{
                posicaofast(particula[i], particula[i+1],&ms, auxvector);
                velocidadefast(particula[i], particula[i+1],&ms, auxvector);
            }
        }
        end_t = clock();
        if(ant == 0){
            ant = (float)(end_t - start_t);
        }
        else{
            ant = (ant + (float)(end_t - start_t))/2;
        }
    }
    printf("%.f\n", ant);
    for(int i=0; particula[i]!= NULL; i++){
        printf("%.13f    %.13f    %.13f     %.13f\n",particula[i][1],particula[i][2],particula[i][3],particula[i][4]);
    }
}