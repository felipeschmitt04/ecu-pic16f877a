# Notas Tecnicas

## Objetivo Academico

O objetivo do projeto foi aplicar conceitos de microprocessadores e
microcontroladores em um sistema embarcado com multiplas entradas e saidas,
temporizacao em tempo real e interacao com hardware simulado.

## Firmware

O firmware esta concentrado em `Software-MPLab/ecu_final.X/main.c` e usa:

- configuracao de bits do PIC16F877A;
- ADC para leitura de sensores;
- Timer1 como base temporal do ciclo do motor;
- Timer2 para duracao do pulso de injecao;
- interrupcoes para acionamento dos eventos de injecao/ignicao;
- EEPROM interna para salvar o mapa ativo;
- LCD 16x2 para interface local.

## Entradas

| Entrada | Funcao |
| --- | --- |
| AN0 | TPS simulado |
| AN1 | RPM simulado |
| AN2 | Sonda lambda simulada |
| AN3 | Temperatura simulada |
| RB0 | Chave de ignicao |
| RB1 | Botao de partida |
| RB2 | Troca de mapa / modo de malha |
| RB3 | Menu do display |

## Saidas

| Saida | Funcao |
| --- | --- |
| RB4 | Bomba de combustivel |
| RB5 | Radiador |
| RB6 | LED check engine |
| RB7 | Motor de partida |
| RC0, RC1, RC2, RC5 | Bicos injetores |
| RE0, RE1 | Pares de ignicao |
| RD0-RD4, RD6 | LCD 16x2 |

## Logica de Controle

O valor de RPM e TPS seleciona uma posicao em uma tabela 16x16. O resultado
define o tempo base de injecao. A ECU tambem aplica regras para partida,
corte de giro, desaceleracao e correcao por sonda lambda no modo de malha
fechada.

## Pontos de Aprendizado

- Configuracao de perifericos em microcontrolador de 8 bits.
- Uso de interrupcoes para eventos temporizados.
- Tratamento de ADC e conversao de grandezas simuladas.
- Organizacao de mapas de calibracao.
- Integracao entre firmware, simulacao e PCB.
- Limites praticos de memoria em microcontroladores pequenos.

## Melhorias Futuras

- Separar o firmware em modulos menores.
- Trocar calculos com ponto flutuante por ponto fixo.
- Criar uma camada de abstracao para LCD e atuadores.
- Criar testes de logica para os mapas e limites.
- Documentar com imagens do Proteus e renders do KiCad.
- Revisar ERC/DRC e registrar os resultados no repositorio.

