#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencm3/cm3/nvic.h>
//#include <libopencm3/stm32/timer.h>
//#include <libopencm3/stm32/dac.h>
//#include <libopencm3/stm32/dma.h>


#include "stdint.h"
#include "stdbool.h"
#include <string.h> //For memcpy



/* Timer 2 count period, 16 microseconds for a 72MHz APB2 clock */
#define PERIOD 1152

/* Globals */
uint8_t waveform[256];

uint8_t yearBase = 2017;
uint8_t monthBase = 03;
uint8_t dayBase = 19;
uint8_t hourBase = 14;
uint8_t minBase = 30;
uint8_t secBase = 0;
uint8_t dayOfWeekBase = 0;

uint8_t year = 2017;
uint8_t month = 03;
uint8_t day = 19;
uint8_t hour = 0;
uint8_t min = 0;
uint8_t sec = 0;
uint8_t dayOfWeek = 0;

const uint8_t monthLengths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


uint8_t alarmDays = 0xFF; //1 if true. LSB is sunday, MSB-1 is saturday
uint8_t hourAlarm = 06;
uint8_t minAlarm = 30;
uint8_t secAlarm = 0;


uint8_t alarmState;




uint8_t nextSend = 'a';
uint8_t messageLen = 0;
#define SENDBUFLEN 64
uint8_t message[64];
uint8_t messageIndex = 0;

uint8_t rxIndex = 0;
#define RECVBUFLEN 5
uint16_t rxBuf[RECVBUFLEN];


static void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	/* Enable GPIOC clock. */
	rcc_periph_clock_enable(RCC_GPIOC);

	/* Enable clocks for GPIO port A (for GPIO_USART2_TX) and USART2. */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART2);
	rcc_periph_clock_enable(RCC_AFIO);
	

	rcc_periph_clock_enable(RCC_DMA2);

}

static void usart_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART2);

	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART2_TX); //USART 2 TX is A2
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_USART2_RX); //USART 2 RX is A3

	usart_set_baudrate(USART2, 9600);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_mode(USART2, USART_MODE_TX_RX);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
	//enable interrupt rx
	USART_CR1(USART2) |= USART_CR1_RXNEIE;
	//enable tx interrupt, as we have something to say at the start
	USART_CR1(USART2) |= USART_CR1_TXEIE;

	
	usart_enable(USART2);
}

static void gpio_setup(void)
{
	/* Set GPIO13 (in GPIO port C) to 'output push-pull'. */
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO13); //PC13 is the LED
	//DAC - PA5
  	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO5);//GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO5);


	//Enable another pin, for turning the light on in the mean time
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO7); 
	gpio_clear(GPIOA, GPIO7);

}

static void nvic_setup(void)
{
	/* Without this the RTC interrupt routine will never be called. */
	nvic_enable_irq(NVIC_RTC_IRQ);
	nvic_set_priority(NVIC_RTC_IRQ, 1);
	
	
	nvic_enable_irq(NVIC_USART2_IRQ);
	nvic_set_priority(NVIC_USART2_IRQ, 2);
}








static bool isLeapYear(uint8_t yearLoc){
	if (yearLoc % 400 != 0){
		if (yearLoc % 100 == 0){
			return false;
		}
		if (yearLoc % 4 != 0){
			return false;
		}
	}
	return true;
}
	
	
	
	
static void incrementDate(void){
	switch (monthLengths[month]){
		case 31:
			if (day < 31){
				day++;
				return;
			} 
			if (month==12){
				month = 1;
				day = 1;
				year++;
			} else{
				month++;
				day = 1;
			}
			break;
		case 30:
			if (day < 31){
				day++;
				return;
			}
			month++;
			day = 1;
			break;
		default:
			//It must be february - hence must check for leap years
			if (day < 28){
				day++;
				return;
			}
			if (day == 28){
				if (isLeapYear(year)){
					day++;
					return;
				} else{
					month++;
					day = 1;
				}
			}
			if (day == 29){
				month ++;
				day =1;
			}
		}
		
		dayOfWeek++;
		if (dayOfWeek > 7){
			dayOfWeek = 0;
		}
		
		
		
	return;
}
	


	
static void setAlarm(uint8_t h, uint8_t m, uint8_t s){
	//Calculates the time from start of day to the alarm
	//	and then sets it
	rtc_set_alarm_time((uint32_t) h*24*60 + m*60 + s);
}


static void setAlarmIfDay(void){
	//If the alarm is on the day of week, set the alarm
	
	alarmState = 0x00; //disable the alarm - FIX ME FIX ME doesn't actually turn it off
	
	if ((alarmDays >> dayOfWeek) & (0x1 <<dayOfWeek)){
		//Alarm is on this day
		setAlarm(hourAlarm, minAlarm, secAlarm);
	}
}



void setSendData(uint16_t data[], uint16_t dataLength){
	// dataLength < SENDBUFLENGTH
	
	
	while (messageIndex>0) {}; //stall while waiting for TX to finish sending
	//	the higher priority interrupt usart interrupt should mean this isn't
	//	too long.	
	messageLen = dataLength;
	memcpy(message, data, dataLength);
	/* Enable transmit interrupt so it sends back the data. */
	USART_CR1(USART2) |= USART_CR1_TXEIE;	
	
	
}







int main(void)
{
	uint16_t i, x;
	for (i = 0; i < 256; i++) {
		if (i < 10) {
			x = 10;
		} else if (i < 121) {
			x = 10 + ((i*i) >> 7);
		} else if (i < 170) {
			x = i/2;
		} else if (i < 246) {
			x = i + (80 - i/2);
		} else {
			x = 10;
		}
		waveform[i] = x;
	}
	
	
	clock_setup();
	gpio_setup();
	
	/* Setup the RTC interrupt. */
	nvic_setup();
	
	usart_setup();
	//timer_setup();
	//dacDMASetup();

	/*
	 * If the RTC is pre-configured just allow access, don't reconfigure.
	 * Otherwise enable it with the LSE as clock source and 0x7fff as
	 * prescale value.
	 */
	rtc_auto_awake(RCC_LSE, 0x7fff);



	/* Enable the RTC interrupt to occur off the SEC flag. */
	rtc_interrupt_enable(RTC_SEC);
	rtc_interrupt_enable(RTC_ALR);
	rtc_set_counter_val(0x0000);

	//set initial alarm
	minAlarm = 50;
	alarmDays = 0b01111111;
	setAlarm(hourAlarm, minAlarm, secAlarm);

	//Set start message
	uint16_t msg[] = {'s', 't', 'a', 'r', 't', '\n'};
	setSendData(msg, 6);
	
	

	while (1) {
		__asm__("nop");

	}

	return 0;
}





void handleMessage(uint16_t msgBuf[]){
	uint16_t data = msgBuf[0];
	switch (data) {
		case 's':
			//setting the time
			data = msgBuf[1];
			yearBase = (uint8_t) data >> 8;
			monthBase = (uint8_t) data;
			
			data = msgBuf[2];
			dayBase = (uint8_t) data >> 8;
			hourBase = (uint8_t) data;
			
			data = msgBuf[3];
			minBase = (uint8_t) data >> 8;
			secBase = (uint8_t) data;
			
			data = msgBuf[4];
			dayOfWeekBase = (uint8_t) data >> 9;
			
			//Set the counter value to zero.
			rtc_set_counter_val(0x0000);
			
			year = yearBase;
			month = monthBase;
			day = dayBase;
			dayOfWeek = dayOfWeekBase;
			
			setAlarmIfDay();
			break;
		case 'a':
			//setting alarm time
			data = msgBuf[1];
			hourAlarm = (uint8_t) data >> 8;
			minAlarm = (uint8_t) data;
			
			data = msgBuf[2];
			secAlarm = (uint8_t) data >> 8;
			alarmDays = (uint8_t) data;
			
		default:
			//no correct message
			{	//C does not allow variable declations right after a label (ie. :)
			uint16_t msg[] = {'E', 'R', 'R', 'O', 'R', ':', 'N', 'o', 't', ' ', 'c', 'o', 'r', 'r', 'e', 'c', 't', ' ', 'm', 's', 'g'};
			setSendData(msg, 21);
			}	
	}
		
	
	
	
}




void rtc_isr(void)
{
	
	
	if (rtc_check_flag (RTC_SEC )){
		
		
		volatile uint32_t j = 0, c = 0;

		/* The interrupt flag isn't cleared by hardware, we have to do it. */
		rtc_clear_flag(RTC_SEC);

		/* Visual output. */
		gpio_toggle(GPIOC, GPIO13);

		c = rtc_get_counter_val();

		hour = c/3600 + hourBase;
		min = (c % 3600)/60 + minBase;
		sec = (c % 3600) % 60 + secBase;
		
		//usart_send_blocking(USART2, 't');
		//usart_send_blocking(USART2, (uint16_t) hour >> 8);
		//usart_send_blocking(USART2, (uint16_t) hour);
		//usart_send_blocking(USART2, ':');
		//usart_send_blocking(USART2, (uint16_t) min);
		//usart_send_blocking(USART2, ':');
		//usart_send_blocking(USART2, (uint16_t) sec);
		//usart_send_blocking(USART2, '\n');


		/* Display the current counter value in binary via USART2. */
		for (j = 0; j < 32; j++) {
			if ((c & (0x80000000 >> j)) != 0) {
		//		usart_send_blocking(USART2, '1');
			} else {
		//		usart_send_blocking(USART2, '0');
			}
		}
		//usart_send_blocking(USART2, '\n');
		
		
		
		if (hour == 24){
			hour = 0;
			rtc_set_counter_val(0x0000);
			incrementDate();
			setAlarmIfDay();
		}
	}
	
	
	
	//check if it's the alarm
	if (rtc_check_flag (RTC_ALR )){
		//Clear the flag
		rtc_clear_flag(RTC_ALR);
		
		switch (alarmState){
			case 0:
				//alarm was off -> turn the light on
				gpio_set(GPIOA, GPIO7);
				alarmState = 0x1;
				//Set the alarm for 5 minutes
				uint32_t now = rtc_get_counter_val();
				rtc_set_alarm_time(now + 30*60);
				break;
			case 1:
				gpio_clear(GPIOA, GPIO7);
				alarmState = 0x00;
				break;
		}
		
	}

}



void usart2_isr(void)
{

	/* Check if we were called because of RXNE. */
	if (((USART_CR1(USART2) & USART_CR1_RXNEIE) != 0) &&
	    ((USART_SR(USART2) & USART_SR_RXNE) != 0)) {

		/* Indicate that we got data. */
		//gpio_toggle(GPIOC, GPIO13);

		/* Retrieve the data from the peripheral. */
		rxBuf[rxIndex] = usart_recv(USART2);
		rxIndex++;
		if (rxIndex>=RECVBUFLEN){
			handleMessage(rxBuf);
			rxIndex = 0;
		}
	}

	/* Check if we were called because of TXE. */
	if (((USART_CR1(USART2) & USART_CR1_TXEIE) != 0) &&
	    ((USART_SR(USART2) & USART_SR_TXE) != 0)) {

		/* Indicate that we are sending out data. */
		// gpio_toggle(GPIOA, GPIO7);

		/* Put data into the transmit register. */
		usart_send(USART2, nextSend);
		
		messageIndex++;
		if (messageIndex >= messageLen){
			messageIndex = 0;
			/* Disable the TXE interrupt as we don't need it anymore. */
			USART_CR1(USART2) &= ~USART_CR1_TXEIE;
		}
		nextSend = message[messageIndex];
	}
}



