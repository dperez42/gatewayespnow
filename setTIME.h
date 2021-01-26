#include <NTPClient.h>  //Biblioteca para Tiempo NTP


// froma actual time
//const char* ntpServer = "pool.ntp.org";
//const long  gmtOffset_sec = 3600;
//const int   daylightOffset_sec = 3600;

  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org",3600, 160000); //+3600 Madrid, 60s to check

void initTimeClient(){
    timeClient.begin();
    Serial.println("NTPClient iniciado.");
}
//----------------------------------------------------------------
//get NTP Time
//----------------------------------------------------------------

String getTimeByNTP(){
  /*
  Serial.println("Entroe");
  struct tm timeinfo;
  // char *buf[40];
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return ;
  }
  Serial.println("Salgo");
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  //2020-10-02T10-8-38Z
  //sprintf(timeString, "%04d-%02d-%02dT%02d-%02d-%02dZ",time_info->tm_year,time_info->tm_mon,time_info->tm_mday,  time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
  //Serial.println(timeString);
  return ;
  */

  // Need to be update.
  if (!timeClient.update()) {
   Serial.print(".");
   timeClient.forceUpdate();
  } else {
    Serial.print("Unable to Update Time");     
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  String formattedDate = timeClient.getFormattedDate();
  Serial.print("Actual Date/Time:");
  Serial.println(formattedDate);
  
  //long unsigned int timestampNTP = timeClient.getEpochTime()- 2*60*60; //correción para funcion posterior
  //Serial.print("timestamp:");
  //Serial.println(timestampNTP);
  //String msg1 = "#T{\"actualTimestamp\": \"" + String(timestampNTP) + "\"}";
  //Serial.println(msg1);
  //Para ajuste de hora invierno o verano ver fecha y cambiar el timestampNTP sumando o restando una hora (3600)
  return formattedDate;
}
