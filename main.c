#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

// arquivo .pio
#include "main.pio.h"

// Limite X
double desenhoX[25] = {1.0, 0.0, 0.0, 0.0, 1.0,
                       0.0, 1.0, 0.0, 1.0, 0.0,
                       0.0, 0.0, 1.0, 0.0, 0.0,
                       0.0, 1.0, 0.0, 1.0, 0.0,
                       1.0, 0.0, 0.0, 0.0, 1.0};
// Numero 0
double desenho0[25] = {0.0, 0.2, 0.2, 0.2, 0.0,
                       0.0, 0.2, 0.0, 0.2, 0.0,
                       0.0, 0.2, 0.0, 0.2, 0.0,
                       0.0, 0.2, 0.0, 0.2, 0.0,
                       0.0, 0.2, 0.2, 0.2, 0.0};
// Numero 1
double desenho1[25] = {0.0, 0.0, 0.2, 0.0, 0.0,
                       0.0, 0.0, 0.2, 0.2, 0.0,
                       0.0, 0.0, 0.2, 0.0, 0.0,
                       0.0, 0.0, 0.2, 0.0, 0.0,
                       0.0, 0.2, 0.2, 0.2, 0.0};
// vetor para criar imagem na matriz de led - 2
double desenho2[25] = {0.0, 0.2, 0.2, 0.2, 0.0,
                       0.0, 0.2, 0.0, 0.0, 0.0,
                       0.0, 0.2, 0.2, 0.2, 0.0,
                       0.0, 0.0, 0.0, 0.2, 0.0,
                       0.0, 0.2, 0.2, 0.2, 0.0};
// Numero 3
double desenho3[25] = {0.0, 0.2, 0.2, 0.2, 0.0,
                       0.0, 0.2, 0.0, 0.0, 0.0,
                       0.0, 0.2, 0.2, 0.2, 0.0,
                       0.0, 0.2, 0.0, 0.0, 0.0,
                       0.0, 0.2, 0.2, 0.2, 0.0};
// Nuemro 4
double desenho4[25] = {0.0, 0.2, 0.0, 0.2, 0.0,
                       0.0, 0.2, 0.0, 0.2, 0.0,
                       0.0, 0.2, 0.2, 0.2, 0.0,
                       0.0, 0.2, 0.0, 0.0, 0.0,
                       0.0, 0.0, 0.0, 0.2, 0.0};
// Numero 5
double desenho5[25] = {0.0, 0.2, 0.2, 0.2, 0.0,
                       0.0, 0.0, 0.0, 0.2, 0.0,
                       0.0, 0.2, 0.2, 0.2, 0.0,
                       0.0, 0.2, 0.0, 0.0, 0.0,
                       0.0, 0.2, 0.2, 0.2, 0.0};
// Numero 6
double desenho6[25] = {0.0, 0.2, 0.2, 0.2, 0.0,
                       0.0, 0.0, 0.0, 0.2, 0.0,
                       0.0, 0.2, 0.2, 0.2, 0.0,
                       0.0, 0.2, 0.0, 0.2, 0.0,
                       0.0, 0.2, 0.2, 0.2, 0.0};
// Numero 7
double desenho7[25] = {0.0, 0.2, 0.2, 0.2, 0.0,
                       0.0, 0.2, 0.0, 0.0, 0.0,
                       0.0, 0.0, 0.0, 0.2, 0.0,
                       0.0, 0.2, 0.0, 0.0, 0.0,
                       0.0, 0.0, 0.0, 0.2, 0.0};
// Numero 8
double desenho8[25] = {0.0, 0.2, 0.2, 0.2, 0.0,
                       0.0, 0.2, 0.0, 0.2, 0.0,
                       0.0, 0.2, 0.2, 0.2, 0.0,
                       0.0, 0.2, 0.0, 0.2, 0.0,
                       0.0, 0.2, 0.2, 0.2, 0.0};
// Numero 9
double desenho9[25] = {0.0, 0.2, 0.2, 0.2, 0.0,
                       0.0, 0.2, 0.0, 0.2, 0.0,
                       0.0, 0.2, 0.2, 0.2, 0.0,
                       0.0, 0.2, 0.0, 0.0, 0.0,
                       0.0, 0.2, 0.2, 0.2, 0.0};

double *desenhos[] = {desenho0, desenho1, desenho2, desenho3, desenho4, desenho5, desenho6, desenho7, desenho8, desenho9, desenhoX};

// rotina para acionar a matrix de leds - ws2812b
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);

// Definição dos pinos dos LEDs e dos botões
#define LED_PIN_R 13
#define LED_PIN_G 11
#define LED_PIN_B 12
#define BTN_PIN_A 5
#define BTN_PIN_B 6
// número de LEDs
#define NUM_PIXELS 25
#define DEBOUNCE_DELAY 350 // Delay de debounce em milissegundos

// pino de saída
#define OUT_PIN 7

// Display
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
char c;
// LED no display

// Variável para debounce da interrupção
static volatile uint64_t last_interrupt_time = 0;
static volatile bool interrupt_flag = false; // Sinalizador da interrupção
static volatile int16_t a = 0;
static volatile bool btnA = false, btnB = false;
static volatile uint64_t last_interrupt_time_A = 0;
static volatile uint64_t last_interrupt_time_B = 0;

// Controle do display
bool cor = true;
ssd1306_t ssd;

// Função para inicialização dos pinos
void inicializar_pinos();

// Função para definir cores dos LEDs
void set_leds(bool red, bool green, bool blue);

// Função de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);

// Função para leds
void leds();

// Função para definir a intensidade das cores --> exemplo aula
uint32_t matrix_rgb(double b, double r, double g);

// Função de callback que será chamada quando a interrupção ocorrer
void on_uart_rx()
{
    // Lê o caractere recebido
    c = uart_getc(uart0);
}

// função principal
int main()
{
    // Declaração de variáveis de escopo
    PIO pio = pio0;
    uint16_t i;
    uint32_t valor_led;
    double r = 0.0, b = 0.0, g = 0.0;

    // Inicializa todos os códigos stdio padrão que estão ligados ao binário.
    stdio_init_all();
    inicializar_pinos();

    // Define a função de callback para a interrupção de recepção
    irq_set_exclusive_handler(UART0_IRQ, on_uart_rx);
    // Habilita a interrupção na UART0
    irq_set_enabled(UART0_IRQ, true);
    // Habilita a interrupção de recepção de dados (RX) na UART
    uart_set_irq_enables(uart0, true, false);

    // configurações da PIO
    uint offset = pio_add_program(pio, &main_program);
    uint sm = pio_claim_unused_sm(pio, true);
    main_program_init(pio, sm, offset, OUT_PIN);

    desenho_pio(desenho0, valor_led, pio, sm, r, g, b);

    // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd);                                         // Configura o display
    ssd1306_send_data(&ssd);                                      // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    while (true)
    {
        // leds();
        if (!gpio_get(BTN_PIN_A))
        {
            // Configuração da interrupção para o botão A (borda de subida e descida)
            gpio_set_irq_enabled_with_callback(BTN_PIN_A, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
            if (btnA)
            {
                ssd1306_draw_string(&ssd, "Verde Ligado", 5, 10);
            }
            else
            {
                ssd1306_draw_string(&ssd, "Verde Desligado", 5, 10);
            }
        }
        else if (!gpio_get(BTN_PIN_B))
        {
            // Configuração da interrupção para o botão B (borda de subida e descida)
            gpio_set_irq_enabled_with_callback(BTN_PIN_B, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
            if (btnB)
            {
                ssd1306_draw_string(&ssd, "Blue Ligado", 5, 10);
            }
            else
            {
                ssd1306_draw_string(&ssd, "Blue Desligado", 5, 10);
            }
        }
        // // Mostrar simbolos da matriz de leds quando digitados 0 ao 9
        // if (c == "0")
        // {
        //     // Chamada da função para imprimir os desenhos
        //     desenho_pio(desenhos[0], valor_led, pio, sm, r, g, b);
        // }
        // else if (c == "1")
        // {
        //     // Chamada da função para imprimir os desenhos
        //     desenho_pio(desenhos[1], valor_led, pio, sm, r, g, b);
        // }
        // else if (c == "2")
        // {
        //     // Chamada da função para imprimir os desenhos
        //     desenho_pio(desenhos[2], valor_led, pio, sm, r, g, b);
        // }
        // else if (c == "3")
        // {
        //     // Chamada da função para imprimir os desenhos
        //     desenho_pio(desenhos[3], valor_led, pio, sm, r, g, b);
        // }
        // else if (c == "4")
        // {
        //     // Chamada da função para imprimir os desenhos
        //     desenho_pio(desenhos[4], valor_led, pio, sm, r, g, b);
        // }
        // else if (c == "5")
        // {
        //     // Chamada da função para imprimir os desenhos
        //     desenho_pio(desenhos[5], valor_led, pio, sm, r, g, b);
        // }
        // else if (c == "6")
        // {
        //     // Chamada da função para imprimir os desenhos
        //     desenho_pio(desenhos[6], valor_led, pio, sm, r, g, b);
        // }
        // else if (c == "7")
        // {
        //     // Chamada da função para imprimir os desenhos
        //     desenho_pio(desenhos[7], valor_led, pio, sm, r, g, b);
        // }
        // else if (c == "8")
        // {
        //     // Chamada da função para imprimir os desenhos
        //     desenho_pio(desenhos[8], valor_led, pio, sm, r, g, b);
        // }
        // else if (c == "9")
        // {
        //     // Chamada da função para imprimir os desenhos
        //     desenho_pio(desenhos[9], valor_led, pio, sm, r, g, b);
        // }

        uart_putc(uart0, c);
        cor = !cor;
        // Atualiza o conteúdo do display com animações
        ssd1306_fill(&ssd, !cor);                     // Limpa o display
        ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
        ssd1306_draw_string(&ssd, "VERDE ", 5, 10);
        ssd1306_draw_string(&ssd, "BLUE ", 5, 25);
        ssd1306_draw_char(&ssd, c, 20, 48); // Imprime na tela o Char
        ssd1306_send_data(&ssd);            // Atualiza o display

        sleep_ms(100);
    }
}

void inicializar_pinos()
{
    stdio_init_all();

    // Inicialização dos LEDs como saída
    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);
    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);
    gpio_init(LED_PIN_B);
    gpio_set_dir(LED_PIN_B, GPIO_OUT);

    // Inicialização dos botões como entrada
    gpio_init(BTN_PIN_A);
    gpio_set_dir(BTN_PIN_A, GPIO_IN);
    gpio_pull_up(BTN_PIN_A); // Pull-up para o botão A

    gpio_init(BTN_PIN_B);
    gpio_set_dir(BTN_PIN_B, GPIO_IN);
    gpio_pull_up(BTN_PIN_B); // Pull-up para o botão B

    // Inicialização/Configuração do Display Oled
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA);                     // Pull up the data line
    gpio_pull_up(I2C_SCL);                     // Pull up the clock line

    // Inicializa a UART0 com baud rate de 115200
    uart_init(uart0, 115200);
    // Configura pino 0 como TX
    gpio_set_function(0, GPIO_FUNC_UART);
    // Configura pino 1 como RX
    gpio_set_function(1, GPIO_FUNC_UART);
    // Habilita o FIFO para evitar sobrecarga de buffer
    uart_set_fifo_enabled(uart0, true);
}
// Função para ligar os leds solicitados
void set_leds(bool red, bool green, bool blue)
{
    gpio_put(LED_PIN_R, red);
    gpio_put(LED_PIN_G, green);
    gpio_put(LED_PIN_B, blue);
}
// Tratamento de interrupções dos botões
static void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint64_t current_time = to_ms_since_boot(get_absolute_time()); // Obtém o tempo atual em milissegundos desde o boot do Raspberry Pi Pico

    if (gpio == BTN_PIN_A && (current_time - last_interrupt_time_A) > DEBOUNCE_DELAY) // Tratamento de Deboucing
    {
        btnA = !btnA;
        gpio_put(LED_PIN_G, btnA);
        last_interrupt_time_A = current_time;
    }
    else if (gpio == BTN_PIN_B && (current_time - last_interrupt_time_B) > DEBOUNCE_DELAY)
    {
        btnB = !btnB;
        gpio_put(LED_PIN_B, btnB);
        last_interrupt_time_B = current_time;
    }
}
// Pisca LEDs para indicar que o sistema está rodando
// void leds()
// {
//     set_leds(1, 0, 0);
//     sleep_ms(200);
//     set_leds(0, 0, 0);
//     sleep_ms(200);
// }
// Converte valores RGB para o formato aceito pela matriz
uint32_t matrix_rgb(double b, double r, double g)
{
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}
// Função para enviar o desenho para a matriz de LEDs
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(desenho[24 - i], desenho[24 - i], g = 0.0);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}
