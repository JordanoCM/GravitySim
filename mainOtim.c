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
    float **particula = malloc(400 * sizeof(float*));
    float *auxvector = malloc(4 * sizeof(float)); 
    char buffer[100];

    if (fgets(buffer, 100, stdin) == NULL) return 1;
    if (sscanf(buffer, "%u %f", &numinter, &ms) != 2) return 1;   

    for(int i = 0; i < 400; i++){
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

    // Arquivo para salvar os quadros da animação
    FILE *f_hist = fopen("historico.txt", "w");

    float ant = 0;
    for(int k = 0; k < numinter; k++){
        start_t = clock();

        for(int i = 0; particula[i] != NULL; i++){
            particula[i][5] = 0;
            particula[i][6] = 0;
        }

        for(int i = 0; particula[i] != NULL; i++){
            for(int j = 0; particula[j] != NULL; j++){
                if(i != j){
                    aceleracaofast(particula[i], particula[j], &constante, auxvector);
                }
            }
        }

        for(int i = 0; particula[i] != NULL; i += 2){ 
            if(particula[i+1] == NULL){
                posicaofast(particula[i], particula[i], &ms, auxvector);
                velocidadefast(particula[i], particula[i], &ms, auxvector);
            }
            else{
                posicaofast(particula[i], particula[i+1], &ms, auxvector);
                velocidadefast(particula[i], particula[i+1], &ms, auxvector);
            }
        }

        // Salva a posição de todas as partículas a cada passo (ou a cada N passos se forem muitos)
        if (k % 10 == 0) { // Salva a cada 10 frames para otimizar
            fprintf(f_hist, "FRAME\n");
            for(int i = 0; particula[i] != NULL; i++){
                fprintf(f_hist, "%f %f\n", particula[i][1], particula[i][2]);
            }
        }

        end_t = clock();
        if(ant == 0) ant = (float)(end_t - start_t);
        else ant = (ant + (float)(end_t - start_t)) / 2;
    }

    fclose(f_hist);

    printf("%.f\n", ant);
    for(int i = 0; particula[i]!= NULL; i++){
        printf("%.13f    %.13f    %.13f     %.13f\n",particula[i][1],particula[i][2],particula[i][3],particula[i][4]);
    }

    for(int i = 0; particula[i] != NULL; i++) free(particula[i]);
    free(particula);
    free(auxvector);
    return 0;
}