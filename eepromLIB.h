#include <EEPROM.h>          //Biblioteca para grabar en EEPROM

//----------------------------------------------------------------
//Save to EEPROM
//----------------------------------------------------------------
void saveToEeprom(int addr, String a) {
  int tamano = a.length(); 
  char inchar[50]; 
  a.toCharArray(inchar, tamano+1);
  for (int i = 0; i < tamano; i++) {
    EEPROM.write(addr+i, inchar[i]);
  }
  for (int i = tamano; i < 50; i++) {
    EEPROM.write(addr+i, 255);
  }
  EEPROM.commit();
}

//----------------------------------------------------------------
//Load from EEPROM
//----------------------------------------------------------------
String loadFromEeprom(int addr) {
   byte lectura;
   String strlectura;
   for (int i = addr; i < addr+50; i++) {
      lectura = EEPROM.read(i);
      if (lectura != 255) {
        strlectura += (char)lectura;
      }
   }
   return strlectura;
}

///
void initEeprom(){
  EEPROM.begin(500); //0: ssid, 50: password, 100:limitDownO3c[50], 150:limitUpO3c[50], 200:sampleRatec[50], 250:key[50]; 300:concSvcc[50]; 350:timeSvcc[50];
  loadFromEeprom(0).toCharArray(ssid, 50);
  loadFromEeprom(50).toCharArray(password, 50);
  Serial.println("Retrieve from EEPROM");
  Serial.print("SSID:");
  Serial.println(ssid);
  Serial.print("PASSWORD:");
  Serial.println(password);
  loadFromEeprom(100).toCharArray(l1, 50);
  loadFromEeprom(150).toCharArray(l2, 50);
  loadFromEeprom(200).toCharArray(t1, 50);
  loadFromEeprom(250).toCharArray(t2, 50);
  l1i = atoi(l1);
  l2i = atoi(l2);
  t1i = atoi(t1);
  t2i = atoi(t2);
  Serial.print("Concentración O3 en ppb no saludable:");
  Serial.println (l1);
  Serial.print("Concentratción O3 en ppb para desinfección:");
  Serial.println (l2);
  Serial.print("Tiempo máximo en servicio:");
  Serial.println (t1);
  Serial.print("Tiempo en minutod para desinfección:");
  Serial.println (t2);
}
