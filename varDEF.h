#include <WiFi.h>
#include <esp_pm.h>
#include <esp_wifi.h>
#include <esp_wifi_types.h>
#include "setTIME.h"
#include "setupESPNOW.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_pm.h"
#include "nvs_flash.h"









#define DEBUG 0
#define LOG 1


int GATE_ID;

char l1[25]; //concentracion de ozono peligrosa en ppb (50)
char l2[25]; //concentracion de ozono en desinfección en ppb (400)
char t1[25]; //tiempo maximo de servicio en min (seguridad)
char t2[25]; //tiempo maximo de desinfección en min
int l1i = 50;
int l2i = 100;
int t1i = 10;
int t2i = 50;

int serviceState = 0; //0: standby 1:Service
int ventilatorState = 0; //0: off 1:on
int desinfectionState = 0;
unsigned long timeStartSvc;         //tiempo en millis de incio de servicio
unsigned long timeSvc;              //tiempo acumulado de servicio
unsigned long timeStartDes;         //tiempo en millis de incio desinfección
unsigned long timeDes;              //tiempo acumulado de desinfeccion


// HTTP SERVERs
String serverName = "http://www.glidingbytes.com/Ozono/guarda_dato";
String serverGetOrder = "http://www.glidingbytes.com/Ozono/setEstado?json={\"id\":\"1\",\"id_state\":\"5\",\"t_svc\":\"120\",\"t_des\":\"15\"}";

unsigned long timerRecieve = 2000;  // Set time to get order from server to 2 seconds 
unsigned long lastTimeRecieve;
unsigned long timerSender = 15000;  // Set time to sent data to server to 15 seconds 
unsigned long lastTimeSender;



//#S{"Ssid":"MIWIFI_2G_RKkD","Password":"C6PrnauD"}
//#S{"Ssid":"iPhone de Paula","Password":"alicia88"}
//#S{"Ssid":"iPhone de DANIEL","Password":"paswordoo"}
//#S{"Ssid":"Anto´s Iphone","Password":"rodri03richi05"}
//#S{"Ssid":"HUAWEI","Password":"password"}
boolean isConnectToWifi = false;
//char* ssid = "MIWIFI_2G_RKkD";
//char* password = "C6PrnauD";
char ssid[50];      
char password[50];

boolean newData = false;

unsigned long timeLastRcv;          //tiempo desde el ultimo dato recibido
unsigned long limitLastRcv = 60000;  //tiempo limite sin recibir datos del sensor 1 minuto
uint32_t last;

unsigned long lastTLedState;
boolean ledStateOn = false;
