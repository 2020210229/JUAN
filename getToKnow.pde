/**
 * Simple Read
 * 
 * Read data from the serial port and change the color of a rectangle
 * when a switch connected to a Wiring or Arduino board is pressed and released.
 * This example works with the Wiring / Arduino program that follows below.
 */


import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;      // Data received from the serial port
static float color1=0; 
static float color2=255;  
static float color3=255;
static int count=0;
float n=0.0625;
int m=0;
static int xpos=0; 
static int ypos=0;
static int num=0;
void setup() 
{
  size(800, 400);
  myPort = new Serial(this,"com4", 38400);  // get connected with arduino
}

int TimeToChange(int time,int x)  //change the color of progress bar
{ if(time>100*x)
    return 255;
  else return 0;
}


boolean mouseOverAlarmRect() {  //whether you put the mouse on the white rectangle
  return ((mouseX>=400)&&(mouseX<=800)&&(mouseY>=0)&&(mouseY<=200));
}
boolean mouseOverProgressRect() {   //whether you put the mouse on the progress bar
  return ((mouseX>=400)&&(mouseX<=800)&&(mouseY>=200)&&(mouseY<=400));
}

int changeTime(int time)   // a func that make progress bar move and halt 
 {if(mouseOverProgressRect())
    return 0;
  else if (!mouseOverProgressRect()&&time>=2400)
    return 2400;
  else 
    return time+1;
 }

int changeX(int x,int m){  // a func that can change the position of the square on x-axis
switch (m){
 case 0:{
 if(x<200)
  x++;
 }break;
 case 1:
  break;
 case 2:{
  if(x>0)
   x--;
   }break;
 case 3:
   break;
 case 4:{
   if(x<200)
     x++;
 } break;
 case 5:{
   if(x>0)
     x--;
 } break;
  }
  return x;
}
int changeY(int y,int n){   // a func that can change the position of the square on y-axis
switch (n){
 case 0:
  break;
 case 1:{
 if(y<200)
  y++;
 }break;
 case 2:
   break;
 case 3:{
  if(y>0)
   y--;
   }break;
 case 4:{
   if(y<200)
     y++;
 } break;
 case 5:{
   if(y>0)
     y--;
 } break;}
  return y;
}

void draw()
{
  if ( myPort.available() > 0) {  // If data is available,
    val = myPort.read();         // read it and store it in val
    
  }
  xpos=changeX(xpos,num);
  ypos=changeY(ypos,num);
  switch (num){                 
    case 0:
       if(xpos==200){
         num++;}
       break;
    case 1:
       if(ypos==200){
         num++;}
       break; 
    case 2:
       if(xpos==0){
         num++;}
       break; 
    case 3:
       if(ypos==0){
         num++;}break;
    case 4:
       if(xpos==200)
        num++;
       break; 
    case 5:
       if(xpos==0)
        num=0;
       break; 
  }
  background(150);// Set background to grey(150)
  if (val==0) {      // If the serial value is 0, the square will turn green(but with some delay)
    color1=0;
    color2=255;
    count=0;
    n=0.0625;
       
  } 
  else {                       // If the serial value is not 0, the square will slightly turn into a red one 
    color1+=n;
    color2-=n;
  }

    
if(color1>=254){              //to keep the color1\2 valid
    color1=254;
    color2=0;
    count++;
if(mouseOverAlarmRect()==true&&count==1){  //first alarm
    color1=0;
    color2=255;
    n=0.25;
    myPort.write('F');}
else if (count==2){  //second alarm
    myPort.write('S');
    }       
  }
  fill(color1,color2,0);
  rect(xpos, ypos, 200, 200);
  
  fill(255,255,255);
  rect(400,0,400,200);
  
 for(int i=1;i<21;i++)    //generate progress bar
   {fill(0,175,TimeToChange(m,i));
    rect(380+20*i,200,40,200);}
  
  m=changeTime(m);

}
