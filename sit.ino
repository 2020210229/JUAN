#include<Wire.h>
const int TrigPin1 = 10; //发送超声波引脚1、2
const int TrigPin2= 13;
const int EchoPin1 = 11;//接收超声波引脚1、2
const int EchoPin2 = 12; 
const int BellPin=9;//有源蜂鸣器引脚
unsigned long int sitTime=0;//用来累加坐姿时间
byte x=1; //用来向从机发送以累加坐着的时间
unsigned long interval=20000; //设定统计起立活动时间的变量
bool rest=0; //0代表正在坐，1代表“应该”起立休息
bool first=1; //控制interval只清零一次的布尔值
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(38400);     
  
  pinMode(TrigPin1, OUTPUT);//TRIG与发射超声波有关，要设为OUTPUT
  pinMode(TrigPin2, OUTPUT);
  pinMode(EchoPin1, INPUT);
  pinMode(EchoPin2, INPUT); //ECHO与接收有关，要设为INPUT
  //超声波首发有关引脚
  pinMode(BellPin,OUTPUT);
}
//获取传感器1返回的距离值
double getDistance1(){
digitalWrite(TrigPin1, LOW);
delayMicroseconds(2);
digitalWrite(TrigPin1, HIGH);
delayMicroseconds(10);
digitalWrite(TrigPin1, LOW);


return pulseIn(EchoPin1, HIGH) / 58.0; 
//返回距离值
  }
//获取传感器2返回的距离值
double getDistance2(){
digitalWrite(TrigPin2, LOW);
delayMicroseconds(2);
digitalWrite(TrigPin2, HIGH);
delayMicroseconds(10);
digitalWrite(TrigPin2, LOW);

return pulseIn(EchoPin2, HIGH) / 58.0; 
//返回距离值
  }
void loop() {
double distance1=getDistance1();
double distance2=getDistance2();
//Serial.print(distance1);
//Serial.print("&&&&&");
//Serial.println(distance2);
if(distance1<=25||distance2<=25){
  if(rest==1&&interval<20000){ //计时小于20s，触发反作弊机制，给蜂鸣器高电平
    digitalWrite(BellPin,HIGH);
    interval+=750;  
    }
  else if(rest==1) 
   {rest=0;  //计时达到20s，不触发机制||允许作弊坚决者坚持20s持续听有源蜂鸣器解除反作弊
    digitalWrite(BellPin,LOW);
    }
   first=1;
   Serial.write(1);
   sitTime+=750;
 Serial.println(sitTime);
  }
 else{
 if(first==1){ //第一次观测到起身将计时清零
  interval=0;
  first=0;
  }
  rest=1; 
  digitalWrite(BellPin,LOW);
  Serial.write(0);
  interval+=750; //之后不断累加
  }  
//两个传感器只要一个采集到小于25cm的距离就通过串口向Processing发送1，否则发送0；活动时间小于20s坐回椅子会让蜂鸣器一直响到interval达到设定的时间间隔为止
if(Serial.available()>0){
  switch(Serial.read()){
   case 'F':
       digitalWrite(BellPin,HIGH);
       delay(500);
       digitalWrite(BellPin,LOW);
       break;
   case 'S':
       digitalWrite(BellPin,HIGH);
       break;
  }  //接收从Processing 返回的数据，‘F'、‘S’分别对应蜂鸣器响一声、一直响
   
  }
 if(sitTime>=10000){
  Serial.print("send begin___");
  Wire.beginTransmission(8);
  Wire.write(x);
  Wire.endTransmission(8);
  sitTime=0;
  Serial.println(x);
  } // 每当累加的维持坐姿时间达到10s时向从机汇报一次
delay(750); //这个delay要调试到和processing的draw函数（近似）同步
}
