# Módulo Sensor de Corrente(MSC25)

## Visão Geral

Este projeto consiste em uma placa de circuito impresso (PCB) com firmware embarcado, projetada para realizar a leitura de alta precisão de um sensor de corrente de efeito Hall e transmitir os dados através do barramento CAN.

O firmware foi desenvolvido em C para microcontroladores AVR, seguindo uma arquitetura modular, não-bloqueante e orientada por eventos, garantindo robustez e tempo de resposta previsível. O sistema é configurado e compilado utilizando um ambiente `avr-gcc` e `make`.

## Principais Características

* **Microcontrolador:** ATmega328P operando a 16MHz.
* **Comunicação:** Interface CAN Bus 2.0A (11-bit) implementada com controlador dedicado MCP2515 e transceptor MCP2561.
* **Leitura de Sensor:** Interface analógica de alta precisão para sensores de corrente, com condicionamento de sinal.
* **Fonte de Alimentação:** Sistema de alimentação robusto com fonte simétrica isolada para o estágio analógico.
* **Modos de Leitura:** Firmware com modos de operação configuráveis em tempo de compilação:
    * **Modo Média Móvel:** Fornece leituras estáveis, ideal para monitoramento de estado.
    * **Modo Instantâneo:** Fornece a última leitura bruta, ideal para captura de picos e transientes de corrente.
* **Arquitetura de Firmware:** Modular, baseada em máquina de estados e interrupções de hardware para tarefas periódicas.

## Detalhes do Hardware

O hardware foi projetado para garantir a integridade do sinal e a robustez da comunicação.

### Fonte de Alimentação

A placa recebe uma tensão de entrada de 18V, que é regulada internamente para os domínios digitais e analógicos:

* **+12V:** Gerado por um regulador LM7812 para alimentar periféricos e o estágio de amplificação.
* **+5V:** Gerado por um regulador LM7805 para alimentar o microcontrolador ATmega328P e o controlador CAN MCP2515.

#### Fonte Simétrica Isolada (±15V)

Para a correta polarização do sensor de corrente e do circuito de condicionamento analógico, foi implementada uma fonte simétrica de ±15V. Esta etapa utiliza dois conversores DC/DC isolados do tipo **PDSE1-S5-S15-S**. A partir da entrada de +5V, cada conversor gera uma tensão de saída isolada (+15V e -15V, respectivamente). A isolação galvânica garante que ruídos do barramento de alimentação principal não interfiram na precisão das medições analógicas.

### Sensor e Condicionamento de Sinal

O circuito foi projetado para interfacear com um sensor de corrente de efeito Hall, como o **SECOHR 200SCV**.

O sinal de saída do sensor é processado por um circuito de condicionamento baseado no amplificador operacional **LM324**. Este estágio tem a função de amplificar e deslocar o nível de tensão (offset) do sinal, ajustando-o para a faixa de leitura do conversor Analógico-Digital (ADC) do ATmega328P (0-5V). O resultado é um sinal robusto e centralizado, que permite a medição de correntes tanto positivas quanto negativas.

## Detalhes do Firmware

O firmware é escrito em C puro (padrão gnu99) e compilado com `avr-gcc`.

### Arquitetura

A lógica principal é controlada por uma máquina de estados (`machine.c`) que é executada em uma frequência fixa de 100 Hz, controlada pela interrupção do `Timer2`. A leitura do ADC é desacoplada da lógica principal, sendo disparada pelo `Timer0` a uma frequência de 1000 Hz e processada através de interrupções (`ISR(ADC_vect)`), garantindo que as amostragens ocorram de forma consistente. A comunicação entre a rotina de interrupção e o laço principal é feita através de flags voláteis para evitar condições de corrida.

### Módulos Principais

* **`main`**: Ponto de entrada, inicialização dos periféricos e laço principal.
* **`adc`**: Gerencia a configuração do ADC, o disparo por timer, a rotina de interrupção e as funções de processamento (média e leitura instantânea). Inclui um placeholder para **linearização** do sensor.
* **`machine`**: Contém a máquina de estados principal e a lógica de aplicação.
* **`can_app`**: Camada de aplicação do protocolo CAN. Responsável por montar e enviar as mensagens nos intervalos corretos.
* **`conf`**: Arquivo de cabeçalho central para todas as configurações de compilação, como ativação de módulos e seleção de modos de operação.

## Protocolo CAN

A placa se identifica na rede CAN e transmite duas mensagens principais.

* **Nome do Módulo:** `MSC25_1`
* **Assinatura (Signature):** `195` (0xC3)

| Mensagem | ID (Hex) | ID (Dec) | Frequência | Tamanho | Payload |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **State** | `0x79` | 121 | 1 Hz | 3 Bytes | `[Assinatura, Estado, Erro]` |
| **Measurements** | `0xF0` | 240 | 10 Hz | 3 Bytes | `[Assinatura, Corrente_L, Corrente_H]` |

## Ambiente de Desenvolvimento

* **Toolchain:** `avr-gcc`, `avr-libc`, `make`
* **Programador:** `avrdude` (configurado para `arduino`)
* **Biblioteca Principal:** `avr-can-lib` (deve estar presente na pasta `lib/`)

## Como Compilar e Gravar

1.  **Preparar Bibliotecas:** Execute o script para compilar a `avr-can-lib` e baixar o `can_ids.h` mais recente.
    ```bash
    ./makelibs.sh
    ```

2.  **Compilar o Projeto:** Use o comando `make` para compilar todo o código-fonte.
    ```bash
    make
    ```

3.  **Limpar o Projeto:** Para apagar os arquivos compilados.
    ```bash
    make clean
    ```

4.  **Gravar na Placa:** Para compilar (se necessário) e gravar o firmware no microcontrolador.
    ```bash
    make flash
    ```
5.  **Fazer Tudo (Limpar, Compilar, Gravar e Monitorar):**
    ```bash
    ./up.sh
    ```
