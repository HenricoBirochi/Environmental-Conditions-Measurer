#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <DHT.h>
#include <EEPROM.h>
#include "pitches.h"

// Definir se o código está sendo executado em um sistema real (1) ou no simulador (0)
#define IS_REAL_SYSTEM 0 

#define col 16     // Número de colunas do display
#define lin 2      // Número de linhas do display
#define ende 0x27  // Endereço do display

#define DHTPIN 7  
#if IS_REAL_SYSTEM
    #define DHTTYPE DHT11  // Tipo de sensor DHT para versão real
    RTC_DS3231 rtc;         // RTC para sistema real
#else
    #define DHTTYPE DHT22  // Tipo de sensor DHT para versão Wokwi
    RTC_DS1307 rtc;        // RTC para simulador Wokwi
#endif

DHT dht(DHTPIN, DHTTYPE);  

char daysOfTheWeek[7][12] = { "Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado" };

byte degreesSymbol[8] = {
  0b11100,
  0b10100,
  0b11100,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

LiquidCrystal_I2C lcd(ende, col, lin);

const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int buttonScreen = 6;
const int buttonConfig = 5;
const int pinLDR = A0;  // Pino Photo-Resistor
const int melodyPin = 7;

void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

float fakeLuminosity() {
  int potValue = analogRead(A3);
  return map(potValue, 0, 1023, 100, 0);
}

int currentScreen;
int buttonScreenState = 0;
int lastButtonScreenState = 0;
int buttonConfigState = 0;
int lastButtonConfigState = 0;
bool configMode = false;
int tempUnit = 0;

// Configuração do intervalo por millis()
unsigned long writeDelay = 0;
unsigned long logDelay = 0;
const long interval = 5000;

// Configurações da EEPROM
const int maxRecords = 100;
const int recordSize = 8;  // Tamanho de cada registro em bytes
int startAddress = 0;
int endAddress = maxRecords * recordSize;
int currentAddress = 0;

int lastLoggedMinute = -1;

int melody[] = {
  NOTE_G4, NOTE_FS4, NOTE_DS4, NOTE_A3, NOTE_GS3, NOTE_E4, NOTE_GS4, NOTE_C5
};

int noteDurations[] = {
  4, 4, 4, 4, 4, 4, 4, 4
};

void setup() {
  currentScreen = 0;
  Serial.begin(9600);
  Serial.println("Sistema Inicializado!");

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buttonScreen, INPUT_PULLUP);
  pinMode(buttonConfig, INPUT_PULLUP);
  pinMode(melodyPin, OUTPUT);
  lastButtonScreenState = digitalRead(buttonScreen);
  lastButtonConfigState = digitalRead(buttonConfig);

  dht.begin();

  if (!rtc.begin()) {
    Serial.println("RTC não encontrado");
    while (1)
      ;
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.createChar(5, degreesSymbol);
  EEPROM.begin();
}

void loop() {

  unsigned long currentMillis = millis();

  buttonScreenState = digitalRead(buttonScreen);
  buttonConfigState = digitalRead(buttonConfig);

  float temp;
  float temperatureC = dht.readTemperature();

  switch (tempUnit) {
    case 0:  // Celsius
      temp = temperatureC;
      break;
    case 1:  // Fahrenheit
      temp = (temperatureC * 1.8) + 32;
      break;
    case 2:  // Kelvin
      temp = temperatureC + 273.15;
      break;
    default:
      // Caso o valor de tempUnit não seja válido
      temp = NAN;  // Define como NaN (Not a Number) para indicar erro
      break;
  }

  float humid = dht.readHumidity();
  float luminosity = analogRead(pinLDR);
  
  #if IS_REAL_SYSTEM
    float lumen = map(luminosity, 444, 969, 0, 100);
  #else
    float lumen = fakeLuminosity();
  #endif

  // Definir a faixa de temperatura
  float minTemp = 15.0;
  float maxTemp = 25.0;
  float maxHumidity = 50;
  float minHumidity = 30;
  float maxLuminosity = 30;

  DateTime now = rtc.now();

  if (temperatureC < minTemp || temperatureC > maxTemp || humid < minHumidity || humid > maxHumidity) {
     if (currentMillis - writeDelay >= interval) {
       writeDelay = currentMillis;
       int tempCInt = (int)(temperatureC * 100);
       int humidInt = (int)(humid * 100);
 
       EEPROM.put(currentAddress, now.unixtime());
       EEPROM.put(currentAddress + 4, tempCInt);
       EEPROM.put(currentAddress + 6, humidInt);

        melody();
 
       getNextAddress();
     }
   }

  const int debounceDelay = 50;
  unsigned long lastDebounceTime = 0;
  unsigned long configStartTime;
  int configEndTime = 2000;

  if (buttonScreenState != lastButtonScreenState && buttonScreenState == HIGH) {
     if (millis() - lastDebounceTime > debounceDelay) {
       configMode = false;
       lcd.clear();
       currentScreen = (currentScreen + 1) % 3;
       lastDebounceTime = millis();
     }
   }
  lastButtonScreenState = buttonScreenState;

  if (buttonConfigState != lastButtonConfigState && buttonConfigState == HIGH) {
    if (millis() - lastDebounceTime > debounceDelay) {
      lcd.clear();
      lastDebounceTime = millis();
      if (!configMode) {
        configMode = true;
        configStartTime = millis();
      } else {
        configStartTime = millis();
        switch (currentScreen) {
          case 0:
            tempUnit = (tempUnit > 1) ? 0 : tempUnit + 1;
            break;
        }
      }
    }
  }
  lastButtonConfigState = buttonConfigState;


  // Exibir a tela correspondente
  if (configMode) {
    switch (currentScreen) {
      case 0:
        lcd.setCursor(0, 0);
        lcd.print("Escala:");
        lcd.setCursor(0, 1);
        switch (tempUnit) {
          case 0:
            lcd.print(">Celsius   ");
            break;
          case 1:
            lcd.print(">Farenheit");
            break;
          case 2:
            lcd.print(">Kelvin    ");
            break;
        }
        break;
      default:
        configEndTime = 0;
        configMode = false;
    }
    if (millis() - configStartTime >= configEndTime) {
      configMode = false;
      lcd.clear();
    }

  } else {
    switch (currentScreen) {
      case 0:
        lcd.setCursor(0, 0);
        lcd.print("Temperatura");
        lcd.setCursor(0, 1);
        lcd.print(temp);
        switch (tempUnit) {
          case 0:
            lcd.write(5);
            lcd.print("C  ");
            break;
          case 1:
            lcd.write(5);
            lcd.print("F  ");
            break;
          case 2:
            lcd.print("K   ");
            break;
        }

        {
          // Mapear a temperatura para os valores das cores
          int red = map(temperatureC, minTemp, maxTemp, 0, 255);
          int blue = map(temperatureC, minTemp, maxTemp, 255, 0);
          int green = 120;
          // Garantir que os valores estejam dentro do intervalo (0-255)
          red = constrain(red, 0, 255);
          blue = constrain(blue, 0, 255);
          // Atualizar a cor do LED RGB
          setColor(red, green, blue);
        }
        break;
      case 1:
        lcd.setCursor(0, 0);
        lcd.print("Umidade");
        lcd.setCursor(0, 1);
        lcd.print(humid);
        lcd.print("%  ");

        {
          int white;  // Declarar a variável 'white' fora do if-else

          if (humid > maxHumidity) {
            // Se a umidade for maior que o máximo, o valor de 'white' será mapeado de 255 para 0
            white = map(humid, minHumidity, maxHumidity, 255, 0);
          } else if (humid < minHumidity) {
            // Se a umidade for menor que o mínimo, o valor de 'white' será mapeado de 0 para 255
            white = map(humid, minHumidity, maxHumidity, 0, 255);
          } else {
            // Se a umidade estiver no intervalo entre minHumidity e maxHumidity, a cor será branca
            white = 255;
          }

          // Garantir que o valor de 'white' esteja dentro do intervalo de 0 a 255
          white = constrain(white, 0, 255);

          // Configurar a cor com base no valor de 'white'
          setColor(white, white, 255);  // Aqui você pode ajustar os valores de R, G, B como desejar
        }

        break;
      case 2:
        lcd.setCursor(0, 0);
        lcd.print("Luminosidade");
        lcd.setCursor(0, 1);
        lcd.print(lumen);
        lcd.print("%  ");
        {
          // Luminosidade: 0-20% Branco (255,255,255) → 30% Amarelo (255,255,0)
          int yellow = map(lumen, (maxLuminosity - 10), maxLuminosity, 255, 0);
          yellow = constrain(yellow, 0, 255);
          int white = map(lumen, 0, maxLuminosity - 10, 255, 0);
          white = constrain(white, 0, 255);
          setColor(255, 255, yellow);  // Transição de branco para amarelo
        }
        break;
    }

    if (currentMillis - logDelay >= interval) {
      logDelay = currentMillis;

      print_log_now();


    }
  }
}

void getNextAddress() {
  currentAddress += recordSize;
  if (currentAddress >= endAddress) {
    currentAddress = startAddress;  // Volta para o começo se atingir o limite
  }
}

void print_log_now() {
        DateTime now = rtc.now();  //CHAMADA DE FUNÇÃO
      Serial.println(F("------------------------------"));
      Serial.print("Data: ");                           //IMPRIME O TEXTO NO MONITOR SERIAL
      Serial.print(now.day(), DEC);                     //IMPRIME NO MONITOR SERIAL O DIA
      Serial.print('/');                                //IMPRIME O CARACTERE NO MONITOR SERIAL
      Serial.print(now.month(), DEC);                   //IMPRIME NO MONITOR SERIAL O MÊS
      Serial.print('/');                                //IMPRIME O CARACTERE NO MONITOR SERIAL
      Serial.print(now.year(), DEC);                    //IMPRIME NO MONITOR SERIAL O ANO
      Serial.print(" / Dia: ");                         //IMPRIME O TEXTO NA SERIAL
      Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);  //IMPRIME NO MONITOR SERIAL O DIA
      Serial.print(" / Horas: ");                       //IMPRIME O TEXTO NA SERIAL
      Serial.print(now.hour(), DEC);                    //IMPRIME NO MONITOR SERIAL A HORA
      Serial.print(':');                                //IMPRIME O CARACTERE NO MONITOR SERIAL
      Serial.print(now.minute(), DEC);                  //IMPRIME NO MONITOR SERIAL OS MINUTOS
      Serial.print(':');                                //IMPRIME O CARACTERE NO MONITOR SERIAL
      Serial.print(now.second(), DEC);                  //IMPRIME NO MONITOR SERIAL OS SEGUNDOS
      Serial.println();                                 //QUEBRA DE LINHA NA SERIAL

      Serial.println(F("------ Dados de Sensores ------"));
      Serial.print(F("Temperatura: "));
      Serial.print(temp);
      Serial.println(F(" °C"));

      Serial.print(F("Umidade: "));
      Serial.print(humid);
      Serial.println(F(" %"));

      Serial.print(F("Luminosidade: "));
      Serial.print(lumen);
      Serial.println(F(" %"));
      Serial.println(F("------------------------------"));
}

void get_full_log_anomalies() {
  Serial.println("Data stored in EEPROM:");
  Serial.println("Timestamp\t\tTemperature\tHumidity");

  for (int address = startAddress; address < endAddress; address += recordSize) {
    long timeStamp;
    int tempInt, humiInt;

    // Ler dados da EEPROM
    EEPROM.get(address, timeStamp);
    EEPROM.get(address + 4, tempInt);
    EEPROM.get(address + 6, humiInt);

    // Converter valores
    float temperature = tempInt / 100.0;
    float humidity = humiInt / 100.0;

    // Verificar se os dados são válidos antes de imprimir
    if (timeStamp != 0xFFFFFFFF) {  // 0xFFFFFFFF é o valor padrão de uma EEPROM não inicializada
      //Serial.print(timeStamp);
      DateTime dt = DateTime(timeStamp);
      Serial.print(dt.timestamp(DateTime::TIMESTAMP_FULL));
      Serial.print("\t");
      Serial.print(temperature);
      Serial.print(" C\t\t");
      Serial.print(humidity);
      Serial.println(" %");
    }
  }
}

void melody() {
    for (int thisNote = 0; thisNote < 8; thisNote++) {

    int noteDuration = 1000 / noteDurations[thisNote];
    tone(7, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(7);
  }
}
