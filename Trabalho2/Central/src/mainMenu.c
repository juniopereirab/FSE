#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "menu.h"
#include "gpio.h"
#include "csv.h"
#include "socketKiller.h"

#define WIDTH 105
#define HEIGHT 40

WINDOW *window;

char *choices[] = {
    "Terreo - Lampada da Sala 01:",
    "Terreo - Lampada da Sala 02:",
    "Terreo - Lampadas do Corredor:",
    "Terreo - Ar-Condicionado:",
    "Terreo - Aspersores de Agua:",
    "Primeiro Andar - Lampada da Sala 101:",
    "Primeiro Andar - Lampada da Sala 102:",
    "Primeiro Andar - Lampadas do Corredor:",
    "Primeiro Andar - Ar-Condicionado:",
    "Ativar o Alarme:",
    "Sair",
};

int optionX = 0;
int optionY = 0;
int choices_length = sizeof(choices) / sizeof(char *);

void printMenu(WINDOW *window, int highlight) {
	int x = 2, y = 4, i;
	box(window, 0, 0);

	for(i = 0; i < choices_length; i++) {
		if (i == choices_length - 1) {
			wattron(window, COLOR_PAIR(2));
		}

		if (highlight == i + 1) {
			wattron(window, A_REVERSE);
			mvwprintw(window, y, x, "%s", choices[i]);
			wattroff(window, A_REVERSE);
		} else {
			mvwprintw(window, y, x, "%s", choices[i]);
		}

		if (i == choices_length - 1) {
			wattroff(window, COLOR_PAIR(2));
		}

		++y;
	}

	wrefresh(window);
}

void clearMenu(WINDOW *window_param) {
	wclear(window_param);
	wborder(window_param, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');  
	wrefresh(window_param);
	delwin(window_param); 
}

void printData(Data data) {
  wattron(window, COLOR_PAIR(data.devOut.lsT01 == 1 ? 3 : 2));
	mvwprintw(window, 4, 41, data.devOut.lsT01 == 1 ? "ON " : "OFF");
	wattroff(window, COLOR_PAIR(data.devOut.lsT01 == 1 ? 3 : 2));

  wattron(window, COLOR_PAIR(data.devOut.lsT02 == 1 ? 3 : 2));
	mvwprintw(window, 5, 41, data.devOut.lsT02 ? "ON " : "OFF");
	wattroff(window, COLOR_PAIR(data.devOut.lsT02 == 1 ? 3 : 2));

  wattron(window, COLOR_PAIR(data.devOut.lcT == 1 ? 3 : 2));
	mvwprintw(window, 6, 41, data.devOut.lcT == 1 ? "ON " : "OFF");
	wattroff(window, COLOR_PAIR(data.devOut.lcT == 1 ? 3 : 2));

  wattron(window, COLOR_PAIR(data.devOut.acT == 1 ? 3 : 2));
	mvwprintw(window, 7, 41, data.devOut.acT == 1 ? "ON " : "OFF");
	wattroff(window, COLOR_PAIR(data.devOut.acT == 1 ? 3 : 2));

  wattron(window, COLOR_PAIR(data.devOut.asp == 1 ? 3 : 2));
	mvwprintw(window, 8, 41, data.devOut.asp == 1 ? "ON " : "OFF");
	wattroff(window, COLOR_PAIR(data.devOut.asp == 1 ? 3 : 2));

  wattron(window, COLOR_PAIR(data.devOut.ls101 == 1 ? 3 : 2));
	mvwprintw(window, 9, 41, data.devOut.ls101 == 1 ? "ON " : "OFF");
	wattroff(window, COLOR_PAIR(data.devOut.ls101 == 1 ? 3 : 2));

  wattron(window, COLOR_PAIR(data.devOut.ls102 == 1 ? 3 : 2));
	mvwprintw(window, 10, 41, data.devOut.ls102 == 1 ? "ON " : "OFF");
	wattroff(window, COLOR_PAIR(data.devOut.ls102 == 1 ? 3 : 2));

  wattron(window, COLOR_PAIR(data.devOut.lc1 == 1 ? 3 : 2));
	mvwprintw(window, 11, 41, data.devOut.lc1 == 1 ? "ON " : "OFF");
	wattroff(window, COLOR_PAIR(data.devOut.lc1 == 1 ? 3 : 2));

  wattron(window, COLOR_PAIR(data.devOut.ac1 == 1 ? 3 : 2));
	mvwprintw(window, 12, 41, data.devOut.ac1 == 1 ? "ON " : "OFF");
	wattroff(window, COLOR_PAIR(data.devOut.ac1 == 1 ? 3 : 2));

  wattron(window, COLOR_PAIR(data.devOut.alarm == 1 ? 3 : 2));
	mvwprintw(window, 13, 41, data.devOut.alarm == 1 ? "ON " : "OFF");
	wattroff(window, COLOR_PAIR(data.devOut.alarm == 1 ? 3 : 2));

  mvwprintw(window, 19, 2, "Temperatura - Terreo: ");
  wattron(window, COLOR_PAIR(1));
  mvwprintw(window, 19, 25, "%4.2f", data.dht22_ground_floor.temperature);
	wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, 20, 2, "Umidade - Terreo: ");
	wattron(window, COLOR_PAIR(1));
  mvwprintw(window, 20, 25, "%4.2f", data.dht22_ground_floor.humidity);
	wattroff(window, COLOR_PAIR(1));

  mvwprintw(window, 19, 40, "Temperatura - Primeiro Andar: ");
  wattron(window, COLOR_PAIR(1));
  mvwprintw(window, 19, 70, "%4.2f", data.dht22_first_floor.temperature);
	wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, 20, 40, "Umidade     - Primeiro Andar: ");
	wattron(window, COLOR_PAIR(1));
  mvwprintw(window, 20, 70, "%4.2f", data.dht22_first_floor.humidity);
	wattroff(window, COLOR_PAIR(1));

	mvwprintw(window, 25, 2, "Alarme Tocando: ");
  wattron(window, COLOR_PAIR(data.devOut.alarmPlaying == 1 ? 3 : 2));
	mvwprintw(window, 25, 18, data.devOut.alarmPlaying == 1 ? "SIM" : "NAO");
	wattroff(window, COLOR_PAIR(data.devOut.alarmPlaying == 1 ? 3 : 2));

  wrefresh(window);
}

void print_devices_in(DevicesIn devIn) {
  mvwprintw(window, 4, 50, "Terreo - Sensor de Presenca:");
	wattron(window, COLOR_PAIR(devIn.spT == 1 ? 3 : 2));
	mvwprintw(window, 4, 89, devIn.spT == 1 ? "ON " : "OFF");
	wattroff(window, COLOR_PAIR(devIn.spT == 1 ? 3 : 2));

  mvwprintw(window, 5, 50, "Terreo - Sensor de Fumaca:");
	wattron(window, COLOR_PAIR(devIn.sfT == 1 ? 3 : 2));
	mvwprintw(window, 5, 89, devIn.sfT == 1 ? "ON " : "OFF");
	wattroff(window, COLOR_PAIR(devIn.sfT == 1 ? 3 : 2));

  mvwprintw(window, 6, 50, "Terreo - Sensor de Janela 01:");
	wattron(window, COLOR_PAIR(devIn.sjT01 == 1 ? 3 : 2));
	mvwprintw(window, 6, 89, devIn.sjT01 == 1 ? "ON " : "OFF");
	wattroff(window, COLOR_PAIR(devIn.sjT01 == 1 ? 3 : 2));

  mvwprintw(window, 7, 50, "Terreo - Sensor de Janela 02:");
	wattron(window, COLOR_PAIR(devIn.sjT02 == 1 ? 3 : 2));
	mvwprintw(window, 7, 89, devIn.sjT02 == 1 ? "ON " : "OFF");
	wattroff(window, COLOR_PAIR(devIn.sjT02 == 1 ? 3 : 2));

  mvwprintw(window, 8, 50, "Terreo - Sensor de Porta:");
	wattron(window, COLOR_PAIR(devIn.spoT == 1 ? 3 : 2));
	mvwprintw(window, 8, 89, devIn.spoT == 1 ? "ON " : "OFF");
	wattroff(window, COLOR_PAIR(devIn.spoT == 1 ? 3 : 2));

  mvwprintw(window, 9, 50, "Primeiro Andar - Sensor de Presenca:");
	wattron(window, COLOR_PAIR(devIn.sp1 == 1 ? 3 : 2));
	mvwprintw(window, 9, 89, devIn.sp1 == 1 ? "ON " : "OFF");
	wattroff(window, COLOR_PAIR(devIn.sp1 == 1 ? 3 : 2));

  mvwprintw(window, 10, 50, "Primeiro Andar - Sensor de Fumaca:");
	wattron(window, COLOR_PAIR(devIn.sf1 == 1 ? 3 : 2));
	mvwprintw(window, 10, 89, devIn.sf1 == 1 ? "ON " : "OFF");
	wattroff(window, COLOR_PAIR(devIn.sf1 == 1 ? 3 : 2));

  mvwprintw(window, 11, 50, "Primeiro Andar - Sensor de Janela 101:");
	wattron(window, COLOR_PAIR(devIn.sj101 == 1 ? 3 : 2));
	mvwprintw(window, 11, 89, devIn.sj101 == 1 ? "ON " : "OFF");
	wattroff(window, COLOR_PAIR(devIn.sj101 == 1 ? 3 : 2));

  mvwprintw(window, 12, 50, "Primeiro Andar - Sensor de Janela 102:");
	wattron(window, COLOR_PAIR(devIn.sj102 == 1 ? 3 : 2));
	mvwprintw(window, 12, 89, devIn.sj102 == 1 ? "ON " : "OFF");
	wattroff(window, COLOR_PAIR(devIn.sj102 == 1 ? 3 : 2));

	mvwprintw(window, 25, 50, "Pessoas: ");
  wattron(window, COLOR_PAIR(1));
	mvwprintw(window, 25, 59, devIn.peopleQuantity >= 10 ? "%d" : "0%d", devIn.peopleQuantity);
	wattroff(window, COLOR_PAIR(1));

  wrefresh(window);
}

void print_header() {
	wattron(window, COLOR_PAIR(4));
	mvwprintw(window, 2, 15, "DISPOSITIVOS DE SAIDA");
	wattroff(window, COLOR_PAIR(4));

	wattron(window, COLOR_PAIR(4));
	mvwprintw(window, 2, 61, "DISPOSITIVOS DE ENTRADA");
	wattroff(window, COLOR_PAIR(4));

	wattron(window, COLOR_PAIR(4));
	mvwprintw(window, 17, 37, "TEMPERATURA E UMIDADE");
	wattroff(window, COLOR_PAIR(4));

	wattron(window, COLOR_PAIR(4));
	mvwprintw(window, 23, 13, "ALARME");
	wattroff(window, COLOR_PAIR(4));

	wattron(window, COLOR_PAIR(4));
	mvwprintw(window, 23, 57, "QUANTIDADE DE PESSOAS");
	wattroff(window, COLOR_PAIR(4));
}

void* menuHandler() {
	int highlight = 1;
	int choice = 0;
	int c;

	initscr();
	start_color(); 
	clear();
	noecho();
	cbreak();
	curs_set(0);
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
     
	window = newwin(HEIGHT, WIDTH, optionY, optionX);
	keypad(window, TRUE);
	refresh();

	dataInit();

	do {
		printHeader();
		printMenu(window, highlight);
		c = wgetch(window);

		switch(c) {
			case KEY_UP:
				if(highlight == 1)
					highlight = choices_length;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == choices_length)
					highlight = 1;
				else
					++highlight;
				break;
			case 10:    
				choice = highlight;

				DevicesOut devOut = recoverDevicesOutData();

                switch(choice) {
                    case 1:
                        if(devOut.lsT01){
                            devOut.lsT01 = 0;
                        }
                        else {
                            devOut.lsT01 = 1;
                        }
                        char *device = "Térreo - Lâmpada da Sala 01";
                        Command command;
                        command.device = device;
                        command.state = devOut.lsT01;
                        writeData(command);
                        break;
                    case 2:
                        if(devOut.lsT02){
                            devOut.lsT02 = 0;
                        }
                        else {
                            devOut.lsT02 = 1;
                        }
                        char *device = "Térreo - Lâmpada da Sala 02";
                        Command command;
                        command.device = device;
                        command.state = devOut.lsT02;
                        writeData(command);
                        break;
                    case 3:
                        if(devOut.lcT){
                            devOut.lcT = 0;
                        }
                        else {
                            devOut.lcT = 1;
                        }
                        char *device = "Térreo - Lâmpadas do Corredor";
                        Command command;
                        command.device = device;
                        command.state = devOut.lcT;
                        writeData(command);
                        break;
                    case 4:
                        if(devOut.acT) {
                            devOut.acT = 0;
                        }
                        else {
                            devOut.acT = 1;
                        }
                        char *device = "Térreo - Ar-Condicionado";
                        Command command;
                        command.device = device;
                        command.state = devOut.acT;
                        writeData(command);
                        break;
                    case 5:
                        if(devOut.asp){
                            devOut.asp = 0;
                        }
                        else{
                            devOut.asp = 1;
                        }
                        char *device = "Térreo - Aspersores de Agua";
                        Command command;
                        command.device = device;
                        command.state = devOut.asp;
                        writeData(command);
                        break;
                    case 6:
                        if(devOut.ls101){
                            devOut.ls101 = 0;
                        }
                        else {
                            devOut.ls101 = 1;
                        }
                        char *device = "Primeiro Andar - Lâmpada da Sala 101";
                        Command command;
                        command.device = device;
                        command.state = devOut.ls101;
                        writeData(command);
                        break;
                    case 7:
                        if(devOut.ls102){
                            devOut.ls102 = 0;
                        }
                        else {
                            devOut.ls102 = 1;
                        }
                        char *device = "Primeiro Andar - Lâmpada da Sala 102";
                        Command command;
                        command.device = device;
                        command.state = devOut.ls102;
                        writeData(command);
                        break;
                    case 8:
                        if(devOut.lc1){
                            devOut.lc1 = 0;
                        }
                        else {
                            devOut.lc1 = 1;
                        }
                        char *device = "Primeiro Andar - Lâmpadas do Corredor";
                        Command command;
                        command.device = device;
                        command.state = devOut.lc1;
                        writeData(command);
                        break;
                    case 9:
                        if(devOut.ac1){
                            devOut.ac1 = 0;
                        }
                        else {
                            devOut.ac1 = 1;
                        }
                        char *device = "Primeiro Andar - Ar-Condicionado";
                        Command command;
                        command.device = device;
                        command.state = devOut.ac1;
                        writeData(command);
                        break;
                    case 10:
                        if(devOut.alarm){
                            devOut.alarm = 0;
                        }
                        else {
                            devOut.alarm = 1;
                        }
                        char *device = "Ativar o Alarme";
                        Command command;
                        command.device = device;
                        command.state = devOut.alarm;
                        writeData(command);
                        break;
                }
				storeDevicesOutUpdate(devOut);
				if (choice == 11) {
					char *message = "Finalizando. . .";
					quitHandler(message);
				}
				break;
			default:
				refresh();
				break;
		}
	} while(1);

	return NULL;
}
