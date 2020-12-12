/*
  2 3口作为软串口接WIFI模块，用IDE自带串口监视器调试  
*/
#include <SoftwareSerial.h>
#include <Wire.h>
#include "edp.c"
#define KEY  "HkJl7CamkwDPbphYFux5sCH9wDM="    //APIkey 
#define ID   "655491010"                          //设备ID
#define PUSH_ID NULL


#define _rxpin      3
#define _txpin      2
#define DBG_UART    Serial    //调试串口
#define WIFI_UART  dbgSerial   //设定联网串口
#define redPin 9 
#define greenPin 10 
#define bluePin 11  //rgbled
SoftwareSerial dbgSerial( _rxpin, _txpin ); // 软串口联网
edp_pkt *pkt;
unsigned long setupMillis; //统计初始化所用时间
char *pend;  //使用strtoul函数必须的参数，程序中并未使用
int sitTime=0; // 累加从主机发来的坐姿持续时间
int Time;  //最终发送至onenet的时间
unsigned long sleepTime=0;  // 通过主机发送信息累加的时间
int count(0); //控制到达设定时间后只发送一次数据的变量
unsigned long beat=0; //控制发送类心跳包的时间变量
/*
* doCmdOk
* 发送命令至模块，从回复中获取期待的关键字
* keyword: 所期待的关键字
* 成功找到关键字返回true，否则返回false
*/
bool doCmdOk(String data, char *keyword)
{
  bool result = false;
  if (data != "")   //对于tcp连接命令，直接等待第二次回复
  {
    WIFI_UART.println(data);  //发送AT指令
    DBG_UART.print("SEND: ");
    DBG_UART.println(data);
    DBG_UART.println(WIFI_UART.readStringUntil('\n'));
  }
  if (data == "AT")   //检查模块存在
    delay(2000);
  else
    
  while (!WIFI_UART.available());  // 等待模块回复
  delay(200);
  if (WIFI_UART.find(keyword))   //返回值判断
  {
    DBG_UART.println("do cmd OK");
    result = true;
  }
  else
  {
    DBG_UART.println("do cmd ERROR");
    result = false;
  }
  while (WIFI_UART.available()) WIFI_UART.read();   //清空串口接收缓存
  delay(500); //指令时间间隔
  return result;
}

/*
* addSitTime 在主机向本机发送数据时，给sitTime加一，并亮一下蓝灯
*/
void addSitTime(){
    byte readValue=Wire.read();
    if(readValue!=0)
       sitTime++;
    analogWrite(bluePin,200);
    delay(500);
    analogWrite(bluePin,0);
  }

 /*
* packetSend
* 将待发数据发送至串口，并释放到动态分配的内存
*/
void packetSend(edp_pkt* pkt)
{
  if (pkt != NULL)
  {
    WIFI_UART.write(pkt->data, pkt->len);    //串口发送
    WIFI_UART.flush();
    free(pkt);              //回收内存
  }
}

/*
* readEdpPkt
* 从串口缓存中读数据到接收缓存
*/


bool readEdpPkt(edp_pkt *p)
{
  int tmp;
  if ((tmp = WIFI_UART.readBytes(p->data + p->len, sizeof(p->data))) > 0 )
  {
    rcvDebug(p->data + p->len, tmp);
    p->len += tmp;
  }
  return true;
}



void rcvDebug(unsigned char *rcv, int len)
{
  int i;

  DBG_UART.print("rcv len: ");
  DBG_UART.println(len, DEC);
  for (i = 0; i < len; i++)
  {
    DBG_UART.print(rcv[i], HEX);
    DBG_UART.print(" ");
  }
  DBG_UART.println("");
}

/*
* breathLight 
* RGB呼吸流水灯
*/
void breathLight(){
  int i;
  for(i=0;i<128;i++)
     {analogWrite(redPin,i);
     delay(10);}
  for(;i>=0;i--)
     {analogWrite(redPin,i);
     delay(10);}
  for(;i<128;i++)
     {analogWrite(greenPin,i);
     delay(10);}
  for(;i>=0;i--)
     {analogWrite(greenPin,i);
     delay(10);}
  for(;i<128;i++)
     {analogWrite(bluePin,i);
     delay(10);}
  for(;i>=0;i--)
     {analogWrite(bluePin,i);
     delay(10);} 
  }
/*
* uploadData
* 向onenet发送久坐时间
*/
void uploadData(){
  DBG_UART.println("UPLOADING...");
  char buff[10];
  const char *p= itoa(Time,buff,10); //int转const char*
  packetSend(packetDataSaveTrans(NULL, "sitTime", p));
  count++; //防止该函数再次被执行
 }
void setup()
{
  char buf[100] = {0};
  int tmp;
  WIFI_UART.begin(9600);
  DBG_UART.begin(9600);
  WIFI_UART.setTimeout(3000);    //设置find超时时间
  delay(3000);
  DBG_UART.println("hello world!");

  delay(2000);
  while (!doCmdOk("AT", "OK"));
  

  while (!doCmdOk("AT+CWMODE=3", "OK"));            //工作模式 STA+AP
  while (!doCmdOk("AT+CWJAP=\"LAPTOP1\",\"hkzm2011\"", "OK")); //连接笔记本热点 热点名、密码可替换
  while (!doCmdOk("AT+CIPSTART=\"TCP\",\"183.230.40.39\",876", "CONNECT")); //连接TCP EDP协议服务器
  while (!doCmdOk("AT+CIPMODE=1", "OK"));           //透传模式
  while (!doCmdOk("AT+CIPSEND", ">"));      //开始发送
  pinMode(8,INPUT);
  pinMode(9, OUTPUT); 
  pinMode(10, OUTPUT); 
  pinMode(11, OUTPUT); 
  Wire.begin(8);//从机
  Wire.onReceive(addSitTime); //设定接收到主机数据的事件
  
}

void loop()
{
  //以下主要与建立edp协议的连接有关
  static int edp_connect = 0;
  bool trigger = false;
  edp_pkt rcv_pkt;
  unsigned char pkt_type;
  int i, tmp;
  char num[10];

  /* EDP 连接 */
  if (!edp_connect)
  {
    while (WIFI_UART.available()) WIFI_UART.read(); //清空串口接收缓存
    packetSend(packetConnect(ID, KEY));             //发送EPD连接包
    while (!WIFI_UART.available());                 //等待EDP连接应答
    if ((tmp = WIFI_UART.readBytes(rcv_pkt.data, sizeof(rcv_pkt.data))) > 0 )
    {
      rcvDebug(rcv_pkt.data, tmp);
      if (rcv_pkt.data[0] == 0x20 && rcv_pkt.data[2] == 0x00 && rcv_pkt.data[3] == 0x00)
      {
        edp_connect = 1;
        DBG_UART.println("EDP connected.");
        analogWrite(greenPin,200);
        delay(500);
        analogWrite(greenPin,0); //edp成功建立连接后闪一下绿灯
      }
      else
        DBG_UART.println("EDP connect error.");
    }
    packetClear(&rcv_pkt);
  }

  while (WIFI_UART.available())
  {
    readEdpPkt(&rcv_pkt);
    if (isEdpPkt(&rcv_pkt))
    {
      pkt_type = rcv_pkt.data[0];
      switch (pkt_type)
      {
        case CMDREQ:
          char edp_command[50];
          char edp_cmd_id[40];
          long id_len, cmd_len, rm_len;
          char datastr[20];
          char val[10];
          memset(edp_command, 0, sizeof(edp_command));
          memset(edp_cmd_id, 0, sizeof(edp_cmd_id));
          edpCommandReqParse(&rcv_pkt, edp_cmd_id, edp_command, &rm_len, &id_len, &cmd_len);
          DBG_UART.print("rm_len: ");
          DBG_UART.println(rm_len, DEC);
          delay(10);
          DBG_UART.print("id_len: ");
          DBG_UART.println(id_len, DEC);
          delay(10);
          DBG_UART.print("cmd_len: ");
          DBG_UART.println(cmd_len, DEC);
          delay(10);
          DBG_UART.print("id: ");
          DBG_UART.println(edp_cmd_id);
          delay(10);
          DBG_UART.print("cmd: ");
          DBG_UART.println(edp_command);

          //数据处理与应用中EDP命令内容对应
          //本例中格式为  sitTime:{VALUE} VALUE由小程序设定 
          sscanf(edp_command, "%[^:]:%s", datastr, val); //截取并另存字符串 这里为截取“：”后的字符串，具体请见sscanf用法
          DBG_UART.println(datastr);
          DBG_UART.println(val);
          String val1=String(val);
          DBG_UART.println(val1);
          val1=val1.substring(1,val1.length()-1); //截取并另存字符串
          DBG_UART.println(val1); 
          sleepTime=strtoul(val1.c_str(),&pend,10);  //str转unsigned long函数 从此处获得设定睡眠时间（XX分钟后入睡）注意一定要转为unsigned long 因为int和unsigned long的储存格式不一样，不能直接比大小
          DBG_UART.println(sleepTime);
          packetSend(packetDataSaveTrans(NULL, datastr, val1.c_str())); //将处理后的数据上传至onenet（方便调试，实际可以删掉）
          analogWrite(greenPin,200);
          delay(400);
          analogWrite(greenPin,0);
          setupMillis=millis();
          break;
        default:
          DBG_UART.print("unknown type: ");
          DBG_UART.println(pkt_type, HEX);
          break;
      }
    }
    //delay(4);
  }
  if (rcv_pkt.len > 0)
    packetClear(&rcv_pkt);

//以下为获得小程序端发来的指令中所含数据后的执行部分
unsigned long currentMillis=millis()-setupMillis;    //减去建立edp连接所用时间，减少误差
DBG_UART.println(currentMillis);
int brightness = analogRead(A0); 
if(brightness<300){
 analogWrite(redPin,0);
  }
if (sleepTime!=0){
    if(sleepTime>2){
          if(currentMillis>=sleepTime*1000*60){
                    if(count==0){
                             Time=sitTime;
                             breathLight();
                             uploadData();   }
                   else 
                             breathLight();
}
            else if(currentMillis>=(sleepTime-2)*1000*60){ 
                   if(brightness>=300)
                         analogWrite(redPin,200);
} 
}
else{
     if(brightness>300)
              analogWrite(redPin,200);
     if(currentMillis>=sleepTime*1000*60&&sleepTime!=0){
                  if(count==0){
                       Time=sitTime;
                       breathLight();
                       uploadData();
                            }
                   else
                       breathLight();
}
}
  

}
delay(250);
beat+=250;
if(beat==20000){
const char* q="beat";
packetSend(packetDataSaveTrans(NULL, "beat", q));
beat=0;
} //此处用来每20s发送一个类似心跳包的数据，以维持edp的长时间连接（否则最长只能连接4分钟）
}
