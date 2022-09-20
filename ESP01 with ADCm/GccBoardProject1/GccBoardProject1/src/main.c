/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include <string.h> // for memset
#include <stdlib.h> // for malloc
#include "SERCOMforUSART.h"

////////////////////////////////////////////////////////////////////////////////////// for adc
struct adc_module adc_instance;#define ADC_SAMPLES 1
uint16_t adc_result_buffer[ADC_SAMPLES];
void adc_complete_callback(const struct adc_module *const module);
void configure_adc(void);
void configure_adc_callbacks(void);
void adjustingForRegisterLevel(void);
void adjustingForinterrupt(void);
// uint16_t to str for writing adc values to data visualizer
uint8_t ReceivedDataFromUart1[100]; 
uint8_t ReceivedDataFromESP8266[255];
uint8_t i = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// esp01 definitions
//#Definitions
uint8_t at[] = "AT\r\n";
uint8_t CWMODE[] = "AT+CWMODE=1\r\n";
uint8_t ESPRESET[] = "AT+RST\r\n";
uint8_t CIPCLOSE[] = "AT+CIPCLOSE\r\n";
uint8_t bekleniyor[] = "WAITING..\r\n";
uint8_t kapatildi[] = "CONNECTION IS CLOSED\r\n";
uint8_t CIPSTARTERROR[] = "AT+CIPSTART Error...\r\n";
uint8_t CIPSTART[] = "AT+CIPSTART CORRECTLY...\r\n";
uint8_t CWJAP[] = "AT+CWJAP=\"NoorTech2-1\",\"Ntech2021!!\"\r\n";
uint8_t thing[] = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n";
uint8_t ayarok[] = "IT IS ADJUSTED AS CLIENT\r\n";
uint8_t agconn[] = "CONNECTING TO WIFI\r\n";
uint8_t agconnok[] = "CONNECTED\r\n";
uint8_t ayar[] = "SETTING UP...\r\n";
uint8_t ayar2[] = "SETTINGS ARE SUCCESSFULLY COMPLETED...\r\n";
uint8_t atsent[] = "COMMAND IS SENT\r\n";
uint8_t espcantfind[] = "ESP CAN'T FIND\r\n"; 
uint8_t atson[] = "OK COMMAND IS RECEIVED SUCCESSFULLY\r\n";
uint8_t ApiRequest[] = "GET https://api.thingspeak.com/update?api_key=CASLZCCNVMHK6XIE&field1=%s\r\n"; // change this place with your api key
uint8_t CIPSEND[] = "AT+CIPSEND=%d\r\n";
uint8_t m = 0;
uint8_t l = 1;
uint8_t t = 0;
uint8_t *stringforesp, *stringforcipsend; 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
int main (void)
{
	system_init();
	delay_init();
	adjustingForRegisterLevel();
	uartInit(115200);
	adjustingForinterrupt();
	configure_adc();
	configure_adc_callbacks();
	uint8_t string[] = "Started\r\n";
	uartWrite(string, sizeof(string)); // testing whether uart 1 is working or not
	
	// for esp01 (1)
	uartWriteforser5(at, sizeof(at));
	while (l)
	{
		uartWriteforser5(at, sizeof(at));	
		uartWrite(espcantfind, sizeof(espcantfind));
	}
	uartWrite(atson, sizeof(atson));
	delay_ms(3500); // IT IS GOOD TIME GIVEN FOR ESP01 TO BE READY
	memset(ReceivedDataFromESP8266, ' ', 255);
	l = 1; // it has been set again because of next command	
	m = 0;
	// for esp01 (2)
	uartWriteforser5(CWMODE, sizeof(CWMODE));
	while (l)
	{
		uartWriteforser5(CWMODE, sizeof(CWMODE));
		uartWrite(ayar, sizeof(ayar));
		
	}
	uartWrite(ayar2, sizeof(ayar2));
	uartWrite(ayarok, sizeof(ayarok));
	uartWrite(agconn,sizeof(agconn));
	delay_ms(3500);
	memset(ReceivedDataFromESP8266, ' ', 255);
	l = 1;
	m = 0;
	// for esp01 (3)
	uartWriteforser5(CWJAP, sizeof(CWJAP));
	while (l){
		uartWrite(agconn,sizeof(agconn));
	
	}
	uartWrite(agconnok, sizeof(agconnok));
	delay_ms(3500); // we are waiting for dummy responses	
	memset(ReceivedDataFromESP8266, ' ', 255);
	l = 1;
	m = 0;
	while (1) {
		adc_read_buffer_job(&adc_instance, adc_result_buffer, ADC_SAMPLES);
		uartWriteforser5(thing, sizeof(thing));
		while (l)
		{
			uartWrite(CIPSTARTERROR, sizeof(CIPSTARTERROR));
			
		}
		// DON'T PUT A DELAY TO HERE, IF YOU DO IT, YOU probably GET A ERROR  
		uartWrite(CIPSTART,sizeof(CIPSTART));
		l = 1;
		m = 0;
		memset(ReceivedDataFromESP8266, ' ', 255);
		
		stringforesp = (uint8_t*) malloc((sizeof(ApiRequest) + 2) * sizeof(uint8_t));
		stringforcipsend = (uint8_t*) malloc((sizeof(CIPSEND)) * sizeof(uint8_t));
		
		sprintf(stringforcipsend, "AT+CIPSEND=%d\r\n", (sizeof(ApiRequest) + 2));
		sprintf(stringforesp, "GET https://api.thingspeak.com/update?api_key=CASLZCCNVMHK6XIE&field1=%d\r\n", adc_result_buffer[0]);
		
		uartWriteforser5(stringforcipsend, (sizeof(CIPSEND)));
		delay_ms(2000);
		if (t)
		{
			t = 0;
			uartWriteforser5(stringforesp, (sizeof(ApiRequest) + 2));
			delay_ms(1000);
			uartWriteforser5(CIPCLOSE, sizeof(CIPCLOSE));
			while (l)
			{
				uartWrite(bekleniyor,sizeof(bekleniyor));
			}
			uartWrite(kapatildi,sizeof(kapatildi));
			delay_ms(1000);
			memset(ReceivedDataFromESP8266, ' ', 255);
			l = 1;
			m = 0;
			free(stringforcipsend);
			free(stringforesp);
		}
		
		else{
				uartWriteforser5(CIPCLOSE, sizeof(CIPCLOSE));
				while (l)
				{
					uartWrite(bekleniyor,sizeof(bekleniyor));
				}
				uartWrite(kapatildi,sizeof(kapatildi));
				delay_ms(1000);
				memset(ReceivedDataFromESP8266, ' ', 255);
				l = 1;
				m = 0;	
				free(stringforcipsend);
				free(stringforesp);		
		}
		
	}
	
}

void USART_EYUP(){
	
	ReceivedDataFromUart1[i] = SERCOM3->USART.DATA.bit.DATA; // This flag is cleared by reading the Data register (DATA) or by disabling the receiver.
	
	if (ReceivedDataFromUart1[i] == '\n')
	{
		uartWrite(ReceivedDataFromUart1 + i, 1);
		i = 0;
	}
	else{
		uartWrite(ReceivedDataFromUart1 + i, 1);
		i += 1;
	}
	
}

void USART_EYUP2(){
	
	if (SERCOM5->USART.INTFLAG.bit.RXC == 1)
	{
		if (!(SERCOM5->USART.STATUS.bit.BUFOVF))
		{
			ReceivedDataFromESP8266[m] = SERCOM5->USART.DATA.bit.DATA; // This flag is cleared by reading the Data register (DATA) or by disabling the receiver.
			if ((m != 0) && (ReceivedDataFromESP8266[m] == 'K'))
			{
				if (ReceivedDataFromESP8266[m - 1] == 'O')
				{
					l = 0;
					m = 0;
				}
			}
			else if (ReceivedDataFromESP8266[m] == '>')
			{
				t = 1; //THIS MEAN THAT YOU CAN NOW ENABLED TO WRITE TO THINGSPEAK
			}
			
			else{
				m += 1;
				if (m == 254){
					// IF YOU BE A 254 YOU GET CLESED TO OVERFLOW, DUE TO THIS
					m = 0;
					memset(ReceivedDataFromESP8266, ' ', 255);
				}
			}
				
		}
	}
	
	else if (SERCOM5->USART.INTFLAG.bit.ERROR)
	{
		uint8_t gonder[] = "AN ERROR OCCURED\r\n"; // probably THIS ERROR OCCURE, BECAUSE OF UART COMMUNICATION
		SERCOM5->USART.INTFLAG.bit.ERROR = 1; // cleared flag
		uartWrite(gonder, sizeof(gonder));
		while (1){
			// INFINITIVE LOOP IF THIS HAPPEN
			uartWriteforser5(ESPRESET, sizeof(ESPRESET));
			delay_ms(2000);
			NVIC_SystemReset();
		}
	}		
}

void adc_complete_callback(const struct adc_module *const module){
	/////// YOU DONT NEED TO DO ANYTHING WHEN ADC CONVERTING IS COMPLETED. BUT MAYBE YOU CAN BLINK A LED ETC. IF YOU WANT
}

void configure_adc(void)
{
	struct adc_config config_adc;
	adc_get_config_defaults(&config_adc);
	config_adc.clock_source = GCLK_GENERATOR_2; // 8 Mhz FOR ADC
	config_adc.gain_factor = ADC_GAIN_FACTOR_DIV2;
	config_adc.clock_prescaler = ADC_CLOCK_PRESCALER_DIV512;
	config_adc.reference = ADC_REFERENCE_INTVCC1;
	config_adc.positive_input = ADC_POSITIVE_INPUT_PIN0;
	config_adc.resolution = ADC_RESOLUTION_10BIT;
	adc_init(&adc_instance, ADC, &config_adc);
	adc_enable(&adc_instance);
	
}

void configure_adc_callbacks(void)
{
	adc_register_callback(&adc_instance, adc_complete_callback, ADC_CALLBACK_READ_BUFFER);
	adc_enable_callback(&adc_instance, ADC_CALLBACK_READ_BUFFER);
}

void adjustingForRegisterLevel(void){
	
	PM->APBCMASK.reg |= (PM_APBCMASK_SERCOM3 | PM_APBCMASK_SERCOM5);
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_SERCOM3_CORE;
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_SERCOM5_CORE;
	PORT->Group[0].PINCFG[22].bit.PMUXEN = 1;
	PORT->Group[0].PMUX[11].bit.PMUXE = 0x2; // pa22 is config as sercom3 pad[0]
	PORT->Group[0].PINCFG[23].bit.PMUXEN = 1;
	PORT->Group[0].PMUX[11].bit.PMUXO = 0x2; // pa23 is config as sercom3 pad[1]

	PORT->Group[0].PINCFG[20].bit.PMUXEN = 1;
	PORT->Group[0].PMUX[10].bit.PMUXE = 0x2; //  is config as sercom5 pad[0]
	PORT->Group[0].PINCFG[21].bit.PMUXEN = 1;
	PORT->Group[0].PMUX[10].bit.PMUXO = 0x2; //  is config as sercom pad[1]
}

void adjustingForinterrupt(void){
	
	system_interrupt_enable_global();
	system_interrupt_set_priority(SYSTEM_INTERRUPT_MODULE_SERCOM5, SYSTEM_INTERRUPT_PRIORITY_LEVEL_0);
	system_interrupt_set_priority(SYSTEM_INTERRUPT_MODULE_SERCOM3, SYSTEM_INTERRUPT_PRIORITY_LEVEL_1);
	system_interrupt_set_priority(SYSTEM_INTERRUPT_MODULE_ADC, SYSTEM_INTERRUPT_PRIORITY_LEVEL_2);	
}