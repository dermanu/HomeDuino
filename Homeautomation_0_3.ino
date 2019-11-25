#include <SPI.h> 
#include <Ethernet.h>
#include <SD.h>
#include <TextFinder.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x94, 0x2A }; 
byte ip[]  = { 192, 168, 1, 20 };
byte sdPin = 4;
short rc_pin=8;
EthernetServer server(80);                           

File webFile;

void setup()
{  
  Ethernet.begin(mac, ip); 
  server.begin();          
  Serial.begin(9600);
  SD.begin(sdPin); 
  pinMode(rc_pin, OUTPUT);
 }

void loop() {
 
  EthernetClient client = server.available();
  
    //////////
    //SENDEN//
    //////////
 if(client){
  TextFinder finder(client);
    if(finder.find("GET")){
      while(finder.findUntil("pin", "\n\r")){
       char typ = client.read();
       
       if(typ == 'A'){
        sendCode("111010111101");
        
       }
       else if(typ == 'B'){
        sendCode("111010111110");
       }
       
       else if(typ == 'C'){
        sendCode("111011011101");
       }
       else if(typ == 'D'){
        sendCode("111011011110");
       }
       else if(typ == 'E'){
        sendCode("111100111101");
       }
       else if(typ == 'F'){
        sendCode("111100111110");
       }
       else if(typ == 'G'){
        sendCode("111101011101");
       }
       else if(typ == 'H'){
        sendCode("111101011110");
       }
       else if(typ == 'I'){
        sendCode("111101101101");
       }
       else if(typ == 'J'){
        sendCode("111101101110");
       }
       else if(typ == 'K'){
        sendCode("111101110101");
       }
       else if(typ == 'L'){
        sendCode("111101110110");
       }
       
      } 
    }
 }
    /////////////
    //WEBSERVER//
    /////////////
    
    boolean current_line_is_blank = true;       
    while (client.connected()) 
    {
      if (client.available())
      {
        char c = client.read();
        if (c == '\n' && current_line_is_blank)
        {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();

         
          webFile = SD.open("index.htm");
          if (webFile)
          {
            while(webFile.available())
            {
              client.write(webFile.read());
            }
            webFile.close();
          }
          break;
        }
        if (c == '\n') 
        {
          current_line_is_blank = true;
        } 
        else if (c != '\r') 
        {
          current_line_is_blank = false;
        }
      }
    }
    delay(1);
    client.stop();
    }
    
  ////////////
  //Funkcode//
  ////////////
  
  boolean sendCode(char code[]){
  for(short z = 0; z<9; z++){ 
    for(short i = 0; i<12; i++){
      sendByte(code[i]);
    } 
    sendByte('x');
  }
  return true;
}
void sendByte(char i) { 
  switch(i){ 
  case '1':{
    digitalWrite(rc_pin,HIGH);
    wait(1); 
    digitalWrite(rc_pin,LOW);
    wait(3);
    digitalWrite(rc_pin,HIGH);
    wait(3);
    digitalWrite(rc_pin,LOW);
    wait(1);
    return;
  }
  case '0':{ 
    digitalWrite(rc_pin,HIGH);
    wait(1);
    digitalWrite(rc_pin,LOW);
    wait(3);
    digitalWrite(rc_pin,HIGH);
    wait(1);
    digitalWrite(rc_pin,LOW);
    wait(3);
    return;
  }
  case 'x':{
    digitalWrite(rc_pin,HIGH);
    wait(1);
    digitalWrite(rc_pin,LOW);
    wait(31);
  }
 
}
}
 
void wait(int x) {
  delayMicroseconds(x*320);
}
