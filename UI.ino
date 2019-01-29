void drawExtraData(void) {
  u8g.setFont(u8g_font_unifontr);
  u8g.firstPage();
  do {
    u8g.drawStr( 0, 17, "O2s" );
    u8g.setPrintPos(25, 17) ;
    u8g.print(int(getOBDdata(OBD_OXSENS)));

    //u8g.drawStr( 0, 32, "OX2");
    //u8g.setPrintPos(25, 32) ;
    //u8g.print( );

    u8g.drawStr( 0, 47, "SEN");
    u8g.setPrintPos(25, 47) ;
    u8g.print( int(getOBDdata(11)));

    u8g.drawStr( 0, 62, "CLD");
    u8g.setPrintPos(25, 62) ;
    u8g.print( int(getOBDdata(12)));

    u8g.drawStr( 65, 17, "DET" );
    u8g.setPrintPos(92, 17) ;
    u8g.print( int(getOBDdata(13)));

    u8g.drawStr( 65, 32, "FE1");
    u8g.setPrintPos(92, 32) ;
    u8g.print( int(getOBDdata(14)));

    u8g.drawStr( 65, 47, "FE2");
    u8g.setPrintPos(92, 47) ;
    u8g.print( int(getOBDdata(15)));

    u8g.drawStr( 65, 62, "STR");
    u8g.setPrintPos(92, 62) ;
    u8g.print( int(getOBDdata(16)));
    u8g.drawVLine(63, 0, 64);
  }
  while ( u8g.nextPage() );
}


void drawExtraFlags(void) {
  u8g.setFont(u8g_font_unifontr);
  u8g.firstPage();
  do {
    u8g.drawStr( 0, 17, "IDL" );
    u8g.setPrintPos(25, 17) ;
    u8g.print(int(getOBDdata(17)));

    u8g.drawStr( 0, 32, "CND");
    u8g.setPrintPos(25, 32) ;
    u8g.print( int(getOBDdata(18)));

    u8g.drawStr( 0, 47, "NEU");
    u8g.setPrintPos(25, 47) ;
    u8g.print( int(getOBDdata(19)));

    u8g.drawStr( 0, 62, "EN1");
    u8g.setPrintPos(25, 62) ;
    u8g.print( int(getOBDdata(20)));

    u8g.drawStr( 65, 17, "EN2" );
    u8g.setPrintPos(92, 17) ;
    u8g.print( int(getOBDdata(21)));

    u8g.drawVLine(63, 0, 64);
  }
  while ( u8g.nextPage() );
}

void DrawCurrentFuelConsuption(void) {
  u8g.setFont(u8g_font_profont15r);
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_profont15r);
    u8g.drawStr( 0, 15, "CURRENT" );
    u8g.drawStr( 106, 15, "L" );
    if (LoggingOn == true) u8g.drawStr( 119, 15, "#" );
    u8g.setPrintPos(59, 15) ;
    u8g.print(trip_obd_fuel_consumption, 1);
    if (getOBDdata(OBD_SPD) > 1)
    {
      u8g.setFont(u8g_font_profont15r);
      u8g.drawStr( 0, 42, "L/100Km" );
      u8g.setFont(u8g_font_profont22r);
      u8g.setPrintPos(0, 60) ;
      u8g.print( LPK, 1);
    } else {
      u8g.setFont(u8g_font_profont15r);
      u8g.drawStr( 0, 42, "L/Hour" );
      u8g.setFont(u8g_font_profont22r);
      u8g.setPrintPos(0, 60) ;
      u8g.print(LPH, 1);
    }
    u8g.setFont(u8g_font_profont15r);
    u8g.drawStr( 60, 42, "Average" );
    u8g.setFont(u8g_font_profont22r);
    u8g.setPrintPos(60, 60) ;
    if (trip_obd_avg_fuel_consumption < 100)
      u8g.print( trip_obd_avg_fuel_consumption, 1);
    else u8g.drawStr( 60, 60, "---" );
  }
  while ( u8g.nextPage() );
}

void DrawTotalFuelConsuption(void) {
  u8g.setFont(u8g_font_profont15r);
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_profont15r);
    u8g.drawStr( 0, 15, "TOTAL" );
    u8g.drawStr( 90, 15, "L" );
    if (LoggingOn == true) u8g.drawStr( 119, 15, "#" );
    u8g.setPrintPos(44, 15) ;
    u8g.print(total_obd_fuel_consumption, 1);
    if (getOBDdata(OBD_SPD) > 1)
    {
      u8g.setFont(u8g_font_profont15r);
      u8g.drawStr( 0, 42, "L/100Km" );
      u8g.setFont(u8g_font_profont22r);
      u8g.setPrintPos(0, 60) ;
      u8g.print( LPK, 1);
    } else {
      u8g.setFont(u8g_font_profont15r);
      u8g.drawStr( 0, 42, "L/Hour" );
      u8g.setFont(u8g_font_profont22r);
      u8g.setPrintPos(0, 60) ;
      u8g.print(LPH, 1);
    }
    u8g.setFont(u8g_font_profont15r);
    u8g.drawStr( 60, 42, "Average" );
    u8g.setFont(u8g_font_profont22r);
    u8g.setPrintPos(60, 60) ;
    if (trip_obd_avg_fuel_consumption < 100)
      u8g.print( total_obd_avg_fuel_consumption, 1);
    else u8g.drawStr( 60, 60, "---" );
  }
  while ( u8g.nextPage() );
}

void drawTimeDistance(void) {
  u8g.setFont(u8g_font_profont15r);
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_profont15r);
    u8g.drawStr( 0, 15, "TOTAL" );
    u8g.drawStr( 90, 15, "KM" );
    if (LoggingOn == true) u8g.drawStr( 119, 15, "#" );
    u8g.setPrintPos(44, 15) ;
    u8g.print(total_run, 1);

    u8g.setFont(u8g_font_profont15r);
    u8g.drawStr( 0, 42, "Avg SPD" );
    u8g.setFont(u8g_font_profont22r);
    u8g.setPrintPos(0, 60) ;
    u8g.print(total_avg_speed, 1);

    u8g.setFont(u8g_font_profont15r);
    u8g.drawStr( 60, 42, "Time (M)" );
    u8g.setFont(u8g_font_profont22r);
    u8g.setPrintPos(60, 60) ;
    u8g.print( float(total_time) / 60000, 1);
  }
  while ( u8g.nextPage() );
}

void drawAllData(void) {
  // graphic commands to redraw the complete screen should be placed here
  u8g.setFont(u8g_font_profont15r);
  u8g.firstPage();
  do {
    u8g.drawStr( 0, 17, "INJ" );
    u8g.setPrintPos(25, 17) ;
    u8g.print(getOBDdata(OBD_INJ));

    u8g.drawStr( 0, 32, "IGN");
    u8g.setPrintPos(25, 32) ;
    u8g.print( int(getOBDdata(OBD_IGN)));

    u8g.drawStr( 0, 47, "IAC");
    u8g.setPrintPos(25, 47) ;
    u8g.print( int(getOBDdata(OBD_IAC)));

    u8g.drawStr( 0, 62, "RPM");
    u8g.setPrintPos(25, 62) ;
    u8g.print( int(getOBDdata(OBD_RPM)));

    u8g.drawStr( 65, 17, "MAP" );
    u8g.setPrintPos(92, 17) ;
    u8g.print( int(getOBDdata(OBD_MAP)));

    u8g.drawStr( 65, 32, "ECT");
    u8g.setPrintPos(92, 32) ;
    u8g.print( int(getOBDdata(OBD_ECT)));

    u8g.drawStr( 65, 47, "TPS");
    u8g.setPrintPos(92, 47) ;
    u8g.print( int(getOBDdata(OBD_TPS)));

    u8g.drawStr( 65, 62, "SPD");
    u8g.setPrintPos(92, 62) ;
    u8g.print( int(getOBDdata(OBD_SPD)));

    u8g.drawVLine(63, 0, 64);
  } while ( u8g.nextPage() ); // end picture loop
} // end void drawalldata
