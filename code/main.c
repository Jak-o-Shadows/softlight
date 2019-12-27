//Known issues:
// 14-05-2017 | Alarms over day boundarys will ONLY work if both days are alarmDays
// 14-05-2017 | Must have an alarm every day



#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/timer.h>
//#include <libopencm3/stm32/dac.h>
//#include <libopencm3/stm32/dma.h>


#include "stdint.h"
#include "stdbool.h"
#include <string.h> //For memcpy



#include "usbSerial.h"


struct Time {
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
};

struct Alarm {
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t hourEnd;
	uint8_t minEnd;
	uint8_t secEnd;
	bool triggered; 
	bool enabled;
	uint8_t alarmDays; //1 if true. LSB is sunday, MSB-1 is saturday
};



/* Timer 2 count period, 16 microseconds for a 72MHz APB2 clock */
#define PERIOD 1152

/* Globals */
uint8_t waveform[256];

uint8_t yearBase = 2018;
uint8_t monthBase = 04;
uint8_t dayBase = 22;
uint8_t hourBase = 19;
uint8_t minBase = 30;
uint8_t secBase = 0;
uint8_t dayOfWeekBase = 1;

//These are global variables - but initialised values are meaningless?
uint8_t year = 2018;
uint8_t month = 04;
uint8_t day = 22;
uint8_t hour = 19;
uint8_t min = 30;
uint8_t sec = 0;
//what day of week 0=sunday -> ++
uint8_t dayOfWeek = 1;

const uint8_t monthLengths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};





#define NUMALARMS 10
struct Alarm alarmList[NUMALARMS];


uint8_t alarmState = 0;
bool override = false;
bool buttonStatus = false;
uint8_t overrideMode = 0;

uint8_t nextSend = 'a';
uint8_t messageLen = 0;
#define SENDBUFLEN 64
uint8_t message[64];
uint8_t messageIndex = 0;

uint8_t rxIndex = 0;
#define RECVBUFLEN 8
uint8_t rxBuf[RECVBUFLEN];


//state machine variables
#define STARTCHAR 0xFF
#define ENDCHAR 0xEF
#define DATALENGTH 40
uint8_t dataID = 0;
uint8_t dataLength = 0;
uint32_t dataCRC = 0;
uint8_t parseIndex = 0;
uint8_t data[DATALENGTH];

//state machine states
static void wait_for_start(void);
static void parse_id(void);
static void parse_length(void);
static void parse_data(void);
static void parse_crc(void);
static void parse_end(void);
//other func
bool checkCRC(uint32_t CRC);

//FSM for recieving data
void parse_data_fsm(void);
void (*parse_next)(void) = wait_for_start;




static void clock_setup(void);
static void usart_setup(void);
static void gpio_setup(void);
static void nvic_setup(void);
static void timer_setup(void);
void setPWMVal(uint32_t val);

static bool isLeapYear(uint8_t yearLoc);
static void setAlarm(uint8_t h, uint8_t m, uint8_t s);
void setSendData(uint16_t msgData[], uint16_t msgDataLength);
void handleMessage(uint8_t id, uint8_t msgBuf[]);



////////////////////////////////////////////////////////
///////////////////uC setup/////////////////////////////
////////////////////////////////////////////////////////


static void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	// GPIO
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOC);
	
	/* Enable clocks for GPIO port A (for GPIO_USART2_TX) and USART2. */
	rcc_periph_clock_enable(RCC_USART2);
	rcc_periph_clock_enable(RCC_AFIO);
	
	//timer 4, CH3, PB8
	rcc_periph_clock_enable(RCC_TIM4);
	
	rcc_periph_clock_enable(RCC_DMA2);

}

static void usart_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART2);

	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO2); //USART 2 TX is A2
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO3); //USART 2 RX is A3

	usart_set_baudrate(USART2, 9600);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_mode(USART2, USART_MODE_TX_RX);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
	//enable interrupt rx
	USART_CR1(USART2) |= USART_CR1_RXNEIE;

	usart_enable(USART2);
}

static void gpio_setup(void)
{
	/* Set GPIO13 (in GPIO port C) to 'output push-pull'. */
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO13); //PC13 is the LED
	//DAC - PB8
  	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO5);//GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO5);

	//button input
	//	B1
	gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO1);
	gpio_set(GPIOB, GPIO1); //pull-up resistor
	
	//button other side
	//	A10
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO10);
	gpio_clear(GPIOA, GPIO10);
	

	//Enable another pin, for turning the light on in the mean time
	//gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ,
	//	      GPIO_CNF_OUTPUT_PUSHPULL, GPIO1); 
	//gpio_clear(GPIOA, GPIO1);
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ,
					GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
					GPIO8);

}

static void nvic_setup(void)
{
	/* Without this the RTC interrupt routine will never be called. */
	nvic_enable_irq(NVIC_RTC_IRQ);
	nvic_set_priority(NVIC_RTC_IRQ, 1);
	
	
	nvic_enable_irq(NVIC_USART2_IRQ);
	nvic_set_priority(NVIC_USART2_IRQ, 2);
}


static void timer_setup(void){
	
	//TIM4, CH3 - PB8
	timer_reset(TIM4);
	
	timer_set_mode(TIM4,
					TIM_CR1_CKD_CK_INT,
					TIM_CR1_CMS_EDGE,
					TIM_CR1_DIR_UP);
	
	timer_set_prescaler(TIM4, 2);
	timer_set_repetition_counter(TIM4, 0);
	timer_enable_preload(TIM4);
	timer_continuous_mode(TIM4);
	timer_set_period(TIM4, 20000);
	
	timer_disable_oc_output(TIM4, TIM_OC3);
	timer_set_oc_mode(TIM4, TIM_OC3, TIM_OCM_PWM1);
	timer_set_oc_value(TIM4, TIM_OC3, 2);
	timer_enable_oc_output(TIM4, TIM_OC3);
	
	timer_enable_counter(TIM4);
}


void setPWMVal(uint32_t val){
	timer_set_oc_value(TIM4, TIM_OC3, val);
}



///////////////////////////////////////////////////////
///////////////// Time Stuff //////////////////////////
///////////////////////////////////////////////////////

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


//static void setAlarmIfDay(void){
//	//If the alarm is on the day of week, set the alarm
//	
//	alarmState = 0x00; //disable the alarm - FIX ME FIX ME doesn't actually turn it off
//	
//	if ((alarmDays >> dayOfWeek) & (0x1 <<dayOfWeek)){
//		//Alarm is on this day
//		setAlarm(hourAlarm, minAlarm, secAlarm);
//	}
//}





////////////////////////////////////////////////////////
//////////////main//////////////////////////////////////
////////////////////////////////////////////////////////




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
	
	//usb
	//usbSetup();
	
	
	//other
	clock_setup();
	gpio_setup();
	timer_setup();
	usart_setup();

	/* Setup the RTC interrupt. */
	nvic_setup();
	
	//timer_setup();
	//dacDMASetup();
	
	//check button status - as a toggle button, but we don't care about what it acutally is
	uint16_t readButton = gpio_port_read(GPIOB);
	
	//while (1) {
		for (int start = 0;start<10;start++){
			setPWMVal(20000+1);
			for (int ticker=1;ticker<1000000/2;ticker++){
				__asm__("nop");
			}
			setPWMVal(0);
			for (int ticker=1;ticker<1000000/2;ticker++){
				__asm__("nop");
			}
			//gpio_toggle(GPIOB, GPIO8);
			gpio_toggle(GPIOC, GPIO13);
			
		}
		for (int start=0;start<40;start++){
			setPWMVal(10000);
			for (int ticker=1;ticker<100000/2;ticker++){
				__asm__("nop");
			}
			setPWMVal(0);
			for (int ticker=1;ticker<100000/2;ticker++){
				__asm__("nop");
			}
			//gpio_toggle(GPIOA, GPIO1);
			gpio_toggle(GPIOC, GPIO13);
		}
		
	//}
	setPWMVal(0);
	
	
	
	//setPWMVal(10000);
	//while (1){};
	
	
	
	
	
	
	
	//}
	/*
	 * If the RTC is pre-configured just allow access, don't reconfigure.
	 * Otherwise enable it with the LSE as clock source and 0x7fff as
	 * prescale value.
	 */
	rtc_auto_awake(RCC_LSE, 0x7fff);
	//rtc_auto_awake(RCC_HSE, 0xFFFE); //FFFE -> 1.05 (0xF3CD)

	/* Enable the RTC interrupt to occur off the SEC flag. */
	rtc_interrupt_enable(RTC_SEC);
	rtc_interrupt_enable(RTC_ALR);
	rtc_set_counter_val(0);

	//set initial alarm
	//minAlarm = 50;
	//alarmDays = 0b01111111;
	//setAlarm(hourAlarm, minAlarm, secAlarm);

	//Set start message
	uint16_t msg[] = {'s', 't', 'a', 'r', 't', '\n'};
	setSendData(msg, 6);
	
	//Turn the LED On
	//	Turn off after the time is set
	gpio_clear(GPIOC, GPIO13);
	
	//Turn lamp off
	//gpio_set(GPIOA, GPIO1);
	
	//Alarm 0
	alarmList[0].hour = 6;
	alarmList[0].min = 20;
	alarmList[0].sec = 0;
	alarmList[0].alarmDays = 0b0111110;
	alarmList[0].hourEnd = 7;
	alarmList[0].minEnd = 0;
	alarmList[0].secEnd = 0;
	alarmList[0].enabled = true;
	alarmList[0].triggered = false;
	//Alarm 1
	alarmList[1].hour = 18;
	alarmList[1].min = 30;
	alarmList[1].sec = 0;
	alarmList[1].alarmDays = 0xFF;
	alarmList[1].hourEnd = 19;
	alarmList[1].minEnd = 10;
	alarmList[1].secEnd = 0;
	alarmList[1].enabled = true;
	alarmList[1].triggered = false;
	//Alarm 2
	alarmList[2].hour = 14;
	alarmList[2].min = 50;
	alarmList[2].sec = 0;
	alarmList[2].alarmDays = 0xFF;
	alarmList[2].hourEnd = 14;
	alarmList[2].minEnd = 55;
	alarmList[2].secEnd = 0;
	alarmList[2].enabled = true;
	alarmList[2].triggered = false;
	for (i=3;i<NUMALARMS;i++){
		alarmList[i].enabled = false;
	}
	
	bool newButton;
	while (1) {
		//setPWMVal(20000);
		__asm__("nop");
		//usbInLoop(); //poll because usb
		readButton = gpio_port_read(GPIOB);
		newButton = (bool) (readButton & GPIO1);
		if (buttonStatus!=newButton){
			buttonStatus = newButton;
			switch (overrideMode){
				case 0:
					setPWMVal(0);
					overrideMode++;
					override = 0;
					break;
				case 1:
					setPWMVal(100);
					overrideMode++;
					override = 1;
					break;
				case 2:
					setPWMVal(500);
					overrideMode++;
					override = 1;
					break;
				case 3:
					setPWMVal(20001);
					overrideMode=0;
					override = 1;
					break;
			}
			for (int i=0;i<5000000;i++){ //delay to prevent bouncing
				__asm__("nop");
			}
		}
		

	}

	return 0;
}






void handleMessage(uint8_t id, uint8_t msgBuf[]){
	switch (id) {
		case 's':
			//setting the time
			
			yearBase = msgBuf[0];
			monthBase = msgBuf[1];
			
			dayBase = msgBuf[2];
			hourBase = msgBuf[3];
			
			minBase = msgBuf[4];
			secBase = msgBuf[5];
			secBase = 0; //not incrementing properly ????
			
			dayOfWeekBase = msgBuf[6];
			
	//		data = msgBuf[1];
	//		yearBase = (uint8_t) data >> 8;
	//		monthBase = (uint8_t) data;
	//		
	//		data = msgBuf[2];
	//		dayBase = (uint8_t) data >> 8;
	//		hourBase = (uint8_t) data;
	//		
	//		data = msgBuf[3];
	//		minBase = (uint8_t) data >> 8;
	//		secBase = (uint8_t) data;
	//		
	//		data = msgBuf[4];
	//		dayOfWeekBase = (uint8_t) data >> 9;
			
			//Set the counter value to zero.
			rtc_set_counter_val(0x0000);
			
			year = yearBase;
			month = monthBase;
			day = dayBase;
			dayOfWeek = dayOfWeekBase;
			
			//set time
			rtc_set_counter_val(hourBase*3600+minBase*60+secBase);
			
			// Flash light to acknowledge that it worked
			for (int start = 0;start<10;start++){
				setPWMVal(20000+1);
				for (int ticker=1;ticker<100000/2;ticker++){
					__asm__("nop");
				}
				setPWMVal(0);
				for (int ticker=1;ticker<100000/2;ticker++){
					__asm__("nop");
				}
				gpio_toggle(GPIOC, GPIO13);
			}
			
			//set alarm
			//setAlarmIfDay();
			
			gpio_set(GPIOC, GPIO13); //turn LED off as time is set - this lets you tell if there is a power outage
			break;
		case 'a':
			//setting alarm time
			
			alarmList[msgBuf[0]].hour = msgBuf[1];
			alarmList[msgBuf[0]].min = msgBuf[2];
			alarmList[msgBuf[0]].sec = msgBuf[3];
			alarmList[msgBuf[0]].alarmDays = msgBuf[4];
			
			alarmList[msgBuf[0]].hourEnd = msgBuf[5];
			alarmList[msgBuf[0]].minEnd = msgBuf[6];
			alarmList[msgBuf[0]].secEnd = msgBuf[7];
			
			alarmList[msgBuf[0]].enabled = true;
			alarmList[msgBuf[0]].triggered = false; //can't be bothered
			
			// Flash light to acknowledge that it worked
			for (int start = 0;start<10;start++){
				setPWMVal(20000+1);
				for (int ticker=1;ticker<100000/2;ticker++){
					__asm__("nop");
				}
				setPWMVal(0);
				for (int ticker=1;ticker<100000/2;ticker++){
					__asm__("nop");
				}
				gpio_toggle(GPIOC, GPIO13);
			}
			
			break;
		case 'o':
			//enable/disable override
			override = (bool) msgBuf[0];
			break;
			
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


		volatile uint32_t c = 0;

		/* The interrupt flag isn't cleared by hardware, we have to do it. */
		rtc_clear_flag(RTC_SEC);

		/* Visual output. */
		gpio_toggle(GPIOC, GPIO13);

		c = rtc_get_counter_val();

		hour = c/3600 + hourBase;
		min = (c % 3600)/60 + minBase;
		sec = (c % 3600) % 60 + secBase;

		// Check if a day has passed
		if (hour >= 24){
			rtc_set_counter_val(0);
			hour = 0;
			incrementDate();
			//setAlarmIfDay();
		}
		
		// Now that time has incremented, check the alarms
		for (int i=0;i<NUMALARMS;i++){
			if (alarmList[i].enabled){
				if( alarmList[i].alarmDays & dayOfWeek ){  // Day of week is a bitfield
					//Alarm is active this day of the week

					uint8_t hourAlarm;
					uint8_t minAlarm;
					//uint8_t secAlarm;

					//What time are we comparing?
					if (!alarmList[i].triggered){
						//normal alarm time
						hourAlarm = alarmList[i].hour;
						minAlarm = alarmList[i].min;
						//secAlarm = alarmList[i].sec;
					} else {
						//alarm has been triggered => we want to switch it if it's time
						hourAlarm = alarmList[i].hourEnd;
						minAlarm = alarmList[i].minEnd;
						//secAlarm = alarmList[i].secEnd;
					}

					//Check if the alarm is triggered
					if (hour == hourAlarm){
						if (min==minAlarm){
							//Set alarm state
							if (!alarmList[i].triggered) {
								alarmList[i].triggered = true;
							} else {
								alarmList[i].triggered = false;
							}
						}
					}
				}
			}
		}


		// Check overall status of the light
		bool on = false;
		for (int i=0;i<NUMALARMS;i++){
			if (alarmList[i].enabled){
				if (alarmList[i].triggered) {
					on = true;
					break;
				}
			}
		}
		

		// Check if the torch button is pressed.
		if (!override) {
			if (on){
				//Turn light on
				setPWMVal(20001);
			} else{
				//Turn light off
				setPWMVal(0);
			}
		} else {
			//gpio_toggle(GPIOA, GPIO1);
		}
		
		
	}

	//check if it's the alarm interrupt
	if (rtc_check_flag (RTC_ALR )){
		//Clear the flag
		rtc_clear_flag(RTC_ALR);		
	}

}



////////////////////////////////////////////////////////
////////// Comm stuff //////////////////////////////////
////////////////////////////////////////////////////////

void wait_for_start(void){
	//check if there is data available
	if (rxIndex <= parseIndex){
		return;
	}
	
	if (rxBuf[parseIndex] == STARTCHAR){
		parse_next = parse_id;
		dataID = 0;
		dataLength = 0;
		dataCRC = 0;
	}
	parseIndex++;
}


void parse_data_fsm(void){
	if (parse_next != NULL){
		parse_next();
	}
}

void parse_id(void){
	//check if there is data available
	if (rxIndex <= parseIndex){
		return;
	}
	
	dataID = rxBuf[parseIndex];
	parseIndex++;
	parse_next = parse_length;
	
}

void parse_length(void){
	//check if there is data available
	if (rxIndex <= parseIndex){
		return;
	}
	
	dataLength = rxBuf[parseIndex];
	parseIndex++;
	parse_next = parse_data;
	
}

void parse_data(void){
	static uint8_t count = 0; //how much of the data recieved
	
	//check if there is data available
	if (rxIndex <= parseIndex){
		return;
	}
	
	data[count] = rxBuf[parseIndex];
	count++;
	parseIndex++;
	
	if (count > DATALENGTH){
		count=0;
		parse_next=wait_for_start;
	}
	
	//transition if recv'ed all
	if (count == dataLength){
		count = 0;
		parse_next = parse_crc;
		return;
	}
}

void parse_crc(void){
	static uint8_t count=0;
	
	//check if there is data available
	if (rxIndex <= parseIndex){
		return;
	}
	
	dataCRC += rxBuf[parseIndex];
	count++;
	parseIndex++;
	
	if (count==2){
		count = 0;
		
		//do crc check
		if (!checkCRC(dataCRC)){
			parse_next = wait_for_start;
			return;
		}
		parse_next = parse_end;
	}
	
	//bit-shift to move data
	dataCRC <<= 8;
}

void parse_end(void){
	//check if there is data available
	if (rxIndex <= parseIndex){
		return;
	}
	
	if (rxBuf[parseIndex] != ENDCHAR){
		parse_next = wait_for_start;
	}
	parseIndex++;
	
	//handle data
	handleMessage(dataID, data);
	
	parse_next = wait_for_start;
}


//other comm functions

bool checkCRC(uint32_t CRC){
	return true;
}


//Send
void setSendData(uint16_t msgData[], uint16_t msgDataLength){
	// msgDataLength < SENDBUFLENGTH
	
	
	while (messageIndex>0) {}; //stall while waiting for TX to finish sending
	//	the higher priority interrupt usart interrupt should mean this isn't
	//	too long.	
	// Multiple length by 2 as were using uint16's, but serial only works well with uint8s
	messageLen = 2*msgDataLength;
	memcpy(message, msgData, 2*msgDataLength);
	nextSend = message[0];
	/* Enable transmit interrupt so it sends back the data. */
	USART_CR1(USART2) |= USART_CR1_TXEIE;	
	
	
}

//interrupt
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
		parse_data_fsm();
		if (rxIndex>=RECVBUFLEN){
			rxIndex = 0;
			parseIndex = 0;
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



