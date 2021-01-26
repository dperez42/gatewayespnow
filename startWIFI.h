//Mandar nueva ssid y password
//#S{"Ssid":"MIWIFI_2G_RKkD","Password":"C6PrnauD"}
//#S{"Ssid":"iPhone de Paula","Password":"alicia88"}
//#S{"Ssid":"iPhone de DANIEL","Password":"paswordoo"}
//#S{"Ssid":"Anto´s Iphone","Password":"rodri03richi05"}
//#S{"Ssid":"HUAWEI","Password":"password"}



void OnWiFiEvent(WiFiEvent_t event)
{
  switch (event) {
 
    case SYSTEM_EVENT_STA_CONNECTED:
      Serial.println("ESP32 Connected to WiFi Network");
      break;
    case SYSTEM_EVENT_AP_START:
      Serial.println("ESP32 soft AP started");
      break;
    case SYSTEM_EVENT_AP_STACONNECTED:
      Serial.println("Station connected to ESP32 soft AP");
      break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
      Serial.println("Station disconnected from ESP32 soft AP");
      break;
    default: break;
  }
}

//--------------------------------------------------------------------
// Connect Wifi
//--------------------------------------------------------------------
void connectSsdi(){
  WiFi.onEvent(OnWiFiEvent);
// Set device as a Wi-Fi Station
    WiFi.mode(WIFI_MODE_APSTA); 
    //WiFi.setSleep(false); //disable sleep mode
    esp_wifi_set_ps(WIFI_PS_NONE);
    WiFi.persistent(false);
    WiFi.disconnect(true);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to ...");
    Serial.print(ssid);
     Serial.print("..");
    Serial.println(password);
    delay(5000); //5
  if (WiFi.status() == WL_CONNECTED){

    Serial.print("Station IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Wi-Fi Channel: ");
    Serial.println(WiFi.channel());
    isConnectToWifi= true;
    if (LOG) {ft_log(1);}
    Serial.println(" Setting as a Wi-Fi AP_STA..");
      
    // Initialize a NTPClient to get time
    //initTimeClient();
   // if(init_ESPNOW())
   //    return;
  }
  if(init_ESPNOW())
       return;
  if (isConnectToWifi == false){
      if (LOG) {ft_log(2);}
     //WiFi.disconnect();
     //WiFi.mode(WIFI_AP);  //wifi interna para comunicar con el sensor
     //Serial.println(" Setting as a Wi-Fi STA..");
  }
  return;
  }
