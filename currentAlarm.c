#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct Time{
int h1;
int h2;
int min1;
int min2;

};
int noofalarm;
struct Time time1={0,0,0,0};
struct Time currentAlarm={0,0,0,0};
struct Time arraytime[4];
int whichis;

void settingCurrentAlarm()
{

                             int minH=arraytime[0].h2+(arraytime[0].h1*10);
                             whichis=0;
                             for(int i=0;i<noofalarm;i++)
                             {
                               int hour=arraytime[i].h2+(arraytime[i].h1*10);
                               if(hour<minH)
                               {
                                 minH=hour;
                                 whichis=i;
                               }
                             }
                             int minM=arraytime[whichis].min2+(arraytime[whichis].min1*10);
                             for(int i=0;i<noofalarm;i++)
                             {
                                 int min=arraytime[i].min2+(arraytime[i].min1*10);
                                 int hour=arraytime[i].h2+(arraytime[i].h1*10);
                                 if(i!=whichis && hour==minH && min<minM)
                                 {
                                   minM=min;
                                   whichis=i;

                                 }
                             }
                             printf("whichis is %d\n",whichis );
                             currentAlarm.h1=arraytime[whichis].h1;
                             currentAlarm.h2=arraytime[whichis].h2;
                             currentAlarm.min1=arraytime[whichis].min1;
                             currentAlarm.min2=arraytime[whichis].min2;



}
int main()
{
  arraytime[0].h1=1;
  arraytime[0].h2=2;
  arraytime[0].min1=2;
  arraytime[0].min2=0;

  arraytime[1].h1=1;
  arraytime[1].h2=2;
  arraytime[1].min1=1;
  arraytime[1].min2=5;

  arraytime[2].h1=1;
  arraytime[2].h2=2;
  arraytime[2].min1=3;
  arraytime[2].min2=5;
  noofalarm=3;

  for(int i=0;i<noofalarm;i++)
  {
    printf("%d%d:%d%d\n",arraytime[i].h1,arraytime[i].h2,arraytime[i].min1,arraytime[i].min2 );
  }
  settingCurrentAlarm();
  printf("%d%d:%d%d\n",currentAlarm.h1,currentAlarm.h2,currentAlarm.min1,currentAlarm.min2 );


  int i;
printf("which is%d\n",whichis );

for(i=whichis;i<noofalarm;i++)
{
   arraytime[i].h1=arraytime[i+1].h1;
  arraytime[i].h2=arraytime[i+1].h2;

  arraytime[i].min1=arraytime[i+1].min1;

  arraytime[i].min2=arraytime[i+1].min2;


}
noofalarm--;
settingCurrentAlarm();
printf("%d%d:%d%d\n",currentAlarm.h1,currentAlarm.h2,currentAlarm.min1,currentAlarm.min2 );



}
