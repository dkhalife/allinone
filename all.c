//**
//  Software Constants
//**

#define ON			1
#define OFF			0
#define VOLTAGE		5000

#define RED			0x01;
#define GREEN		0x02;
#define BLUE		0x04;

#define RGB			PORTE
#define RGB1		RA4_bit
#define RGB2		RA5_bit

#define BACKLIGHT	RC7_bit

#define UP			7
#define DOWN		6
#define LEFT		5
#define RIGHT		4
#define OK			3
#define BACK		2

#define THERMO1		0
#define THERMO2		1
#define T1			RC0_bit
#define T2			RC1_bit

#define VOLT1		2
#define VOLT2		3
#define LIGHT		4 // FIX THIS WHEN DOING THE LIGHT SENSOR PART

#define LIMIT_COLD	0
#define LIMIT_HOT	25

//**
// Aliases
//**
#define Load		EEPROM_Read
#define Save		EEPROM_Write

//**
//  Addresses
//**
#define SET_BACKLIGHT	0x80
#define SET_TEMPERATURE 0x81
#define SET_VOLTAGE		0x82
#define SET_RGB1		0x83
#define SET_RGB2		0x84

//**
//  LCD Configuration
//**

sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D4 at RB0_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D7 at RB3_bit;

sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;

//**
//  Variable Definitions
//**

unsigned int i = 0;
signed int Cnt1 = 0;
signed int Cnt2 = 0;
signed int Cnt3 = 0;
unsigned int adc_rd1;
unsigned int adc_rd2;
unsigned int adc_rd3;
unsigned int rgb1_set;
unsigned int rgb2_set;
unsigned int temp_set;
unsigned int screen = 1;
unsigned int nextscreen = 0;
unsigned int oldstate[] = {0,0,0,0,0,0,0,0};

long tlong1;
long tlong2;
long cntr1;
long cntr2;

unsigned char Flag=2;
unsigned char Str1[7];
unsigned char Str2[7];
unsigned char Str3[7];

//**
//  Special Characters
//**
const char DEGREE[] = {14,10,14,0,0,0,0,0};
const char ARROW_UP[] = {4,14,31,0,0,0,0,0};
const char ARROW_DOWN[] = {0,0,0,0,0,31,14,4};
const char ARROW_LEFT[] = {0,2,6,14,14,6,2,0};
const char ARROW_RIGHT[] = {0,8,12,14,14,12,8,0};
const char POINTER[] = {0,8,12,14,12,8,0,0};
const char DELTA[] = {0,4,4,10,10,17,31,0};

void CC_Degree(char pos_row, char pos_char) {
	char i;
	LCD_Cmd(64);
	for (i = 0; i<=7; i++) LCD_Chr_Cp(DEGREE[i]);
	LCD_Cmd(_LCD_RETURN_HOME);
	LCD_Chr(pos_row, pos_char, 0);
}

void CC_Arrow_Up(char pos_row, char pos_char) {
	char i;
	LCD_Cmd(72);
	for (i = 0; i<=7; i++) LCD_Chr_Cp(ARROW_UP[i]);
	LCD_Cmd(_LCD_RETURN_HOME);
	LCD_Chr(pos_row, pos_char, 1);
}

void CC_Arrow_Down(char pos_row, char pos_char) {
	char i;
	LCD_Cmd(80);
	for (i = 0; i<=7; i++) LCD_Chr_Cp(ARROW_DOWN[i]);
	LCD_Cmd(_LCD_RETURN_HOME);
	LCD_Chr(pos_row, pos_char, 2);
}

void CC_Arrow_Left(char pos_row, char pos_char) {
	char i;
	LCD_Cmd(88);
	for (i = 0; i<=7; i++) LCD_Chr_Cp(ARROW_LEFT[i]);
	LCD_Cmd(_LCD_RETURN_HOME);
	LCD_Chr(pos_row, pos_char, 3);
}

void CC_Arrow_Right(char pos_row, char pos_char) {
	char i;
	LCD_Cmd(96);
	for (i = 0; i<=7; i++) LCD_Chr_Cp(ARROW_RIGHT[i]);
	LCD_Cmd(_LCD_RETURN_HOME);
	LCD_Chr(pos_row, pos_char, 4);
}

void CC_Pointer(char pos_row, char pos_char) {
	char i;
	LCD_Cmd(104);
	for (i = 0; i<=7; i++) LCD_Chr_Cp(POINTER[i]);
	LCD_Cmd(_LCD_RETURN_HOME);
	LCD_Chr(pos_row, pos_char, 5);
}

void CC_Delta(char pos_row, char pos_char) {
	char i;
	LCD_Cmd(112);
	for (i = 0; i<=7; i++) LCD_Chr_Cp(DELTA[i]);
	LCD_Cmd(_LCD_RETURN_HOME);
	LCD_Chr(pos_row, pos_char, 6);
}

//**
//  Button Input
//**
int Input(int pin){
	if (Button(&PORTD, pin, 1, 1)) oldstate[pin] = 1;
	if (oldstate[pin] && Button(&PORTD, pin, 1, 0)){
		oldstate[pin] = 0;
		return ON;
	}
	return OFF;
}

//**
//  TMR0 timer interrupt service routine
//  The program jumps to ISR at every 5ms
//**
void interrupt(){
	TMR0L=100;
	//**
	//  Interrup Re-Configuration:
	//  0b
	//  - Disable all interrupts (0)
	//  - Disable all interrupts (0)
	//  - Enable the TMR0 overflow interrupt (1)
	//  - Disables the INT0 external interrupt (0)
	//  - Disables the RB port change interrupt (0)
	//  - TMR0 register did not overflow (0)
	//  - The INT0 external interrupt did not occur (0)
	//  - None of the RB<7:4> pins have changed state (0)
	//  - Total : 0b00100000;
	//**
	INTCON=0x20;
	
	//**
	//  Light Sensor 
	//**
	cntr1++;
	cntr2++;
	if(screen==20){
		Flag++;
		if(Flag>2){
			Flag=1;
		}
		RGB=0x00;
		RGB1=OFF;
		RGB2=OFF;
		switch(Flag){
			case 1:
				if(rgb1_set==0xFF){
					RGB2=OFF;
					RGB1=ON;
					if((Cnt1<=LIMIT_COLD && temp_set==0xFF) || (Cnt1<=(LIMIT_COLD*9/5+32) && temp_set==0x00)){
						RGB=BLUE;
					}
					else{
						if((Cnt1<=LIMIT_HOT && temp_set==0xFF) || (Cnt1<=(LIMIT_HOT*9/5+32) && temp_set==0x00)){
							RGB=GREEN;
						}
						else{
							RGB=RED;
						}
					}
				}
			break;

			case 2:
				if(rgb2_set==0xFF){
					RGB1=OFF;
					RGB2=ON;
					if((Cnt2<=LIMIT_COLD && temp_set==0xFF) || (Cnt2<=(LIMIT_COLD*9/5+32) && temp_set==0x00)){
						RGB=BLUE;
					}
					else{
						if((Cnt2<=LIMIT_HOT && temp_set==0xFF) || (Cnt2<=(LIMIT_HOT*9/5+32) && temp_set==0x00)){
							RGB=GREEN;
						}
						else{
							RGB=RED;
						}
					}
				}
			break;
		}
	}
}

//**
//  Main Program
//**
void main() {
	//**
	//  Oscillator Configuration:
	//  o 0b0
	//  o 4 MHz (110)
	//  o Primary system clock (1)
	//  o Stable frequency (1)
	//  o System clock : Internal RC (10)
	//  o Total : 0b01101110
	//**
	OSCCON=0x6E;
	//**
	//  Comparator Configuration:
	//  o Disable all
	//  o Total : 0b00000111;
	//**
	CMCON=0x07;
	//**
	//  A/D Configuration:
	//  o 0b00
	//  o Select An0 (0000)
	//  o Convertor idle (0)
	//  o A/D Convertor enabled (1)
	//  o Total : 0b00000010;
	//**
	ADCON0=0x02;
	//**
	//  A/D Configuration:
	//  o 0b00
	//  o VRef - = VSS (0)
	//  o VRef + = VDD (0)
	//  o AN0..AN3 Analogue (1011)
	//  o Total : 0b00001011;
	//**
	ADCON1=0x0B;
	//**
	//  Timer0 Configuration:
	//  o 0b
	//  o Enable Timer0 (1)
	//  o Timer0 is configured as an 8-bit timer/counter (1)
	//  o Internal instruction cycle clock (0)
	//  o Increment on low-to-high transition on T0CKI pin (0)
	//  o Timer0 prescaler is assigned. Timer0 clock input comes from prescaler output (0)
	//  o 1:32 Prescale value (100)
	//  o Total : 0b11000100;
	//**
	T0CON=0xC4;
	// Formula : Time = (4 x Clock Period) x Prescaler x (256 - TMR0L)
	TMR0L=100;
	//**
	//  Interrup Configuration:
	//  o 0b
	//  o Enables all unmasked interrupts (1)
	//  o Disable all interrupts (0)
	//  o Enable the TMR0 overflow interrupt (1)
	//  o Disables the INT0 external interrupt (0)
	//  o Disables the RB port change interrupt (0)
	//  o TMR0 register did not overflow (0)
	//  o The INT0 external interrupt did not occur (0)
	//  o None of the RB<7:4> pins have changed state (0)
	//  o Total : 0b10100000;
	//**
	INTCON=0xA0;
	//**
	//  PORT Directions
	//  o Inputs
	//    RA0..RA3 , RD2..RD7
	//  o Outputs
	//    RA4..RA7 , PORTB , PORTC, RD0..RD1 , PORTE
	//**
	TRISA=0x0F;
	TRISB=0x00;
	TRISC=0x00;
	TRISD=0xFC;
	TRISE=0x00;

	//**
	//  Initialization
	//**
	PORTA=PORTA & TRISA;
	PORTB=0x00;
	PORTC=0x00;
	PORTD=PORTD & TRISD;
	PORTE=0x00;

	Lcd_Init();
	Delay_ms(500);
	Lcd_Cmd(_LCD_CLEAR);
	Lcd_Cmd(_LCD_CURSOR_OFF);

	if(Load(SET_BACKLIGHT)==0xFF){
		BACKLIGHT=ON;
	}
        
	//**
	//  Welcome Message
	//**
	for(i=0; i<4; i++){
		Lcd_Out(1,3,"Please wait:");
		Lcd_Out(2,8,"..");
		Delay_ms(250);
		Lcd_Out(2,7,"....");
		Delay_ms(250);
		Lcd_Out(2,6,"......");
		Delay_ms(250);
		Lcd_Out(2,5,"........");
		Delay_ms(250);
		Lcd_Cmd(_LCD_CLEAR);
	}
        
	while(1){
		screen=nextscreen;
		switch(screen){
			default:
			case 0:
				Lcd_Out(1,3,"Thermometer");
				CC_Pointer(1,1);
				Lcd_Out(2,3,"Voltmeter");
				CC_Arrow_Down(2,16);
				
				while(screen==nextscreen){
					if(Input(DOWN)){
						nextscreen=1;
					}
					if(Input(OK)){
						nextscreen=20;
					}
				}
			break;

			case 1:
				Lcd_Out(1,3,"Thermometer");
				CC_Arrow_Up(1,16);
				Lcd_Out(2,3,"Voltmeter");
				CC_Pointer(2,1);
				CC_Arrow_Down(2,16);

				while(screen==nextscreen){
					if(Input(UP)){
						nextscreen=0;
					}
					if(Input(DOWN)){
						nextscreen=22;
					}
					if(Input(OK)){
						nextscreen=21;
					}
				}
			break;
			
			case 22 :
				Lcd_Out(1,3,"Voltmeter");
				CC_Arrow_Up(1,16);
				Lcd_Out(2,3,"Light Sensor");
				CC_Pointer(2,1);
				CC_Arrow_Down(2,16);

				while(screen==nextscreen){
					if(Input(UP)){
						nextscreen=1;
					}
					if(Input(DOWN)){
						nextscreen=2;
					}
					if(Input(OK)){
						nextscreen=23;
					}
				}
			break;

			case 2:
				Lcd_Out(1,3,"Light Sensor");
				CC_Arrow_Up(1,16);
				Lcd_Out(2,3,"Settings");
				CC_Pointer(2,1);
				CC_Arrow_Down(2,16);

				while(screen==nextscreen){
					if(Input(UP)){
						nextscreen=22;
					}
					if(Input(DOWN)){
						nextscreen=3;
					}
					if(Input(OK)){
						nextscreen=5;
					}
				}
			break;

			case 3:
				Lcd_Out(1,3,"Settings");
				CC_Arrow_Up(1,16);
				Lcd_Out(2,3,"Turn Off");
				CC_Pointer(2,1);

				while(screen==nextscreen){
					if(Input(UP)){
						nextscreen=2;
					}
					if(Input(OK)){
						nextscreen=4;
					}
				}
			break;

			case 4:
				BACKLIGHT=OFF;
				Lcd_Cmd(_LCD_TURN_OFF);
				asm{
					SLEEP;
				}
			break;

			case 5:
				Lcd_Out(1,3,"Temp Unit");
				CC_Pointer(1,1);
				Lcd_Out(2,3,"Voltage Unit");
				CC_Arrow_Down(2,16);

				while(screen==nextscreen){
					if(Input(DOWN)){
						nextscreen=6;
					}
					if(Input(BACK)){
						nextscreen=2;
					}
					if(Input(OK)){
						if(Load(SET_TEMPERATURE)==0xFF){
							nextscreen=10;
						}
						else{
							nextscreen=11;
						}
					}
				}
			break;

			case 6:
				Lcd_Out(1,3,"Temp Unit");
				CC_Arrow_Up(1,16);
				Lcd_Out(2,3,"Voltage Unit");
				CC_Pointer(2,1);
				CC_Arrow_Down(2,16);

				while(screen==nextscreen){
					if(Input(UP)){
						nextscreen=5;
					}
					if(Input(DOWN)){
						nextscreen=7;
					}
					if(Input(BACK)){
						nextscreen=2;
					}
					if(Input(OK)){
						if(Load(SET_VOLTAGE)==0){
							nextscreen=12;
						}
						else{
							nextscreen=13;
						}
					}
				}
			break;

			case 7:
				Lcd_Out(1,3,"Voltage Unit");
				CC_Arrow_Up(1,16);
				Lcd_Out(2,3,"Backlight");
				CC_Pointer(2,1);
				CC_Arrow_Down(2,16);

				while(screen==nextscreen){
					if(Input(UP)){
						nextscreen=6;
					}
					if(Input(DOWN)){
						nextscreen=8;
					}
					if(Input(BACK)){
						nextscreen=2;
					}
					if(Input(OK)){
						if(Load(SET_BACKLIGHT)==0){
							nextscreen=14;
						}
						else{
							nextscreen=15;
						}
					}
				}
			break;

			case 8:
				Lcd_Out(1,3,"Backlight");
				CC_Arrow_Up(1,16);
				Lcd_Out(2,3,"RGB1");
				CC_Pointer(2,1);
				CC_Arrow_Down(2,16);

				while(screen==nextscreen){
					if(Input(UP)){
						nextscreen=7;
					}
					if(Input(DOWN)){
						nextscreen=9;
					}
					if(Input(BACK)){
						nextscreen=2;
					}
					if(Input(OK)){
						if(Load(SET_RGB1)==0){
							nextscreen=16;
						}
						else{
							nextscreen=17;
						}
					}
				}
			break;

			case 9:
				Lcd_Out(1,3,"RGB1");
				CC_Arrow_Up(1,16);
				Lcd_Out(2,3,"RGB2");
				CC_Pointer(2,1);

				while(screen==nextscreen){
					if(Input(UP)){
						nextscreen=8;
					}
					if(Input(BACK)){
						nextscreen=2;
					}
					if(Input(OK)){
						if(Load(SET_RGB2)==0){
							nextscreen=18;
						}
						else{
							nextscreen=19;
						}
					}
				}
			break;

			case 10:
				Lcd_Out(1,3,"Temp Unit");
				CC_Degree(2,8);
				Lcd_Out(2,9,"C");
				CC_Arrow_Right(2,16);
				while(screen==nextscreen){
					if(Input(RIGHT)){
						nextscreen=11;
					}
					if(Input(OK)){
						Save(SET_TEMPERATURE,0xFF);
						nextscreen=5;
					}
					if(Input(BACK)){
						nextscreen=5;
					}
				}
			break;

			case 11:
				Lcd_Out(1,3,"Temp Unit");
				CC_Degree(2,8);
				Lcd_Out(2,9,"F");
				CC_Arrow_Left(2,1);
				while(screen==nextscreen){
					if(Input(LEFT)){
						nextscreen=10;
					}
					if(Input(OK)){
						Save(SET_TEMPERATURE,0x00);
						nextscreen=5;
					}
					if(Input(BACK)){
						nextscreen=5;
					}
				}
			break;
			
			case 12:
				Lcd_Out(1,3,"Volt Unit");
				Lcd_Out(2,9,"V");
				CC_Arrow_Right(2,16);
				while(screen==nextscreen){
					if(Input(RIGHT)){
						nextscreen=13;
					}
					if(Input(OK)){
						Save(SET_VOLTAGE,0);
						nextscreen=6;
					}
					if(Input(BACK)){
						nextscreen=6;
					}
				}
			break;

			case 13:
				Lcd_Out(1,3,"Volt Unit");
				Lcd_Out(2,9,"mV");
				CC_Arrow_Left(2,1);
				while(screen==nextscreen){
					if(Input(LEFT)){
						nextscreen=12;
					}
					if(Input(OK)){
						Save(SET_VOLTAGE,0xFF);
						nextscreen=6;
					}
					if(Input(BACK)){
						nextscreen=6;
					}
				}
			break;
			
			case 14:
				Lcd_Out(1,3,"Backlight");
				Lcd_Out(2,8,"Off");
				CC_Arrow_Right(2,16);
				while(screen==nextscreen){
					if(Input(RIGHT)){
						nextscreen=15;
					}
					if(Input(OK)){
						BACKLIGHT=OFF;
						Save(SET_BACKLIGHT,0);
						nextscreen=7;
					}
					if(Input(BACK)){
						nextscreen=7;
					}
				}
			break;

			case 15:
				Lcd_Out(1,3,"Backlight");
				Lcd_Out(2,8,"On");
				CC_Arrow_Left(2,1);
				while(screen==nextscreen){
					if(Input(LEFT)){
						nextscreen=14;
					}
					if(Input(OK)){
						BACKLIGHT=ON;
						Save(SET_BACKLIGHT,0xFF);
						nextscreen=7;
					}
					if(Input(BACK)){
						nextscreen=7;
					}
				}
			break;
			
			case 16:
				Lcd_Out(1,3,"RGB1");
				Lcd_Out(2,8,"Off");
				CC_Arrow_Right(2,16);
				while(screen==nextscreen){
					if(Input(RIGHT)){
						nextscreen=17;
					}
					if(Input(OK)){
						Save(SET_RGB1,0);
						nextscreen=8;
					}
					if(Input(BACK)){
						nextscreen=8;
					}
				}
			break;

			case 17:
				Lcd_Out(1,3,"RGB1");
				Lcd_Out(2,8,"On");
				CC_Arrow_Left(2,1);
				while(screen==nextscreen){
					if(Input(LEFT)){
						nextscreen=16;
					}
					if(Input(OK)){
						Save(SET_RGB1,0xFF);
						nextscreen=8;
					}
					if(Input(BACK)){
						nextscreen=8;
					}
				}
			break;
			
			case 18:
				Lcd_Out(1,3,"RGB2");
				Lcd_Out(2,8,"Off");
				CC_Arrow_Right(2,16);
				while(screen==nextscreen){
					if(Input(RIGHT)){
						nextscreen=19;
					}
					if(Input(OK)){
						Save(SET_RGB2,0);
						nextscreen=9;
					}
					if(Input(BACK)){
						nextscreen=9;
					}
				}
			break;

			case 19:
				Lcd_Out(1,3,"RGB2");
				Lcd_Out(2,8,"On");
				CC_Arrow_Left(2,1);
				while(screen==nextscreen){
					if(Input(LEFT)){
						nextscreen=18;
					}
					if(Input(OK)){
						Save(SET_RGB2,0xFF);
						nextscreen=9;
					}
					if(Input(BACK)){
						nextscreen=9;
					}
				}
			break;
			
			case 20:
				T1=ON;
				T2=ON;
				rgb1_set=0x00;
				rgb2_set=0x00;
				temp_set=0x00;
				//**
				//  Sensors Load
				//**
				for(i=0; i<4; i++){
					Lcd_Out(1,1,"Starting sensors");
					Lcd_Out(2,8,"..");
					Delay_ms(250);
					Lcd_Out(2,7,"....");
					Delay_ms(250);
					Lcd_Out(2,6,"......");
					Delay_ms(250);
					Lcd_Out(2,5,"........");
					Delay_ms(250);
					Lcd_Cmd(_LCD_CLEAR);
				}
				
				CC_Arrow_Left(2,1);
				rgb1_set=Load(SET_RGB1);
				rgb2_set=Load(SET_RGB2);
				temp_set=Load(SET_TEMPERATURE);
				while(screen==nextscreen){
					if(cntr1 >= 728){
						// Channel 1
						adc_rd1  = ADC_read(THERMO1);
						tlong1 = (long)adc_rd1 * VOLTAGE;
						tlong1 = tlong1 / 1023;
						tlong1 = tlong1 - 500;
						tlong1 = tlong1 / 10;
						Cnt1   = (int)tlong1;
						if(Load(SET_TEMPERATURE)==0){
							Cnt1 = Cnt1 * 9/5 +32;
						}
						IntToStr(Cnt1,Str1);
						Lcd_Out(1,3,Str1);
						Lcd_Out(1,2,"T1:");
						CC_Degree(1,9);
						if(Load(SET_TEMPERATURE)!=0){
							Lcd_Out(1,10,"C");
						}
						else{
							Lcd_Out(1,10,"F");
						}
						// Channel 2
						adc_rd2  = ADC_read(THERMO2);
						tlong2 = (long)adc_rd2 * VOLTAGE;
						tlong2 = tlong2 / 1023;
						tlong2 = tlong2 - 500;
						tlong2 = tlong2 / 10;
						Cnt2   = (int)tlong2;
						if(Load(SET_TEMPERATURE)==0){
							Cnt2 = Cnt2 * 9/5 +32;
						}
						IntToStr(Cnt2,Str2);
						Lcd_Out(2,3,Str2);
						Lcd_Out(2,2,"T2:");
						// Cnt3
						Cnt3 = Cnt1 - Cnt2;
						CC_Delta(1,15);
						Lcd_Out(1,16,"T");
						IntToStr(Cnt3,Str3);
						Lcd_Out(2,9,Str3);
						CC_Degree(2,9);
						CC_Degree(2,15);
						if(Load(SET_TEMPERATURE)!=0){
							Lcd_Out(2,10,"C");
							Lcd_Out(2,16,"C");
						}
						else{
							Lcd_Out(2,10,"F");
							Lcd_Out(2,16,"F");
						}
						cntr1=0;
						Delay_ms(500);
					}
					if(Input(BACK)){
						T1=OFF;
						T2=OFF;
						nextscreen=0;
					}
				}
			break;
			
			case 21:				
				CC_Arrow_Left(2,1);
				while(screen==nextscreen){
					// Channel 1
					adc_rd1  = ADC_read(VOLT1);
					tlong1 = (long)adc_rd1 * VOLTAGE;
					tlong1 = tlong1 / 1023;
					Cnt1   = (int)tlong1;
					if(Load(SET_VOLTAGE)!=0){
						Cnt1 = Cnt1 * 1000;
					}
					IntToStr(Cnt1,Str1);
					Lcd_Out(1,5,Str1);
					Lcd_Out(1,2,"V1:");
					if(Load(SET_VOLTAGE)==0){
						Lcd_Out(1,10,"V");
					}
					else{
						Lcd_Out(1,9,"mV");
					}
					// Channel 2
					adc_rd2  = ADC_read(VOLT2);
					tlong2 = (long)adc_rd2 * VOLTAGE;
					tlong2 = tlong2 / 1023;
					Cnt2   = (int)tlong2;
					if(Load(SET_VOLTAGE)!=0){
						Cnt2 = Cnt2 * 1000;
					}
					IntToStr(Cnt2,Str2);
					Lcd_Out(2,5,Str2);
					Lcd_Out(2,2,"V2:");
					// Cnt3
					Cnt3 = Cnt1 - Cnt2;
					CC_Delta(1,15);
					Lcd_Out(1,16,"V");
					IntToStr(Cnt3,Str3);
					Lcd_Out(2,9,Str3);
					if(Load(SET_VOLTAGE)==0){
						Lcd_Out(2,10,"V");
						Lcd_Out(2,16,"V");
					}
					else{
						Lcd_Out(2,9,"mV");
						Lcd_Out(2,15,"mV");
					}
					if(Input(BACK)){
						nextscreen=1;
					}
					Delay_ms(500);
				}
			break;
			
			case 23:
				CC_Arrow_Left(2,1);
				while(screen==nextscreen){
					if(cntr2 >= 7812){	// Wait enough ~ 1s
						//ADCON1 = 0x00 ;         // set PORTA as analog input
						adc_rd3 = Adc_Read(LIGHT) ;      // read AN4 ADC (RA5)
						//ADCON1 = 0x07 ;         // set PORTA as digital IO
						if(adc_rd3 < 8){		// Light
							Lcd_Out(1,9," Day ");
						}
						else if(adc_rd3 > 16){	// Dark
							Lcd_Out(1,9,"Night");
						}
						else{
							Lcd_Out(1,8,"Unknown");
						}
						cntr2=0;
					}
					if(Input(BACK)){
						nextscreen=22;
					}
				}
			break;
		}
		Lcd_Cmd(_LCD_CLEAR);
	}
}