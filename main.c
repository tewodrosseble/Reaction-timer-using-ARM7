#include <NXP/iolpc2124.h>
#include <stdio.h>

void initializeLCD(void);
void writeCommand(char);
void writeData(char);
void initializeLPC(void);
void delay(unsigned int);
void switchOnOff(void);
void displayIntOnLCD(int c);
void sendStringToLCD(char *str);

void main() {
    int ran = rand()% 2000;
    initializeLPC();
    initializeLCD();
    int timer = 0;

    while (IO0PIN_bit.P0_12 == 1) {
        while (IO0PIN_bit.P0_11 == 0) {
            if (IO0PIN_bit.P0_12 == 0) {
                break;
            }
            delay(ran);
            switchOnOff();
        }
        delay(1);
        timer++;
    }

    IO0CLR_bit.P0_11 = 1;

    if (timer <= 1) {
        char text[] = "Too Early!";
        for (int i = 0; i < sizeof(text); i++) {
            writeData(text[i]);
        }
    } else {
        displayIntOnLCD(timer);
        writeData(' ');
        writeData('m');
        writeData('s');
    }
}

void initializeLPC() {
    PINSEL0 = 0;
    IO0DIR = 0X0FFF;
    IO0DIR_bit.P0_12 = 0;
    T0TCR_bit.CE = 0;
    T0PR = 47999;
    T0TC = 0;
}

void delay(unsigned int x) {
    T0TC = 0;
    T0TCR_bit.CE = 1;
    while (T0TC < x) {
        if (IO0PIN_bit.P0_12 == 0) {
            break;
        }
    }
}

void switchOnOff() {
    IO0SET_bit.P0_11 = 1;
}

void writeCommand(char cmd) {
    IO0CLR_bit.P0_8 = 1;
    delay(1);
    IO0CLR_bit.P0_9 = 1;
    delay(1);
    IO0SET = cmd;
    delay(1);
    IO0SET_bit.P0_10 = 1;
    delay(1);
    IO0CLR_bit.P0_10 = 1;
    delay(1);
    IO0CLR = 0x000000FF;
}

void writeData(char data) {
    IO0SET_bit.P0_8 = 1;
    delay(1);
    IO0CLR_bit.P0_9 = 1;
    delay(1);
    IO0SET = data;
    delay(1);
    IO0SET_bit.P0_10 = 1;
    delay(1);
    IO0CLR_bit.P0_10 = 1;
    delay(1);
    IO0CLR = 0x000000FF;
}

void initializeLCD() {
    writeCommand(0x38);
    delay(1);
    writeCommand(0x06);
    delay(1);
    writeCommand(0x0f);
    delay(1);
    writeCommand(0x01);
    delay(1);
}

void sendStringToLCD(char *str) {
    while (*str) {
        char x = *str++;
        writeData(x);
    }
}

void displayIntOnLCD(int value) {
    char buffer[16];
    sprintf(buffer, "%02d", value);
    sendStringToLCD(buffer);
}
