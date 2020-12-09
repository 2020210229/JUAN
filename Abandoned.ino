//引入ESP8266.h头文件，建议使用教程中修改后的文件
#include "ESP8266.h"
#include "SoftwareSerial.h"
#include <Wire.h>
#include<avr/pgmspace.h>
//配置ESP8266WIFI设置
#define SSID "assholes"    //填写2.4GHz的WIFI名称，不要使用校园网
#define PASSWORD "hkzm2011"//填写自己的WIFI密码
#define HOST_NAME "api.heclouds.com"  //API主机名称，连接到OneNET平台，无需修改
#define DEVICE_ID "654170577"       //填写自己的OneNet设备ID
#define HOST_PORT (80)                //API端口，连接到OneNET平台，无需修改
String APIKey = "h6ASd1Ay=OdUg54hC=tXLCgULzA="; //与设备绑定的APIKey
String comdata="";
unsigned long sleepTime(5);
#define redPin 9 
#define greenPin 10 
#define bluePin 11 
unsigned long setupMillis;
char *pend;
int sitTime=0;
int Time=0;
int count=0;
//定义ESP8266所连接的软串口
/*********************
 * 该实验需要使用软串口
 * Arduino上的软串口RX定义为D3,
 * 接ESP8266上的TX口,
 * Arduino上的软串口TX定义为D2,
 * 接ESP8266上的RX口.
 * D3和D2可以自定义,
 * 但接ESP8266时必须恰好相反
 *********************/
SoftwareSerial mySerial(3,2);
ESP8266 wifi(mySerial);
void gettime(){

     while (mySerial.available() > 0){
        comdata += char(mySerial.read());  //每次读一个char字符，并相加
        delay(1);
    }
    if (comdata.length() > 0){
      comdata=comdata.substring(312,comdata.length()-18);
      Serial.println(comdata);
    }
    else{Serial.println("nothing received");}
    }


void setup()
{
  
  mySerial.begin(115200); //初始化软串口
  Serial.begin(9600);     //初始化串口
 do{getData();}while(comdata=="");
do{deleteData();}while(sleepTime==0);

  pinMode(9, OUTPUT); 
  pinMode(10, OUTPUT); 
  pinMode(11, OUTPUT); 
  setupMillis= millis();
  Wire.begin(8);
  Wire.onReceive(addSitTime); 
}



void getData(){
 Serial.print("setup begin\r\n");
  //以下为ESP8266初始化的代码
  Serial.print("FW Version: ");
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStation()) {
    Serial.print("to station ok\r\n");
  } else {
    Serial.print("to station err\r\n");
  }

  //ESP8266接入WIFI
  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print("Join AP success\r\n");
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.print("Join AP failure\r\n");
  }

  Serial.println("");
  mySerial.println("AT+UART_CUR=9600,8,1,0,0");
  mySerial.begin(9600);
  Serial.println("setup end\r\n");
    if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
      Serial.print("create tcp ok\r\n");
      //拼接POST请求字符串
      String postString = "GET /devices/";
      postString += DEVICE_ID;
      postString += "/datastream/setTime HTTP/1.1";
      postString += "\r\n";
      postString += "api-key: ";
      postString += APIKey;
      postString += "\r\n";
      postString += "Host: api.heclouds.com\r\n\r\n";
      const char *postArray = postString.c_str(); //将str转化为char数组
      Serial.println(postArray);
      wifi.send((const uint8_t *)postArray, strlen(postArray)); //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
      Serial.println("send success");
      gettime();
    } else {
      Serial.println("create tcp err\r\n");
    }
  }
  
void deleteData(){
        if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
      Serial.print("create tcp ok\r\n");
      //拼接POST请求字符串
      String postString = "DELETE /devices/";
      postString += DEVICE_ID;
      postString += "/datastreams/setTime HTTP/1.1";
      postString += "\r\n";
      postString += "api-key:";
      postString += APIKey;
      postString += "\r\n";
      postString += "Host:api.heclouds.com\r\n";
      postString += "Connection: close\r\n\r\n";
      
      const char *postArray = postString.c_str(); //将str转化为char数组
      Serial.println(postArray);
      wifi.send((const uint8_t *)postArray, strlen(postArray)); //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
      Serial.println("send success");
      sleepTime= strtoul(comdata.c_str(),&pend,10) ; 
      Serial.println(sleepTime)    ;
      if (wifi.releaseTCP()) { //释放TCP连接
        Serial.print("release tcp ok\r\n");
      } else {
        Serial.print("release tcp err\r\n");
      }
    } else {
      Serial.println("create tcp err\r\n");
    }
    }


void uploadData(){
  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print("Join AP success\r\n");
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.print("Join AP failure\r\n");
  }

  Serial.println("");
  mySerial.println("AT+UART_CUR=9600,8,1,0,0");
  mySerial.begin(9600);
  Serial.println("setup end\r\n");
char buff[10];
    if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
      Serial.print("create tcp ok\r\n");
      String jsonToSend="{\"SitTime\":";
      itoa(Time,buff,10);
      jsonToSend += "\"" + String(buff) + "\"}";
      //拼接POST请求字符串
      String postString = "POST /devices/";
      postString += DEVICE_ID;
      postString += "/datapoints?type=3 HTTP/1.1";
      postString += "\r\n";
      postString += "api-key:";
      postString += APIKey;
      postString += "\r\n";
      postString += "Host:api.heclouds.com\r\n";
      postString += "Connection:close\r\n";
      postString += "Content-Length:";
      postString += jsonToSend.length();
      postString += "\r\n";
      postString += "\r\n";
      postString += jsonToSend;
      postString += "\r\n";
      postString += "\r\n";
      postString += "\r\n";

      const char *postArray = postString.c_str(); //将str转化为char数组

      Serial.println(postArray);
      wifi.send((const uint8_t *)postArray, strlen(postArray)); //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
      Serial.println("send success");
      if (wifi.releaseTCP()) { //释放TCP连接
        Serial.print("release tcp ok\r\n");
      } else {
        Serial.print("release tcp err\r\n");
      }
      postArray = NULL; //清空数组，等待下次传输数据
    } else {
      Serial.print("create tcp err\r\n");
    }
    }

void breathLight(){
  int i;
  for(i=0;i<128;i++)
     {analogWrite(redPin,i);
     delay(10);}
  for(;i>0;i--)
     {analogWrite(redPin,i);
     delay(10);}
  for(;i<128;i++)
     {analogWrite(greenPin,i);
     delay(10);}
  for(;i>0;i--)
     {analogWrite(greenPin,i);
     delay(10);}
  for(;i<128;i++)
     {analogWrite(bluePin,i);
     delay(10);}
  for(;i>0;i--)
     {analogWrite(bluePin,i);
     delay(10);} 
     uploadData();
  }

void addSitTime(){
    sitTime=Wire.read();
    Serial.print("TIME");
    Serial.println(sitTime);
  }
  
void loop(){

int brightness = analogRead(A0);
if(brightness<300){
 analogWrite(redPin,0);
  }
unsigned long currentMillis= millis();
Serial.print(brightness);
Serial.println(currentMillis-setupMillis);
if(sleepTime>1){
  
if(currentMillis-setupMillis>=sleepTime*1000*60){
 if(count==0){
  Time=sitTime;
 breathLight();   
  }
  else if (count==1)
  breathLight();
  else 
  uploadData();
}

else if(currentMillis-setupMillis>=(sleepTime-1)*1000*60&&brightness>300){ 
 analogWrite(redPin,200);
} 

}
else{
if(brightness>300)
analogWrite(redPin,200);
if(currentMillis-setupMillis>=sleepTime*1000*60){
 if(count==0){
  Time=sitTime;
 breathLight();   
  }
  else
  breathLight();
}
}
delay(1000);
}
