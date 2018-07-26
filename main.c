// ---------------------------------------------------------------------------
// GLOBAL DEFINES
#define F_CPU 16000000L // run CPU at 16 MHz
#define LED 5 // Boarduino LED on PB5
#define ClearBit(x,y) x &= ~_BV(y) // equivalent to cbi(x,y)
#define SetBit(x,y) x |= _BV(y) // equivalent to sbi(x,y)
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7
#define inf 999
// ---------------------------------------------------------------------------
// INCLUDES
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "lcd.c"
#include "rtc.c"
// ---------------------------------------------------------------------------
// TYPEDEFS
typedef uint8_t byte;
typedef int8_t sbyte;
// ---------------------------------------------------------------------------
// MISC ROUTINES
void MainLoop();
void GetKeyPressed();
void setalarmstart();
void getkeypressed2();
//FILE *fp;
struct Time{
int h1;
int h2;
int min1;
int min2;

};

struct Time time1={inf,inf,inf,inf};
struct Time currentAlarm={inf,inf,inf,inf};
struct Time arraytime[4]={inf};
int cursorpoint;
int noofalarm=0;
int whichis;
int flagOfSettingAlarm=0;
int modechange=0;//0 for time show ,1 for setalarm

void InitAVR()
{
DDRB = 0x3F;
DDRC = 0x00;
DDRD=0xFF;
}
 void setAlarm2()
 {
                              int minH=arraytime[0].h2+(arraytime[0].h1*10);
                              whichis=0;
				                      int i;
                              for(i=0;i<noofalarm;i++)
                              {
                                int hour=arraytime[i].h2+(arraytime[i].h1*10);
                                if(hour<minH)
                                {
                                  minH=hour;
                                  whichis=i;
                                }
                              }
                              int minM=arraytime[whichis].min2+(arraytime[whichis].min1*10);

                              for( i=0;i<noofalarm;i++)
                              {
                                  int min=arraytime[i].min2+(arraytime[i].min1*10);
                                  int hour=arraytime[i].h2+(arraytime[i].h1*10);
                                  if(i!=whichis && hour==minH && min<minM)
                                  {
                                    minM=min;
                                    whichis=i;

                                  }
                              }
                              currentAlarm.h1=arraytime[whichis].h1;
                              currentAlarm.h2=arraytime[whichis].h2;
                              currentAlarm.min1=arraytime[whichis].min1;
                              currentAlarm.min2=arraytime[whichis].min2;
}
void settingCurrentAlarm(){

	                 byte hours, minutes, seconds;
                            DS1307_GetTime(&hours,&minutes,&seconds);
                            byte tempHour,tempMin,tempSec;
                            tempHour=hours;
                            tempMin=minutes;
                            tempSec=seconds;
                            byte h,m;
                            byte temp;
                             temp= tempHour>>4;

                            h=temp*10;
                            tempHour &= 0x0F;

                            h+=tempHour;

                            //byte h=(byte)c-60-3*9;

                            temp= tempMin>>4;

                            m=temp*10;
                            tempMin &= 0x0F;

                            m+=tempMin;

                            //int h=12;
                            //int m=20;
                            int index=0;
                            int mainflag=1;
                            //int MinH;
                            int arrayofuselesstime[noofalarm];int i;
                            //discarding loop
                            for( i=0;i<noofalarm;i++)
                            {
                              byte hour=arraytime[i].h2+(arraytime[i].h1*10);
                              byte min=arraytime[i].min2+(arraytime[i].min1*10);
                              if(hour<h || (hour==h && min<m))
                              {
                                arrayofuselesstime[index]=i;
                                index++;
                              }
                            }
                            // for(int i=0;i<index;i++)
                            // {
                            //   printf("i is %d\n",arrayofuselesstime[i]);
                            // }
                            byte minH;
                            //setting 1st minH-------------------
                           for(i=0;i<noofalarm;i++)
                           {
                             int flag=0;int j;
                             for( j=0;j<index;j++)
                             {
                               //printf("in setting i is %d and array is %d\n",i, arrayofuselesstime[j]);
                               if(i==arrayofuselesstime[j])
                               {
                                 flag=1;
                               //  mainflag=1;
                                 break;
                               }
                             }
                           //  printf("flag is %d\n",flag );
                             if(flag==0)
                             {
                               minH=arraytime[i].h2+(arraytime[i].h1*10);
                               whichis=i;
                             //  printf("minH is %d and which is %d\n",minH,i);
                               //whichis=i;
                               mainflag=0;
                               break;
                             }
                           }
                          if(mainflag==1)
                          {
				setAlarm2();
                          // currentAlarm.h1=currentAlarm.h2=currentAlarm.min1=currentAlarm.min2=inf;

                          }
                          //finding real min H--------------------------------------
                          else{
                            //mainflag=1;
                            //printf("after-----\n");
				int i;
                             for( i=0;i<noofalarm;i++)
                             {
                               byte hour=arraytime[i].h2+(arraytime[i].h1*10);
                               int flag=0;int j;
                               for( j=0;j<index;j++)
                               {
                                 //printf("i is %d and array is %d\n",i, arrayofuselesstime[j]);
                                 if(i==arrayofuselesstime[j])
                                 {
                                   flag=1;
                                 //  printf("in flag change\n");
                                   break;
                                 }
                               }
                               if(flag==0 && hour<minH)
                               {
                                 minH=hour;
                                 whichis=i;
                                 //mainflag=0;
                                 //printf("after minH is %d and which is %d\n",minH,i);
                                 //whichis=i;


                               }

                             }


                             byte minM;

                             minM=arraytime[whichis].min2+(arraytime[whichis].min1*10);



                            for( i=0;i<noofalarm;i++)
                            {
                              byte min=arraytime[i].min2+(arraytime[i].min1*10);
                              byte hour=arraytime[i].h2+(arraytime[i].h1*10);
                              int flag=0; int j;
                              for( j=0;j<index;j++)
                              {
                                if(i==arrayofuselesstime[j])
                                {
                                  flag=1;
                                  break;
                                }
                              }
                              if(flag==0 && i!=whichis && hour==minH && min<minM)
                              {
                                minM=min;
                                whichis=i;
                              }

                            }

                           //  printf("whichis is %d\n",whichis );
                             currentAlarm.h1=arraytime[whichis].h1;
                             currentAlarm.h2=arraytime[whichis].h2;
                             currentAlarm.min1=arraytime[whichis].min1;
                             currentAlarm.min2=arraytime[whichis].min2;



                          }
}




void GetKeyPressedToDelete()
{
	LCD_Clear();
	while(1){

			LCD_Clear();
			if(noofalarm==0) {LCD_String("No upcoming");LCD_Line(1); LCD_String("alarm!");}
			else{
			//LCD_String("Alarms:");
			//if(arraytime[0].h1!=inf&&arraytime[0].h2!=inf&&arraytime[0].min1!=inf&&arraytime[0].min2!=inf)
			//LCD_Line(1);
                        LCD_Integer(arraytime[0].h1);
                        LCD_Integer(arraytime[0].h2);
                         LCD_String(":");
                        LCD_Integer(arraytime[0].min1);
                        LCD_Integer(arraytime[0].min2);
			LCD_Line(1);int i;
			for(i=1;i<noofalarm;i++)
                         {
                        LCD_Integer(arraytime[i].h1);
                        LCD_Integer(arraytime[i].h2);
                         LCD_String(":");
                        LCD_Integer(arraytime[i].min1);
                        LCD_Integer(arraytime[i].min2);
			LCD_String(" ");}
			}

	_delay_ms(10);
     int r,c;

      PORTA|= 0X0F;
      //LCD_Line(1);
      for(c=0;c<4;c++)
      {
         DDRA&=~(0XFF);

         DDRA|=(0X80>>c);
        // LCD_Integer (c);
         for(r=0;r<4;r++)
         {
            if(!(PINA & (0X08>>r)))
            {
		if(c==0)
                  {
                    if(r==0 && cursorpoint==8)
                      {

                      }
                    else if(r==1) {} //LCD_String("B");
                    else if(r==2) //"C"
                      {
                       //if(modechange==0)
                         //{
                           // modechange=1;
				//_delay_ms(50);
                           setalarmstart();

                          //}

                      // LCD_String("C");
                     }
                    else if(r==3)  {}//LCD_String("D");
                  }

                else if(r==0 && c==1)
                  {//_delay_ms(50);

			//LCD_Clear();
			int i;
		for(i=0;i<noofalarm;i++)
			{
   			arraytime[i].h1=arraytime[i+1].h1;
  			arraytime[i].h2=arraytime[i+1].h2;

  			arraytime[i].min1=arraytime[i+1].min1;

  			arraytime[i].min2=arraytime[i+1].min2;


			}noofalarm--;//_delay_ms(50);
			LCD_Clear();settingCurrentAlarm();

                  }
                else if( r==0 && c==2)
                 {
			//_delay_ms(50);
			int i;
		for(i=1;i<noofalarm;i++)
			{
   			arraytime[i].h1=arraytime[i+1].h1;
  			arraytime[i].h2=arraytime[i+1].h2;

  			arraytime[i].min1=arraytime[i+1].min1;

  			arraytime[i].min2=arraytime[i+1].min2;


			}noofalarm--;//_delay_ms(50);
			LCD_Clear();settingCurrentAlarm();
                  }
		else if( r==0 && c==3)
		{
			//_delay_ms(50);
			int i;
		for(i=2;i<noofalarm;i++)
			{
   			arraytime[i].h1=arraytime[i+1].h1;
  			arraytime[i].h2=arraytime[i+1].h2;

  			arraytime[i].min1=arraytime[i+1].min1;

  			arraytime[i].min2=arraytime[i+1].min2;


			}noofalarm--;//_delay_ms(50);
			LCD_Clear();settingCurrentAlarm();
		}
		else
		{}
		_delay_ms(20);
            }
         }
      }
}
}




void GetKeyPressed()
   {
      int r,c;

      PORTA|= 0X0F;
      //LCD_Line(1);
      for(c=0;c<4;c++)
      {
         DDRA&=~(0XFF);

         DDRA|=(0X80>>c);
        // LCD_Integer (c);
         for(r=0;r<4;r++)
         {
            if(!(PINA & (0X08>>r)))
            {
		      if(c==0)
                  {
                    if(r==0 && cursorpoint==8) //"A"
                      {
                        LCD_String("DONE"); noofalarm++;
                        cursorpoint=0;
                        arraytime[noofalarm-1]=time1;
		                    settingCurrentAlarm();           _delay_ms(50);
                        LCD_Clear() ;
                        LCD_Integer (noofalarm+1);
                        LCD_Goto(2,0);

                        LCD_String("HH:MM");
                        LCD_Goto(8,0);//2st arg detemines which line,1st arg determines position in that line

                        LCD_String("S");
                        LCD_Goto(cursorpoint,0);
                      }
                    else if(r==1) //"B"
                      {
                          LCD_Clear();
			if(noofalarm==0||arraytime[0].h1==inf) {LCD_String("No upcoming");LCD_Line(1); LCD_String("alarm!");}
			else {
			LCD_String("Alarms:");
			//if(arraytime[0].h1!=inf&&arraytime[0].h2!=inf&&arraytime[0].min1!=inf&&arraytime[0].min2!=inf)
			//LCD_Line(1);
                        LCD_Integer(arraytime[0].h1);
                        LCD_Integer(arraytime[0].h2);
                         LCD_String(":");
                        LCD_Integer(arraytime[0].min1);
                        LCD_Integer(arraytime[0].min2);
			LCD_Line(1);int i;
			for(i=1;i<noofalarm;i++)
                         {
                        LCD_Integer(arraytime[i].h1);
                        LCD_Integer(arraytime[i].h2);
                         LCD_String(":");
                        LCD_Integer(arraytime[i].min1);
                        LCD_Integer(arraytime[i].min2);
			LCD_String(" ");}
			}




                      }
                    else if(r==2) //"C"
                      {
                       //if(modechange==1)
                         //{
                           modechange=0;

			_delay_ms(50);LCD_Clear();
  			//settingCurrentAlarm();
                           MainLoop();
                         // }

                      // LCD_String("C");
                     }
                    else if(r==3) //"D"
                        {

                           LCD_Clear();
                        LCD_String("Press the number of alarm to delete ");
			_delay_ms(50);
			GetKeyPressedToDelete();

                        }
                  }
                else if(r==3 && c==3)  //"#"
                  {
                    if(cursorpoint==0 || cursorpoint==3 || cursorpoint==6) cursorpoint=cursorpoint+2;
                     else cursorpoint++;//going forward;
                    if(cursorpoint==9) cursorpoint=0;

                    LCD_Goto(cursorpoint,0);

                  }
                  else if(r==3 && c==1) //"*"
                  {
                   if(cursorpoint==2 || cursorpoint==5 || cursorpoint==8) cursorpoint=cursorpoint-2;
                   else  cursorpoint--;//going backward;
                    if(cursorpoint==-1) cursorpoint=8;
                    LCD_Goto(cursorpoint,0);

                  }
                else if(r*3+c==0x0b)
                  {
                    if(cursorpoint==8 || cursorpoint==0){//nothing
                     }
                    else
                      {
                        if(cursorpoint==2) time1.h1=0;//saving data to time array
                        else if(cursorpoint==3) time1.h2=0;
                        else if(cursorpoint==5) time1.min1=0;
                        else if(cursorpoint==6) time1.min2=0;

                         LCD_Integer (0);
                         if(cursorpoint==0 || cursorpoint==3 || cursorpoint==6) cursorpoint=cursorpoint+2;
                         else cursorpoint++;//going forward;
                         LCD_Goto(cursorpoint,0);
                       }


                  }
                else
                 {
                    if(cursorpoint==8){//nothing
                    }
                    else if(cursorpoint==0 && r==0 && c==1) {
			//noofalarm++;
			if(noofalarm==3)
				{LCD_Clear();
				LCD_String("Already 3 alarms are set!");
				//noofalarm--;
				}
			else{
			LCD_Integer (noofalarm+1);cursorpoint=cursorpoint+2;LCD_Goto(cursorpoint,0);}}
                    else if(cursorpoint==0 && (r!=0 || c!=1)) {} //ekhane ki holo?
                    else
                    {
                        if(cursorpoint==2) time1.h1=r*3+c;//saving data to time array
                        else if(cursorpoint==3) time1.h2=r*3+c;
                        else if(cursorpoint==5) time1.min1=r*3+c;
                        else if(cursorpoint==6) time1.min2=r*3+c;

                         LCD_Integer (r*3+c);
                         if(cursorpoint==0 || cursorpoint==3 || cursorpoint==6) cursorpoint=cursorpoint+2;
                         else cursorpoint++;//going forward;
                         LCD_Goto(cursorpoint,0);
                   }

                  }
		_delay_ms(20);
            }
         }
      }

      //return 0xFF;//Indicate No key pressed
   }


void setalarmstart()
{
       //LCD_Integer (modechange);
       //_delay_ms(20);
       LCD_Clear();
       LCD_Goto(0,0);

      LCD_String("#");
      LCD_Goto(2,0);

      LCD_String("HH:MM");
      LCD_Goto(8,0);//2st arg detemines which line,1st arg determines position in that line

     LCD_String("S");
      int key;
      cursorpoint=0;
      //noofalarm=0;
      LCD_Goto(cursorpoint,0);
      while(1)
      {
         GetKeyPressed(); //Get the keycode of pressed key

      }
}
void getkeypressed2()
{
     int r,c;

      PORTA|= 0X0F;
      //LCD_Line(1);
      for(c=0;c<4;c++)
      {
         DDRA&=~(0XFF);

         DDRA|=(0X80>>c);
        // LCD_Integer (c);
         for(r=0;r<4;r++)
         {
            if(!(PINA & (0X08>>r)))
            {
		if(c==0)
                  {
                    if(r==0 && cursorpoint==8)
                      {

                      }
                    else if(r==1) {} //LCD_String("B");
                    else if(r==2)
                      {
                       //if(modechange==0)
                         //{
                            modechange=1;_delay_ms(50);
                            setalarmstart();

                          //}

                      // LCD_String("C");
                     }
                    else if(r==3)  {}//LCD_String("D");
                  }

                else if(r*3+c==0x0b)
                  {

                  }
                else
                 {

                  }
		_delay_ms(20);
            }
         }
      }

}

void FlashLED()
{
SetBit(PORTB,LED);
msDelay(250);
ClearBit(PORTB,LED);
msDelay(250);
}




void ShowDevices()
// Scan I2C addresses and display addresses of all devices found
{
LCD_Line(1); LCD_String("Found:");
byte addr = 1;
while (addr>0)
{
LCD_Char(' ');
addr = I2C_FindDevice(addr);
if (addr>0) LCD_Hex(addr++);
}
}



void LCD_TwoDigits(byte data)
// helper function for WriteDate()
// input is two digits in BCD format
// output is to LCD display at current cursor position
{
//char c;
byte temp = data>>4;
LCD_Char(temp+'0');
//c=temp+'0';
data &= 0x0F;
LCD_Char(data+'0');
//c+=data+'0';
//int k=(int)c;
}




void WriteDate()
{
byte months, days, years;
DS1307_GetDate(&months,&days,&years);
LCD_TwoDigits(months);
LCD_Char('/');
LCD_TwoDigits(days);
LCD_Char('/');
LCD_TwoDigits(years);
}




void WriteTime()
{
byte hours, minutes, seconds;
DS1307_GetTime(&hours,&minutes,&seconds);
LCD_TwoDigits(hours);
LCD_Char(':');
LCD_TwoDigits(minutes);
LCD_Char(':');
LCD_TwoDigits(seconds);

LCD_Char(' ');
byte hour=currentAlarm.h2+(currentAlarm.h1*10);
byte min=currentAlarm.min2+(currentAlarm.min1*10);

//LCD_Integer(hour);
//LCD_Integer(min);

byte tempHour,tempMin,tempSec;
tempHour=hours;
tempMin=minutes;
byte h,m;
byte temp;
 temp= tempHour>>4;
h=temp*10;
tempHour &= 0x0F;
h+=tempHour;

//byte h=(byte)c-60-3*9;

temp= tempMin>>4;
m=temp*10;
tempMin &= 0x0F;
m+=tempMin;
//byte m=(byte)c-60-9*2;

//LCD_Integer(h);
//LCD_Integer(m);
if(h==hour && m==min)
{


if(!flagOfSettingAlarm)
{
 int i;
for(i=whichis;i<noofalarm;i++)
{
   arraytime[i].h1=arraytime[i+1].h1;
  arraytime[i].h2=arraytime[i+1].h2;

  arraytime[i].min1=arraytime[i+1].min1;

  arraytime[i].min2=arraytime[i+1].min2;


}
noofalarm--;

flagOfSettingAlarm=1;
//settingCurrentAlarm();
}
PORTD=0b01000000;
}
else
{
flagOfSettingAlarm=0;
if(noofalarm==0)
{
 currentAlarm.h1=currentAlarm.h2=currentAlarm.min1=currentAlarm.min2=inf;
time1.h1=time1.h2=time1.min1=time1.min2=inf;
arraytime[0].h1=arraytime[0].h2=arraytime[0].min1=arraytime[0].min2=inf;
}
settingCurrentAlarm();
PORTD=0x0;
}

}




void LCD_TimeDate()
{
LCD_Line(0); WriteTime();
//LCD_Line(1); WriteDate();
}
//
//
//---------------------------------------------------------------------------
///PROGRAM LOOP



void MainLoop()
{
while(1)
{
LCD_Line(0);

LCD_TimeDate();

LCD_Line(1);
//LCD_Integer (1);
//LCD_String("current alarm: ");
if(currentAlarm.h1==inf||currentAlarm.h2==inf||currentAlarm.min1==inf||currentAlarm.min2==inf)
{LCD_String("No Alarm!");}
else{
LCD_Integer(currentAlarm.h1);
LCD_Integer(currentAlarm.h2);
LCD_String(":");
LCD_Integer(currentAlarm.min1);
LCD_Integer(currentAlarm.min2);
}
getkeypressed2();

}
}

// ---------------------------------------------------------------------------
// MAIN PROGRAM
int main(void)
{

// InitAVR();
//       //Wait for LCD To Start
// 	LCD_Init();
//       //_delay_loop_2(0);
// 	_delay_ms(50);
// setalarmstart();
//
//
//



InitAVR(); // set port direction
LCD_Init();
I2C_Init();
LCD_String("Ready.");
ShowDevices(); // show that I2C is working OK
msDelay(50);
LCD_Clear();
//Lcd4_Clear();
//SetTimeDate();
MainLoop(); // display time

}
