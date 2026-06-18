/*
 * File:   main.c
 * Author: Felipe Luan Schmitt
 * Project: ECU
 * Subject: Microprocessadores e Microcontroladores
 * Course: Engenharia da Computacao
 * University: UFSC Campus Ararangua
 */

/*
 * BITS DE CONFIGURACAO
 */
#pragma config FOSC = HS        // Oscilador High-Speed (HS)
#pragma config WDTE = ON      // Watchdog Timer ON
#pragma config PWRTE = ON       // Power-up Timer ON
#pragma config BOREN = ON       // Brown-out Reset ON
#pragma config LVP = OFF        // Low-Voltage Programming OFF
#pragma config CPD = OFF        // Data EEPROM Code Protection OFF
#pragma config WRT = OFF        // Flash Program Memory Write Enable OFF
#pragma config CP = OFF         // Flash Program Memory Code Protection OFF

/*
 * INCLUDES
 */
#include <xc.h>
#include <pic16f877a.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define _XTAL_FREQ 20000000 //Frequencia do crystal a 20MHz

#include "lcd.h"

#define MAP_SIZE 256 

// --- MAPA 1: ECONÔMICO ---
const uint8_t MAPA_ECONOMICO[MAP_SIZE] = {
    20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50,
    22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52,
    24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 55,
    26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 55, 58,
    28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 55, 58, 61,
    30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 55, 58, 61, 65,
    32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 55, 58, 61, 65, 70,
    34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 55, 58, 61, 65, 70, 75,
    36, 38, 40, 42, 44, 46, 48, 50, 52, 55, 58, 61, 65, 70, 75, 80,
    38, 40, 42, 44, 46, 48, 50, 52, 55, 58, 61, 65, 70, 75, 80, 85,
    40, 42, 44, 46, 48, 50, 52, 55, 58, 61, 65, 70, 75, 80, 85, 90,
    42, 44, 46, 48, 50, 52, 55, 58, 61, 65, 70, 75, 80, 85, 90, 95,
    44, 46, 48, 50, 52, 55, 58, 61, 65, 70, 75, 80, 85, 90, 95, 100,
    46, 48, 50, 52, 55, 58, 61, 65, 70, 75, 80, 85, 90, 95, 100, 105,
    48, 50, 52, 55, 58, 61, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110,
    50, 52, 55, 58, 61, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 120
};

// --- MAPA 2: PERFORMANCE ---
const uint8_t MAPA_PERFORMANCE[MAP_SIZE] = {
    25, 28, 31, 34, 37, 40, 43, 46, 50, 55, 60, 65, 70, 75, 80, 85,
    28, 31, 34, 37, 40, 43, 46, 50, 55, 60, 65, 70, 75, 80, 85, 90,
    31, 34, 37, 40, 43, 46, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95,
    34, 37, 40, 43, 46, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 105,
    37, 40, 43, 46, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 105, 115,
    40, 43, 46, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 105, 115, 125,
    43, 46, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 105, 115, 125, 135,
    46, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 105, 115, 125, 135, 145,
    50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 105, 115, 125, 135, 145, 155,
    55, 60, 65, 70, 75, 80, 85, 90, 95, 105, 115, 125, 135, 145, 155, 165,
    60, 65, 70, 75, 80, 85, 90, 95, 105, 115, 125, 135, 145, 155, 165, 175,
    65, 70, 75, 80, 85, 90, 95, 105, 115, 125, 135, 145, 155, 165, 175, 185,
    70, 75, 80, 85, 90, 95, 105, 115, 125, 135, 145, 155, 165, 175, 185, 195,
    75, 80, 85, 90, 95, 105, 115, 125, 135, 145, 155, 165, 175, 185, 195, 205,
    80, 85, 90, 95, 105, 115, 125, 135, 145, 155, 165, 175, 185, 195, 205, 215,
    85, 90, 95, 105, 115, 125, 135, 145, 155, 165, 175, 185, 195, 205, 215, 225
};

/*
 * PINOS DA LCD.H
 */
#define D4 PORTDbits.RD0
#define D5 PORTDbits.RD1
#define D6 PORTDbits.RD2
#define D7 PORTDbits.RD3
#define RS PORTDbits.RD4
#define EN PORTDbits.RD6

/*
 * PINOS DO PIC
 */
//PORTA
#define FASE PORTAbits.RA4

//PORTB (entradas)
#define SW_IGNICAO PORTBbits.RB0
#define SW_PARTIDA PORTBbits.RB1
#define SW_MAPA PORTBbits.RB2
#define SW_MENU PORTBbits.RB3

//PORTB (saidas)
#define BOMBA_COMB PORTBbits.RB4
#define RADIADOR PORTBbits.RB5
#define LED_CHECK PORTBbits.RB6
#define MOTOR_PARTIDA PORTBbits.RB7

//PORTC
#define BICO_1 PORTCbits.RC0
#define BICO_2 PORTCbits.RC1
#define BICO_3 PORTCbits.RC2
#define BICO_4 PORTCbits.RC5

//PORTE
#define VELA_1_4 PORTEbits.RE0
#define VELA_2_3 PORTEbits.RE1

// RPM MAX
#define RPM_LIMITE 6500

/*
 * PROTOTIPO DE FUNCOES
 */
void setup_hardware(void);
void setup_adc(void);
void setup_timers(void);
void ler_botoes(void);
void ler_sensores(void);
void calcular_logica_motor(void);
void gerenciar_atuadores_aux(void);
void atualizar_display_lcd(void);
void desligar_sistema(void);
uint16_t read_adc(uint8_t channel);
void carregar_mapa_para_cache(uint8_t mapa_id);
void internal_eeprom_write(uint8_t addr, uint8_t data);
uint8_t internal_eeprom_read(uint8_t addr);
void itoa(uint16_t valor, char* buffer);

/*
 * VARIAVEIS GLOBAIS
 */
// flags de estado
volatile uint8_t g_ignicao_ligada = 0; //0 = desligado, controlado pelo RB0
volatile uint8_t g_motor_girando = 0; //1 = RPM > 500
volatile uint8_t g_modo_partida = 0; // 1 = botao de partida pressionado

// variaveis do motor
volatile uint16_t g_timer1_preload = 0; // valor de recarga do timer1
volatile uint8_t g_tempo_injecao_pr2 = 0; // valor de regarga do timer2

// maquina de estados do motor
static uint8_t s_engine_step = 0; // 0-3, representa o ciclo de 4 tempos do motor

// leitura de sensores
volatile uint16_t g_adc_tps = 0;
volatile uint16_t g_adc_rpm_sim = 0;
volatile uint16_t g_adc_temp = 0;
volatile uint16_t g_adc_sonda = 0;
volatile uint16_t g_rpm_real = 0;

// estados da interface
volatile uint8_t g_modo_display = 0;
volatile uint8_t g_mapa_ativo = 1; // 1 = ECO, 2 = PERF
volatile uint8_t g_modo_malha = 0; // 0 = ABERTA (Padrão), 1 = FECHADA

// timers de debounce
static uint8_t s_btn_menu_debounce = 0;
static uint8_t s_btn_mapa_debounce = 0;
static uint16_t s_btn_mapa_hold_timer = 0;

// estados do display
#define MODO_GERAL  0
#define MODO_MAPA   1
#define MODO_AVISOS 2

/*
 * ROTINA DE INTERRUPCAO
 */
/*
 * Função que gerencia os eventos em tempo real do motor. Possui a prioridade mais alta do sistema embarcado
 * e interrompe qualquer outra operação para garantir o funcionamento correto da injeção e ignição.
 * O Timer1 se baseia no RPM para acionar a ignição nas bobinas em pares (1-4 e 2-3) e iniciar a injeção no 
 * bico do pistão seguinte da ordem de combustão (1-3-4-2).
 * A interrupção do Timer2 ocorre quando o tempo de abertura do bico de injeção termina, assim, cortando
 * a alimentação dos injetores. Ele define a largura de pulso calculada com base na lógica do mapa de injeção
 */
void __interrupt() ISR(void)
{
    CLRWDT(); //limpar o WDT
    
    // interrupcao timer1 (relogio do motor)
    if(PIR1bits.TMR1IF){
        // recarrega o timer1 para a proxima interrupcao
        TMR1H = (g_timer1_preload >> 8);
        TMR1L = (g_timer1_preload & 0xFF);
        
        // condicao se a ignicao e motor estao ligados
        if(g_ignicao_ligada && g_motor_girando){
            // configura o timer2 para desligar o bico
            PR2 = g_tempo_injecao_pr2; // carrega o tempo de pulso
            T2CONbits.TMR2ON = 1; // liga o timer2
            
            // maquina de estados do motor
            switch(s_engine_step){
                case 0:
                    VELA_1_4 = 1; // pulsa nas velas 1 e 4
                    BICO_3 = 1; // abre o bico1
                    VELA_1_4 = 0;
                    break;
                
                case 1:
                    VELA_2_3 = 1;
                    BICO_4 = 1;
                    VELA_2_3 = 0;
                    break;
                    
                case 2:
                    VELA_1_4 = 1;
                    BICO_2 = 1;
                    VELA_1_4 = 0;
                    break;
                    
                case 3:
                    VELA_2_3 = 1;
                    BICO_1 = 1;
                    VELA_2_3 = 0;
                    break;
            }
            s_engine_step++;
            if (s_engine_step > 3) {
                s_engine_step = 0; // reseta o ciclo
            }
        }
        PIR1bits.TMR1IF = 0; // limpa flag do timer1
    }
    
    // interrupcao timer2 (fim do pulso de injecao)
    if(PIR1bits.TMR2IF){
        // desliga o timer2
        T2CONbits.TMR2ON = 0;
        
        // desliga todos os bicos
        BICO_1 = 0;
        BICO_2 = 0;
        BICO_3 = 0;
        BICO_4 = 0;
        
        PIR1bits.TMR2IF = 0; // limpa a flag
    }
}

/*
 * LOOP MAIN
 */
/*
 * Função principal da ECU, incializa o hardware, recupera funções salvas da EEPROM interna e gerencia
 * o ciclo de funcionamento. Opera em dois estados principais, ativo e em stand-by (quando o carro está desligado).
 * O WDT é limpo a cada ciclo.
 */
void main(void) {
    setup_hardware(); //configura TRIS,ADCON1, OPTION_REG
    
    Lcd_Init(); //inicializa o display
    Lcd_Clear(); //limpa a tela
    
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("ECU INICIADA"); //texto de inicializacao
    __delay_ms(1000);
    
    setup_adc(); // configura o conversor AD
    setup_timers(); // configura o timer1 (RPM) e timer2 (Pulso)
    
    INTCONbits.GIE = 1; // interrupcoes globais
    INTCONbits.PEIE = 1; //interrupcoes de perifericosc
    // INTCONbits.INTE = 1; // interrupcao externa (RB0)
    
    // persistencia de dados da eeprom interna para buscar o ultimo
    // estado ativo do mapa de injeção
    g_mapa_ativo = internal_eeprom_read(0);
    if(g_mapa_ativo != 1 && g_mapa_ativo != 2) {
    g_mapa_ativo = 1;
}
    
    Lcd_Clear();
    
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String("ECU DESLIGADA");
    g_ignicao_ligada = 0; // Garante que comece desligada

    while(1)
    {
        CLRWDT(); // limpa o WDT
        
        if (SW_IGNICAO == 0) 
        {
            if (g_ignicao_ligada == 0) { 
                g_ignicao_ligada = 1;
                Lcd_Clear(); 
            }

            ler_botoes(); // leitura dos botoes da interface
            ler_sensores(); // leitura dos sensores analogicos
            
            if (SW_PARTIDA == 0) g_modo_partida = 1;
            else g_modo_partida = 0;
            
            calcular_logica_motor(); // calcula o tempo de injeção com base nos mapas
            gerenciar_atuadores_aux(); // gerencia os atuadores 
            atualizar_display_lcd(); // atualiza o display
            __delay_ms(1);
        }
        else 
        {
            if (g_ignicao_ligada == 1) {
                g_ignicao_ligada = 0;
                desligar_sistema(); // garante que os atuadores fiquem desligados
            }
        }
    }
    return;
}

/*
 * FUNCOES DE LEITURA (ADC E BOTOES) 
 */
uint16_t read_adc(uint8_t channel){
    ADCON0bits.CHS = channel; //seleciona o canal (0-4)
    __delay_us(20);
    ADCON0bits.GO_nDONE = 1; // inicia a conversao
    while(ADCON0bits.GO_nDONE); // espera a conversao acabar
    return ((ADRESH << 8) | ADRESL); // retorna o resultado de 10 bits
}

void ler_sensores(void){
    g_adc_tps = read_adc(0); // AN0 = TPS
    g_adc_rpm_sim = read_adc(1); // AN1 = RPM simulado
    g_adc_sonda = read_adc(2); // AN2 = sonda lambda simulada
    g_adc_temp = read_adc(3); // AN3 = g_adc_temp
}

// --- FUNCAO DE LEITURA DOS BOTOES ---
void ler_botoes(void){
    // logica do botao MENU
    if (SW_MENU == 0 && s_btn_menu_debounce == 0) {
        s_btn_menu_debounce = 1; 
        g_modo_display++;
        
        if (g_modo_display > MODO_AVISOS) { 
            g_modo_display = MODO_GERAL; 
        }
    } else if (SW_MENU == 1) {
        s_btn_menu_debounce = 0;
    }
    // logica do botao menu, quando apertado troca entre mapas
    // quando segurado alterna entre malha aberta e fechada
    if (SW_MAPA == 0) { 
        if (s_btn_mapa_debounce == 0) {
            s_btn_mapa_debounce = 1; 
            s_btn_mapa_hold_timer = 0; 
        }
        
        if (s_btn_mapa_debounce == 1) {
            s_btn_mapa_hold_timer++;

            if (s_btn_mapa_hold_timer > 400) { 
                g_modo_malha = !g_modo_malha; 
                
                Lcd_Clear();
                Lcd_Set_Cursor(1,1);
                if(g_modo_malha) Lcd_Write_String("MALHA FECHADA");
                else Lcd_Write_String("MALHA ABERTA");
                __delay_ms(500); 
                Lcd_Clear();
                
                s_btn_mapa_debounce = 2; 
            }
        }
    } else { 
        if (s_btn_mapa_debounce == 1) { 
            if (g_mapa_ativo == 1) {
                g_mapa_ativo = 2;
            } else {
                g_mapa_ativo = 1;
            }
            if(g_motor_girando == 0){
                internal_eeprom_write(0, g_mapa_ativo);
            }
            
            Lcd_Clear(); 
        }
        
        s_btn_mapa_debounce = 0; 
        s_btn_mapa_hold_timer = 0;
    }
}

// --- CÁLCULOS DE MAPA E TIMERS ---
/*
 * Funcao que realiza os calculos matematicos e fisicos para determinar
 * o tempo de ignição e a quantidade de combustivel (tempo de injecao) com
 * base no mapa.
 * Faz interpolacao do TPS x RPM para buscar o tempo base.
 * Realiza correcao em tempo real com base na sonda lambda
 * Rev limiter para manter o giro maximo em 6500
 * Enriquecimento de mistura para partida com motor frio
 * Corte de combustível em desaceleracao
 */
void calcular_logica_motor(void) {
    g_rpm_real = (uint16_t)(g_adc_rpm_sim * 8); // converte a leitura adc para uma escada realista (0-1023 para 0-8184 RPM)

    // maquina de estados do motor
    if (g_motor_girando == 0) {
        if (g_modo_partida == 1 && g_rpm_real > 300) {
            g_motor_girando = 1;
        } else {
            g_motor_girando = 0;
            T1CONbits.TMR1ON = 0;
            s_engine_step = 0;
            BICO_1 = 0; BICO_2 = 0; BICO_3 = 0; BICO_4 = 0;
            return;
        }
    } else {
        // se a rotacao cair de 400 RPM o motor "morre"
        if (g_rpm_real < 400) {
            g_motor_girando = 0;
        }
    }

    // busca o RPM e o TPS e converte de 10 bits para 4 usando bitwise shift
    uint8_t rpm_index = g_adc_rpm_sim >> 6;
    uint8_t tps_index = g_adc_tps >> 6;
    uint16_t index_linear = (rpm_index * 16) + tps_index; // converte a matriz 2D para 1D
    
    uint8_t tempo_base;

    // seleciona o mapa
    if (g_mapa_ativo == 1) {
        tempo_base = MAPA_ECONOMICO[index_linear];
    } else {
        tempo_base = MAPA_PERFORMANCE[index_linear];
    }
    
    float tempo_final_float = (float)tempo_base;
    
    if (g_modo_malha == 1) {
        float afr_alvo = 14.7; // Alvo Fixo
        
        float afr_lido = 10.0 + ((float)g_adc_sonda * 10.0 / 1023.0); // conversao do ADC para o AFR (de 10 a 20)
        
        if (afr_lido > 9.0) {
            float correcao = afr_alvo / afr_lido; // calculo da correcao
            
            // limitador de correcao para impedir correcoes absurdas
            if (correcao > 1.30) correcao = 1.30; 
            if (correcao < 0.70) correcao = 0.70; 
            
            tempo_final_float = tempo_final_float / correcao;
        }
    }
    
    if (g_rpm_real > RPM_LIMITE) {
        tempo_final_float = 0; // corta injeção (tempo zero)
    }
    
    uint8_t tps_minimo = (g_adc_tps < 20); // considerando margem do sensor
    uint8_t rpm_cutoff = (g_rpm_real > 1800);
    
    if (tps_minimo && rpm_cutoff) {
        tempo_final_float = 0; // corta combustível para economizar
    }
    
    // enriquecimento de partida
    if (g_modo_partida) {
        tempo_final_float = tempo_final_float * 1.3;
    }
    
    // protecao contra overflow do timer2
    if (tempo_final_float > 255.0) tempo_final_float = 255.0;
    
    // calculo do timer1
    if (g_rpm_real > 0) {
        uint32_t ticks = 18750000 / g_rpm_real;
        g_timer1_preload = 65535 - (uint16_t)ticks;
    }
    
    g_tempo_injecao_pr2 = (uint8_t)tempo_final_float;
    T1CONbits.TMR1ON = 1;
}

/*
 * Funcao que controla os atuadores que nao dependem diretamente do giro do motor.
 * Controla a bomba de combustivel.
 * Acionar ventoinha do radiador quando a temperatura esta elevada.
 * Responsavel pelo motor de partida.
 * Diagnostico de alertas de falha.
 */
void gerenciar_atuadores_aux(void){
    static uint16_t bomba_prime_timer = 0;
    if(g_ignicao_ligada && !g_motor_girando && bomba_prime_timer == 0){
        bomba_prime_timer = 2000;
    }
    
    if(g_motor_girando){
        BOMBA_COMB = 1;
        bomba_prime_timer = 0; 
    } else if(bomba_prime_timer > 0){
        BOMBA_COMB = 1;
        bomba_prime_timer--;
    } else {
        BOMBA_COMB = 0; 
    }
    
    float temperatura = (float)g_adc_temp * 0.4887f;
    
    if(temperatura > 95.0){
        RADIADOR = 1; 
    } else if (temperatura < 90.0){ 
        RADIADOR = 0;
    }
    
    if(g_modo_partida && !g_motor_girando){
        MOTOR_PARTIDA = 1;
    } else {
        MOTOR_PARTIDA = 0;
    }
    
    uint8_t erro_tps = (g_adc_tps > 1020 || g_adc_tps < 3);
    uint8_t erro_temp_alta = (temperatura > 95.0); 
    uint8_t erro_corte_giro = (g_rpm_real > RPM_LIMITE); 
    
    if (erro_tps || erro_temp_alta || erro_corte_giro) {
        LED_CHECK = 1;
    } else {
        LED_CHECK = 0; 
    }
}

/*
 * Foi necessaria implementar para converter numero inteiro de 16 bits em
 * string de caracteres por conta da memoria sprintf consumir muita memoria.
*/
void itoa(uint16_t valor, char* buffer) {
    uint8_t i = 0;
    uint16_t divisor = 10000; // maximo divisor para 16 bits
    uint8_t digito_iniciado = 0; // flag para controlar os zeros a esquerda

    if (valor == 0) { // caso especial do valor ser zero absoluto
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    // loop de decomposicao decimal (milhar > centena > dezena > unidade)
    while (divisor > 0) {
        uint8_t digito = (valor / divisor) % 10; // extrai o digito mais significativo atual
        if (digito > 0 || digito_iniciado) { // so imprime se for > 0 ou algo ja ter sido imprimido antes
            buffer[i] = digito + '0';  // converte int para char com base na tabela ascii
            i++;
            digito_iniciado = 1; // marca a flag que comecaram numeros validos
        } else if (divisor == 1 && digito_iniciado == 0) { // tratamento apra o utimo digito ser zero
            buffer[i] = '0';
            i++;
        }
        divisor /= 10; // passa para a proxima casa decimal
    }
    buffer[i] = '\0'; // finaliza a string
}

/*
 * Funcao responsavel por atualizar o display.
 * Possui 3 modos principais, um geral, que possui dois submodos (sport e normal).
 * Um de informacoes sobre o mapa e a malha.
 * E outro sobre informacoes do motor, mostra temperatura e avisos de falhas.
 */
void atualizar_display_lcd(void){
    static uint8_t lcd_counter = 0;
    lcd_counter++;
    
    if(lcd_counter < 100) return;
    lcd_counter = 0;
    
    char lcd_buffer[17];
    char lcd_buffer2[17];
    char num_buffer[7];
    char *mapa_nome;
    
    switch(g_modo_display){
        case MODO_GERAL:
        {
            // modo sport
            if (g_mapa_ativo == 2) {
                Lcd_Set_Cursor(1, 1);
                
                uint8_t barras = (uint8_t)((uint32_t)g_rpm_real * 16 / 6500);
                
                if (barras > 16) barras = 16; 
                
                for (uint8_t i = 0; i < 16; i++) {
                    if (i < barras) {
                        Lcd_Write_Char(0xFF);
                    } else {
                        Lcd_Write_Char(' ');  
                    }
                }
                
                uint8_t tps_percent = (uint8_t)(((uint32_t)g_adc_tps * 100) / 1023);
                
                Lcd_Set_Cursor(2, 1);
                strcpy(lcd_buffer, "SPORT  TPS:");
                
                itoa(tps_percent, num_buffer);  
                strcat(lcd_buffer, num_buffer);
                strcat(lcd_buffer, "%");
                
                while(strlen(lcd_buffer) < 16) {
                    strcat(lcd_buffer, " ");
                }
                Lcd_Write_String(lcd_buffer);
            }
            
            // modo normal
            else {
                uint8_t tps_percent = (uint8_t)(((uint32_t)g_adc_tps * 100) / 1023);
                
                itoa(g_rpm_real, num_buffer);
                strcpy(lcd_buffer, "RPM: ");
                strcat(lcd_buffer, num_buffer);
                
                while(strlen(lcd_buffer) < 10) { 
                    strcat(lcd_buffer, " ");
                }

                strcat(lcd_buffer, "TPS:");
                itoa(tps_percent, num_buffer);
                strcat(lcd_buffer, num_buffer);
                strcat(lcd_buffer, "%");
                
                while(strlen(lcd_buffer) < 16) {
                    strcat(lcd_buffer, " ");
                }
                
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String(lcd_buffer);

                float afr_lido = 10.0 + ((float)g_adc_sonda * 10.0 / 1023.0);
                
                uint8_t afr_int = (uint8_t)afr_lido;
                uint8_t afr_dec = (uint8_t)((afr_lido - (float)afr_int) * 10.0f);
                
                itoa(afr_int, num_buffer);
                strcpy(lcd_buffer2, "AFR: ");
                strcat(lcd_buffer2, num_buffer);
                strcat(lcd_buffer2, ".");
                
                itoa(afr_dec, num_buffer);
                strcat(lcd_buffer2, num_buffer);

                while(strlen(lcd_buffer2) < 16) {
                    strcat(lcd_buffer2, " ");
                }

                Lcd_Set_Cursor(2, 1);
                Lcd_Write_String(lcd_buffer2);
            }
            break;
        }
        case MODO_MAPA:
        {
            Lcd_Set_Cursor(1, 1);
            if (g_mapa_ativo == 1) {
                Lcd_Write_String("MAPA: ECONOMICO ");
            } else {
                Lcd_Write_String("MAPA: PERFORMANCE");
            }

            Lcd_Set_Cursor(2, 1);
            if (g_modo_malha == 1) {
                Lcd_Write_String("MALHA: FECHADA  ");
            } else {
                Lcd_Write_String("MALHA: ABERTA   ");
            }
            break;
        }
        case MODO_AVISOS:
        {
            float temp = (float)g_adc_temp * 0.4887f;
            uint8_t temp_int = (uint8_t)temp;
            uint8_t temp_dec = (uint8_t)((temp - (float)temp_int) * 10.0f);
            
            strcpy(lcd_buffer, "TEMP: ");
            
            if (temp_int < 10) strcat(lcd_buffer, " "); 
            
            itoa(temp_int, num_buffer);
            strcat(lcd_buffer, num_buffer);
            strcat(lcd_buffer, ".");
            itoa(temp_dec, num_buffer);
            strcat(lcd_buffer, num_buffer);
            strcat(lcd_buffer, " C");
            
            while(strlen(lcd_buffer) < 16) strcat(lcd_buffer, " ");

            Lcd_Set_Cursor(1, 1);
            Lcd_Write_String(lcd_buffer);
            
            Lcd_Set_Cursor(2, 1);
            
            if (g_rpm_real > RPM_LIMITE) {
                Lcd_Write_String("CORTE DE GIRO!  ");
            }
            else if (temp > 95.0) {
                Lcd_Write_String("ALTA TEMPERATURA");
            }
            else if (g_adc_tps > 1020 || g_adc_tps < 3) {
                Lcd_Write_String("ERRO SENSOR TPS ");
            }
            else {
                Lcd_Write_String("SISTEMAS OK     ");
            }
            break;
        }    
    }
}

void desligar_sistema(void) {
    BOMBA_COMB = 0;
    RADIADOR = 0;
    MOTOR_PARTIDA = 0;
    LED_CHECK = 0;
    
    VELA_1_4 = 0;
    VELA_2_3 = 0;
    BICO_1 = 0;
    BICO_2 = 0;
    BICO_3 = 0;
    BICO_4 = 0;
    
    T1CONbits.TMR1ON = 0;
    T2CONbits.TMR2ON = 0;
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String("ECU DESLIGADA");
}

/*
 * FUNCOES PARA EEPROM INTERNA (AFR)
 */
void internal_eeprom_write(uint8_t addr, uint8_t data){
    // apontar para o endereco
    EEADR = addr;
    // coloca o dado no registrador de dados
    EEDATA = data;
    // habilita acesso a EEPROM
    EECON1bits.EEPGD = 0; // 0 = aponta para a EEPROM
    EECON1bits.WREN = 1; // 1 = habilita escrita
    
    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    INTCONbits.GIE = 1;
    
    while(EECON1bits.WR);
    EECON1bits.WREN = 0;
}

uint8_t internal_eeprom_read(uint8_t addr){
    EEADR = addr;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;
    return EEDATA;
}

/*
 * SETUP DE PORTS
 */
void setup_hardware(void){
    //zerar todas as saidas
    PORTA = 0x00; 
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    //configurar TRIS
    TRISA = 0b00111111; //RA0-RA4 como entradas
    TRISB = 0b00001111; //RB0-RB3 como entradas, RB4-RB7 como saidas
    TRISC = 0b10000000; //RX como entrada
    TRISD = 0b00000000; //todos saidas (LCD)
    TRISE = 0b00000000; //todos saidas (velas)
    
    //configurar ADCON1
    ADCON1 = 0x02; //RA0-RA3 analogicos, RA4 e PORTE digitais
    
    //configurar PULL-UP no PORTB
    OPTION_REGbits.nRBPU = 0;
    OPTION_REGbits.INTEDG = 0;
}

/*
 * SETUP CONVERSOR A/D
 */
void setup_adc(void){
    ADCON0 = 0b10000001; //configura modulo ADC
    ADCON1bits.ADFM = 1; //10 bits de precisao
}

/*
 * SETUP I2C
 */
void setup_i2c(void){
    //configurar baud rate
    //formula para 100 kHz eh (fosc / (4 * f_i2c)) - 1
    // (20000000 / (4 * 100000)) - 1
    SSPADD = 49;
    
    //configurar modo de operacao
    SSPCONbits.SSPM = 0b1000; //modo Mestre I2C
    SSPCONbits.SSPEN = 1; //habilita o modulo MSSP
    
    //configura o status
    SSPSTATbits.SMP = 1; //desabilita slew rate
    SSPSTATbits.CKE = 0; //configura o clock padrao
}

/*
 * SETUP TIMERS
 */
void setup_timers(void){
    //timer1
    T1CONbits.TMR1CS = 0; //clock interno
    T1CONbits.T1CKPS = 0b11; // prescaler 1:8
    T1CONbits.TMR1ON = 0; // o timer1 desligado
    PIE1bits.TMR1IE = 1; // interrupcao do timer1
    T1CONbits.T1OSCEN = 0;
    
    //timer2
    T2CONbits.T2CKPS = 0b10; // prescaler 1:16
    T2CONbits.TOUTPS = 0b1111; // postscaler 1:16
    T2CONbits.TMR2ON = 0; // timer2 desligado
    PIE1bits.TMR2IE = 1; // interrupcao do timer2
}