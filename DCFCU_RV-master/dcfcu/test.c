#include <avr/io.h>
#include <util/delay.h>
#include "lcd4.h"
#include "adc.h"
#define F_CPU 8000000UL

///////   VARIABLE    //////////////////

double getTemp(int x);

// this lines have been deleted for tetisng purpose


char array[10],array2[10];     //   1 4 6 2 0  01246
char f1arr[10],f2arr[10];
int flag1,flag2,flag3,flag4=0;

int  temp=0; int DC_volt=0;
int f1=0;int f2=0; int count=0;
 int smoke=0;


double getTemp(int x)
{     
	int val=0;
	long R=0;
	double Thermister=0;
	val = ReadADC(x);		// store adc value on val register 
	R=((10230000/val) - 10000);  // calculate the resistance 
	Thermister = log(R);	// calculate natural log of resistance 
	// Steinhart-Hart Thermistor Equation: 
	//  Temperature in Kelvin = 1 / (A + B[ln(R)] + C[ln(R)]^3)		
	//  where A = 0.001129148, B = 0.000234125 and C = 8.76741*10^-8  
	Thermister = 1 / (0.001129148 + (0.000234125 * Thermister) + (0.0000000876741 * Thermister * Thermister * Thermister));
	Thermister = Thermister - 273.15;  // convert kelvin to �C 
	
	return Thermister;
}


void main()
{
	DDRD=0xff;  //lcd interface 00001000
	DDRC=0x00;   // adc intterface
	DDRB =0xff;   // fan connection 
	//int  temp=0; int DC_volt=0;
	//int f1=0;int f2=0; int smoke=0;
	
	
	////////////////////  PWM SETUP TIMER 1   ///////////////////
	
	// set PWM for 50% duty cycle at 10bit
	TCCR1A |= (1 << COM1A1);
	// set non-inverting mode
	TCCR1A |= (1 << WGM11) | (1 << WGM10);
	// set 10bit phase corrected PWM Mode
	TCCR1B |= (1 << CS11);
	
		
	//////////////  INITIALIZATIONS   ////////////////
	
	Lcd_init();
	initADC();	
	
	Lcd_Cmd(0x80);
	Lcd_String("  Welcome to    ");
	Lcd_Cmd(0xc0);
	Lcd_String("     DCFCU  ");
	_delay_ms(2000);
	Lcd_Cmd(0x01);
	
	
	//////////////  WHILE CONDITION FOR COMPLETE CODE SETUP   //////////////////////
	while (1) 
	{
		
		f2= ReadADC(3);
		f1= ReadADC(4);
		
		smoke=ReadADC(2);
	///////////  ADC READ FOR TEMP.  AND VOLTAGE READ /////////////////////
	
	temp = getTemp(1);// store temperature value on temp resistor
	
	DC_volt = ReadADC(0);                     // Voltage  read  55.5 ==5 ==998 == 48v     0--5  0--1023 10bit 
	DC_volt = (DC_volt*55.5)/1024 ;    // 55.1
	                
	 ///  TEMP. AND VOLTAGE CONVERSION INTO STRING AND DISPLAYED  /////////////////////////
	
	itoa(temp,array,10);
	itoa(DC_volt,array2,10);
	
	//itoa(smoke,f1arr,10);
	//itoa(f2,f2arr,10);
	//
	//Lcd_Cmd(0x80);
	//Lcd_String(f1arr);
	//Lcd_Cmd(0xc0);
	//Lcd_String(f2arr);
	//_delay_ms(2000);
	//Lcd_Cmd(0x01);

	/////////////////////////  CONDITION FOR FAN SPEED CONTROL   ////////////////////

    if (temp<25 && smoke>1000)
	 {
		
		 OCR1A= 0;//
		 PORTB&= ~(1<<2);  //  smoke clear
		 PORTB&= ~(1<<3);    //f1 clear 
		 PORTB&= ~(1<<4);    // f2 clear
		 
		 itoa(f1,f1arr,10);
		 itoa(f2,f2arr,10);
		 
		 Lcd_Cmd(0x80);
		 Lcd_String(f1arr);
		 Lcd_Cmd(0xc0);
		 Lcd_String(f2arr);
		 _delay_ms(2000);
		 Lcd_Cmd(0x01);
		
		
		Lcd_Cmd(0x80);
		Lcd_String("T=");
		Lcd_Cmd(0x82);
		Lcd_String(array);
		Lcd_Cmd(0xc0);
		Lcd_String("V=");
		Lcd_Cmd(0xc2);
		Lcd_String(array2);
		_delay_ms(2000);  
		
											Lcd_Cmd(0x01);
											_delay_ms(500);
		
											Lcd_Cmd(0X80);
											Lcd_String("F1 STOP");
											Lcd_Cmd(0xc0);
											Lcd_String("F2 STOP");
											_delay_ms(3000);
		
																Lcd_Cmd(0x01);
																_delay_ms(500);
		
																//PORTD&=~(1<<3);1/
	  }  
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	  
   if ((temp>=25 && temp<=30 ) )
	{
	flag1=1;	
	
	if ((flag1==1 && smoke >1000))
	{
		if (( f1>1000 && f2 >1000 ) && (count==0))
	      {
			  //itoa(f1,f1arr,10);
			  //itoa(f2,f2arr,10);
			  //
			  //Lcd_Cmd(0x80);
			  //Lcd_String(f1arr);
			  //Lcd_Cmd(0xc0);
			  //Lcd_String(f2arr);
			  //_delay_ms(2000);
			  //Lcd_Cmd(0x01);
			  
				OCR1A= 300;//
				PORTB &= ~(1<<2);
				PORTB&=~(1<<3);
				PORTB&=~(1<<4);
			
					Lcd_Cmd(0x80);
					Lcd_String("T=");
					Lcd_Cmd(0x82);
					Lcd_String(array);
			
							Lcd_Cmd(0xc0);
							Lcd_String("V=");
							Lcd_Cmd(0xc2);
							Lcd_String(array2);
							_delay_ms(2000);
		
			
										Lcd_Cmd(0x01);
										_delay_ms(500);
										
										Lcd_Cmd(0X80);
										Lcd_String("F1 10% SPEED");
										Lcd_Cmd(0xc0);
										Lcd_String("F2 10% SPEED");
										_delay_ms(3000);
			
											Lcd_Cmd(0x01);
											_delay_ms(500);
											flag1=0;count=1;
		  }
	
		else if (( f1<1000 && f2 <1000 ) && (count==1 ))
		{
			OCR1A= 300;//
			PORTB &= ~(1<<2);
			PORTB&=~(1<<3);
			PORTB&=~(1<<4);
			
						Lcd_Cmd(0x80);
						Lcd_String("T=");
						Lcd_Cmd(0x82);
						Lcd_String(array);
			
											Lcd_Cmd(0xc0);
											Lcd_String("V=");
											Lcd_Cmd(0xc2);
											Lcd_String(array2);
											_delay_ms(2000);
			
																	Lcd_Cmd(0x01);
																	_delay_ms(500);
			
																	Lcd_Cmd(0X80);
																	Lcd_String("F1 10% SPEED");
																	Lcd_Cmd(0xc0);
																	Lcd_String("F2 10% SPEED");
																	_delay_ms(3000);
			
																	Lcd_Cmd(0x01);
																	_delay_ms(500);
																	flag1=0;count=1;
		}
		
		
		
		else if((f1<1000 && f2 >1000) && (count==1))
		{
			
		  OCR1A= 300;//
		  PORTB &= ~(1<<2);
		  PORTB&=~(1<<3);
		  PORTB|=(1<<4);
		
						Lcd_Cmd(0x80);
						Lcd_String("T=");
						Lcd_Cmd(0x82);
						Lcd_String(array);
		
								Lcd_Cmd(0xc0);
								Lcd_String("V=");
								Lcd_Cmd(0xc2);
								Lcd_String(array2);
								_delay_ms(2000);
								
		
													Lcd_Cmd(0x01);
													_delay_ms(500);
		
														Lcd_Cmd(0X80);
														Lcd_String("F1 10% SPEED");
														Lcd_Cmd(0xc0);
														Lcd_String("F2 FAIL");
														_delay_ms(3000);
		
												Lcd_Cmd(0x01);
												
												flag1=0; count=1;
		}
		//
		else if ((f1>1000 && f2<1000) && (count==1))
		{
			
			OCR1A= 300;//
			PORTB &= ~(1<<2);
			PORTB|=(1<<3);
			PORTB&=~(1<<4);
			
					Lcd_Cmd(0x80);
					Lcd_String("T=");
					Lcd_Cmd(0x82);
					Lcd_String(array);
			//
									Lcd_Cmd(0xc0);
									Lcd_String("V=");
									Lcd_Cmd(0xc2);
									Lcd_String(array2);
									_delay_ms(2000);
			
															Lcd_Cmd(0x01);
															_delay_ms(500);
			
															Lcd_Cmd(0X80);
															Lcd_String("F1 FAIL");
															Lcd_Cmd(0xc0);
															Lcd_String("F2 10% SPEED");
															_delay_ms(3000);
															Lcd_Cmd(0x01);
														    flag1=0; count=1;
		}
	
	
	
		else if ((f1>1000 && f2>1000)  && (count==1))
		{
			
			OCR1A= 300;//
			PORTB &= ~(1<<2);
			PORTB|=(1<<3);
			PORTB|=(1<<4);
			
						Lcd_Cmd(0x80);
						Lcd_String("T=");
						Lcd_Cmd(0x82);
						Lcd_String(array);
			
										Lcd_Cmd(0xc0);
										Lcd_String("V=");
										Lcd_Cmd(0xc2);
										Lcd_String(array2);
										_delay_ms(2000);
			
													Lcd_Cmd(0x01);
													_delay_ms(500);
			
													Lcd_Cmd(0X80);
													Lcd_String("F1 FAIL");
													Lcd_Cmd(0xc0);
													Lcd_String("F2 FAIL");
													_delay_ms(3000);
			
													Lcd_Cmd(0x01);
												   flag1=0; count=1;
		}
	}

	}
	//////////////////////////////testing//////////////////////////////////////////////////////////
	
	  if ((temp>=31 && temp<=35 ) )
	  {
		  flag1=1;
		  
		  if ((flag1==1 && smoke >1000))
		  {
			  if (( f1>1000 && f2 >1000 ) && (count==0))
			  {
				  //itoa(f1,f1arr,10);
				  //itoa(f2,f2arr,10);
				  //
				  //Lcd_Cmd(0x80);
				  //Lcd_String(f1arr);
				  //Lcd_Cmd(0xc0);
				  //Lcd_String(f2arr);
				  //_delay_ms(2000);
				  //Lcd_Cmd(0x01);
				  
				  OCR1A= 500;//
				  PORTB &= ~(1<<2);
				  PORTB&=~(1<<3);
				  PORTB&=~(1<<4);
				  
						  Lcd_Cmd(0x80);
						  Lcd_String("T=");
						  Lcd_Cmd(0x82);
						  Lcd_String(array);
				  
								  Lcd_Cmd(0xc0);
								  Lcd_String("V=");
								  Lcd_Cmd(0xc2);
								  Lcd_String(array2);
								  _delay_ms(2000);
				  
				  
							  Lcd_Cmd(0x01);
							  _delay_ms(500);
				  
							  Lcd_Cmd(0X80);
							  Lcd_String("F1 25% SPEED");
							  Lcd_Cmd(0xc0);
							  Lcd_String("F2 25% SPEED");
							  _delay_ms(3000);
				  
				  Lcd_Cmd(0x01);
				  _delay_ms(500);
				  flag1=0;count=1;
			  }
			  
			  else if (( f1<1000 && f2 <1000 ) && (count==1 ))
			  {
				  OCR1A= 500;//
				  PORTB &= ~(1<<2);
				  PORTB&=~(1<<3);
				  PORTB&=~(1<<4);
				  
					  Lcd_Cmd(0x80);
					  Lcd_String("T=");
					  Lcd_Cmd(0x82);
					  Lcd_String(array);
				  
							  Lcd_Cmd(0xc0);
							  Lcd_String("V=");
							  Lcd_Cmd(0xc2);
							  Lcd_String(array2);
							  _delay_ms(2000);
							  
							  Lcd_Cmd(0x01);
							  _delay_ms(500);
				  
								  Lcd_Cmd(0X80);
								  Lcd_String("F1 25% SPEED");
								  Lcd_Cmd(0xc0);
								  Lcd_String("F2 25% SPEED");
								  _delay_ms(3000);
				  
								  Lcd_Cmd(0x01);
								  _delay_ms(500);
								  flag1=0;count=1;
			  }
			  
			  
			  
			  else if((f1<1000 && f2 >1000) && (count==1))
			  {
				  
				  OCR1A= 500;//
				  PORTB &= ~(1<<2);
				  PORTB&=~(1<<3);
				  PORTB|=(1<<4);
				  
					  Lcd_Cmd(0x80);
					  Lcd_String("T=");
					  Lcd_Cmd(0x82);
					  Lcd_String(array);
				  
							  Lcd_Cmd(0xc0);
							  Lcd_String("V=");
							  Lcd_Cmd(0xc2);
							  Lcd_String(array2);
							  _delay_ms(2000);
				  
				  
							  Lcd_Cmd(0x01);
							  _delay_ms(500);
				  
									  Lcd_Cmd(0X80);
									  Lcd_String("F1 25% SPEED");
									  Lcd_Cmd(0xc0);
									  Lcd_String("F2 FAIL");
									  _delay_ms(3000);
				  
									  Lcd_Cmd(0x01);
				  
									  flag1=0; count=1;
			  }
			  //
			  else if ((f1>1000 && f2<1000) && (count==1))
			  {
				  
				  OCR1A= 500;//
				  PORTB &= ~(1<<2);
				  PORTB|=(1<<3);
				  PORTB&=~(1<<4);
				  
						  Lcd_Cmd(0x80);
						  Lcd_String("T=");
						  Lcd_Cmd(0x82);
						  Lcd_String(array);
				  //
								  Lcd_Cmd(0xc0);
								  Lcd_String("V=");
								  Lcd_Cmd(0xc2);
								  Lcd_String(array2);
								  _delay_ms(2000);
				  
								  Lcd_Cmd(0x01);
								_delay_ms(500);
				  
										  Lcd_Cmd(0X80);
										  Lcd_String("F1 FAIL");
										  Lcd_Cmd(0xc0);
										  Lcd_String("F2 25% SPEED");
										  _delay_ms(3000);
										  Lcd_Cmd(0x01);
										  flag1=0; count=1;
			  }
			  
			  
			  
			  else if ((f1>1000 && f2>1000)  && (count==1))
			  {
				  
				  OCR1A= 0;//
				  PORTB &= ~(1<<2);
				  PORTB|=(1<<3);
				  PORTB|=(1<<4);
				  
					  Lcd_Cmd(0x80);
					  Lcd_String("T=");
					  Lcd_Cmd(0x82);
					  Lcd_String(array);
				  
							  Lcd_Cmd(0xc0);
							  Lcd_String("V=");
							  Lcd_Cmd(0xc2);
							  Lcd_String(array2);
							  _delay_ms(2000);
				  
							  Lcd_Cmd(0x01);
							  _delay_ms(500);
				  
									  Lcd_Cmd(0X80);
									  Lcd_String("F1 FAIL");
									  Lcd_Cmd(0xc0);
									  Lcd_String("F2 FAIL");
									  _delay_ms(3000);
				  
									  Lcd_Cmd(0x01);
									  flag1=0; count=1;
			  }
		  }
	  }
	
	
	/////////////////////////////////////////////////////////////////////////////////////////
	
	    
   
   
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	if ((temp>=36 && temp<=40 ) )
	{
		flag1=1;
		
		if ((flag1==1 && smoke >1000))
		{
			if (( f1>1000 && f2 >1000 ) && (count==0))
			{
				
				
				OCR1A= 700;//
				PORTB &= ~(1<<2);
				PORTB&=~(1<<3);
				PORTB&=~(1<<4);
				
						Lcd_Cmd(0x80);
						Lcd_String("T=");
						Lcd_Cmd(0x82);
						Lcd_String(array);
						
						Lcd_Cmd(0xc0);
						Lcd_String("V=");
						Lcd_Cmd(0xc2);
						Lcd_String(array2);
						_delay_ms(2000);
				
				
								Lcd_Cmd(0x01);
								_delay_ms(500);
				
								Lcd_Cmd(0X80);
								Lcd_String("F1 50% SPEED");
								Lcd_Cmd(0xc0);
								Lcd_String("F2 50% SPEED");
								_delay_ms(3000);
				
								Lcd_Cmd(0x01);
								_delay_ms(500);
								flag1=0;count=1;
			}
			
			else if (( f1<1000 && f2 <1000 ) && (count==1 ))
			{
				OCR1A= 700;//
				PORTB &= ~(1<<2);
				PORTB&=~(1<<3);
				PORTB&=~(1<<4);
				
				Lcd_Cmd(0x80);
				Lcd_String("T=");
				Lcd_Cmd(0x82);
				Lcd_String(array);
				
							Lcd_Cmd(0xc0);
							Lcd_String("V=");
							Lcd_Cmd(0xc2);
							Lcd_String(array2);
							_delay_ms(2000);
							
							Lcd_Cmd(0x01);
							_delay_ms(500);
				
									Lcd_Cmd(0X80);
									Lcd_String("F1 50% SPEED");
									Lcd_Cmd(0xc0);
									Lcd_String("F2 50% SPEED");
									_delay_ms(3000);
				
									Lcd_Cmd(0x01);
									_delay_ms(500);
									flag1=0;count=1;
			}
			
			
			
			else if((f1<1000 && f2 >1000) && (count==1))
			{
				
				OCR1A= 700;//
				PORTB &= ~(1<<2);
				PORTB&=~(1<<3);
				PORTB|=(1<<4);
				
						Lcd_Cmd(0x80);
						Lcd_String("T=");
						Lcd_Cmd(0x82);
						Lcd_String(array);
				
									Lcd_Cmd(0xc0);
									Lcd_String("V=");
									Lcd_Cmd(0xc2);
									Lcd_String(array2);
									_delay_ms(2000);
				
				
												Lcd_Cmd(0x01);
												_delay_ms(500);
				
												Lcd_Cmd(0X80);
												Lcd_String("F1 50% SPEED");
												Lcd_Cmd(0xc0);
												Lcd_String("F2 FAIL");
												_delay_ms(3000);
				
												Lcd_Cmd(0x01);
				
												flag1=0; count=1;
			}
			//
			else if ((f1>1000 && f2<1000) && (count==1))
			{
				
				OCR1A= 700;//
				PORTB &= ~(1<<2);
				PORTB|=(1<<3);
				PORTB&=~(1<<4);
				
						Lcd_Cmd(0x80);
						Lcd_String("T=");
						Lcd_Cmd(0x82);
						Lcd_String(array);
				//
								Lcd_Cmd(0xc0);
								Lcd_String("V=");
								Lcd_Cmd(0xc2);
								Lcd_String(array2);
								_delay_ms(2000);
				
								Lcd_Cmd(0x01);
								_delay_ms(500);
				
										Lcd_Cmd(0X80);
										Lcd_String("F1 FAIL");
										Lcd_Cmd(0xc0);
										Lcd_String("F2 50% SPEED");
										_delay_ms(3000);
										Lcd_Cmd(0x01);
										flag1=0; count=1;
			}
			
			
			
			else if ((f1>1000 && f2>1000)  && (count==1))
			{
				
				OCR1A= 0;//
				PORTB &= ~(1<<2);
				PORTB|=(1<<3);
				PORTB|=(1<<4);
				
						Lcd_Cmd(0x80);
						Lcd_String("T=");
						Lcd_Cmd(0x82);
						Lcd_String(array);
				
									Lcd_Cmd(0xc0);
									Lcd_String("V=");
									Lcd_Cmd(0xc2);
									Lcd_String(array2);
									_delay_ms(2000);
				
									Lcd_Cmd(0x01);
									_delay_ms(500);
				
										Lcd_Cmd(0X80);
										Lcd_String("F1 FAIL");
										Lcd_Cmd(0xc0);
										Lcd_String("F2 FAIL");
										_delay_ms(3000);
				
										Lcd_Cmd(0x01);
										flag1=0; count=1;
			}
		}
	}
	
	
	
		   
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
	  if (temp>=40 && temp<=55) 
	  {
		  flag1=1;
		  
		  if ((flag1==1 && smoke >1000))
		  {
			  if (( f1>1000 && f2 >1000 ) && (count==0))
			  {
				  
				  
						  OCR1A= 1023;//
						  PORTB &= ~(1<<2);
						  PORTB&=~(1<<3);
						  PORTB&=~(1<<4);
				  
						  Lcd_Cmd(0x80);
						  Lcd_String("T=");
						  Lcd_Cmd(0x82);
										  Lcd_String(array);
										  
										  Lcd_Cmd(0xc0);
										  Lcd_String("V=");
										  Lcd_Cmd(0xc2);
										  Lcd_String(array2);
										  _delay_ms(2000);
										  
										  
										  Lcd_Cmd(0x01);
										  _delay_ms(500);
										  
													  Lcd_Cmd(0X80);
													  Lcd_String("F1 100% SPEED");
													  Lcd_Cmd(0xc0);
													  Lcd_String("F2 100% SPEED");
													  _delay_ms(3000);
				  
													  Lcd_Cmd(0x01);
													  _delay_ms(500);
													  flag1=0;count=1;
			  }
			  
			  else if (( f1<1000 && f2 <1000 ) && (count==1 ))
			  {
				  OCR1A= 1023;//
				  PORTB &= ~(1<<2);
				  PORTB&=~(1<<3);
				  PORTB&=~(1<<4);
				  
						  Lcd_Cmd(0x80);
						  Lcd_String("T=");
						  Lcd_Cmd(0x82);
						  Lcd_String(array);
				  
									  Lcd_Cmd(0xc0);
									  Lcd_String("V=");
									  Lcd_Cmd(0xc2);
									  Lcd_String(array2);
									  _delay_ms(2000);
									  
									  Lcd_Cmd(0x01);
									  _delay_ms(500);
				  
												  Lcd_Cmd(0X80);
												  Lcd_String("F1 100% SPEED");
												  Lcd_Cmd(0xc0);
												  Lcd_String("F2 100% SPEED");
												  _delay_ms(3000);
				  
												  Lcd_Cmd(0x01);
												  _delay_ms(500);
												  flag1=0;count=1;
			  }
			  
			  
			  
			  else if((f1<1000 && f2 >1000) && (count==1))
			  {
				  
				  OCR1A= 1023;//
				  PORTB &= ~(1<<2);
				  PORTB&=~(1<<3);
				  PORTB|=(1<<4);
				  
							  Lcd_Cmd(0x80);
							  Lcd_String("T=");
							  Lcd_Cmd(0x82);
							  Lcd_String(array);
				  
										  Lcd_Cmd(0xc0);
										  Lcd_String("V=");
										  Lcd_Cmd(0xc2);
										  Lcd_String(array2);
										  _delay_ms(2000);
				  
				  
										  Lcd_Cmd(0x01);
										  _delay_ms(500);
				  
													  Lcd_Cmd(0X80);
													  Lcd_String("F1 100% SPEED");
													  Lcd_Cmd(0xc0);
													  Lcd_String("F2 FAIL");
													  _delay_ms(3000);
				  
													  Lcd_Cmd(0x01);
				  
													  flag1=0; count=1;
			  }
			  
			  else if ((f1>1000 && f2<1000) && (count==1))
			  {
				  
				  OCR1A= 1023;//
				  PORTB &= ~(1<<2);
				  PORTB|=(1<<3);
				  PORTB&=~(1<<4);
				  
						  Lcd_Cmd(0x80);
						  Lcd_String("T=");
						  Lcd_Cmd(0x82);
						  Lcd_String(array);
				  //
									  Lcd_Cmd(0xc0);
									  Lcd_String("V=");
									  Lcd_Cmd(0xc2);
									  Lcd_String(array2);
									  _delay_ms(2000);
									  
									  Lcd_Cmd(0x01);
									  _delay_ms(500);
				  
												  Lcd_Cmd(0X80);
												  Lcd_String("F1 FAIL");
												  Lcd_Cmd(0xc0);
												  Lcd_String("F2 100% SPEED");
												  _delay_ms(3000);
												  Lcd_Cmd(0x01);
												  flag1=0; count=1;
			  }
			  
			  
			  
			  else if ((f1>1000 && f2>1000)  && (count==1))
			  {
				  
				  OCR1A= 0;//
				  PORTB &= ~(1<<2);
				  PORTB|=(1<<3);
				  PORTB|=(1<<4);
				  
						  Lcd_Cmd(0x80);
						  Lcd_String("T=");
						  Lcd_Cmd(0x82);
						  Lcd_String(array);
				  
								  Lcd_Cmd(0xc0);
								  Lcd_String("V=");
								  Lcd_Cmd(0xc2);
								  Lcd_String(array2);
								  _delay_ms(2000);
				  
								  Lcd_Cmd(0x01);
								  _delay_ms(500);
				  
										  Lcd_Cmd(0X80);
										  Lcd_String("F1 FAIL");
										  Lcd_Cmd(0xc0);
										  Lcd_String("F2 FAIL");
										  _delay_ms(3000);
				  
										  Lcd_Cmd(0x01);
										  flag1=0; count=1;
			  }
		  }
	  }    
 
 
   else  if (smoke<=1000 )
   {
	   PORTB |= (1<<2);
	    OCR1A = 0;
	    PORTB|=(1<<3);
	    PORTB|=(1<<4);
	   Lcd_Cmd(0x80);
	   Lcd_String("T=");
	   Lcd_Cmd(0x82);
	   Lcd_String(array);
	   
					   Lcd_Cmd(0xc0);
					   Lcd_String("V=");
					   Lcd_Cmd(0xc2);
					   Lcd_String(array2);
					   _delay_ms(2000);
	   
									   Lcd_Cmd(0x01);
									   _delay_ms(500);
	   
									   Lcd_Cmd(0X80);
									   Lcd_String("F1 STOP");
									   Lcd_Cmd(0xc0);
									   Lcd_String("F2 STOP");
									   _delay_ms(3000);
	   
														   Lcd_Cmd(0x01);
														   _delay_ms(500);
														   
														   Lcd_Cmd(0x80);
														   Lcd_String("SMOKE ALARM ONN");
														   _delay_ms(3000);
														   Lcd_Cmd(0X01);
														   _delay_ms(500);
	   
	  
  
  }
  
  
  
  //else if ((f1 > 1000  && f2>1000) &&  (temp >10  && smoke >1000))
  //{
	  //PORTB &=~(1<<2);
	  //PORTB|=(1<<3);
	  //PORTB|=(1<<4);
					  //Lcd_Cmd(0x80);
					  //Lcd_String("T=");
					  //Lcd_Cmd(0x82);
					  //Lcd_String(array);
	  //
										  //Lcd_Cmd(0xc0);
										  //Lcd_String("V=");
										  //Lcd_Cmd(0xc2);
										  //Lcd_String(array2);
										  //_delay_ms(2000);
	  //
										  //Lcd_Cmd(0x01);
										  //_delay_ms(500);
	  //
															  //Lcd_Cmd(0X80);
															  //Lcd_String("F1 FAIL");
															  //Lcd_Cmd(0xc0);
															  //Lcd_String("F2 FAIL");
															  //_delay_ms(3000);
	  //
															  //Lcd_Cmd(0x01);
															  //_delay_ms(500);
	  //
																			  //Lcd_Cmd(0x80);
																			  //Lcd_String("SMOKE ALARM OFF");
																			  //_delay_ms(3000);
																			  //Lcd_Cmd(0X01);
																			  //_delay_ms(500);
																			  //
//
   //}*/

 }
			

}


