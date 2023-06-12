# Arduino biblioteka za DS18B20 senzor

Ova Arduino biblioteka omogućava jednostavno korišćenje DS18B20 senzora temperature bez korišćenja postojećih biblioteka.

## Instalacija

1. Preuzmite najnoviju verziju biblioteke kao ZIP arhivu.
2. U Arduino IDE-u idite na "Sketch" -> "Include Library" -> "Add .ZIP Library".
3. Izaberite preuzetu ZIP arhivu biblioteke.
4. Biblioteka će biti instalirana i spremna za upotrebu.

## Upotreba

1. Uključite biblioteku u vaš Arduino kod dodavanjem linije `#include <DS18B20.h>`.
2. Kreirajte objekat DS18B20 senzora: `DS18B20 ds18b20(pin)`, gde `pin` predstavlja broj pina na kojem je senzor priključen.
3. Inicijalizujte senzor pozivom metode `ds18b20.init()`.
4. Pročitajte temperaturu u Celzijusima pozivom metode `ds18b20.getTemperaturaC()`.
5. Pročitajte temperaturu u Farenhajtima pozivom metode `ds18b20.getTemperaturaF()`.

## Metode

- `void init()`: Inicijalizuje DS18B20 senzor.
- `float getTemperaturaC()`: Čita temperaturu sa senzora i vraća vrednost u Celzijusima.
- `float getTemperaturaF()`: Čita temperaturu sa senzora i vraća vrednost u Farenhajtima.
- `void postaviRezoluciju(uint8_t rezolucija)`: Postavlja rezoluciju senzora (9-12 bita).
- `void postaviAlarmnuTemperaturu(float temperatura)`: Postavlja alarmnu temperaturu senzora.
- `bool jeAlarmAktiviran()`: Proverava da li je alarm aktiviran na senzoru.
- `void onemoguciParazitnoNapajanje()`: Onemogućava parazitno napajanje senzora.
- `void dozvoliParazitnoNapajanje()`: Dozvoljava parazitno napajanje senzora.
- `void sacuvajPostavke()`: Sprema postavke (rezolucija, alarmna temperatura, adresa) u EEPROM memoriju.
- `void ucitajPostavke()`: Učitava postavke (rezolucija, alarmna temperatura, adresa) iz EEPROM memorije.

## Primer koda

```cpp
#include <DS18B20.h>

#define DS18B20_PIN 2

DS18B20 ds18b20(DS18B20_PIN);

void setup() {
  Serial.begin(9600);
  ds18b20.init();
}

void loop() {
  float temperaturaC = ds18b20.getTemperaturaC();
  float temperaturaF = ds18b20.getTemperaturaF();

  Serial.print("Temperatura u C: ");
  Serial.println(temperaturaC);
  Serial.print("Temperatura u F: ");
  Serial.println(temperaturaF);

  delay(1000);
}
```