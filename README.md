# Projeto Embarcatech: Interrupção

## Descrição

Este projeto implementa o controle de uma matriz 5x5 de LEDs endereçáveis WS2812 usando um microcontrolador RP2040 na placa BitDogLab. Além disso, um LED RGB e dois botões são utilizados para interação do usuário. O objetivo é exibir números de 0 a 9 na matriz de LEDs e permitir a navegação entre eles utilizando os botões.

## Funcionalidades

- 🔴 O LED vermelho do LED RGB pisca continuamente 5 vezes por segundo.
- 🔼 O botão A incrementa o número exibido na matriz de LEDs.
- 🔽 O botão B decrementa o número exibido na matriz de LEDs.
- 🔢 A matriz de LEDs WS2812 exibe os números de 0 a 9 em um formato fixo.
- 🛠️ Implementação de debouncing via software para os botões.
- ⚡ Uso de interrupções (IRQ) para lidar com os eventos dos botões.

## Componentes Utilizados

- 🖥️ **Placa BitDogLab** com microcontrolador RP2040.
- 🔳 **Matriz 5x5 de LEDs WS2812** (conectada à GPIO 7).
-  **LED RGB** (conectado às GPIOs 11, 12 e 13).
- 🔘 **Botão A** (conectado à GPIO 5).
- 🔘 **Botão B** (conectado à GPIO 6).

## Estrutura do Código

O código é modularizado da seguinte forma:

- **`main()`**: Configura os pinos e inicializa o programa principal.
- **`pinos()`**: Inicializa os LEDs e botões.
- **`gpio_irq_handler()`**: Manipula as interrupções dos botões, incrementando ou decrementando o número exibido.
- **`numeros_matriz()`**: Define os padrões de exibição dos números na matriz de LEDs.
- **`cor(r, g, b)`**: Define a cor de um pixel.
- **`colorir(pixel_grb)`**: Envia o valor de cor para um pixel da matriz de LEDs.
- **`piscar_led()`**: Faz o LED vermelho piscar continuamente.

## Requisitos de Desenvolvimento

✅ Uso da biblioteca padrão do RP2040 para manipulação de GPIOs.
✅ Utilização da biblioteca PIO para controle dos LEDs WS2812.
✅ Implementação de interrupções (IRQ) para os botões.
✅ Implementação de debouncing via software para evitar leituras erradas nos botões.

## Como Executar 🚀

1️⃣ Compile o código utilizando um ambiente de desenvolvimento compatível com RP2040 (como o SDK do Raspberry Pi Pico).

2️⃣ Carregue o código na placa BitDogLab.

3️⃣ Pressione os botões A e B para alterar o número exibido na matriz de LEDs.
4️⃣ Observe o LED vermelho piscando constantemente.

## Conclusão 🎯

Este projeto combina hardware e software para demonstrar o uso de interrupções, manipulação de LEDs WS2812 e debouncing em botões, proporcionando uma aplicação prática e interativa do microcontrolador RP2040.

