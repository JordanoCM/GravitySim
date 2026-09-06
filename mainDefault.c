#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
clock_t start_t, end_t;
float aceleracao(float m2, float c1, float c2){
    if(((c2-c1)*(c2-c1))<0.000000000001)
        return 0;
    if(c2>c1)
        return  0.00000000006674*(m2/((c2-c1)*(c2-c1)));
    else
        return  -0.00000000006674*(m2/((c2-c1)*(c2-c1)));
}
float velocidade(float vini, float acel, float ms){
    return (vini + acel*(ms));
}
float posicao(float vini, float posini, float ms, float acel){
    return posini + vini*(ms) + acel*((ms)*(ms))*0.5;
}
int main(){
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
        for(int i=0; particula[i]!=NULL;i++){
            particula[i][5]=0;
            particula[i][6]=0;
        }
        for(int i=0; particula[i] != NULL; i++){
            for(int j=0;particula[j]!= NULL; j++){
                if(i!=j){
                    particula[i][5] += aceleracao(particula[j][0],particula[i][1],particula[j][1]);
                    particula[i][6] += aceleracao(particula[j][0],particula[i][2],particula[j][2]);
                }
        }
        }
        for(int i=0; particula[i] != NULL; i++){
            particula[i][1] = posicao(particula[i][3],particula[i][1],ms,particula[i][5]);
            particula[i][2] = posicao(particula[i][4],particula[i][2],ms,particula[i][6]);
            particula[i][3] = velocidade(particula[i][3],particula[i][5],ms);
            particula[i][4] = velocidade(particula[i][4],particula[i][6],ms);
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