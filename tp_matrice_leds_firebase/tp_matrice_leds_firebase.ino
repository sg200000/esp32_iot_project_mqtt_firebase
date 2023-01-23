#include <Arduino.h>
#include <WiFi.h>
#include <MaxMatrix.h>
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert Firebase project API Key
#define API_KEY "AIzaSyAjmm45wJdxTn93o61t5chjhFUb4CURFi0"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "iot-matrix-controller-default-rtdb.firebaseio.com"

const char *SSID = "Unknown";
const char *PWD = "GUOUIHAJ_family@2019";

// MQTT client
WiFiClient wifiClient;

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

char * top = "/matrice1";

int DIN = 18;
int CS = 19;
int CLK = 21;

MaxMatrix matrice(DIN, CS, CLK, 1);

int x = 3;
int y = 3;

void setup() {
  // put your setup code here, to run once:
  matrice.init();
  matrice.setIntensity(5);
  Serial.begin(9600);
  matrice.setDot(x, y, true);
  connectToWiFi();
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.RTDB.getBool(&fbdo, "isChanged")) {
    if (fbdo.to<bool>()){
      if (Firebase.RTDB.getString(&fbdo, "direction")) { 
        control(fbdo.to<String>());
        Firebase.RTDB.setBool(&fbdo, "isChanged", false);
      }
    }
  } else {
    Serial.println(fbdo.errorReason());
  }
}

void connectToWiFi() {
  Serial.print("Connectiog to ");
 
  WiFi.begin(SSID, PWD);
  Serial.println(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected."); 
}

void control(String command){
    char c = command[0];
    Serial.println(c);
      if (c=='d') x++;
      if (c=='g') x--;
      if (c=='b') y++;
      if (c=='h') y--;
      if (x>7) x=0;
      if (x<0) x=7;
      if (y>7) y=0;
      if (y<0) y=7;
      matrice.clear();
      matrice.setDot(x,y,true);
    
}
      
  
