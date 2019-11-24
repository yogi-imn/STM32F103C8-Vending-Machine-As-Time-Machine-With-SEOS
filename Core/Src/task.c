/*
 * task.c
 *
 *  Created on: Nov 24, 2019
 *      Author: Yogi Iman
 */

#include "task.h"
#include "stm32f4xx.h"
//#include "main.h"
//#include "device.h"
//#include "uart.h"
//#include <iostream>
//using namespace std;

unsigned char PB500_OK(void);
unsigned char PB1000_OK(void);
unsigned char PBPROSES_OK(void);
unsigned char PBBATAL_OK(void);

//Private constant-----------------------
#define Nilai500    HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET)
#define Bukan500	HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET)
#define Nilai1000   HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET)
#define Bukan1000   HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_RESET)
#define Drop		HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET)
#define Keep		HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_RESET)
#define ReturnU		HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,GPIO_PIN_SET)
#define NOReturnU	HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,GPIO_PIN_RESET)
#define buzzerON   	HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_SET)
#define buzzerOFF  	HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_RESET)
#define NO 0
#define U500 1
#define U1000 2
//Private variable-----------------------
enum state {start,st0,st500,st1000,stk500,stk1000,stopen} Kondisi;
volatile uint8_t bouncing1=0xFF;
volatile uint8_t bouncing2=0xFF;
volatile uint8_t bouncing3=0xFF;
volatile uint8_t bouncing4=0xFF;
volatile uint8_t pressed=NO;
volatile uint16_t timeout;

void Task_Init(void){
	timeout=0;
	Kondisi=start;
}

void Task_Run(void){
	switch(Kondisi)
	{
	case start:
	{
		printf("***** Vending Machine *****\n\r");
		if (++timeout > 1000) //dari scheduler sebesar 2ms*1000= 2 detik
		{
			printf("Saldo Rp0 \n\r");
			printf("Masukkan Koin Senilai Rp1000 \n\r");
			Kondisi=st0;
			timeout=0;
		}
		break;
	}
	case st0:
	{
		checksensor();

		if(pressed==U500){
			if(PBPROSES_OK()){
				printf("Saldo Rp500 \n\r");
				printf("Masukkan Lagi Koin Rp500 \n\r");
				Kondisi=st500;
				pressed=NO;
			}
			else if(PBBATAL_OK()){
				printf("Saldo Rp0 \n\r");
				Kondisi=stk500;
				pressed=NO;
			}
		}
		if(pressed==U1000){
			if(PBPROSES_OK()){
				printf("Saldo Rp1000 \n\r");
				Kondisi=stopen;
				pressed=NO;
			}
			else if(PBBATAL_OK()){
				printf("Saldo Rp0 \n\r");
				Kondisi=stk1000;
				pressed=NO;
			}
		}
		break;
	}
	case st500:
	{
		checksensor();

		if(pressed==U500){
			if(PBPROSES_OK()){
				printf("Saldo Rp1000 \n\r");
				Kondisi=stopen;
				pressed=NO;
			}
			else if(PBBATAL_OK()){
				printf("Saldo Rp0 \n\r");
				Kondisi=stk1000;
				pressed=NO;
			}
		}
		if(pressed==U1000){
			printf("Saldo Rp1500 \n\r");
			printf("Mengembalikan Rp500 \n\r");
			Kondisi=stk500;
			//pressed=NO;
		}
		break;
	}
	case st1000:
	{
		if(PBPROSES_OK()){
			Kondisi=stopen;
		}
		else if(PBBATAL_OK()){
			printf("Saldo Rp0 \n\r");
			Kondisi=stk1000;
		}
		break;
	}
	case stk500:
	{
		if(pressed==U1000){
			printf("Silahkan ambil Rp500 \n\r");
			Kondisi=st1000;
			pressed=NO;
			printf("Ambil minuman? \n\r");
		}
		printf("Silahkan ambil Rp500 \n\r");
		Kondisi=start;
		break;
	}
	case stk1000:
	{
		printf("Silahkan ambil Rp1000 \n\r");
		Kondisi=start;
		break;
	}
	case stopen:
	{
		printf("Silahkan ambil minuman \n\r");
		Kondisi=start;
		break;
	}
	}
}

void checksensor (void){
	if(PB500_OK()){
		printf("Proses? \n\r");
		pressed=U500;
		timeout=0;
	}
	if(PB1000_OK()){
		printf("Proses? \n\r");
		pressed=U1000;
		timeout=0;
	}
}

unsigned char PB500_OK(void){
	static unsigned char bouncing=0xFF;
	unsigned char detectedFLag=0;
	if(HAL_GPIO_ReadPin(PB500_GPIO_Port,PB500_Pin)== GPIO_PIN_RESET){
		bouncing=(bouncing<<1);
	} else {
		bouncing= (bouncing<<1)|1;
	}
	if (bouncing==0x03) {
		printf("Koin 500 \n\r");
		detectedFLag=1;
	}
	return detectedFLag;
}

unsigned char PB1000_OK(void){
	static unsigned char bouncing=0xFF;
	unsigned char detectedFLag=0;
	if(HAL_GPIO_ReadPin(PB1000_GPIO_Port,PB1000_Pin)== GPIO_PIN_RESET){
		bouncing=(bouncing<<1);
	} else {
		bouncing= (bouncing<<1)|1;
	}
	if (bouncing==0x03) {
		printf("Koin 1000 \n\r");
		detectedFLag=1;
	}
	return detectedFLag;
}

unsigned char PBPROSES_OK(void){
	static unsigned char bouncing=0xFF;
	unsigned char detectedFLag=0;
	if(HAL_GPIO_ReadPin(PBPROSES_GPIO_Port,PBPROSES_Pin)== GPIO_PIN_RESET){
		bouncing=(bouncing<<1);
	} else {
		bouncing= (bouncing<<1)|1;
	}
	if (bouncing==0x03) {
		printf("Memproses \n\r");
		detectedFLag=1;
	}
	return detectedFLag;
}

unsigned char PBBATAL_OK(void){
	static unsigned char bouncing=0xFF;
	unsigned char detectedFLag=0;
	if(HAL_GPIO_ReadPin(PBBATAL_GPIO_Port,PBBATAL_Pin)== GPIO_PIN_RESET){
		bouncing=(bouncing<<1);
	} else {
		bouncing= (bouncing<<1)|1;
	}
	if (bouncing==0x03) {
		printf("Membatalkan \n\r");
		detectedFLag=1;
	}
	return detectedFLag;
}
