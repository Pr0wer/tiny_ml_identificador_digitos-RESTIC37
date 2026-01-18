#ifndef PICO_TFLM_BRIDGE_H
#define PICO_TFLM_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

// Função para inicializar a IA (carregar modelo, alocar memória)
// Retorna 0 se sucesso, 1 se erro
int tflm_init(void);

// Função para rodar a inferência
// input_data: ponteiro para os 784 bytes/floats da imagem
// output_scores: ponteiro para onde salvar as 10 probabilidades
// Retorna o índice da classe vencedora (0-9)
int tflm_run_inference(const float* input_data, float* output_scores);

#ifdef __cplusplus
}
#endif

#endif // PICO_TFLM_BRIDGE_H