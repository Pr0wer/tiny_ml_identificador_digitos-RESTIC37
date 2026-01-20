# Classificação de Dígitos MNIST com TinyML no RP2040 (BitDogLab)

**Alunos:** Emyle Santana da Silva, Davi Dantas Mendez Sanchez e Juan Pablo Azevedo Sousa  

---

## Objetivo Geral
Implementar um pipeline completo de TinyML para classificar dígitos manuscritos (0-9) do dataset MNIST. O sistema executa a inferência de uma Rede Neural Convolucional (CNN) localmente no microcontrolador RP2040, recebendo a imagem enviada por um computador via USB e retornando a predição em tempo real.

---

## Descrição Funcional
O sistema opera através de uma arquitetura **Host-Device** via comunicação Serial:

### **1. Computador**
- Executa um script em Python (`script.py`) responsável por:
  - Carregar o dataset MNIST de teste.
  - Selecionar uma imagem (28x28 pixels).
  - Converter a matriz da imagem em um vetor linear de 784 bytes brutos (uint8).
  - Enviar os bytes sequencialmente para a placa via porta Serial.

### **2. RP2040 / BitDogLab**
- O firmware (`tiny_mnist.c`) aguarda o recebimento do buffer de 784 bytes.
- **Pré-processamento:** Realiza a normalização dos dados recebidos (divisão por 255.0f) para converter de inteiros para ponto flutuante, formato esperado pelo modelo.
- **Inferência:** Executa o interpretador do **TensorFlow Lite Micro** sobre o modelo treinado.
- **Saída:** Retorna via Serial o dígito identificado e o nível de confiança da predição.

---

## Estrutura do Código

- **`tiny_mnist.c`** Arquivo principal em C. Gerencia a inicialização da USB/UART, o loop de recepção dos bytes da imagem e chama as funções de inferência.

- **`tflm_wrapper.cpp` & `.h`** Camada de abstração (Wrapper) em C++ que integra o código C com a biblioteca TensorFlow Lite Micro.
  - *Configuração:* Define a **Tensor Arena** (ajustada para 130KB neste projeto) para alocar memória suficiente para o modelo Float32.

- **`mnist_model_data.h`** Arquivo de cabeçalho contendo o modelo da rede neural (treinado no Google Colab) convertido para um array de bytes hexadecimal (`unsigned char`).

- **`script.py`** Script de automação de testes no PC. Permite visualizar a imagem enviada e monitorar a resposta da placa.

---

## Detalhes Técnicos do Modelo
- **Arquitetura:** Rede Neural Convolucional (Conv2D + MaxPooling + Dense).
- **Formato:** TensorFlow Lite (`.tflite`) exportado como Array C.
- **Quantização:** Não utilizada (**Float32**) para manter a precisão original do treinamento no Keras.
- **Tamanho do Modelo:** ~90 KB (Armazenado na Flash).
- **Entrada:** Tensor de [1, 28, 28, 1].

---

## ⚙️ Instalação e Uso

### 1. Pré-requisitos
- **VS Code** configurado com as extensões Raspberry Pi Pico e C/C++.
- **Python 3.x** instalado no computador.
- Bibliotecas Python necessárias:
```bash
  pip install pyserial numpy tensorflow matplotlib
```
### 2. Compilação e Gravação 
1. Clone o repositório e abra no VS Code.
2. 
   - Crie a pasta de build e compile:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```
   - Ou use o botão “Build” do VS Code com a extensão Raspberry Pi Pico.
3. Conecte a placa BitDogLab segurando o botão **BOOTSEL**.
4. Copie o arquivo `.uf2` gerado para o drive `RPI-RP2`.

### 3. Execução dos Testes (Software)
1. Identifique a porta COM da placa (ex: `COM3` no Windows ou `/dev/ttyACM0` no Linux).
2. No arquivo `script.py`, ajuste a variável `PORTA_SERIAL` conforme necessário.
3. Execute o script:
   ```bash
   python script.py
   ```
   - O terminal exibirá a imagem enviada e a resposta da placa, por exemplo:
  > `[PLACA]: Número 7 identificado!`

  > `[PLACA]: Confiança: 99.8%`
