/*
  DS18B20 biblioteka - Biblioteka za korišćenje DS18B20 senzora temperature
  Autor: Radenko Bogdanovic
  Datum: Datum kreiranja biblioteke
  Verzija: 1.0.0
  GitHub: https://github.com/rasho/DS18B20
*/

#ifndef DS18B20_h
#define DS18B20_h

#include <Arduino.h>
#include <EEPROM.h>

class DS18B20 {
  public:
    DS18B20(uint8_t pin);
    void pocetak();
    void pokreniKonverziju();
    bool jeKonverzijaZavrsena();
    float getTemperaturaC();
    float getTemperaturaF();
    void postaviRezoluciju(uint8_t rezolucija);
    void postaviAlarmnuTemperaturu(float temperatura);
    bool jeAlarmAktiviran();
    void onemoguciParazitnoNapajanje();
    void dozvoliParazitnoNapajanje();
    void sacuvajPostavke();
    void ucitajPostavke();

  private:
    uint8_t _pin;
    uint8_t _adresa[8];
    bool _konverzijaZavrsena;
    bool _alarmAktiviran;
    uint8_t _rezolucija;
    float _alarmnaTemperatura;
    uint16_t _eepromAdresa;
};

DS18B20::DS18B20(uint8_t pin) {
  _pin = pin;
  _eepromAdresa = 0;
}

void DS18B20::pocetak() {
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  delayMicroseconds(480);
  pinMode(_pin, INPUT_PULLUP);
  delayMicroseconds(60);
  if (!digitalRead(_pin)) {
    digitalWrite(_pin, HIGH);
    pinMode(_pin, INPUT);
  }
  delayMicroseconds(480);
}

void DS18B20::pokreniKonverziju() {
  digitalWrite(_pin, LOW);
  pinMode(_pin, OUTPUT);
  delayMicroseconds(2);
  pinMode(_pin, INPUT);
  delayMicroseconds(80);
  digitalWrite(_pin, HIGH);
  delayMicroseconds(480);
  pinMode(_pin, INPUT);
  _konverzijaZavrsena = false;
}

bool DS18B20::jeKonverzijaZavrsena() {
  if (!_konverzijaZavrsena) {
    if (digitalRead(_pin) == LOW) {
      _konverzijaZavrsena = true;
    }
  }
  return _konverzijaZavrsena;
}

float DS18B20::getTemperaturaC() {
  byte podaci[9];
  memset(podaci, 0, sizeof(podaci));

  digitalWrite(_pin, LOW);
  pinMode(_pin, OUTPUT);
  delayMicroseconds(2);
  pinMode(_pin, INPUT_PULLUP);
  delayMicroseconds(15);

  for (uint8_t i = 0; i < 9; i++) {
    podaci[i] = 0;
    for (uint8_t j = 0; j < 8; j++) {
      pinMode(_pin, OUTPUT);
      digitalWrite(_pin, LOW);
      delayMicroseconds(2);
      pinMode(_pin, INPUT_PULLUP);
      delayMicroseconds(15);
      podaci[i] |= digitalRead(_pin) << j;
      delayMicroseconds(60);
    }
  }

  if (podaci[0] == 0x28) {
    int16_t sirovaTemperatura = ((podaci[1] << 8) | podaci[0]) >> 4;
    float temperaturaC = static_cast<float>(sirovaTemperatura) * 0.0625;
    return temperaturaC;
  }
  return NAN;
}

float DS18B20::getTemperaturaF() {
  float temperaturaC = getTemperaturaC();
  if (!isnan(temperaturaC)) {
    float temperaturaF = (temperaturaC * 9.0 / 5.0) + 32.0;
    return temperaturaF;
  }
  return NAN;
}

void DS18B20::postaviRezoluciju(uint8_t rezolucija) {
  if (rezolucija >= 9 && rezolucija <= 12) {
    uint8_t rezolucijaPodaci = (rezolucija - 9) << 5;
    digitalWrite(_pin, LOW);
    pinMode(_pin, OUTPUT);
    delayMicroseconds(2);
    for (uint8_t i = 0; i < 8; i++) {
      if (rezolucijaPodaci & 0x01) {
        pinMode(_pin, INPUT_PULLUP);
      } else {
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, LOW);
      }
      delayMicroseconds(2);
      pinMode(_pin, INPUT_PULLUP);
      delayMicroseconds(60);
      rezolucijaPodaci >>= 1;
    }
    _rezolucija = rezolucija;
  }
}

void DS18B20::postaviAlarmnuTemperaturu(float temperatura) {
  int16_t alarmnaTemperatura = static_cast<int16_t>(temperatura * 16);

  digitalWrite(_pin, LOW);
  pinMode(_pin, OUTPUT);
  delayMicroseconds(2);
  for (int i = 0; i < 16; i++) {
    if (alarmnaTemperatura & 0x01) {
      pinMode(_pin, INPUT_PULLUP);
    } else {
      pinMode(_pin, OUTPUT);
      digitalWrite(_pin, LOW);
    }
    delayMicroseconds(2);
    pinMode(_pin, INPUT_PULLUP);
    delayMicroseconds(60);
    alarmnaTemperatura >>= 1;
  }
  _alarmnaTemperatura = temperatura;
}

bool DS18B20::jeAlarmAktiviran() {
  _alarmAktiviran = false;

  digitalWrite(_pin, LOW);
  pinMode(_pin, OUTPUT);
  delayMicroseconds(2);
  pinMode(_pin, INPUT_PULLUP);
  delayMicroseconds(15);

  if (digitalRead(_pin) == LOW) {
    _alarmAktiviran = true;
  }

  return _alarmAktiviran;
}

void DS18B20::onemoguciParazitnoNapajanje() {
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
}

void DS18B20::dozvoliParazitnoNapajanje() {
  pinMode(_pin, INPUT_PULLUP);
}

void DS18B20::sacuvajPostavke() {
  EEPROM.put(_eepromAdresa, _rezolucija);
  _eepromAdresa += sizeof(_rezolucija);
  EEPROM.put(_eepromAdresa, _alarmnaTemperatura);
  _eepromAdresa += sizeof(_alarmnaTemperatura);
  EEPROM.put(_eepromAdresa, _adresa);
  _eepromAdresa += sizeof(_adresa);
}

void DS18B20::ucitajPostavke() {
  EEPROM.get(_eepromAdresa, _rezolucija);
  _eepromAdresa += sizeof(_rezolucija);
  EEPROM.get(_eepromAdresa, _alarmnaTemperatura);
  _eepromAdresa += sizeof(_alarmnaTemperatura);
  EEPROM.get(_eepromAdresa, _adresa);
  _eepromAdresa += sizeof(_adresa);
}

#endif
