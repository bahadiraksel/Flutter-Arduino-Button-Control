#include <Arduino.h>
#include <ESP8266WiFi.h> //import for wifi functionality
#include <WebSocketsServer.h> //import for websocket

#define ledpin D2 //defining the OUTPUT pin for LED
#define ledpin2 D3

const char *ssid =  "HelloHPC - Wifi";   //Wifi SSID (Name)   
const char *pass =  "12345678"; //wifi password

WebSocketsServer webSocket = WebSocketsServer(81); //websocket init with port 81

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
//webscket event method
    String cmd = "";
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.println("Websocket is disconnected");
            //case when Websocket is disconnected
            break;
        case WStype_CONNECTED:{
            //wcase when websocket is connected
            Serial.println("Websocket is connected");
            Serial.println(webSocket.remoteIP(num).toString());
            webSocket.sendTXT(num, "connected");}
            Serial.println(num);
            break;
        case WStype_TEXT:
            cmd = "";
            for(int i = 0; i < length; i++) {
                cmd = cmd + (char) payload[i]; 
            } //merging payload to single string
            Serial.println(cmd);

            if(cmd == "poweron"){ //when command from app is "poweron"
                digitalWrite(ledpin, HIGH);   //make ledpin output to HIGH  
            }else if(cmd == "poweroff"){
                digitalWrite(ledpin, LOW);    //make ledpin output to LOW on 'pweroff' command.
            }
            else if(cmd == "butonac"){
              digitalWrite(ledpin2, HIGH);
            }
             else if(cmd == "butonkapa"){
              digitalWrite(ledpin2, LOW);
            }

            // webSocket.sendTXT(num, cmd+":success");
            //webSocket.sendTXT(num,":success");
            webSocket.sendTXT(num,cmd= cmd+":success");

          /*  if(cmd == "butonac"){ //when command from app is "poweron"
                digitalWrite(ledpin2, HIGH);   //make ledpin output to HIGH  
            }else if(cmd == "butonkapa"){
                digitalWrite(ledpin2, LOW);    //make ledpin output to LOW on 'pweroff' command.
            }

            // webSocket.sendTXT(num, cmd+":success");
            //webSocket.sendTXT(num,":success");
            webSocket.sendTXT(num,cmd= cmd+":success");*/
            Serial.println(num);
             //send response to mobile, if command is "poweron" then response will be "poweron:success"
             //this response can be used to track down the success of command in mobile app.
            break;
        case WStype_FRAGMENT_TEXT_START:
            break;
        case WStype_FRAGMENT_BIN_START:
            break;
        case WStype_BIN:
            hexdump(payload, length);
            break;
        default:
            break;
    }
}

void setup() {
   pinMode(ledpin, OUTPUT); //set ledpin (D2) as OUTPUT pin
   pinMode(ledpin2, OUTPUT);
   Serial.begin(9600); //serial start

   Serial.println("Connecting to wifi");
   
   IPAddress apIP(192, 168, 0, 1);   //Static IP for wifi gateway
   WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0)); //set Static IP gateway on NodeMCU
   WiFi.softAP(ssid, pass); //turn on WIFI

   webSocket.begin(); //websocket Begin
   webSocket.onEvent(webSocketEvent); //set Event for websocket
   Serial.println("Websocket is started");
}

void loop() {
   webSocket.loop(); //keep this line on loop method
}
