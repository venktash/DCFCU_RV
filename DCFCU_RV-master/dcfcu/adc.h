
#ifndef ADC_H_
#define ADC_H_
/*
//int conversion( );
uint16_t  ReadADC(uint8_t ch);
void adc_init();

void adc_init()
{
	ADMUX=0x40;  //0100 0000
	ADCSRA=0xA7;
}

//int  conversion()
//{
	//int x;
	//ADCSRA=ADCSRA|(1<<ADSC);
	//while((ADCSRA&(1<<ADIF))==0){}
	//x=ADC;
	//return x;//ascii(x);
//}

uint16_t  ReadADC(uint8_t ch)
{
	ch=ch&0b00000111; 		     // Select ADC Channel
	ADMUX|=ch;                                             // Start Single conversion
	ADCSRA|=(0b01000000);                       // Set ADSC
	
	// Wait for conversion to complete
	while(!(ADCSRA & (0b00010000)));     // Wait for ADIF bit of ADCSRA


	ADCSRA|=(00010000); 		    // Clear ADIF by writing one to it
	return(ADC);
}
*/



int ReadADC(uint8_t ch)
{
	ADC=0;
	//Select ADC Channel ch must be 0-7
	ADMUX = ADMUX & 0b11100000; //ADMUX7,6 is to select VREF and ADCMUX 5 is ADLAR
	ch=ch&0b00011111;    //7
	ADMUX|=ch;		       // selecting the ADC channel
	//Start Single conversion
	ADCSRA |= (1<<ADSC);
	//Wait for conversion to complete
	while(!(ADCSRA&(1<<ADIF)));	// ADIF 1 when conversion completes
	//Clear ADIF by writing one to it
	ADCSRA|=(1<<ADIF); // thisis how ADIF is cleared
	return(ADC);
}
void initADC()
{
	ADMUX=(1<<REFS0);  // For Aref=AVcc;
	ADCSRA=(1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS1); //Prescalar div factor =128
}



#endif /* ADC_H_ */