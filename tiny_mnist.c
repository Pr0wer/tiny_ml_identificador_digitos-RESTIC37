#include <stdio.h>
#include "pico/stdlib.h"
#include "tflm_wrapper.h"

#define TAMANHO_IMG 784
#define NUM_CLASSES 10 // Dígitos 0–9

int main()
{
    stdio_init_all(); // Inicializa USB para comunicação serial
    sleep_ms(3000);

    printf("=== Sistema TinyML BitDogLab (MNIST) ===\n");
    printf("Inicializando Rede Neural Convolucional...\n");

    if (tflm_init() != 0)
    {
        printf("Falha ao iniciar o modelo TFLite\n");
        while (true) tight_loop_contents();
    }

    printf("Modelo TFLite inicializado com sucesso!\n");
    printf("Aguardando imagem (784 bytes) via USB-CDC...\n");

    // Buffers
    float imagem[TAMANHO_IMG];
    float probabilidades[NUM_CLASSES];

    int byte;
    int digito_inferido;

    while (true)
    {
        // Recebe exatamente 784 bytes (binário)
        for (int i = 0; i < TAMANHO_IMG; i++)
        {
            byte = getchar();
            if (byte == EOF) {
                printf("Erro: leitura interrompida.\n");
                break;
            }
            imagem[i] = (float)byte / 255.0f; // Normaliza para 0.0–1.0
        }

        printf("Imagem recebida. Processando inferência...\n");

        // Inferência
        digito_inferido = tflm_run_inference(imagem, probabilidades);

        if (digito_inferido >= 0)
        {
            printf("\n--- RESULTADO ---\n");
            printf("Dígito identificado: %d\n", digito_inferido);
            printf("Confiança: %.2f%%\n", probabilidades[digito_inferido] * 100.0f);
            
            printf("--- FIM ---\n");
        }
        else
        {
            printf("Erro na inferência.\n");
        }

        printf("\nPronto para nova imagem...\n");
    }
}