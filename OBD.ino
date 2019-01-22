float getOBDdata(byte OBDdataIDX) {
  float returnValue;
  switch (OBDdataIDX) {
    case 0:// UNKNOWN
      returnValue = ToyotaData[0];
      break;
    case OBD_INJ: //  Время впрыска форсунок  =X*0.125 (мс)
      returnValue = ToyotaData[OBD_INJ] * 0.125; //Время впрыска форсунок
      break;
    case OBD_IGN: // Угол опережения зажигания X*0.47-30 (град)
      returnValue = ToyotaData[OBD_IGN] * 0.47 - 30;
      break;
    case OBD_IAC: //  Состояние клапана ХХ Для разных типов КХХ разные формулы: X/255*100 (%)
      //  X (шаг)
      returnValue = ToyotaData[OBD_IAC] * 0.39215; ///optimize divide
      break;
    case OBD_RPM: //Частота вращения коленвала X*25(об/мин)
      returnValue = ToyotaData[OBD_RPM] * 25;
      break;
    case OBD_MAP: //Расходомер воздуха (MAP/MAF)
      //  X*0.6515 (кПа)
      //  X*4.886 (мм.ртут.столба)
      //  X*0.97 (кПа) (для турбомоторов)
      //  X*7.732 (мм.рт.ст) (для турбомоторов)
      //  X*2(гр/сек) MAF
      //  X/255*5 (Вольт) (напряжение на расходомере)
      returnValue = ToyotaData[OBD_MAP] * 2; //Сырые данные
      break;
    case OBD_ECT: // Температура двигателя (ECT)
      // В зависимости от величины Х разные формулы:
      // 0..14:          =(Х-5)*2-60
      // 15..38:        =(Х-15)*0.83-40
      // 39..81:        =(Х-39)*0.47-20
      // 82..134:      =(Х-82)*0.38
      // 135..179:    =(Х-135)*0.44+20
      // 180..209:    =(Х-180)*0.67+40
      // 210..227:    =(Х-210)*1.11+60
      // 228..236:    =(Х-228)*2.11+80
      // 237..242:    =(Х-237)*3.83+99
      // 243..255:    =(Х-243)*9.8+122
      // Температура в градусах цельсия.
      if (ToyotaData[OBD_ECT] >= 243)
        returnValue = ((float)(ToyotaData[OBD_ECT] - 243) * 9.8) + 122;
      else if (ToyotaData[OBD_ECT] >= 237)
        returnValue = ((float)(ToyotaData[OBD_ECT] - 237) * 3.83) + 99;
      else if (ToyotaData[OBD_ECT] >= 228)
        returnValue = ((float)(ToyotaData[OBD_ECT] - 228) * 2.11) + 80.0;
      else if (ToyotaData[OBD_ECT] >= 210)
        returnValue = ((float)(ToyotaData[OBD_ECT] - 210) * 1.11) + 60.0;
      else if (ToyotaData[OBD_ECT] >= 180)
        returnValue = ((float)(ToyotaData[OBD_ECT] - 180) * 0.67) + 40.0;
      else if (ToyotaData[OBD_ECT] >= 135)
        returnValue = ((float)(ToyotaData[OBD_ECT] - 135) * 0.44) + 20.0;
      else if (ToyotaData[OBD_ECT] >= 82)
        returnValue = ((float)(ToyotaData[OBD_ECT] - 82) * 0.38);
      else if (ToyotaData[OBD_ECT] >= 39)
        returnValue = ((float)(ToyotaData[OBD_ECT] - 39) * 0.47) - 20.0;
      else if (ToyotaData[OBD_ECT] >= 15)
        returnValue = ((float)(ToyotaData[OBD_ECT] - 15) * 0.83) - 40.0;
      else
        returnValue = ((float)(ToyotaData[OBD_ECT] - 15) * 2.0) - 60.0;
      break;
    case OBD_TPS: // Положение дроссельной заслонки
      // X/2(градусы)
      // X/1.8(%)
      returnValue = ToyotaData[OBD_TPS] / 1.8;
      break;
    case OBD_SPD: // Скорость автомобиля (км/час)
      returnValue = ToyotaData[OBD_SPD];
      break;
    //  Коррекция для рядных/ коррекция первой половины
    case OBD_OXSENS:
      returnValue = (float)ToyotaData[OBD_OXSENS] * 0.01953125;
      break;
    //Коррекция второй половины
    /*
      case OBD_OXSENS2:// Lambda2 tst
      returnValue = (float)ToyotaData[OBD_OXSENS2] * 0.01953125;
      break;*/
    //  читаем Байты флагов побитно
    case 11:
      returnValue = bitRead(ToyotaData[11], 0);  //  Переобогащение после запуска 1-Вкл
      break;
    case 12:
      returnValue = bitRead(ToyotaData[11], 1); //Холодный двигатель 1-Да
      break;
    case 13:
      returnValue = bitRead(ToyotaData[11], 4); //Детонация 1-Да
      break;
    case 14:
      returnValue = bitRead(ToyotaData[11], 5); //Обратная связь по лямбда зонду 1-Да
      break;
    case 15:
      returnValue = bitRead(ToyotaData[11], 6); //Дополнительное обогащение 1-Да
      break;
    case 16:
      returnValue = bitRead(ToyotaData[12], 0); //Стартер 1-Да
      break;
    case 17:
      returnValue = bitRead(ToyotaData[12], 1); //Признак ХХ (Дроссельная заслонка) 1-Да(Закрыта)
      break;
    case 18:
      returnValue = bitRead(ToyotaData[12], 2); //Кондиционер 1-Да
      break;
    case 19:
      returnValue = bitRead(ToyotaData[13], 3); //Нейтраль 1-Да
      break;
    case 20:
      returnValue = bitRead(ToyotaData[14], 4); //Смесь  первой половины 1-Богатая, 0-Бедная
      break;
    case 21:
      returnValue = bitRead(ToyotaData[14], 5); //Смесь второй половины 1-Богатая, 0-Бедная
      break;
    default: // DEFAULT CASE (in no match to number)
      // send "error" value
      returnValue =  9999.99;
  } // end switch
  // send value back
  return returnValue;
} // end void getOBDdata


void ChangeState() {
  static uint8_t ID, EData[TOYOTA_MAX_BYTES];
  static boolean InPacket = false;
  static unsigned long StartMS;
  static uint16_t BitCount;
  int state = digitalRead(ENGINE_DATA_PIN);
  digitalWrite(LED_PIN, state);
  if (InPacket == false)  {
    if (state == MY_HIGH)   {
      StartMS = millis();
    }   else   { // else  if (state == MY_HIGH)
      if ((millis() - StartMS) > (15 * 8))   {
        StartMS = millis();
        InPacket = true;
        BitCount = 0;
      } // end if  ((millis() - StartMS) > (15 * 8))
    } // end if  (state == MY_HIGH)
  }  else   { // else  if (InPacket == false)
    uint16_t bits = ((millis() - StartMS) + 1 ) / 8; // The +1 is to cope with slight time errors
    StartMS = millis();
    // process bits
    while (bits > 0)  {
      if (BitCount < 4)  {
        if (BitCount == 0)
          ID = 0;
        ID >>= 1;
        if (state == MY_LOW)  // inverse state as we are detecting the change!
          ID |= 0x08;
      }   else    { // else    if (BitCount < 4)
        uint16_t bitpos = (BitCount - 4) % 11;
        uint16_t bytepos = (BitCount - 4) / 11;
        if (bitpos == 0)      {
          // Start bit, should be LOW
          if ((BitCount > 4) && (state != MY_HIGH))  { // inverse state as we are detecting the change!
            ToyotaFailBit = BitCount;
            InPacket = false;
            break;
          } // end if ((BitCount > 4) && (state != MY_HIGH))
        }  else if (bitpos < 9)  { //else TO  if (bitpos == 0)
          EData[bytepos] >>= 1;
          if (state == MY_LOW)  // inverse state as we are detecting the change!
            EData[bytepos] |= 0x80;
        } else { // else if (bitpos == 0)
          // Stop bits, should be HIGH
          if (state != MY_LOW)  { // inverse state as we are detecting the change!
            ToyotaFailBit = BitCount;
            InPacket = false;
            break;
          } // end if (state != MY_LOW)
          if ( (bitpos == 10) && ((bits > 1) || (bytepos == (TOYOTA_MAX_BYTES - 1))) ) {
            ToyotaNumBytes = 0;
            ToyotaID = ID;
            for (uint16_t i = 0; i <= bytepos; i++)
              ToyotaData[i] = EData[i];
            ToyotaNumBytes = bytepos + 1;
            if (bits >= 16)  // Stop bits of last byte were 1's so detect preamble for next packet
              BitCount = 0;
            else  {
              ToyotaFailBit = BitCount;
              InPacket = false;
            }
            break;
          }
        }
      }
      ++BitCount;
      --bits;
    } // end while
  } // end (InPacket == false)
} // end void change
