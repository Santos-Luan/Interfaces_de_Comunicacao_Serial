# Projeto: Exibição de Números em Matriz de LEDs e Display OLED com Raspberry Pi Pico

## Descrição
Este projeto utiliza um Raspberry Pi Pico para exibir números em uma matriz de LEDs WS2812B e em um display OLED SSD1306. O sistema permite a seleção dos números através de entrada via UART e botões físicos, com debounce implementado.

## Recursos Principais
- Exibição de números de 0 a 9 na matriz de LEDs.
- Controle de LEDs RGB via botões.
- Atualização dinâmica do display OLED com entrada serial (UART).
- Debounce para botoes físicos.
- Uso da PIO para controle dos LEDs WS2812B.

## Hardware Necessário
- Raspberry Pi Pico
- Matriz de LEDs WS2812B (5x5)
- Display OLED 128x64 (SSD1306)
- Botões de entrada (2 unidades)
- Resistores Pull-up
- Fios jumpers

## Conexões
| Componente     | Pino Raspberry Pi Pico |
|---------------|----------------------|
| LED Matriz WS2812B | GPIO 7 |
| Botão A       | GPIO 5 |
| Botão B       | GPIO 6 |
| LED Vermelho  | GPIO 13 |
| LED Verde     | GPIO 11 |
| LED Azul      | GPIO 12 |
| OLED SDA      | GPIO 14 |
| OLED SCL      | GPIO 15 |
| UART TX       | GPIO 0 |
| UART RX       | GPIO 1 |

## Bibliotecas Utilizadas
- **pico/stdlib.h**: Funções padrão para o Raspberry Pi Pico.
- **hardware/pio.h**: Interface para controle do PIO.
- **hardware/gpio.h**: Manipulação de GPIOs.
- **hardware/uart.h**: Controle da comunicação UART.
- **hardware/i2c.h**: Controle do barramento I2C para comunicação com o display OLED.
- **ssd1306.h**: Biblioteca para controle do display OLED SSD1306.
- **font.h**: Definições de fontes para exibição no display.

## Compilação e Execução
### 1. Instalar o SDK do Raspberry Pi Pico
Certifique-se de que o **Pico SDK** está corretamente instalado e configurado.

### 2. Clonar o Repositório
```sh
 git clone <URL_DO_REPOSITORIO>
 cd <PASTA_DO_PROJETO>
```

### 3. Compilar o Projeto
```sh
mkdir build
cd build
cmake ..
make
```

### 4. Upload do Arquivo para o Pico
Conecte o Raspberry Pi Pico no modo USB e copie o arquivo **.uf2** gerado para ele.

### 5. Monitorar a Saída Serial
Para visualizar as mensagens enviadas pelo Raspberry Pi Pico:
```sh
screen /dev/ttyUSB0 115200  # Linux/MacOS
```
Ou utilize um software como PuTTY no Windows.

## Uso
1. Pressione os botões para alterar os LEDs RGB.
2. Envie números de 0 a 9 pela UART para exibir na matriz de LEDs.
3. O display OLED irá atualizar conforme a entrada via UART.

4. Segue o link para o video: https://drive.google.com/file/d/1B1E8OY5NJ-KH8OHlO2rHJjeO9Y8lP9_7/view?usp=drivesdk

