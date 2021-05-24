/*
 * lcd4.h
 *
 * Created: 10-02-2021 13:10:55
 *  Author: Venktash kumar
 */ 


#ifndef LCD4_H_
#define LCD4_H_

#define lcd PORTD
#define rs PD0
#define rw PD1
#define en PD2

void Lcd_init()
{
	Lcd_Cmd(0x02);
	Lcd_Cmd(0x28);
	Lcd_Cmd(0x06);
	Lcd_Cmd(0x0C);
	//Lcd_Cmd(0x01);
	Lcd_Cmd(0x80);
	
}
void Lcd_Data(char data)
{
	lcd = data&0xf0;
	lcd|=(1<<rs);
	lcd&=~(1<<rw);
	lcd|=(1<<en);
	_delay_ms(5);
	lcd&=~(1<<en);
	
	_delay_ms(10);
	
	lcd = (data<<4)&0xf0;
	lcd|=(1<<rs);
	lcd&=~(1<<rw);
	lcd|=(1<<en);
	_delay_ms(5);
	lcd&=~(1<<en);
	
}

void Lcd_Cmd(char cmd)
{
	lcd = cmd&0xf0;
	lcd&=~(1<<rs);
	lcd&=~(1<<rw);
	lcd|=(1<<en);
	_delay_ms(5);
	lcd&=~(1<<en);
	
	_delay_ms(10);
	
	lcd = (cmd<<4)&0xf0;
	lcd&=~(1<<rs);
	lcd&=~(1<<rw);
	lcd|=(1<<en);
	_delay_ms(5);
	lcd&=~(1<<en);
}

void Lcd_String(char *str)
{
	while(*str!='\0')
	{
		Lcd_Data(*str);
		str++;
	}
}





#endif /* LCD4_H_ */