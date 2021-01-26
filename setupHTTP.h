#include <HTTPClient.h> //Biblioteca para HTTP

//----------------------------------------------------------------
//send HTTP to get orders
//----------------------------------------------------------------

void listenHTTP(){
    String Message;
    Message = serverGetOrder;
    delay(15);
    HTTPClient http;
    http.setTimeout(6000);
    WiFiClient cli;
    http.begin(cli,Message.c_str());
    int httpRespondeCode = http.GET();
    delay(15);
    if (DEBUG)
    {
      Serial.println(Message);
    }
    if (httpRespondeCode > 0){
      if (DEBUG){
        Serial.print("HTTP Response code: ");
        Serial.println(httpRespondeCode);
      }
      String payload = http.getString();
      if (DEBUG){
      Serial.println(payload);
      }
      
      if (payload[0]=='#'){       //todos los mensajes recibidos empiezan por #
         if (payload[1]=='J'){     //Inicio Servicio
           if (serviceState == 0) {
              initService();
              } else { 
              stopService();
              }
          }
      }
                
    }
    else {
      if (DEBUG){
        Serial.print("Error code: ");
        Serial.println(httpRespondeCode);
      }
     }
    http.end(); 
}

//----------------------------------------------------------------
//send HTTP
//----------------------------------------------------------------

void sendHTTP(){
      //{"time":"2020-10-02T10-8-38Z","id_node":"00000001","O3":"1962.00","Temp":"22.50","Hum":"53.20","Bat":"685.00"}
    //msg = msg.substring(1,msg.length()-1);
    //String Message = serverName + "?json={\"time\":\""+ getInitTimeByNTP() + "\"," + msg + "}";     
    //String Message = serverName + "?json={\"time\":\""+ getInitTimeByNTP() + "\",\"id_node\":\"" + String(GATE_ID) + "\",\"O3\":\"" + String(myDataSensor.o3) + "\",\"Temp\":\"" + String(myDataSensor.temp) + "\",\"Hum\":\"" + String(myDataSensor.hum)+"\"}"; 
    String Message = serverName + "?json={\"time\":\""+ getTimeByNTP() + "\",\"id_gw\":\"" + "00000001" + "\",\"id_node\":\"" + "00000001" + "\",\"O3\":\"" + String(myDataSensor.o3) + "\",\"Temp\":\"" + String(myDataSensor.temp) + "\",\"Hum\":\"" + String(myDataSensor.hum)+ "\",\"Bat\":\"" + String(myDataSensor.bat)+"\"}"; 
    if (DEBUG){
      Serial.println(Message);
    }
    delay(5);
    HTTPClient http;
    http.begin(Message.c_str());
    int httpRespondeCode = http.GET();
    if (httpRespondeCode > 0){
      if (DEBUG){
        Serial.print("HTTP Response code: ");
        Serial.println(httpRespondeCode);
      }
      String payload = http.getString();
      if (DEBUG){
        Serial.println(payload);      
      }
    }
    else {
      if (DEBUG){
        Serial.print("Error code: ");
        Serial.println(httpRespondeCode);
      }
     }
    http.end(); 
}
