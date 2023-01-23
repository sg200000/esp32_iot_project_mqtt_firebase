#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <MaxMatrix.h>

int DIN = 18;
int CS = 19;
int CLK = 21;
int x = 3;
int y = 3;

char * top = "/matrice1";

MaxMatrix matrice(DIN, CS, CLK, 1);

int x = 3;
int y = 3;


const char* SSID = "Unknown";
const char* PWD = "GUOUIHAJ_family@2019";
WiFiClient espClient;
PubSubClient client(espClient);

void setup(){
    Serial.begin(9600);
    WiFi.begin(SSID, PWD);
    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }
    client.setServer("broker.hivemq.com", 1883);
    matrice.init();
    matrice.setIntensity(5); // 0-15
    matrice.setDot(x,y,true);
}
void callback(char* topic, byte* payload, unsigned int length) {
    
   
   if(payload[0]=='d') x++;
   if(payload[0]=='g') x--;
   if(payload[0]=='b') y++;
   if(payload[0]=='h') y--;
   if(x>7) x=0;
   if(x<0) x=7;
   if(y>7) y=0;
   if(y<0) y=7;
   matrice.clear();
   matrice.setDot(x,y,true);

}
void loop(){
  if (!client.connected()) {
    if (client.connect("ouss-said-esp")) {
      client.subscribe(top);
      client.setCallback(callback);
    } 
    
  }
  client.loop();
  delay(1);
 }
