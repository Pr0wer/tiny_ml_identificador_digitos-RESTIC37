#include <stdio.h>
#include "pico/stdlib.h"
#include "tflm_wrapper.h"

#define TAMANHO_IMG 784
#define NUM_CLASSES 10 // Digitos 0-9

void show_menu_serial();

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    // Inicializa a Rede Neural Convolucional
    printf("Inicializando Rede Neural Convolucional...\n");
    if (tflm_init() != 0)
    {
        printf("Falha ao iniciar o modelo TFLite\n");
        while (true)
        {
            tight_loop_contents();
        }
    }
    printf("Modelo TFLite inicializado com sucesso!\n");
    sleep_ms(200);

    // Buffer para bytes da imagem e possibilidades
    float imagem[TAMANHO_IMG];
    float probabilidades[NUM_CLASSES];

    int byte;
    int digito_inferido;
    while (true) 
    {   
        show_menu_serial();

        // Aguarda pelo envio de todos os bytes (TAMANHO_IMG)
        for (int i = 0; i < TAMANHO_IMG; i++)
        {
            byte = getchar();

            // Normaliza e armazena na respectiva posição do buffer
            imagem[i] = byte / 255.0f;
        }

        printf("IMAGEM RECEBIDA! Processando...\n");
        
        // Faz a inferência com a imagem recebida
        digito_inferido = tflm_run_inference(imagem, probabilidades);
        if (digito_inferido >= 0)
        {
            printf("-=-=-=-=-=-=-=-=-\n");
            printf("Número %d identificado!\n", digito_inferido);
            printf("Confiança: %.2f\n", probabilidades[digito_inferido] * 100.0f);
            printf("-=-=-=-=-=-=-=-=-\n");
        }
        else
        {
            printf("Erro na execução da IA. Por favor, tente novamente.\n");
        }
    }
}

void show_menu_serial()
{
    printf("-=-=-=- Identificador de digitos com IA -=-=-=-\n");
    printf("Envie os bytes da sua imagem 28x28 de modo sequencial na entrada serial\n");
    printf("para identificação!\n");
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
}
