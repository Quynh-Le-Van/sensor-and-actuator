/*
   Terminal Module is like a chat box. It allows you to send and receive commands between your
   board and smartphone.
   NOTE:
   1)For Arduino Mega Connect Bluetooth on Serial3 pins.
   2)For Arduino Uno/Nano library uses SoftwareSerial,hence pin 2 and pin 3 are used
   as RX and TX pins respectively on SoftwareSerial.Hence with arduino Uno
   follow below connections for bluetooth.
   UNO  - BLUETOOTH
   2    - TX
   3    - RX
   3)For Uno/Nano keep bluetooth Baudrate below 38400.

   You can reduce the size of library compiled by enabling only those modules that you want
   to use. For this first define CUSTOM_SETTINGS followed by defining INCLUDE_modulename.

   Explore more on: https://thestempedia.com/docs/dabble/terminal-module/
*/

#define CUSTOM_SETTINGS
#define INCLUDE_TERMINAL_MODULE
#define INCLUDE_NOTIFICATION_MODULE
#define INCLUDE_MUSIC_MODULE
#define MAX 1000000
#define MIN -1000000


#include <Dabble.h>
#include<time.h>




String intruction1="Gõ Help :để xem các câu lệnh";
String intruction2="Gõ On :để bật thiết bị";
String intruction3="Gõ Off :để tắt thiết bị";
String intruction4="Gõ find :để tìm vị trí";
String intruction5="Gõ warming on :để bật chống trộm";
String intruction6="Gõ warming off :để tắt chống trộm";
String intruction7="Gõ check on :để kiểm tra rung động";
String intruction8="Gõ check off :để tắt kiểm tra";
String intruction9="Sản phẩm của Quỳnh Thái Bảo";


String Serialdata = "";
int modeOnOff=0 , modeCheck=0 , modeWarm=0,modeHelp;
int flat = 0,flag=1;
int valueSensor;
int countsW=0,countsF=0,countsC=0;


void setup() {
    pinMode(A0,INPUT);
    pinMode(12,OUTPUT);
    Serial.begin(9600);       
    Dabble.begin(9600);  
    Dabble.waitForAppConnection(); 
    Notification.clear();
    Notification.setTitle("Cảnh báo từ ProTeenTeen");
}

void Intruction()
{
        Terminal.println(String(intruction1));
        delay(50);
        Terminal.println(String(intruction2));
        delay(50);
        Terminal.println(String(intruction3));
        delay(50);
        Terminal.println(String(intruction4));
        delay(50);
        Terminal.println(String(intruction5));
        delay(50);
        Terminal.println(String(intruction6));
        delay(50);
        Terminal.println(String(intruction7));
        delay(50);
        Terminal.println(String(intruction8));
        delay(50);
        Terminal.println(String(intruction9));
        
}

void MakeSound(int value)
{
    for(int i=0;i<value;i++)
    {

        tone(12,400,500);
        delay(250);
        tone(12,0,500);
        delay(250);
    }
}


void FindPosition(String str)
{
    if(str=="find")
    {
        MakeSound(10);
        Notification.notifyPhone(String("Đã phát tín hiệu lần ")+countsF);
        countsF++;
        delay(100);
    }
}


void Warming(int value,int hz)
{   
    if (value<hz)
    {
        if(flat==0)
        {
            flat=1;
            for(int i=0;i<2;i++)
            {
                Notification.notifyPhone(countsW+ String(" -Warming "));
                Music.play("Q3");
                Music.addToQueue("Q3");
                
                countsW++;
                MakeSound(10);
                delay(1000);
            }   
        }

        MakeSound(10);
    }
    else
        flat=0;
}



void CheckVibration(String str,int valueSs)
{   int val=0;
    for(int i=1;i<=valueSs;i++)
    {   val=analogRead(A0);
      
        if(val>1022)
        {
            Terminal.println(i+String(".Ổn định : ")+val);
        }
        else if(val<1022&&val>500)
        {
            Terminal.println(i+String(".Rung động nhẹ : ")+val);
        }
        else 
            Terminal.println(i+String(".Rung động mạnh : ")+val);

        delay(500);
    }
    
}


void loop() {

    Dabble.processInput();             

    Serialdata="";
    valueSensor=analogRead(A0);
    modeHelp=0;
//    Serial.println(1023-valueSensor);
    // hướng dẫn
    if(flag==1)
        Intruction();

    //lấy giá trị từ smartphone
    if(Terminal.available())
    {
        while (Terminal.available() != 0)
        {
            Serialdata = String(Serialdata + char(Terminal.read()));
        }
        Serial.println(Serialdata);
    }

    // kiểm tra bật hay tắt thiết bị 

    if(Serialdata=="On")
        modeOnOff=1;
    
    if(Serialdata=="Off")
        modeOnOff=0;
    if(Serialdata=="Help")
        modeHelp=1;

    // bật thiết bị  
    if(modeOnOff==1)
    {

        // ứng dụng tìm vị trí 
        FindPosition(Serialdata);


        // ứng dụng cảnh báo 
        if(Serialdata=="warming on")
            modeWarm=1;
        if(Serialdata=="warming off") 
            modeWarm=0;

        if(modeWarm==1)
            Warming(valueSensor,900);
        if(modeWarm==0)
            Warming(MAX,MIN); // tắt 


        // ứng dụng kiểm tra rung động 
        if(Serialdata=="check on")
            modeCheck=1;
        if(Serialdata=="check off")
            modeCheck=0;
        
        if(modeCheck==1)
        {
            Warming(MAX,MIN);  // tắt 
            CheckVibration(Serialdata, 10);
        }
        if(modeCheck==0)
        {
            // do nothing 
        }
    }


    // tắt thiết bị 
    if(modeOnOff==0)
    {
        // do nothing 
    }

    // xem hướng dẫn 
    if(modeHelp==1)
    {
        Intruction();
    }
    flag=0;
}
    
