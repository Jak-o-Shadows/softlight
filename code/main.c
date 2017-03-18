#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/dac.h>
#include <libopencm3/stm32/dma.h>


#include "stdint.h"
#include "stdbool.h"



/* Timer 2 count period, 16 microseconds for a 72MHz APB2 clock */
#define PERIOD 1152

/* Globals */
uint8_t waveform[256];

uint8_t yearBase = 0;
uint8_t monthBase = 0;
uint8_t dayBase = 0;
uint8_t hourBase = 0;
uint8_t minBase = 0;
uint8_t secBase = 0;
uint8_t dayOfWeekBase = 0;

uint8_t year = 0;
uint8_t month = 0;
uint8_t day = 0;
uint8_t hour = 0;
uint8_t min = 0;
uint8_t sec = 0;
uint8_t dayOfWeek = 0;

const uint8_t monthLengths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


uint8_t alarmDays = 0; //1 if true. LSB is sunday, MSB-1 is saturday
uint8_t hourAlarm = 0;
uint8_t minAlarm = 0;
uint8_t secAlarm = 0;


uint8_t alarmState;



static void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	/* Enable GPIOC clock. */
	rcc_periph_clock_enable(RCC_GPIOC);

	/* Enable clocks for GPIO port A (for GPIO_USART1_TX) and USART1. */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART1);
	
	rcc_periph_clock_enable(RCC_TIM2);
	rcc_periph_reset_pulse(RST_TIM2);
	
	rcc_periph_clock_enable(RCC_DMA2);

	rcc_periph_clock_enable(RCC_DAC);
	
}

static void usart_setup(void)
{
	/* Setup GPIO pin GPIO_USART1_TX/GPIO9 on GPIO port A for transmit. */
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

	/* Setup UART parameters. */
	usart_set_baudrate(USART1, 38400);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART1);
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
}



static void timer_setup(void) {
	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_continuous_mode(TIM2);
	timer_set_period(TIM2, 1150);
	timer_disable_oc_output(TIM2, TIM_OC2 | TIM_OC3 | TIM_OC4);
	timer_enable_oc_output(TIM2, TIM_OC1);
	timer_disable_oc_clear(TIM2, TIM_OC1);
	timer_disable_oc_preload(TIM2, TIM_OC1);
	timer_set_oc_slow_mode(TIM2, TIM_OC1);
	timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_TOGGLE);
	timer_set_oc_value(TIM2, TIM_OC1, 500);
	timer_disable_preload(TIM2);

	timer_set_master_mode(TIM2, TIM_CR2_MMS_COMPARE_OC3REF);
	timer_enable_counter(TIM2);
}



static void dacDMASetup(void){
	const uint32_t dma = DMA2;
	const uint32_t ch = DMA_CHANNEL3;
	
    dac_set_waveform_generation(DAC_CR_WAVE1_DIS);
    dac_dma_disable(CHANNEL_1);
    dma_disable_channel(dma, ch);
    dma_channel_reset(dma, ch);

    dma_enable_circular_mode(dma, ch);
    dma_set_peripheral_address(dma, ch, (uint32_t) &DAC_DHR8R1);
    dma_set_memory_address(dma, ch, (uint32_t) &waveform);
    dma_set_memory_size(dma, ch, DMA_CCR_MSIZE_8BIT);
    dma_set_peripheral_size(dma, ch, DMA_CCR_PSIZE_8BIT);
    dma_set_read_from_memory(dma, ch);
    dma_set_number_of_data(dma, ch, 256);

    dma_set_priority(dma, ch, DMA_CCR_PL_VERY_HIGH);

    dma_enable_memory_increment_mode(dma, ch);
    dma_disable_peripheral_increment_mode(dma, ch);

    dac_dma_enable(CHANNEL_1);
    dma_enable_channel(dma, ch);
    
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


static void setAlarmIfDay(){
	//If the alarm is on the day of week, set the alarm
	
	alarmState = 0x00; //disable the alarm - FIX ME FIX ME doesn't actually turn it off
	
	if ((alarmDays >> dayOfWeek) & (0x1 <<dayOfWeek)){
		//Alarm is on this day
		setAlarm(hourAlarm, minAlarm, secAlarm);
	}
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
	usart_setup();
	//timer_setup();
	//dacDMASetup();

	/*
	 * If the RTC is pre-configured just allow access, don't reconfigure.
	 * Otherwise enable it with the LSE as clock source and 0x7fff as
	 * prescale value.
	 */
	rtc_auto_awake(RCC_LSE, 0x7fff);

	/* Setup the RTC interrupt. */
	nvic_setup();

	/* Enable the RTC interrupt to occur off the SEC flag. */
	rtc_interrupt_enable(RTC_SEC);
	rtc_interrupt_enable(RTC_ALR);
	rtc_set_counter_val(0x0000);


	uint16_t data;

	minAlarm = 1;
	alarmDays = 0b01111111;
	setAlarm(hourAlarm, minAlarm, secAlarm);

	//Set the alarm

	while (1) {
		data = usart_recv_blocking(USART1);
		switch (data) {
			case 's':
				//setting the time
				data = usart_recv_blocking(USART1);
				yearBase = (uint8_t) data >> 8;
				monthBase = (uint8_t) data;
				
				data = usart_recv_blocking(USART1);
				dayBase = (uint8_t) data >> 8;
				hourBase = (uint8_t) data;
				
				data = usart_recv_blocking(USART1);
				minBase = (uint8_t) data >> 8;
				secBase = (uint8_t) data;
				
				data = usart_recv_blocking(USART1);
				dayOfWeekBase = (uint8_t) data >> 9;
				
				//Set the counter value to zero.
				rtc_set_counter_val(0x0000);
				
				year = yearBase;
				month = monthBase;
				day = dayBase;
				dayOfWeek = dayOfWeekBase;
				
				setAlarmIfDay();
				
				
				break;
			default:
				//no correct message
				usart_send_blocking(USART1, 1);
			
			
		}
		
	}

	return 0;
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
		
		usart_send_blocking(USART1, 't');
		usart_send_blocking(USART1, (uint16_t) hour >> 8);
		usart_send_blocking(USART1, (uint16_t) hour);
		usart_send_blocking(USART1, ':');
		usart_send_blocking(USART1, (uint16_t) min);
		usart_send_blocking(USART1, ':');
		usart_send_blocking(USART1, (uint16_t) sec);
		usart_send_blocking(USART1, '\n');


		/* Display the current counter value in binary via USART1. */
		for (j = 0; j < 32; j++) {
			if ((c & (0x80000000 >> j)) != 0) {
				usart_send_blocking(USART1, '1');
			} else {
				usart_send_blocking(USART1, '0');
			}
		}
		usart_send_blocking(USART1, '\n');
		
		
		
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
				rtc_set_alarm_time(now + 5*60);
				break;
			case 1:
				gpio_clear(GPIOA, GPIO7);
				alarmState = 0x00;
				break;
		}
		
	}


	
	
	

	
}
