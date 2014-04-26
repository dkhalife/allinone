#line 1 "I:/Documents/Projects/Pic Tools/Active Projects/All-In-One/all.c"
#line 57 "I:/Documents/Projects/Pic Tools/Active Projects/All-In-One/all.c"
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





signed int Cnt1 = 0;
signed int Cnt2 = 0;
signed int Cnt3 = 0;
unsigned int i = 0;
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




int Input(int pin){
 if (Button(&PORTD, pin, 1, 1)) oldstate[pin] = 1;
 if (oldstate[pin] && Button(&PORTD, pin, 1, 0)){
 oldstate[pin] = 0;
 return  1 ;
 }
 return  0 ;
}





void interrupt(){
 TMR0L=100;
#line 197 "I:/Documents/Projects/Pic Tools/Active Projects/All-In-One/all.c"
 INTCON=0x20;




 cntr1++;
 cntr2++;
 if(screen==20){
 Flag++;
 if(Flag>2){
 Flag=1;
 }
  PORTE =0x00;
  RA4_bit = 0 ;
  RA5_bit = 0 ;
 switch(Flag){
 case 1:
 if(rgb1_set==0xFF){
  RA5_bit = 0 ;
  RA4_bit = 1 ;
 if((Cnt1<= 0  && temp_set==0xFF) || (Cnt1<=( 0 *9/5+32) && temp_set==0x00)){
  PORTE = 0x04; ;
 }
 else{
 if((Cnt1<= 25  && temp_set==0xFF) || (Cnt1<=( 25 *9/5+32) && temp_set==0x00)){
  PORTE = 0x02; ;
 }
 else{
  PORTE = 0x01; ;
 }
 }
 }
 break;

 case 2:
 if(rgb2_set==0xFF){
  RA4_bit = 0 ;
  RA5_bit = 1 ;
 if((Cnt2<= 0  && temp_set==0xFF) || (Cnt2<=( 0 *9/5+32) && temp_set==0x00)){
  PORTE = 0x04; ;
 }
 else{
 if((Cnt2<= 25  && temp_set==0xFF) || (Cnt2<=( 25 *9/5+32) && temp_set==0x00)){
  PORTE = 0x02; ;
 }
 else{
  PORTE = 0x01; ;
 }
 }
 }
 break;
 }
 }
}




void main() {









 OSCCON=0x6E;





 CMCON=0x07;








 ADCON0=0x02;








 ADCON1=0x0B;
#line 301 "I:/Documents/Projects/Pic Tools/Active Projects/All-In-One/all.c"
 T0CON=0xC4;

 TMR0L=100;
#line 317 "I:/Documents/Projects/Pic Tools/Active Projects/All-In-One/all.c"
 INTCON=0xA0;







 TRISA=0x0F;
 TRISB=0x00;
 TRISC=0x00;
 TRISD=0xFC;
 TRISE=0x00;




 PORTA=PORTA & TRISA;
 PORTB=0x00;
 PORTC=0x00;
 PORTD=PORTD & TRISD;
 PORTE=0x00;

 Lcd_Init();
 Delay_ms(500);
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Cmd(_LCD_CURSOR_OFF);

 if( EEPROM_Read ( 0x80 )==0xFF){
  RC7_bit = 1 ;
 }




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
 if(Input( 6 )){
 nextscreen=1;
 }
 if(Input( 3 )){
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
 if(Input( 7 )){
 nextscreen=0;
 }
 if(Input( 6 )){
 nextscreen=22;
 }
 if(Input( 3 )){
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
 if(Input( 7 )){
 nextscreen=1;
 }
 if(Input( 6 )){
 nextscreen=2;
 }
 if(Input( 3 )){
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
 if(Input( 7 )){
 nextscreen=22;
 }
 if(Input( 6 )){
 nextscreen=3;
 }
 if(Input( 3 )){
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
 if(Input( 7 )){
 nextscreen=2;
 }
 if(Input( 3 )){
 nextscreen=4;
 }
 }
 break;

 case 4:
  RC7_bit = 0 ;
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
 if(Input( 6 )){
 nextscreen=6;
 }
 if(Input( 2 )){
 nextscreen=2;
 }
 if(Input( 3 )){
 if( EEPROM_Read ( 0x81 )==0xFF){
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
 if(Input( 7 )){
 nextscreen=5;
 }
 if(Input( 6 )){
 nextscreen=7;
 }
 if(Input( 2 )){
 nextscreen=2;
 }
 if(Input( 3 )){
 if( EEPROM_Read ( 0x82 )==0){
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
 if(Input( 7 )){
 nextscreen=6;
 }
 if(Input( 6 )){
 nextscreen=8;
 }
 if(Input( 2 )){
 nextscreen=2;
 }
 if(Input( 3 )){
 if( EEPROM_Read ( 0x80 )==0){
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
 if(Input( 7 )){
 nextscreen=7;
 }
 if(Input( 6 )){
 nextscreen=9;
 }
 if(Input( 2 )){
 nextscreen=2;
 }
 if(Input( 3 )){
 if( EEPROM_Read ( 0x83 )==0){
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
 if(Input( 7 )){
 nextscreen=8;
 }
 if(Input( 2 )){
 nextscreen=2;
 }
 if(Input( 3 )){
 if( EEPROM_Read ( 0x84 )==0){
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
 if(Input( 4 )){
 nextscreen=11;
 }
 if(Input( 3 )){
  EEPROM_Write ( 0x81 ,0xFF);
 nextscreen=5;
 }
 if(Input( 2 )){
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
 if(Input( 5 )){
 nextscreen=10;
 }
 if(Input( 3 )){
  EEPROM_Write ( 0x81 ,0x00);
 nextscreen=5;
 }
 if(Input( 2 )){
 nextscreen=5;
 }
 }
 break;

 case 12:
 Lcd_Out(1,3,"Volt Unit");
 Lcd_Out(2,9,"V");
 CC_Arrow_Right(2,16);
 while(screen==nextscreen){
 if(Input( 4 )){
 nextscreen=13;
 }
 if(Input( 3 )){
  EEPROM_Write ( 0x82 ,0);
 nextscreen=6;
 }
 if(Input( 2 )){
 nextscreen=6;
 }
 }
 break;

 case 13:
 Lcd_Out(1,3,"Volt Unit");
 Lcd_Out(2,9,"mV");
 CC_Arrow_Left(2,1);
 while(screen==nextscreen){
 if(Input( 5 )){
 nextscreen=12;
 }
 if(Input( 3 )){
  EEPROM_Write ( 0x82 ,0xFF);
 nextscreen=6;
 }
 if(Input( 2 )){
 nextscreen=6;
 }
 }
 break;

 case 14:
 Lcd_Out(1,3,"Backlight");
 Lcd_Out(2,8,"Off");
 CC_Arrow_Right(2,16);
 while(screen==nextscreen){
 if(Input( 4 )){
 nextscreen=15;
 }
 if(Input( 3 )){
  RC7_bit = 0 ;
  EEPROM_Write ( 0x80 ,0);
 nextscreen=7;
 }
 if(Input( 2 )){
 nextscreen=7;
 }
 }
 break;

 case 15:
 Lcd_Out(1,3,"Backlight");
 Lcd_Out(2,8,"On");
 CC_Arrow_Left(2,1);
 while(screen==nextscreen){
 if(Input( 5 )){
 nextscreen=14;
 }
 if(Input( 3 )){
  RC7_bit = 1 ;
  EEPROM_Write ( 0x80 ,0xFF);
 nextscreen=7;
 }
 if(Input( 2 )){
 nextscreen=7;
 }
 }
 break;

 case 16:
 Lcd_Out(1,3,"RGB1");
 Lcd_Out(2,8,"Off");
 CC_Arrow_Right(2,16);
 while(screen==nextscreen){
 if(Input( 4 )){
 nextscreen=17;
 }
 if(Input( 3 )){
  EEPROM_Write ( 0x83 ,0);
 nextscreen=8;
 }
 if(Input( 2 )){
 nextscreen=8;
 }
 }
 break;

 case 17:
 Lcd_Out(1,3,"RGB1");
 Lcd_Out(2,8,"On");
 CC_Arrow_Left(2,1);
 while(screen==nextscreen){
 if(Input( 5 )){
 nextscreen=16;
 }
 if(Input( 3 )){
  EEPROM_Write ( 0x83 ,0xFF);
 nextscreen=8;
 }
 if(Input( 2 )){
 nextscreen=8;
 }
 }
 break;

 case 18:
 Lcd_Out(1,3,"RGB2");
 Lcd_Out(2,8,"Off");
 CC_Arrow_Right(2,16);
 while(screen==nextscreen){
 if(Input( 4 )){
 nextscreen=19;
 }
 if(Input( 3 )){
  EEPROM_Write ( 0x84 ,0);
 nextscreen=9;
 }
 if(Input( 2 )){
 nextscreen=9;
 }
 }
 break;

 case 19:
 Lcd_Out(1,3,"RGB2");
 Lcd_Out(2,8,"On");
 CC_Arrow_Left(2,1);
 while(screen==nextscreen){
 if(Input( 5 )){
 nextscreen=18;
 }
 if(Input( 3 )){
  EEPROM_Write ( 0x84 ,0xFF);
 nextscreen=9;
 }
 if(Input( 2 )){
 nextscreen=9;
 }
 }
 break;

 case 20:
  RC0_bit = 1 ;
  RC1_bit = 1 ;
 rgb1_set=0x00;
 rgb2_set=0x00;
 temp_set=0x00;



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
 rgb1_set= EEPROM_Read ( 0x83 );
 rgb2_set= EEPROM_Read ( 0x84 );
 temp_set= EEPROM_Read ( 0x81 );
 while(screen==nextscreen){
 if(cntr1 >= 728){

 adc_rd1 = ADC_read( 0 );
 tlong1 = (long)adc_rd1 *  4500 ;
 tlong1 = tlong1 / 1023;
 tlong1 = tlong1 - 500;
 tlong1 = tlong1 / 10;
 Cnt1 = (int)tlong1;
 if( EEPROM_Read ( 0x81 )==0){
 Cnt1 = Cnt1 * 9/5 +32;
 }
 IntToStr(Cnt1,Str1);
 Lcd_Out(1,3,Str1);
 Lcd_Out(1,2,"T1:");
 CC_Degree(1,9);
 if( EEPROM_Read ( 0x81 )!=0){
 Lcd_Out(1,10,"C");
 }
 else{
 Lcd_Out(1,10,"F");
 }

 adc_rd2 = ADC_read( 1 );
 tlong2 = (long)adc_rd2 *  4500 ;
 tlong2 = tlong2 / 1023;
 tlong2 = tlong2 - 500;
 tlong2 = tlong2 / 10;
 Cnt2 = (int)tlong2;
 if( EEPROM_Read ( 0x81 )==0){
 Cnt2 = Cnt2 * 9/5 +32;
 }
 IntToStr(Cnt2,Str2);
 Lcd_Out(2,3,Str2);
 Lcd_Out(2,2,"T2:");

 Cnt3 = Cnt1 - Cnt2;
 CC_Delta(1,15);
 Lcd_Out(1,16,"T");
 IntToStr(Cnt3,Str3);
 Lcd_Out(2,9,Str3);
 CC_Degree(2,9);
 CC_Degree(2,15);
 if( EEPROM_Read ( 0x81 )!=0){
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
 if(Input( 2 )){
  RC0_bit = 0 ;
  RC1_bit = 0 ;
 nextscreen=0;
 }
 }
 break;

 case 21:
 CC_Arrow_Left(2,1);
 while(screen==nextscreen){

 adc_rd1 = ADC_read( 2 );
 tlong1 = (long)adc_rd1 *  4500 ;
 tlong1 = tlong1 / 1023;
 Cnt1 = (int)tlong1;
 if( EEPROM_Read ( 0x82 )!=0){
 Cnt1 = Cnt1 * 1000;
 }
 IntToStr(Cnt1,Str1);
 Lcd_Out(1,5,Str1);
 Lcd_Out(1,2,"V1:");
 if( EEPROM_Read ( 0x82 )==0){
 Lcd_Out(1,10,"V");
 }
 else{
 Lcd_Out(1,9,"mV");
 }

 adc_rd2 = ADC_read( 3 );
 tlong2 = (long)adc_rd2 *  4500 ;
 tlong2 = tlong2 / 1023;
 Cnt2 = (int)tlong2;
 if( EEPROM_Read ( 0x82 )!=0){
 Cnt2 = Cnt2 * 1000;
 }
 IntToStr(Cnt2,Str2);
 Lcd_Out(2,5,Str2);
 Lcd_Out(2,2,"V2:");

 Cnt3 = Cnt1 - Cnt2;
 CC_Delta(1,15);
 Lcd_Out(1,16,"V");
 IntToStr(Cnt3,Str3);
 Lcd_Out(2,9,Str3);
 if( EEPROM_Read ( 0x82 )==0){
 Lcd_Out(2,10,"V");
 Lcd_Out(2,16,"V");
 }
 else{
 Lcd_Out(2,9,"mV");
 Lcd_Out(2,15,"mV");
 }
 if(Input( 2 )){
 nextscreen=1;
 }
 Delay_ms(500);
 }
 break;

 case 23:
 CC_Arrow_Left(2,1);
 while(screen==nextscreen){
 if(cntr2 >= 7812){

 adc_rd3 = Adc_Read( 4 ) ;

 if(adc_rd3 < 8){
 Lcd_Out(1,9," Day ");
 }
 else if(adc_rd3 > 16){
 Lcd_Out(1,9,"Night");
 }
 else{
 Lcd_Out(1,8,"Unknown");
 }
 cntr2=0;
 }
 if(Input( 2 )){
 nextscreen=22;
 }
 }
 break;
 }
 Lcd_Cmd(_LCD_CLEAR);
 }
}
