#if defined(SDCARD)
void writeHeader() {
#ifdef LOGGING_FULL
  file.print(F("INJ_OBD;INJ_HW;IGN;IAC;RPM;MAP;ECT;TPS;SPD;OXSENS;ASE;COLD;DET;OXf;Re-ENRICHMENT;STARTER;IDLE;A/C;NEUTRAL;Ex1;Ex2;AVG SPD;LPK_OBD;LPH_OBD;LPH_INJ;TOTAL_OBD;TOTAL_INJ;AVG_OBD;AVG_INJ"));
#else
  file.print(F("INJ_OBD;INJ_HW;IGN;IAC;RPM;MAP;ECT;TPS;SPD;O2SENS;EXHAUST;AVG SPD;LPH_INJ;TOTAL_INJ;AVG_INJ;CURR_INJ;CURR_RUN;TOTAL_RUN;"));
#endif
  file.println();
  if (!file.sync() || file.getWriteError())  error("write error");
}
void logData() {
  file.print(getOBDdata(OBD_INJ)); file.write(';'); file.print(float(INJ_TIME) / 1000); file.write(';'); file.print(getOBDdata(OBD_IGN));  file.write(';');  file.print(getOBDdata(OBD_IAC));  file.write(';');
  file.print(getOBDdata(OBD_RPM)); file.write(';'); file.print(getOBDdata(OBD_MAP));  file.write(';'); file.print(getOBDdata(OBD_ECT));  file.write(';');
  file.print(getOBDdata(OBD_TPS)); file.write(';');  file.print(getOBDdata(OBD_SPD));  file.write(';'); file.print(getOBDdata(OBD_OXSENS)); file.write(';'); file.print(getOBDdata(20)); file.write(';');
#ifdef LOGGING_FULL
  file.print(getOBDdata(11)); file.write(';'); file.print(getOBDdata(12)); file.write(';'); file.print(getOBDdata(13)); file.write(';'); file.print(getOBDdata(14)); file.write(';');
  file.print(getOBDdata(15)); file.write(';'); file.print(getOBDdata(16)); file.write(';'); file.print(getOBDdata(17)); file.write(';'); file.print(getOBDdata(18)); file.write(';');
  file.print(getOBDdata(19)); file.write(';'); file.print(getOBDdata(20)); file.write(';'); file.print(getOBDdata(21)); file.write(';');
#endif
  file.print(total_avg_speed); file.write(';');                                                                   //AVG_SPD       ok
  file.print(float(INJ_TIME) / 1000 * getOBDdata(OBD_RPM)*Ls * 0.18); file.write(';');                          //LPH_INJ     ok
  file.print(total_consumption_inj); file.write(';'); //TOTAL_INJ   ok
  file.print(avg_consumption_inj);  file.write(';');     //!AVG_INJ
  file.print(current_consumption_inj);  file.write(';'); //!CURR_INJ

  file.print(current_run);   file.write(';');    //CURR_RUN ok
  file.print(total_run); file.write(';');//RUN_TOTAL      ok

  file.println();
  if (!file.sync() || file.getWriteError())  error("write error");
}

void SDinit(){
  if (!sd.begin(SS, SD_SCK_MHZ(50))) {
    sd.initErrorHalt();
  }
  while (sd.exists(fileName)) {
    if (fileName[BASE_NAME_SIZE + 1] != '9') {
      fileName[BASE_NAME_SIZE + 1]++;
    } else if (fileName[BASE_NAME_SIZE] != '9') {
      fileName[BASE_NAME_SIZE + 1] = '0';
      fileName[BASE_NAME_SIZE]++;
    } else {
      error("Can't create file name");
    }
  }
  if (!file.open(fileName, O_CREAT | O_WRITE )) {
    error("file.open");
  }
}
#endif
