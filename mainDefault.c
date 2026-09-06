#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

float velocidade(float vini, float acel, float ms) {
    return vini + acel * ms;
}

float posicao(float vini, float posini, float ms, float acel) {
    return posini + vini * ms + 0.5f * acel * ms * ms;
}

int main() {
    unsigned int numinter;
    float ms;
    float G = 0.00000000006674f;
    
    float **particula = malloc(400 * sizeof(float*));
    char buffer[100];
    
    if (fgets(buffer, 100, stdin) == NULL) {
        free(particula);
        return 1;
    }
    if (sscanf(buffer, "%u %f", &numinter, &ms) != 2) {
        free(particula);
        return 1;
    }
    
    int total = 0;
    for (int i = 0; i < 400; i++) {
        particula[i] = malloc(7 * sizeof(float));
        if (fgets(buffer, 100, stdin) == NULL) {
            free(particula[i]);
            particula[i] = NULL;
            break;
        }
        if (sscanf(buffer, "%f %f %f %f %f", &particula[i][0], &particula[i][1], &particula[i][2], &particula[i][3], &particula[i][4]) != 5) {
            free(particula[i]);
            particula[i] = NULL;
            break;
        }
        particula[i][5] = 0.0f;
        particula[i][6] = 0.0f;
        total++;
    }

    clock_t start_t, end_t;
    float ant = 0;

    for (int k = 0; k < numinter; k++) {
        start_t = clock();
        
        // Reseta as acelerações para o passo atual
        for (int i = 0; i < total; i++) {
            particula[i][5] = 0.0f;
            particula[i][6] = 0.0f;
        }
        
        // Cálculo da gravidade 2D usando a distância euclidiana correta
        for (int i = 0; i < total; i++) {
            for (int j = 0; j < total; j++) {
                if (i != j) {
                    float dx = particula[j][1] - particula[i][1];
                    float dy = particula[j][2] - particula[i][2];
                    float dist_sq = dx * dx + dy * dy;
                    
                    // Evita divisão por zero caso haja sobreposição
                    if (dist_sq > 1e-12f) {
                        float dist = sqrtf(dist_sq);
                        float f = G * particula[j][0] / (dist_sq * dist);
                        particula[i][5] += f * dx;
                        particula[i][6] += f * dy;
                    }
                }
            }
        }
        
        // Atualização de posições e velocidades
        for (int i = 0; i < total; i++) {
            float xi = particula[i][1];
            float yi = particula[i][2];
            float vxi = particula[i][3];
            float vyi = particula[i][4];
            
            particula[i][1] = posicao(vxi, xi, ms, particula[i][5]);
            particula[i][2] = posicao(vyi, yi, ms, particula[i][6]);
            particula[i][3] = velocidade(vxi, particula[i][5], ms);
            particula[i][4] = velocidade(vyi, particula[i][6], ms);
        }
        
        end_t = clock();
        float diff = (float)(end_t - start_t);
        if (ant == 0) {
            ant = diff;
        } else {
            ant = (ant + diff) / 2;
        }
    }
    
    printf("%.f\n", ant);
    for (int i = 0; i < total; i++) {
        printf("%.13f    %.13f    %.13f     %.13f\n", particula[i][1], particula[i][2], particula[i][3], particula[i][4]);
        free(particula[i]);
    }
    free(particula);
    
    return 0;
}