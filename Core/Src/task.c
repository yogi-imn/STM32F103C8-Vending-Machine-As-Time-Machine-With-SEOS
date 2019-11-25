/*
 * task.c
 *
 *  Created on: Nov 24, 2019
 *      Author: Yogi Iman
 */

#include "task.h"
#include "stm32f1xx.h"
#include "main.h"
#include "uart.h"
//#include "device.h"
//#include "uart.h"
//#include <iostream>
//using namespace std;

unsigned char PB500_OK(void);
unsigned char PB1000_OK(void);
unsigned char PBPROSES_OK(void);
unsigned char PBBATAL_OK(void);

//Private constant-----------------------
#define Drop		HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET)
#define Keep		HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET)
#define ON			HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_RESET)
#define OFF			HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET)
#define UDrop		HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_RESET)
#define UKeep		HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET)
#define buzzerON   	HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_SET)
#define buzzerOFF  	HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_RESET)
#define NO 0
#define U500 1
#define U1000 2
//Private variable-----------------------
enum state {start,st0,st500,st1000,stk500,stk1000,stopen,delay,buzzerdelay} Kondisi;
unsigned int jeda;
unsigned char Kondisibaru;
volatile uint8_t bouncing1=0xFF;
volatile uint8_t bouncing2=0xFF;
volatile uint8_t bouncing3=0xFF;
volatile uint8_t bouncing4=0xFF;
volatile uint8_t pressed=NO;
volatile uint16_t timeout;

void Task_Init(void){
	timeout=0;
	Kondisi=start;
	Kondisibaru=st0;
}

void Task_Run(void){
	//ON;
	switch(Kondisi)
	{
	case start:
	{
		OFF;
		UKeep;
		Keep;
		UART_print("***** Vending Machine *****\n\r");
		UART_print("Saldo Rp0 \n\r");
		UART_print("Masukkan Koin Senilai Rp1000 \n\r");
		Kondisi=st0;
		timeout=0;
		break;
	}
	case st0:
	{
		if(PB500_OK()){
			UART_print("Saldo Rp500 \n\r");
			jeda=100;
			Kondisibaru=st500;
			UART_print("Masukkan lagi Rp500 u/ Ambil \n\r");
			UART_print("Tekan Cancel u/ Batal \n\r");
			Kondisi=buzzerdelay;
		}
		else if(PB1000_OK()){
			UART_print("Saldo Rp1000 \n\r");
			jeda=100;
			Kondisibaru=st1000;
			UART_print("Tekan Proses u/ Ambil \n\r");
			UART_print("Tekan Cancel u/ Batal \n\r");
			Kondisi=buzzerdelay;
		}
		if(PBBATAL_OK()){
			Kondisi=start;
		}
		break;
	}
	case st500:
	{
		if(PB500_OK()){
			UART_print("Saldo Rp1000 \n\r");
			UART_print("Ambil Minuman? \n\r");
			jeda=100;
			Kondisibaru=st1000;
			UART_print("Tekan Proses u/ Ambil \n\r");
			UART_print("Tekan Cancel u/ Batal \n\r");
			Kondisi=buzzerdelay;
		}
		else if(PB1000_OK()){
			UART_print("Saldo Lebih Rp500 \n\r");
			jeda=800;
			Kondisibaru=stk500;
			pressed=U500;
			Kondisi=buzzerdelay;
		}
		if(PBBATAL_OK()){
			UART_print("Saldo Rp0 \n\r");
			jeda=500;
			Kondisibaru=stk500;
			Kondisi=buzzerdelay;
		}
		break;
	}

	case st1000:
	{
		OFF;
		UKeep;
		if(PB500_OK()){
			UART_print("Saldo Lebih Rp500 \n\r");
			jeda=800;
			Kondisibaru=stk500;
			pressed=U500;
			Kondisi=buzzerdelay;
		}
		else if(PB1000_OK()){
			UART_print("Saldo Lebih Rp1000 \n\r");
			jeda=800;
			Kondisibaru=stk1000;
			pressed=U1000;
			Kondisi=buzzerdelay;
		}
		if(PBPROSES_OK()){
			//UART_print("Silahkan ambil minuman \n\r");
			jeda=100;
			Kondisibaru=stopen;
			Kondisi=buzzerdelay;
		}
		else if(PBBATAL_OK()){
			UART_print("Saldo Rp0 \n\r");
			jeda=500;
			Kondisibaru=stk1000;
			Kondisi=buzzerdelay;
		}
		break;
	}
	case stk500:
	{
		if(pressed==U500){
			ON;
			UART_print("Silahkan ambil Rp500 \n\r");
			jeda=1000;
			Kondisibaru=st1000;
			UART_print("Ambil minuman? \n\r");
			UART_print("Tekan Proses u/ Ambil \n\r");
			UART_print("Tekan Cancel u/ Batal \n\r");
			Kondisi=delay;
		}
		else if(pressed==NO){
		UART_print("Silahkan ambil Rp500 \n\r");
		ON;
		jeda=1000;
		Kondisibaru=start;
		Kondisi=delay;
		}
		pressed=NO;
		break;
	}
	case stk1000:
	{
		UDrop;
		if(pressed==U1000){
			UDrop;
			UART_print("Silahkan ambil Rp1000 \n\r");
			jeda=1000;
			Kondisibaru=st1000;
			UART_print("Ambil minuman? \n\r");
			UART_print("Tekan Proses u/ Ambil \n\r");
			UART_print("Tekan Cancel u/ Batal \n\r");
			Kondisi=delay;
		}
		else if(pressed==NO) {
		UDrop;
		UART_print("Silahkan ambil Rp1000 \n\r");
		jeda=1000;
		Kondisibaru=start;
		Kondisi=delay;
		}
		pressed=NO;
		break;
	}
	case stopen:
	{
		Drop;
		UART_print("Silahkan ambil minuman \n\r");
		jeda=2000;
		Kondisibaru=start;
		Kondisi=buzzerdelay;
		break;
	}
	case delay:
	{
		//jeda--;
		if(jeda--==0){
			Kondisi=Kondisibaru;
		}
		break;
	}
	case buzzerdelay:
	{
		buzzerON;
		//jeda--;
		if(jeda--==0){
			Kondisi=Kondisibaru;
			buzzerOFF;
		}
		break;
	}
	}
}

unsigned char PB500_OK(void){
	static unsigned char bouncing1=0xFF;
	unsigned char detectedFLag=0;
	if(HAL_GPIO_ReadPin(PB500_GPIO_Port,PB500_Pin)== GPIO_PIN_RESET){
		bouncing1=(bouncing1<<1);
	} else {
		bouncing1=(bouncing1<<1)|1;
	}
	if (bouncing1==0x03) {
		UART_print("Koin 500 \n\r");
		detectedFLag=1;
	}
	return detectedFLag;
}

unsigned char PB1000_OK(void){
	static unsigned char bouncing2=0xFF;
	unsigned char detectedFLag=0;
	if(HAL_GPIO_ReadPin(PB1000_GPIO_Port,PB1000_Pin)== GPIO_PIN_RESET){
		bouncing2=(bouncing2<<1);
	} else {
		bouncing2=(bouncing2<<1)|1;
	}
	if (bouncing2==0x03) {
		UART_print("Koin 1000 \n\r");
		detectedFLag=1;
	}
	return detectedFLag;
}

unsigned char PBPROSES_OK(void){
	static unsigned char bouncing3=0xFF;
	unsigned char detectedFLag=0;
	if(HAL_GPIO_ReadPin(PBPROSES_GPIO_Port,PBPROSES_Pin)== GPIO_PIN_RESET){
		bouncing3=(bouncing3<<1);
	} else {
		bouncing3=(bouncing3<<1)|1;
	}
	if (bouncing3==0x03) {
		UART_print("Memproses \n\r");
		detectedFLag=1;
	}
	return detectedFLag;
}

unsigned char PBBATAL_OK(void){
	static unsigned char bouncing4=0xFF;
	unsigned char detectedFLag=0;
	if(HAL_GPIO_ReadPin(PBBATAL_GPIO_Port,PBBATAL_Pin)== GPIO_PIN_RESET){
		bouncing4=(bouncing4<<1);
	} else {
		bouncing4=(bouncing4<<1)|1;
	}
	if (bouncing4==0x03) {
		UART_print("Membatalkan \n\r");
		detectedFLag=1;
	}
	return detectedFLag;
}
