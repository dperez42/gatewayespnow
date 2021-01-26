//************************************************************
//Mandar nueva ssid y password
//#S{"Ssid":"MIWIFI_2G_RKkD","Password":"C6PrnauD"}
//#S{"Ssid":"iPhone de Paula","Password":"alicia88"}
//#S{"Ssid":"iPhone de DANIEL","Password":"paswordoo"}
//#S{"Ssid":"Anto´s Iphone","Password":"rodri03richi05"}
//#S{"Ssid":"HUAWEI","Password":"password"}
//Mandar nueva configuración:
// t1: Máximo tiempo de servicio, t2: Tiempo de desinfección
// l1: Limite mínimo de O3, l2: Concentración de O3 para desinfección
//#C{"l1":"50","l2":"400", "t1":"120", "t2":"10"}
//Inicio de servicio:
//#J{}
//************************************************************


#include "pinDEF.h"
#include "varDEF.h"
#include "logs.h"
#include <U8x8lib.h>        //Biblioteca para Pantalla
#include "ArduinoJson.h"     //Biblioteca para Json
#include "BluetoothSerial.h" //Biblioteca para Bluetooth
#include "actionLIB.h"
#include "startWIFI.h"
#include "setupESPNOW.h"
#include "setupHTTP.h"
#include "auxLIB.h"
#include "eepromLIB.h"

void showUp();


////// BluetoothSerial /////////
BluetoothSerial SerialBT;
String msgBT;

////// Display ////////////////
#ifdef DISPLAY_OK
U8X8_SSD1306_128X64_NONAME_HW_I2C display(/*rst*/ 16, /*scl*/ 15, /*sda*/ 4);
#endif

//----------------------------------------------------------------
//new wifi from app
//----------------------------------------------------------------
void newSsid(String msg){
  char JSONssid[100];
  const char *ssidConf;
  const char *passConf;
  int i = 2;
  while (i < msg.length()){
    JSONssid[i-2] = msg[i];
    i++;
  }
  JSONssid[i-2] = '\0';
  Serial.println("One wifi recieved.");
  StaticJsonDocument<200> JSONBuffer;
  DeserializationError error = deserializeJson(JSONBuffer, JSONssid);
  if (error) {   //Check for errors in parsing
    Serial.println("Parsing failed");
    //delay(100);
   } else {
      ssidConf = JSONBuffer["Ssid"];
      passConf = JSONBuffer["Password"]; 
      Serial.println(ssidConf);
      Serial.println(passConf);
      saveToEeprom(0,ssidConf);
      saveToEeprom(50,passConf);
      loadFromEeprom(0).toCharArray(ssid, 50);
      loadFromEeprom(50).toCharArray(password, 50);
      Serial.println ("New SSID save");
      Serial.print("ssdi:");
      Serial.println (ssid);
      Serial.print("password:");
      Serial.println (password);
      ESP.restart();
      return;  
      }
}

//--------------------------------------------------------------------
// Led State O3
//--------------------------------------------------------------------
void ledsState(){
  int duration = 500;
  if ((millis() - lastTLedState) > duration) {
    if (ledStateOn == true) {
      if (myDataSensor.o3 >= l1i) {
        ledcWrite(2 , 0);
        ledcWrite(1 , 0);} 
        else {
          ledcWrite(2 , 0);
          ledcWrite(1 , 0);
          }
      ledStateOn = false;
    } 
    else {
      if (myDataSensor.o3 < l1i) {
        ledcWrite(2 , 1500);} 
        else {
          ledcWrite(1 , 1500);
          }      
        ledStateOn = true;
    }
    lastTLedState = millis();
  }
  return;
} 

//--------------------------------------------------------------------
// Led State Service
//--------------------------------------------------------------------
void ledsService(){
    if (serviceState == 0) {
        ledcWrite(0 , 0);
    } 
    else {
        ledcWrite(0 , 1500);
    }
  return;
}

//----------------------------------------------------------------
//New config from app
//----------------------------------------------------------------
void newConfig(String msg){
  char JSONconfig[100];
  const char *l1Conf;
  const char *l2Conf;
  const char *t1Conf;
  const char *t2Conf;
  int i = 2;
  while (i < msg.length()){
    JSONconfig[i-2] = msg[i];
    i++;
  }
  JSONconfig[i-2] = '\0';
  Serial.println("New config recieved.");
  StaticJsonDocument<200> JSONBuffer;
  DeserializationError error = deserializeJson(JSONBuffer, JSONconfig);
  if (error) {   //Check for errors in parsing
    Serial.println("Parsing failed");
    //delay(100);
   } else {
      l1Conf = JSONBuffer["l1"]; //limite O3 ozono = 50 ppb
      l2Conf = JSONBuffer["l2"]; //limite O3 ozono desinfeccion = 400 ppb
      t1Conf = JSONBuffer["t1"]; //tiempo de tratamiento en minutos = 10 mins
      t2Conf = JSONBuffer["t2"]; //tiempo de tratamiento en minutos = 10 mins
      Serial.println ("New Config recieve:");
      Serial.print("Concentración O3 en ppb no saludable:");
      Serial.println (l1Conf);
      Serial.print("Concentratción O3 en ppb para desinfección:");
      Serial.println (l2Conf);
      Serial.print("Tiempo limite en minutos para servicio:");
      Serial.println (t1Conf);
      Serial.print("Tiempo en minutos para desinfección:");
      Serial.println (t2Conf);
      //Salvo en memoria )0:ssid, 50:password, 100:l1, 150;l2, 200:t2
      saveToEeprom(100,l1Conf);
      saveToEeprom(150,l2Conf);
      saveToEeprom(200,t1Conf);
      saveToEeprom(250,t2Conf);
      //Grabo de memoria
      loadFromEeprom(100).toCharArray(l1, 50);
      loadFromEeprom(150).toCharArray(l2, 50);
      loadFromEeprom(200).toCharArray(t1, 50);
      loadFromEeprom(250).toCharArray(t2, 50);
      Serial.println ("New Config save");
      Serial.print("Concentración O3 en ppb no saludable:");
      Serial.println (l1);
      Serial.print("Concentración O3 en ppb para desinfección:");
      Serial.println (l2);
      Serial.print("Tiempo máximo en servicio:");
      Serial.println (t1);
      Serial.print("Tiempo en minutod para desinfección:");
      Serial.println (t2);
      l1i = atoi(l1);
      l2i = atoi(l2);
      t1i = atoi(t1);
      t2i = atoi(t2);  
    display.setCursor(5,3);
    display.print("  "); 
    display.setCursor(13,3);
    display.print("   ");        
    display.setCursor(5,4);
    display.print("   ");
    display.setCursor(14,4);
    display.print("  ");  
      return;  
      }
}

void setup() {
  // put your setup code here, to run once:
  
  // Init Serial Monitor
  Serial.begin(115200);

  /// RELAY setup
  digitalWrite(PIN_RELAY, LOW);
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_BUTTON, INPUT);
  
  ///////LEDS Y BUZZER INIT////
  // LEDS ledcWrite(channel , volume) ledcSetup(PWM channel(0 to 15), PWM frequency(5000 to 8000), resolution (1 to 16)) ledcAttachPin(pin, channel); 
  /*
  ledcSetup(0,5000,12);
  ledcAttachPin(PIN_SVC,0);
  ledcSetup(1,5000,12);
  ledcAttachPin(PIN_RED,1);
  ledcSetup(2,500,12);
  ledcAttachPin(PIN_GREEN,2);
  */
  
  GATE_ID = getIdEsp32();

  initEeprom();

    //DISPLAY setup
  display.begin();
  display.setPowerSave(0);
  display.setFont(u8x8_font_chroma48medium8_r);
  display.refreshDisplay();
  display.setCursor(0,0);
  display.print(GATE_ID);
  Serial.println("Init Display");
  
  // Set device as a Wi-Fi Station
  connectSsdi();
  
  // BLUETOOTH setup
  SerialBT.begin("OzoneView " + String(GATE_ID)); //Bluetooth gateway name
  //Serial.println("Bluetooth iniciado con nombre OzoneView " + GATE_ID);
 
  //inicio los timers
  lastTimeRecieve = millis();
  lastTimeSender = millis();
}

void loop() {
     //check wifi connect
   //getTimeByNTP();
   if (WiFi.status() != WL_CONNECTED){
    Serial.println("try to connect to external wifi");
    isConnectToWifi == false;
    connectSsdi();
  }
  
  //Leds y reset button
  //ledsState();
  //ledsService();
  
  if (digitalRead(PIN_BUTTON)){reset();}

  //lee el serial o BT
  display.setCursor(0,2);
  display.print("                ");
  msgBT = "";
  while (Serial.available()){
    if (Serial.available()>0){
      char c = Serial.read();
      msgBT += c;
      }
      String mSR = "SR:" + String(msgBT.c_str());
           Serial.println(mSR);
     display.setCursor(0,2);
     display.print(mSR);
    }
    
     while (SerialBT.available()){
    if (SerialBT.available()>0){
      char c = SerialBT.read();
      msgBT += c;
      }
      String mBT = "BT:" + String(msgBT.c_str());
      display.setCursor(0,2);
      display.print(mBT);
  }
  


    if (msgBT[0]=='#'){       //todos los mensajes recibidos por BT empiezan #
      
      if (msgBT[1]=='S'){     //Nueva SSDI y PASSWORD
        isConnectToWifi == false;
        newSsid(msgBT);
      }
      if (msgBT[1]=='C'){     //Nuevos CONFIG límites 1 y 2
        newConfig(msgBT);     // Save new config
      }
      
      if (msgBT[1]=='J'){     //Inicio Servicio
       if (serviceState == 0) {
          initService();
          } else { 
          stopService();
          }
      }
    }

   
    // Send message via ESP-NOW
    if (millis() - last > 1000) {     //&& (isConnect == true )
        myDataGateway.l1 = l1i;
        myDataGateway.l2 = l2i;
        // Send message via ESP-NOW
        esp_err_t result = esp_now_send(ESP_NOW_RECEIVER, (uint8_t *) &myDataGateway, sizeof(myDataGateway));         
        if (result == ESP_OK) {
          if (DEBUG){
              Serial.println("###########################################");
              Serial.println("sent");
              Serial.print("Limite inferior(l1): ");
              Serial.print(myDataGateway.l1);
              Serial.print(", Limite desinfección(l2): ");
              Serial.println(myDataGateway.l2);
              Serial.printf("sent: %3u on channel: %u\n", myDataGateway, WiFi.channel());
              Serial.println("Sent with success");
              Serial.println("###########################################");
          }
     } else {
        if (DEBUG){
           Serial.println("###########################################");
           Serial.println("Error sending the data");
           Serial.println("###########################################");
        }
     }
     last = millis();
    }
    
    
 //Control de ventilador si O3 > max permitido o está en servicio esta encendio y sensor on line
    if ((myDataSensor.o3 >= l1i || serviceState>0) && ((millis()-timeLastRcv) < limitLastRcv)){
      ventilatorState = 1;
      initVentilator();
    } else {
      ventilatorState = 0;
      stopVentilator();
    }
    
    //Check service state
    if (serviceState > 0){
    timeSvc = millis()-timeStartSvc;
    if (myDataSensor.o3 > l2i){
      if (desinfectionState == 0){
      timeStartDes = millis();
      desinfectionState = 1;
      } else {
          timeDes = millis() - timeStartDes;
          if (timeDes/(1000*60) >= t2i){
            desinfectionState = 0;
            timeDes = 0;
            display.setCursor(6,7);
            display.print("  ");
            stopService();
          }
      }
    } else {
      desinfectionState = 0;
      timeDes = 0;
    }

    //Si se ha superado el tiempo de servicio max: parada por seguridad o no hay sensor
    if (timeSvc/(60*1000) >= t1i || (millis()-timeLastRcv) > limitLastRcv){        
      stopService();
    }
    }
    
    showUp();
    
    //Check sensor is connected
    display.setCursor(9,0);
    display.print("       ");
    if ( (millis()-timeLastRcv) > limitLastRcv){
          display.setCursor(9,0);
          display.print("SEN OFF");
          Serial.println("SEN OFF");
    }
        
   //Check for orders
   if ( ( millis() - lastTimeRecieve > timerRecieve) && ( isConnectToWifi == true ))
   {
    listenHTTP();
    lastTimeRecieve=millis();
   }
   
   //Send data
   if ( ( millis() - lastTimeSender > timerSender) && ( isConnectToWifi == true ) && newData == true)
    {
    sendHTTP();
    newData = false;
    lastTimeSender=millis();
    }
    
}

void showUp(){
    display.setPowerSave(false); 
      if (isConnectToWifi == true){
    display.setCursor(12,7);
    display.setInverseFont(1);
    display.print("WIFI"); 
    display.setInverseFont(0);
    } else {
    display.setCursor(12,7);
    display.print("WIFI"); 
    }
    if ( serviceState>0 ){
    display.setCursor(0,7);
    display.setInverseFont(1);
    display.print("SVC"); 
     display.setInverseFont(0);
    display.setCursor(4,7);
    display.print(timeSvc/(60*1000));
    if  (desinfectionState == 1){
    display.setCursor(6,7);
    display.setInverseFont(1);
    display.print(timeDes/(60*1000));
     display.setInverseFont(0);
    } else { 
    display.setCursor(6,7);
    display.print("   "); 
    }   
    } else {
    display.setCursor(0,7);
    display.setInverseFont(0);
    display.print("SVC"); 
    display.setCursor(4,7);
    display.print("   "); 
    display.setCursor(6,7);
    display.print("   "); 
    }
    if  (desinfectionState == 1){
    display.setCursor(6,7);
    display.setInverseFont(1);
    display.print(timeDes/(60*1000));
     display.setInverseFont(0);
    } else { 
    display.setCursor(6,7);
    display.print("   "); 
    }
    if (ventilatorState==1){
      display.setCursor(8,7);
      display.setInverseFont(1);
      display.print("VEN");
      display.setInverseFont(0);
    } else {
      display.setCursor(8,7);
      display.print("VEN");
    }
   
    display.setCursor(0,3);
    display.print("Lseg:");  
    display.setCursor(5,3);
    display.print(int(myDataGateway.l1)); 
    display.setCursor(7,3);
    display.print(",Ldes:");
    display.setCursor(13,3);
    display.print(int(myDataGateway.l2));        
    display.setCursor(0,4);
    display.print("Tmax:");
    display.setCursor(5,4);
    display.print(int(t1i));
    display.setCursor(8,4);
    display.print(",Tdes:");
    display.setCursor(14,4);
    display.print(int(t2i));
    display.setCursor(0,5);
    display.print("Tx from: ");
    display.setCursor(8,5);
    display.print((int)myDataSensor.id);
    display.setCursor(0,6);
    display.print("O3:");
    display.setCursor(3,6);
    display.print("    ");
    display.setCursor(3,6);
    display.print((int)myDataSensor.o3);
    display.setCursor(7,6);
    display.print("T:");
    display.setCursor(9,6);
    display.print("   ");
    display.setCursor(9,6);
    display.print((int)myDataSensor.temp);
    display.setCursor(12,6);
    display.print("H:");
    display.setCursor(14,6);
    display.print("  ");
    display.setCursor(14,6);
    display.print((int)myDataSensor.hum);
    
  }
