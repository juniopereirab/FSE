# Projeto 2 - FSE

## Introdução

Este projeto tem por objetivo a criação de um sistema distribuído de automação predial para monitoramento e acionamento de sensores e dispositivos de um prédio de 2 andares. O sistema foi desenvolvido para funcionar em um conjunto de placas Raspberry Pi com um servidor central responsável pelo controle e interface com o usuário e servidores distribuídos para leitura e acionamento dos dispositivos. Dentre os dispositivos envolvidos estão o monitoramento de temperatura e umidade, sensores de presença, sensores de fumaça, sensores de contagem de pessoas, sensores de abertura e fechamento de portas e janelas, acionamento de lâmpadas, aparelhos de ar-condicionado, alarme e aspersores de água em caso de incêndio.

## Dados do Aluno

| Nome                          | Matrícula  |
| ----------------------------- | ---------- |
| Damaso Junio Pereira Brasileo | 17/0031438 |

## Dependências

[WiringPi GPIO](http://wiringpi.com/)

[ncurses](https://invisible-island.net/ncurses/)

## Arquitetura do Sistema

![Architecture](assets/architecture.png)

## Sensores e Dispositivos

![Sensors and Devices](assets/building_devices.png)

## Como Executar

Ao realizar o clone deste repositório em ambas placas Raspberry Pi, em cada um dos terminais, digite os comandos:

### Terminal 1 - Servidor Central

```bash
$ cd Central
$ make
$ make run ou ./bin/bin
```

### Terminal 2 - Servidor Distribuído

```bash
$ cd Distribuido
$ make
$ make run ou ./bin/bin
```

**_OBS: Executar primeiro o servidor central e, em seguida, executar o servidor distribuído._**

## Relatório - Dispositivos de Saída

O log dos comandos acionados dos dispositivos de saída se encontra na pasta `Central/logs/data.csv`.

### Apresentação do Sistema Central

![Servidor Central](assets/menu.png)

## Referências

[DHT22](https://github.com/nebulx29/dht22)

[Biblioteca WiringPi GPIO](http://wiringpi.com/)

[Raspberry Pi Pinout](https://pinout.xyz/)

[Debounce](https://www.filipeflop.com/blog/debounce-o-que-e-e-como-resolver-via-software/)
