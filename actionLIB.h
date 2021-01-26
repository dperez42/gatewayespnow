
//--------------------------------------------------------------------
// Init Service
//--------------------------------------------------------------------
void initService(){
  Serial.println("Inicializando Servicio + Rele");
  digitalWrite(PIN_RELAY,HIGH);
  serviceState = 1;
  timeStartSvc = millis();
  return;
}

//--------------------------------------------------------------------
// Stop Service
//--------------------------------------------------------------------
void stopService(){
     Serial.println("Parando Servicio + Rele");
     digitalWrite(PIN_RELAY,LOW);
     serviceState = 0;
     return;
}

//--------------------------------------------------------------------
// Init Ventilator
//--------------------------------------------------------------------
void initVentilator(){
  digitalWrite(PIN_VENT,HIGH);
  ventilatorState = 1;
  return;
}

//--------------------------------------------------------------------
// Stop Ventilator
//--------------------------------------------------------------------
void stopVentilator(){
     digitalWrite(PIN_VENT,LOW);
     ventilatorState = 0;
     return;
}
