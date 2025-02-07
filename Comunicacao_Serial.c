#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include <string.h>
#include "font.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/clocks.h"
#include "led.pio.h"
#include "ssd1306.h"

#define endereco 0x3C

// Definições de pinos
#define LED_PIN_RED 13
#define LED_PIN_GREEN 11
#define LED_PIN_BLUE 12
#define BUTTON_A 5
#define BUTTON_B 6
#define I2C_SDA 14
#define I2C_SCL 15
#define I2C_PORT i2c1

// Variáveis globais
PIO pio; // Inicializa a estrutura da PIO
uint sm; // Inicializa a máquina de estado
ssd1306_t ssd; // Inicializa a estrutura do display

// Matriz
double led[10][25] = {
        {0, 1, 1, 1, 0,
         0, 1, 0, 1, 0,
         0, 1, 0, 1, 0,
         0, 1, 0, 1, 0,
         0, 1, 1, 1, 0,
        }, // 0
        {0, 0, 1, 0, 0,
         0, 1, 1, 0, 0,
         0, 0, 1, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
        }, // 1
        {0, 1, 1, 1, 0,
         0, 0, 0, 1, 0,
         0, 1, 1, 1, 0,
         0, 1, 0, 0, 0,
         0, 1, 1, 1, 0,
        },  // 2
        {0, 1, 1, 1, 0,
         0, 0, 0, 1, 0,
         0, 1, 1, 1, 0,
         0, 0, 0, 1, 0,
         0, 1, 1, 1, 0,
        }, // 3
        {0, 1, 0, 1, 0,
         0, 1, 0, 1, 0,
         0, 1, 1, 1, 0,
         0, 0, 0, 1, 0,
         0, 0, 0, 1, 0,
        }, // 4
        {0, 1, 1, 1, 0,
         0, 1, 0, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 0, 1, 0,
         0, 1, 1, 1, 0,
        }, // 5
        {0, 1, 1, 1, 0,
         0, 1, 0, 0, 0,
         0, 1, 1, 1, 0,
         0, 1, 0, 1, 0,
         0, 1, 1, 1, 0,
        },  // 6
        {0, 1, 1, 1, 0,
         0, 0, 0, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 1, 0, 0,
         0, 0, 1, 0, 0,
        }, // 7
        {0, 1, 1, 1, 0,
         0, 1, 0, 1, 0,
         0, 1, 1, 1, 0,
         0, 1, 0, 1, 0,
         0, 1, 1, 1, 0,
        }, // 8
        {0, 1, 1, 1, 0,
         0, 1, 0, 1, 0,
         0, 1, 1, 1, 0,
         0, 0, 0, 1, 0,
         0, 1, 1, 1, 0,
        }  // 9
    };

// Sequência de LEDs
int sequencia[25] = {
    0, 1, 2, 3, 4,
    9, 8, 7, 6, 5,
    10, 11, 12, 13, 14,
    19, 18, 17, 16, 15,
    20, 21, 22, 23, 24
};

// Melhorar a intensidade
uint32_t cores(double vermelho)
{
  unsigned char R;
  R = vermelho * 100; // Ajusta a intensidade do vermelho
  return (R << 16); // Retorna o valor do vermelho (deslocamento de 16 bits) 
}

// Converte os valores da matriz de LEDs para a matriz de LEDs da PIO
void display_num(int number) {
    for (int i = 0; i < 25; i++) {
        uint32_t valor_led = cores(led[number][sequencia[24 - i]]);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

static volatile uint num = 0; // Inicializa o número
static volatile uint32_t ultima = 0; // Inicializa o tempo do último evento

// Função de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time()); // Obtém o tempo atual

    // Inverte a cor
    bool cor = true;
    cor = !cor; 

    // Debounce: evita múltiplas leituras em um curto intervalo de tempo
    if (tempo_atual - ultima > 200) {
        if (gpio == BUTTON_A) { 
            //Mudar o estado do led verde ao pressionar o botão A
            gpio_put(LED_PIN_GREEN, !gpio_get(LED_PIN_GREEN));
            
            // Exibe uma mensagem no display
            ssd1306_fill(&ssd, !cor); // Limpa o display
            ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
            ssd1306_draw_string(&ssd, "BOTAO A", 20, 30); // Desenha uma stringstring      
            ssd1306_send_data(&ssd); // Atualiza o display
            
            // Exibe o número no serial monitor
            printf("Botão A pressionado, estado do LED alterado\n");

        } else if (gpio == BUTTON_B) {
            //Mudar o estado do led azul ao pressionar o botão B
            gpio_put(LED_PIN_BLUE, !gpio_get(LED_PIN_BLUE));
            printf("Botão B pressionado, estado do LED alterado\n");

            // Exibe uma mensagem no display
            ssd1306_fill(&ssd, !cor); // Limpa o display
            ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
            ssd1306_draw_string(&ssd, "BOTAO B", 20, 30); // Desenha uma stringstring      
            ssd1306_send_data(&ssd); // Atualiza o display
        }   
            // Registra o tempo do último evento
            ultima = tempo_atual;
        }
}

int main()
{
    // Inicializa o stdio
    stdio_init_all();

    // Inicializa os LEDs e os botões
    gpio_init(BUTTON_A);
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_pull_up(BUTTON_B);
    gpio_init(LED_PIN_RED);
    gpio_init(LED_PIN_GREEN);
    gpio_init(LED_PIN_BLUE);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    gpio_set_dir(LED_PIN_BLUE, GPIO_OUT);

    // Inicializa a PIO
    pio = pio0;
    uint offset = pio_add_program(pio, &led_program); 
    uint sm = pio_claim_unused_sm(pio, true);
    led_program_init(pio, sm, offset, 7);

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
 
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    char mensagem[2] = {0};  // Buffer para armazenar a mensagem

    // Configura a interrupção
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);


while (true) {

    int number;

    int c = getchar_timeout_us(1000000);  // Aguarda até 1 segundo por um caractere
    if (c != PICO_ERROR_TIMEOUT && c >= 32 && c <= 126) {  // Garante que o caractere é imprimível
        mensagem[0] = (char)c;
        mensagem[1] = '\0';  // Termina a string corretamente

    // Verifica se o caractere é um número
    if (c >= '0' && c <= '9') {
        number = c - '0'; // Converte o caractere para um número

        // Exibe o número no display a partir do switch case
        switch(number){
            case 0:
                display_num(0);
                break;
            case 1:
                display_num(1);
                break;
            case 2:
                display_num(2);
                break;
            case 3:
                display_num(3);
                break;
            case 4:
                display_num(4);
                break;
            case 5:
                display_num(5);
                break;
            case 6:
                display_num(6);
                break;
            case 7:
                display_num(7);
                break;
            case 8:
                display_num(8);
                break;
            case 9:
                display_num(9);
                break;
        }
    }

        // Limpa o display
        ssd1306_fill(&ssd, false);

        // Exibe o caractere capturado na posição (0, 0)
        char str[2] = {c, '\0'};
        ssd1306_draw_string(&ssd, str, 0, 0);

        // Atualiza o display
        ssd1306_send_data(&ssd);

        
        printf("Caractere exibido: %c\n", c);
    }

    sleep_ms(1000);  // Aguarda um pouco antes da próxima leitura
    
}
   

    return 0;
}

