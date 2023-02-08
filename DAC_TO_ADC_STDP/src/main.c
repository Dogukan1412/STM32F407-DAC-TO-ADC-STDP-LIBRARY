
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

//DAC channel1 with DAC_OUT1 (PA4) as output
//DAC channel2 with DAC_OUT2 (PA5) as output


void GPIO_Config(void);
void ADC_Config(void);
void DAC1_Config(void);
void delay(uint32_t);
uint16_t Read_Adc();

uint16_t dac_value, adc_value;
float voltage;

int main(void)
{
	GPIO_Config();
	ADC_Config();
	DAC1_Config();

  while (1)
  {
	  // creating triangle waveform

	 int D_Value = 0;
	 while(D_Value < 4000) {
		 DAC_SetChannel1Data(DAC_Align_12b_R, D_Value);
		 D_Value+=100;
		 dac_value = DAC_GetDataOutputValue(DAC_Channel_1);				// we can see variable in the dac1 using this function
		 voltage = (3.3 * dac_value) / 4096;							// calculate voltage
		 adc_value = Read_Adc();										// read adc value from DAC1(PA4) pin using jumper
		 delay(210000);													// wait 10ms
	 }

	 while(D_Value > 1) {
		 DAC_SetChannel1Data(DAC_Align_12b_R, D_Value);
		 D_Value-=100;
		 dac_value = DAC_GetDataOutputValue(DAC_Channel_1);				// we can see variable in the dac1 using this function
		 voltage = (3.3 * dac_value) / 4096;							// calculate voltage
		 adc_value = Read_Adc();										// read adc value from DAC1(PA4) pin using jumper
		 delay(210000);													// wait 10ms
	 }

	 // Don't forget to connect the PA1 and PA4 pins to each other using a jumper!
  }
}


void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO = {0};

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO.GPIO_Mode = GPIO_Mode_AN;
	GPIO.GPIO_Pin = GPIO_Pin_4;
	GPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO);


	GPIO_InitTypeDef adc = {0};

	adc.GPIO_Mode = GPIO_Mode_AN;
	adc.GPIO_OType = GPIO_OType_PP;
	adc.GPIO_Pin = GPIO_Pin_1;
	adc.GPIO_PuPd = GPIO_PuPd_DOWN;
	adc.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &adc);
}


void ADC_Config(void)
{
	ADC_InitTypeDef adc = {0};
	ADC_CommonInitTypeDef adc_com = {0};

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	adc_com.ADC_DMAAccessMode = DISABLE;
	adc_com.ADC_Mode = ADC_Mode_Independent;
	adc_com.ADC_Prescaler = ADC_Prescaler_Div2;
	adc_com.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles;
	ADC_CommonInit(&adc_com);

	adc.ADC_ContinuousConvMode = ENABLE;
	adc.ADC_DataAlign = ADC_DataAlign_Right;
	adc.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	adc.ADC_Resolution = ADC_Resolution_12b;
	adc.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &adc);
	ADC_Cmd(ADC1, ENABLE);

}


void DAC1_Config(void)
{
	DAC_InitTypeDef dac = {0};

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	dac.DAC_WaveGeneration = DAC_WaveGeneration_None;
	dac.DAC_Trigger = DAC_Trigger_None;
	dac.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

	DAC_Init(DAC_Channel_1, &dac);
	DAC_Cmd(DAC_Channel_1, ENABLE);
}


uint16_t Read_Adc()
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_112Cycles);    // Configures for the selected ADC regular channel its corresponding rank in the sequencer and its sample time
	ADC_SoftwareStartConv(ADC1);                                                   // starting adc conversion
	while(!(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)));                               // checking Conversion complete
	return ADC_GetConversionValue(ADC1);
}


void delay(uint32_t time)
{
	while(time--);
}



/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
