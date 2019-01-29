#define Ncyl 6.0      //6 форсунок
#define Ninjection 1  //впрыск 1 раз за цикл

#if defined(SDCARD) //DEFINE модуля записи на SD карту
#include "SdFat.h"
#include <SPI.h>
#define SS 5 // Номер ноги SS SD модуля
#define FILE_BASE_NAME "Data"   //шаблон имени файла
#define error(msg) sd.errorHalt(F(msg)) //ошибки при работе с SD
#endif

//DEFINE OBD READER
#define  MY_HIGH  HIGH //LOW    // I have inverted the Eng line using an Opto-Coupler, if yours isn't then reverse these low & high defines.
#define  MY_LOW   LOW //HIGH
#define  TOYOTA_MAX_BYTES  24
#define OBD_INJ 1 //Injector pulse width (INJ)
#define OBD_IGN 2 //Ignition timing angle (IGN)
#define OBD_IAC 3 //Idle Air Control (IAC)
#define OBD_RPM 4 //Engine speed (RPM)
#define OBD_MAP 5 //Manifold Absolute Pressure (MAP)
#define OBD_ECT 6 //Engine Coolant Temperature (ECT)
#define OBD_TPS 7 // Throttle Position Sensor (TPS)
#define OBD_SPD 8 //Speed (SPD)
#define OBD_OXSENS 9 // Лямбда 1
//#define OBD_OXSENS2 10 // Лямбда 2 на V-образных движка. У меня ее нету.


#if defined(INJECTOR)
//по сигналу с форсунок


#endif
