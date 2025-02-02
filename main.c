#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "main.pio.h"

#define FREQUENCY 800000 
#define NUM_PIXELS 25 

// PINOS
#define WS2812 7
#define LED_GREEN 11
#define LED_BLUE 12
#define LED_RED 13
#define BUTTON_A 5
#define BUTTON_B 6

uint8_t RED = 0; // Intensidade vermelho
uint8_t GREEN = 0; // Intensidade verde
uint8_t BLUE = 12; // Intensidade azul

static volatile int num = 0; // Número exibido na matriz
static volatile uint32_t last_time_contador = 0; // Armazena o tempo do contador 
static volatile uint32_t last_time_led = 0; // Armazena o tempo do led

//Funções
void pinos();
void numeros_matriz(); // Exibe o numero na matriz de LEDs
static void gpio_irq_handler(uint gpio, uint32_t events); // Interrupções
static inline uint32_t cor(uint8_t r, uint8_t g, uint8_t b); // Cor dos pixels
static inline void colorir(uint32_t pixel_grb); // Colore os pixels da matriz
void piscar_led(); // Pisca o LED vermelho

    

int main(){

    stdio_init_all();
    pinos();
    uint offset = pio_add_program(pio0, &ws2812_program);
    ws2812_program_init(pio0, 0, offset, WS2812, 800000);
    numeros_matriz();

    // Configuração da interrupção com callback
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (true) {
        piscar_led();
    }

}

void numeros_matriz(){
    
    bool numero[10][NUM_PIXELS] = {

    //0
    {0, 1, 1, 1, 0, 
     0, 1, 0, 1, 0, 
     0, 1, 0, 1, 0, 
     0, 1, 0, 1, 0, 
     0, 1, 1, 1, 0},

    //1
    {0, 0, 1, 0, 0, 
     0, 0, 1, 0, 0, 
     0, 0, 1, 0, 0, 
     0, 0, 1, 0, 0, 
     0, 0, 1, 0, 0},

    //2
    {0, 1, 1, 1, 0, 
     0, 1, 0, 0, 0, 
     0, 1, 1, 1, 0, 
     0, 0, 0, 1, 0, 
     0, 1, 1, 1, 0},

    //3
    {0, 1, 1, 1, 0, 
     0, 0, 0, 1, 0, 
     0, 1, 1, 1, 0, 
     0, 0, 0, 1, 0, 
     0, 1, 1, 1, 0},

    //4
    {0, 1, 0, 0, 0, 
     0, 0, 0, 1, 0, 
     0, 1, 1, 1, 0, 
     0, 1, 0, 1, 0, 
     0, 1, 0, 1, 0},

    //5
    {0, 1, 1, 1, 0, 
     0, 0, 0, 1, 0, 
     0, 1, 1, 1, 0, 
     0, 1, 0, 0, 0, 
     0, 1, 1, 1, 0},

    //6
    {0, 1, 1, 1, 0, 
     0, 1, 0, 1, 0, 
     0, 1, 1, 1, 0, 
     0, 1, 0, 0, 0, 
     0, 1, 1, 1, 0},

    //7
    {0, 1, 0, 0, 0, 
     0, 0, 0, 1, 0, 
     0, 1, 0, 0, 0, 
     0, 0, 0, 1, 0, 
     0, 1, 1, 1, 0},

    //8
    {0, 1, 1, 1, 0, 
     0, 1, 0, 1, 0, 
     0, 1, 1, 1, 0, 
     0, 1, 0, 1, 0, 
     0, 1, 1, 1, 0},

    //9
    {0, 1, 1, 1, 0, 
     0, 0, 0, 1, 0, 
     0, 1, 1, 1, 0, 
     0, 1, 0, 1, 0, 
     0, 1, 1, 1, 0},
     
     };

    uint32_t var_cor = cor(RED, GREEN, BLUE);
    for (int i = 0; i < NUM_PIXELS; i++){

        if (numero[num][i])
            colorir(var_cor); 
        else
            colorir(0);  
    }    
}

void pinos(){

    // Inicializa LEDs
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_put(LED_GREEN, 0);
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_put(LED_BLUE, 0);
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_put(LED_RED, 0);
    // Inicializa botões A e B
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);          
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);          

}


static inline uint32_t cor(uint8_t r, uint8_t g, uint8_t b){

    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b); 

}


static inline void colorir(uint32_t pixel_grb){

    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u); 

}


void gpio_irq_handler(uint gpio, uint32_t events){

    uint32_t current_time = to_us_since_boot(get_absolute_time());
    
    // Verifica se passou tempo suficiente desde o último evento
    if(current_time - last_time_contador > 300000){ // 300 ms de debouncing

        if(gpio == BUTTON_A && num < 9)
            printf("Novo numero = %d\n", ++num);
        if(gpio == BUTTON_B && num > 0)
            printf("Novo numero = %d\n", --num);
        last_time_contador = current_time; 
        numeros_matriz();
    }
}


void piscar_led(){

    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if(current_time - last_time_led > 100000){
        gpio_put(LED_RED, !(gpio_get(LED_RED)));
        last_time_led = current_time; 
    }
}


