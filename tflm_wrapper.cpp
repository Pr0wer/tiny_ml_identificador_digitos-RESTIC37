#include "tflm_wrapper.h"
#include "mnist_model_data.h" // Seu modelo

// Bibliotecas TFLM
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"

// Variáveis Globais (para manter o estado entre as chamadas)
static const tflite::Model* model = nullptr;
static tflite::MicroInterpreter* interpreter = nullptr;
static tflite::MicroMutableOpResolver<10>* resolver = nullptr; // Ajuste o número se precisar
static uint8_t* tensor_arena = nullptr;
static const int kTensorArenaSize = 60 * 1024; // 60KB

// Função de Inicialização (C++)
int tflm_init(void) {
    model = tflite::GetModel(mnist_model_tflite);
    if (model->version() != TFLITE_SCHEMA_VERSION) return 1;

    // Alocar resolvedor na heap ou estático
    static tflite::MicroMutableOpResolver<10> static_resolver;
    static_resolver.AddConv2D();
    static_resolver.AddMaxPool2D();
    static_resolver.AddFullyConnected();
    static_resolver.AddReshape();
    static_resolver.AddSoftmax();
    static_resolver.AddRelu();
    
    resolver = &static_resolver;

    // Alocar arena
    static uint8_t static_tensor_arena[kTensorArenaSize];
    tensor_arena = static_tensor_arena;

    // Criar Intérprete
    static tflite::MicroInterpreter static_interpreter(
        model, *resolver, tensor_arena, kTensorArenaSize, nullptr);
    interpreter = &static_interpreter;

    if (interpreter->AllocateTensors() != kTfLiteOk) return 1;

    return 0; // Sucesso
}

// Função de Inferência (C++)
int tflm_run_inference(const float* input_data, float* output_scores) {
    TfLiteTensor* input = interpreter->input(0);
    TfLiteTensor* output = interpreter->output(0);

    // Copiar dados para o modelo
    for (int i = 0; i < 784; i++) {
        input->data.f[i] = input_data[i];
    }

    // Rodar
    if (interpreter->Invoke() != kTfLiteOk) return -1;

    // Pegar resultados
    float max_score = 0;
    int max_index = 0;
    for (int i = 0; i < 10; i++) {
        output_scores[i] = output->data.f[i];
        if (output_scores[i] > max_score) {
            max_score = output_scores[i];
            max_index = i;
        }
    }
    return max_index;
}