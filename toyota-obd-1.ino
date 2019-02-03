// ToyotaOBD1_Reader
// In order to read the data from the OBD connector, short E1 + TE2. then to read the data connect to VF1.
// Note the data line output is 12V - connecting it directly to one of the arduino pins might damage (proabably) the board
// This is made for diaply with an OLED display using the U8glib - which allow wide range of display types with minor adjusments.
// Many thanks to GadgetFreak for the greate base code for the reasding of the data.
// If you want to use invert line - note the comments on the MY_HIGH and the INPUT_PULLUP in the SETUP void.

#include "U8glib.h"
#include <EEPROM.h>
#include <MD_KeySwitch.h>
#include "DEFINES.h"
#define Ls 0.003965888 //производительсность форсунки литров в секунду // базовый 0.004 или 240cc

//#define SDCARD //закомментировать если не нужен функционал логирования на SD карту.
//обязательно закомментировать если не подключен SD модуль. Иначе программа будет не стартовать.
#define INJECTOR //чтение сигнала с форсунок. Как показала практика расход по ОБД считается достаточно точно. Смысла использовать сигнал с форсунок нету.
//#define LOGGING_FULL    //Запись на SD всех данных
#define DEBUG_OUTPUT true // for debug option - swith output to Serial
//DEFINE пинов под входы-выходы
#define LED_PIN          13
#define ENGINE_DATA_PIN 2 //VF1 PIN
#define TOGGLE_BTN_PIN 4 //screen change PIN
#if defined(INJECTOR)
#define INJECTOR_PIN 3 //engine injector PIN
volatile unsigned long Injector_Open_Duration = 0;
volatile unsigned long INJ_TIME = 0;
volatile unsigned long InjectorTime1 = 0;
volatile unsigned long InjectorTime2 = 0;
volatile uint32_t num_injection = 0;
volatile uint16_t rpm_inj = 0;
volatile float total_duration_inj, current_duration_inj;
volatile float total_consumption_inj, current_consumption_inj;
volatile uint32_t current_time_inj, total_time_inj;
#endif




//DEFINE констант расходомера

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
#if defined(SDCARD)
SdFat sd;
SdFile file;
#endif

MD_KeySwitch S(TOGGLE_BTN_PIN, HIGH);
byte CurrentDisplayIDX = 1;
float current_run = 0;
float total_run = 0;
float total_avg_speed;
float avg_speed;
unsigned long current_time = 0;
unsigned long total_time = 0;

volatile float total_duration_inj
float  cycle_obd_inj_dur, trip_obd_inj_dur, total_obd_inj_dur, trip_obd_fuel_consumption,
       total_obd_fuel_consumption, trip_obd_avg_fuel_consumption, total_obd_avg_fuel_consumption; //по обд протоколу


float LPK, LPH, LPH_INJ;
bool flagNulSpeed = true;
volatile uint8_t ToyotaNumBytes, ToyotaID, ToyotaData[TOYOTA_MAX_BYTES];
volatile uint16_t ToyotaFailBit = 0;
boolean LoggingOn = false;

void setup() {
#if defined(SDCARD)
  char fileName[13] = FILE_BASE_NAME "00.csv";
  const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
#endif
  noInterrupts();
  Serial.begin(115200);
  ReadEEPROM();
  S.begin();
  S.enableDoublePress(true);
  S.enableLongPress(true);
  S.enableRepeat(false);
  S.enableRepeatResult(false);
  S.setDoublePressTime(300);
  S.setLongPressTime(2000);
  if (DEBUG_OUTPUT) {
    Serial.println("system Started");
    Serial.print("Read float from EEPROM: ");
    Serial.println(total_run, 3);
    Serial.println(total_time, 3);
    Serial.println(total_obd_avg_fuel_consumption, 3);
    //   Serial.println(total_obd_inj_dur_ee, 3);
  }
#if defined(SDCARD)
  SDinit();
  writeHeader();
#endif
#if defined(INJECTOR)
  InjectorInit();
  pinMode(INJECTOR_PIN, INPUT); // Injector PIN
  attachInterrupt(digitalPinToInterrupt(INJECTOR_PIN), InjectorTime, CHANGE); //setup Interrupt for data line
#endif
  pinMode(ENGINE_DATA_PIN, INPUT); // VF1 PIN
  pinMode(LED_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ENGINE_DATA_PIN), ChangeState, CHANGE); //setup Interrupt for data line
  pinMode(TOGGLE_BTN_PIN, INPUT);           // кнопка СЛЕД. ЭКРАН
  CurrentDisplayIDX = 1; // set to display 1
  drawScreenSelector();
  t = millis();
  interrupts();
  delay(10);
} // END VOID SETUP

void loop(void) {
  unsigned long new_t;
  unsigned int diff_t;
  switch (S.read())
  {
    case MD_KeySwitch::KS_NULL: break;
    case MD_KeySwitch::KS_PRESS:    ent(); break;
    case MD_KeySwitch::KS_DPRESS:   {
        if (LoggingOn == false) LoggingOn = true; else LoggingOn = false;
      } break;
    case MD_KeySwitch::KS_LONGPRESS: cleardata(); break;
    case MD_KeySwitch::KS_RPTPRESS: break;
  }
  if (ToyotaNumBytes > 0)  {    // if found bytes
    new_t = millis();
    if (new_t > t) {
      diff_t = new_t - t;
      cycle_obd_inj_dur = getOBDdata(OBD_RPM) / 60000.0  * Ncyl * Ninjection * (float)diff_t  * getOBDdata(OBD_INJ) / 2.0; //Время открытых форсунок за 1 такт данных. В МС
      //форсунка срабатывает раз в 2 оборота КВ
      //время цикла мс в с. Получаем кол-во срабатываний за время цикла. Умножаем на время открытия форсунки, получаем время открытия 6 форсунок В МИЛЛИСЕКУНДАХ
      current_run += (float)diff_t / 3600000 * getOBDdata(OBD_SPD);  //Пройденное расстояние с момента включения. В КМ
      total_run += (float)diff_t / 3600000 * getOBDdata(OBD_SPD);    //Полное пройденное расстояние. EEPROM. В КМ
      trip_obd_inj_dur += cycle_obd_inj_dur;                                                              //Время открытых форсунок за поездку        В МС
      total_obd_inj_dur += cycle_obd_inj_dur;                                                           //Время открытых форсунок за все время. EEPROM    В МС
      total_time += diff_t;                         //полное пройденное время в миллисекундах лимит ~49 суток. EEPROM
      current_time += diff_t;             //Время в пути в миллисекундах с момента включения
      total_avg_speed = total_run / (float)total_time * 3600000;           // средняя скорость за все время. км\ч
      avg_speed = current_run / (float)current_time * 3600000 ;       //average speed
      trip_obd_fuel_consumption = trip_obd_inj_dur  * Ls / 1000.0;    //потребление топлива за поездку в литрах
      total_obd_fuel_consumption = total_obd_inj_dur  * Ls / 1000.0;  //потребление топлива за все время. Из ЕЕПРОМ в литрах
      trip_obd_avg_fuel_consumption = 100.0 * trip_obd_fuel_consumption / current_run; //средний расход за поездку
      total_obd_avg_fuel_consumption = 100.0 * total_obd_fuel_consumption / total_run;
      LPK = 100 / getOBDdata(OBD_SPD) * (getOBDdata(OBD_INJ) * getOBDdata(OBD_RPM) * Ls * 0.18);
      LPH = getOBDdata(OBD_INJ) * getOBDdata(OBD_RPM) * Ls * 0.18;
      t = millis();
    }
#if defined(INJECTOR)
    //по сигналу с форсунок
    // total_avg_consumption = 100 * total_consumption_inj / total_run;
    // avg_consumption_inj = 100 * current_consumption_inj / current_run; //average l/100km for unleaded fuel     //вроде норм
#endif
    drawScreenSelector(); // draw screen
    ToyotaNumBytes = 0;     // reset the counter.
  } // end if (ToyotaNumBytes > 0)
  if (getOBDdata(OBD_SPD) == 0 && flagNulSpeed == false)  {   //Запись данных в еепром когда остановка авто
    SaveEEPROM();
    flagNulSpeed = true;                                  //запрет повторной записи
  }
  if (getOBDdata(OBD_SPD) != 0) flagNulSpeed = false;     //начали двигаться - разрешаем запись
#if defined(SDCARD)
  if (millis() % 500 < 50 && LoggingOn == true) {         //каждую 0.5с запись в лог данных по двоному нажатию на кнопку
    logData();
  }
#endif
  //  if (millis() % 5000 < 50) autoscreenchange();      // ротация экранов
}

void drawScreenSelector(void) {
  if (CurrentDisplayIDX == 1) DrawCurrentFuelConsuption();
  else if (CurrentDisplayIDX == 2) DrawTotalFuelConsuption();
  else if (CurrentDisplayIDX == 3) drawTimeDistance();
  else if (CurrentDisplayIDX == 4) drawAllData();
  else if (CurrentDisplayIDX == 5) drawExtraData();
  else if (CurrentDisplayIDX == 6) drawExtraFlags();
} // end drawScreenSelector()

void autoscreenchange() {
  CurrentDisplayIDX++;
  if (CurrentDisplayIDX > 3) CurrentDisplayIDX = 1;
  drawScreenSelector();
}

void ent() {//ПЕРЕКЛЮЧЕНИЕ ЭКРАНОВ
  CurrentDisplayIDX++;
  if (CurrentDisplayIDX > 6) CurrentDisplayIDX = 1;
  drawScreenSelector();
}
