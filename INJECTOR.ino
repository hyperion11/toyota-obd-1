#if defined(INJECTOR)
void InjectorInit() {
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // set entire TCCR1B register to 0
  TCCR1B |= (1 << CS12);//prescaler 256 --> Increment time = 256/16.000.000= 16us
  TIMSK1 |= (1 << TOIE1); // enable Timer1 overflow interrupt
  TCNT1 = 3036; // counter initial value so as to overflow every 1sec: 65536 - 3036 = 62500 * 16us = 1sec (65536 maximum value of the timer)
    // set and initialize the TIMER1
 }


void InjectorTime() { // it is called every time a change occurs at the gasoline injector signal and calculates gasoline injector opening time, during the 1sec interval
  if (digitalRead(INJECTOR_PIN) == LOW) {
    InjectorTime1 = micros();
  }
  if (digitalRead(INJECTOR_PIN) == HIGH) {
    InjectorTime2 = micros();
  }
  if (InjectorTime2 > InjectorTime1) {
    if ((InjectorTime2 - InjectorTime1) > 500 && (InjectorTime2 - InjectorTime1) < 15000) {  // Замечено что при резком сбросе оборотов всплески > 15мс=15000мкс. Вангую что это торможение двигателем и форсунки отключается.
      Injector_Open_Duration += (InjectorTime2 - InjectorTime1) * Ncyl;    //Суммирование времени открытия Ncyl форсунок для подсчета суммарного расхода. в микросекундах.
      INJ_TIME = InjectorTime2 - InjectorTime1;                          //длительность впрыска
    }
  }
}
ISR(TIMER1_OVF_vect) { //TIMER1 overflow interrupt -- occurs every 1sec -- it holds the time (in seconds) and also prevents the overflowing of some variables
  total_duration_inj += (float)Injector_Open_Duration / 1000;  // Полный время открытия форсунок по проводу в миллисекундах. Хранится в EEPROM.
  current_duration_inj += (float)Injector_Open_Duration / 1000; // Время открытия форсунок по проводу в миллисекундах за поездку.
  total_consumption_inj = total_duration_inj / 1000 * Ls;
  current_consumption_inj = current_duration_inj / 1000 * Ls;
  Injector_Open_Duration = 0;
  TCNT1 = 3036;
}
#endif
