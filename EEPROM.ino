void ReadEEPROM(){
  EEPROM.get(104, total_run);
  EEPROM.get(108, total_time);
  EEPROM.get(200, total_inj_dur);
  EEPROM.get(204, trip_inj_dur);
  // EEPROM.get(208, total_closed_duration);
 }

void SaveEEPROM(){
    EEPROM.put(104, total_run);
    EEPROM.put(108, total_time);
    EEPROM.put(200, total_inj_dur);
    EEPROM.put(204, trip_inj_dur);
    //  EEPROM.put(208, total_closed_duration);
}

void cleardata() {
  int i;
  for (i = 104; i <= 112; i++) {
    EEPROM.update(i, 0);
  }
  for (i = 200; i <= 212; i++) {
    EEPROM.update(i, 0);
  }
  ReadEEPROM();

}
