# Comunicação Serial

<h2> # Funcionalidades do Projeto</h2>
<h3>1. Modificação da Biblioteca font.h</h3>
<p>• Adicionar caracteres minúsculos à biblioteca font.h. Use sua criatividade para criar os novos
caracteres.</p>

<p> Adição no arquivo "font.h":</p>

```sh
// Adição de caracteres minusculos

0x20, 0x54, 0x54, 0x54, 0x78, 0x00, 0x00, 0x00, //a
0x7f, 0x48, 0x48, 0x48, 0x30, 0x00, 0x00, 0x00, //b
0x38, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00, //c
0x30, 0x48, 0x48, 0x48, 0x7f, 0x00, 0x00, 0x00, //d
0x38, 0x54, 0x54, 0x54, 0x18, 0x00, 0x00, 0x00, //e
0x08, 0x7e, 0x09, 0x09, 0x00, 0x00, 0x00, 0x00, //f
0x18, 0xa4, 0xa4, 0xa4, 0x7c, 0x00, 0x00, 0x00, //g
0x7f, 0x08, 0x08, 0x08, 0x70, 0x00, 0x00, 0x00, //h
0x00, 0x00, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00, //i
0x40, 0x80, 0x80, 0x7a, 0x00, 0x00, 0x00, 0x00, //j
0x7f, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x00, //k
0x00, 0x00, 0x7f, 0x40, 0x00, 0x00, 0x00, 0x00, //l
0x7c, 0x04, 0x78, 0x04, 0x78, 0x00, 0x00, 0x00, //m
0x7c, 0x08, 0x04, 0x04, 0x78, 0x00, 0x00, 0x00, //n
0x38, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00, //o
0xfc, 0x24, 0x24, 0x24, 0x18, 0x00, 0x00, 0x00, //p
0x18, 0x24, 0x24, 0x24, 0xfc, 0x00, 0x00, 0x00, //q
0x7c, 0x08, 0x04, 0x04, 0x08, 0x00, 0x00, 0x00, //r
0x48, 0x54, 0x54, 0x54, 0x24, 0x00, 0x00, 0x00, //s
0x04, 0x3f, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, //t
0x3c, 0x40, 0x40, 0x40, 0x7c, 0x00, 0x00, 0x00, //u
0x1c, 0x20, 0x40, 0x20, 0x1c, 0x00, 0x00, 0x00, //v
0x3c, 0x40, 0x30, 0x40, 0x3c, 0x00, 0x00, 0x00, //w
0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, //x
0x1c, 0xa0, 0xa0, 0xa0, 0x7c, 0x00, 0x00, 0x00, //y
0x44, 0x64, 0x54, 0x4c, 0x44, 0x00, 0x00, 0x00, //z
```
<p>Adição no arquivo "ssd1306.c"</p>

   ```sh
void ssd1306_draw_char(ssd1306_t *ssd, char c, uint8_t x, uint8_t y)
{
  uint16_t index = 0;
  char ver=c;
  if (c >= 'A' && c <= 'Z') 
  {
    index = (c - 'A' + 11) * 8; 
  }else  if (c >= '0' && c <= '9')
  {
    index = (c - '0' + 1) * 8; 
  } else if (c >= 'a' && c <= 'z') // Para letras minúsculas
  {
    index = (c - 'a' + 37) * 8; // Para letras minúsculas
  }
   ```
<h3>2. Entrada de caracteres via PC</h3>
<p>• Utilize o Serial Monitor do VS Code para digitar os caracteres.</p>
<p>• Cada caractere digitado no Serial Monitor deve ser exibido no display SSD1306.</p>

   ```sh
    int c = getchar_timeout_us(1000000); 
    if (c != PICO_ERROR_TIMEOUT && c >= 32 && c <= 126) {  
        mensagem[0] = (char)c;
        mensagem[1] = '\0'; 
    // Restante do código...
    }
   ```
   ```sh
    // Limpa o display
    ssd1306_fill(&ssd, false);

    // Exibe o caractere capturado na posição (0, 0)
    char str[2] = {c, '\0'};
    ssd1306_draw_string(&ssd, str, 0, 0);

    // Atualiza o display
    ssd1306_send_data(&ssd);

        
    printf("Caractere exibido: %c\n", c);
   ```

   <p>Quando um número entre 0 e 9 for digitado, um símbolo correspondente ao número deve ser exibido, também, na matriz 5x5 WS2812.</p>

   ```sh
    // Verifica se o caractere é um número
    if (c >= '0' && c <= '9') {
        number = c - '0'; 
        
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
   ```

<h3>3. Interação com o Botão A</h3>
<p>• Pressionar o botão A deve alternar o estado do LED RGB Verde (ligado/desligado).</p>
<p>• A operação deve ser registrada de duas formas:</p>
o Uma mensagem informativa sobre o estado do LED deve ser exibida no display SSD1306</p>
o Um texto descritivo sobre a operação deve ser enviado ao Serial Monitor.</p>

   ```sh
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

    }
   ```
<h3>4. Interação com o Botão B</h3>
<p>• Pressionar o botão B deve alternar o estado do LED RGB Azul (ligado/desligado).</p>
<p>• A operação deve ser registrada de duas formas:</p>
o Uma mensagem informativa sobre o estado do LED deve ser exibida no display SSD1306</p>
o Um texto descritivo sobre a operação deve ser enviado ao Serial Monitor.</p>

   ```sh
else if (gpio == BUTTON_B) {
    //Mudar o estado do led azul ao pressionar o botão B
    gpio_put(LED_PIN_BLUE, !gpio_get(LED_PIN_BLUE));
    printf("Botão B pressionado, estado do LED alterado\n");

    // Exibe uma mensagem no display
    ssd1306_fill(&ssd, !cor);
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); 
    ssd1306_draw_string(&ssd, "BOTAO B", 20, 30);     
    ssd1306_send_data(&ssd); 
}  
   ```

