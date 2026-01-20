import serial
import time
import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt

# =================================================================
# CONFIGURAÇÕES
# =================================================================
PORTA_SERIAL = 'COM5' # Porta Serial conectada a placa
BAUD_RATE = 115200 
INDICE_IMAGEM = 0  # índice da imagem no Array de teste do MNIST (0 a 9999)

def main():
    # Carrega o Dataset MNIST 
    print("Carregando dataset MNIST...")
    (_, _), (X_test, y_test) = tf.keras.datasets.mnist.load_data()
    
    # Seleciona a imagem com base no índice definido
    img_original = X_test[INDICE_IMAGEM]
    rotulo_real = y_test[INDICE_IMAGEM]
    
    print(f"\nSelecionada imagem índice {INDICE_IMAGEM}")
    print(f"Número real: {rotulo_real}")

    # Mostra a imagem na tela para conferência
    plt.imshow(img_original, cmap='gray')
    plt.title(f"Enviando numero: {rotulo_real}")
    plt.axis('off')
    plt.show(block=False)
    plt.pause(2) # Mostra por 2 segundos e continua
    plt.close()

    # Prepara os dados para envio
    # Transforma matriz 28x28 em vetor de 784 posições
    img_flat = img_original.flatten().astype(np.uint8)
    
    # Converte para bytes puros para envio serial
    dados_para_enviar = img_flat.tobytes()
    
    # Verifica o tamanho (tem que ser exatamente 784)
    print(f"Tamanho dos dados: {len(dados_para_enviar)} bytes")

    # Comunicação Serial com a Placa
    try:
        print(f"Conectando na porta {PORTA_SERIAL}...")
        ser = serial.Serial(PORTA_SERIAL, BAUD_RATE, timeout=3)
        time.sleep(2) # Espera a placa reiniciar após conexão
        
        # Limpa qualquer lixo que esteja no buffer
        ser.reset_input_buffer()
        
        print("Enviando dados...")
        # Envia os 784 bytes de uma vez
        ser.write(dados_para_enviar)
        print("Envio concluído! Aguardando resposta do BitDogLab...")

        # Ler a resposta da placa
        start_time = time.time()
        while (time.time() - start_time) < 5: # Espera até 5 segundos por resposta
            if ser.in_waiting > 0:
                # Lê a linha e decodifica
                linha = ser.readline().decode('utf-8', errors='ignore').strip()
                if linha:
                    print(f"[PLACA]: {linha}")
                    # Se a placa confirmar a predição, podemos sair do loop
                    if "Confiança" in linha or "Número" in linha:
                        # Continua lendo um pouco mais para pegar todas as linhas
                        time.sleep(0.1) 
            else:
                time.sleep(0.1)

        ser.close()
        print("\nConexão encerrada.")

    except serial.SerialException:
        print(f"ERRO: Não foi possível abrir a porta {PORTA_SERIAL}.")
        print("Verifique se o cabo está conectado e se fechou outros terminais.")
    except Exception as e:
        print(f"Ocorreu um erro: {e}")

if __name__ == "__main__":
    main()