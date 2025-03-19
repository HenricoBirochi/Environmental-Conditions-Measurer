#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <DHT.h>
#include <EEPROM.h>
#include "pitches.h"

// Definir se o código está sendo executado em um sistema real (1) ou no simulador (0)
#define IS_REAL_SYSTEM 0

#define col 16             // Número de colunas do display
#define lin 2              // Número de linhas do display
#define ende 0x27          // Endereço do display

#define DHTPIN 7
#if IS_REAL_SYSTEM
#define DHTTYPE DHT11      // Tipo de sensor DHT para versão real
RTC_DS3231 rtc;            // RTC para sistema real
#else
#define DHTTYPE DHT22      // Tipo de sensor DHT para versão Wokwi
RTC_DS1307 rtc;            // RTC para simulador Wokwi
#endif

#define SPEAKER_PIN 3      // Pino do Buzzer

DHT dht(DHTPIN, DHTTYPE);  // DHT d

char daysOfTheWeek[7][12] = { "Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado" };

LiquidCrystal_I2C lcd(ende, col, lin);

const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int buttonScreen = 6;
const int buttonConfig = 5;
const int pinLDR = A0;  // Pino Photo-Resistor

// Parte Sonora:
int melodyPirate[] = {
  NOTE_A3, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_C4, NOTE_D4,
  NOTE_A3, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4,
  NOTE_A3, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_D4,
  NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_D4, NOTE_D4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_F4, NOTE_D4, NOTE_E4
};

float noteDurationsPirate[] = {
  2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 2, 0.67, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 
  0.5, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 2, 0.67, 2, 2, 1, 1, 1, 2, 0.67, 2, 2, 1, 1, 2, 2, 0.5
};

void playMelodyPirate() {
    for (int thisNote = 0; thisNote < 61; thisNote++) {

    float noteDuration = 150 / noteDurationsPirate[thisNote];
    tone(SPEAKER_PIN, melodyPirate[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.40;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(SPEAKER_PIN);
  }
}

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

void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

float fakeLuminosity() {
  int potValue = analogRead(A3);
  return map(potValue, 0, 1023, 0, 100);
}

int currentScreen;
int buttonScreenState = 0;
int lastButtonScreenState = 0;
int buttonConfigState = 0;
int lastButtonConfigState = 0;
bool configMode = false;
int tempUnit = 0;
int callLog = 0;
unsigned long screenButtonPressTime = 0;
const int holdTime = 3000; // Tempo necessário para ativar a tela 3 (3 segundos)


// Configuração do intervalo por millis()
unsigned long writeDelay = 0;
unsigned long logDelay = 0;
const long interval = 5000;

// Configurações da EEPROM
const int maxRecords = 60;
const int recordSize = 10;  // Tamanho de cada registro em bytes
int startAddress = 0;
int endAddress = maxRecords * recordSize;
int currentAddress = 0;

int lastLoggedMinute = -1;

void setup() {
  playMelodyPirate();
  
  currentScreen = 0;
  Serial.begin(9600);
  Serial.println("Sistema Inicializado!");

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buttonScreen, INPUT_PULLUP);
  pinMode(buttonConfig, INPUT_PULLUP);
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
  shipGoing();
  lcd.clear();
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
  float lumen = map(luminosity, 444, 969, 100, 0);
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

  if (temperatureC < minTemp || temperatureC > maxTemp || humid < minHumidity || humid > maxHumidity || lumen > maxLuminosity) {
    if (currentMillis - writeDelay >= interval) {
      writeDelay = currentMillis;
      int tempCInt = (int)(temperatureC * 100);
      int humidInt = (int)(humid * 100);
      int lumenInt = (int)(lumen * 100);

      EEPROM.put(currentAddress, now.unixtime());
      EEPROM.put(currentAddress + 4, tempCInt);
      EEPROM.put(currentAddress + 6, humidInt);
      EEPROM.put(currentAddress + 8, lumenInt);
      getNextAddress();
    }
    tone(SPEAKER_PIN, 1000, 500);
  }

  const int debounceDelay = 50;
  unsigned long lastDebounceTime = 0;
  unsigned long configStartTime;
  int configEndTime = 2000;

  if (buttonScreenState != lastButtonScreenState && buttonScreenState == HIGH) {
    if (millis() - lastDebounceTime > debounceDelay) {
      configMode = false;
      lcd.clear();
      currentScreen = (currentScreen + 1) % 4;
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
          case 3:
            callLog = (callLog > 0) ? 0 : callLog + 1;
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
      case 3:
        lcd.setCursor(0, 0);
        lcd.print("Ajustes:");
        lcd.setCursor(0, 1);
        switch (callLog) {
          case 0:
            lcd.print(">Printar Log");
            break;
          case 1:
            lcd.print(">Chamar EEPROM");
            if (millis() - configStartTime >= configEndTime) {
              lcd.setCursor(0, 1);
              lcd.print("Puxando Dados...");
              get_log();           // Chama a função get_log() após 5 segundos
              configMode = false;  // Sai do modo de configuração
              lcd.clear();         // Limpa a tela
            }
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
        lcd.createChar(5, degreesSymbol);
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
      case 3:
        IconMenu();
        break;
    }

    if (currentMillis - logDelay >= interval) {
      logDelay = currentMillis;
    }
  }
}

void getNextAddress() {
  currentAddress += recordSize;
  if (currentAddress >= endAddress) {
    currentAddress = startAddress;  // Volta para o começo se atingir o limite
  }
}

void get_log() {
  Serial.println("Data stored in EEPROM:");
  Serial.println("Timestamp\t\tTemperature\tHumidity\tLuminosity");

  for (int address = startAddress; address < endAddress; address += recordSize) {
    long timeStamp;
    int tempInt, humiInt, lumenInt;

    // Ler dados da EEPROM
    EEPROM.get(address, timeStamp);
    EEPROM.get(address + 4, tempInt);
    EEPROM.get(address + 6, humiInt);
    EEPROM.get(address + 8, lumenInt);

    // Converter valores
    float temperature = tempInt / 100.0;
    float humidity = humiInt / 100.0;
    float luminosity = lumenInt / 100.0;


    // Verificar se os dados são válidos antes de imprimir
    if (timeStamp != 0xFFFFFFFF) {  // 0xFFFFFFFF é o valor padrão de uma EEPROM não inicializada
      //Serial.print(timeStamp);
      DateTime dt = DateTime(timeStamp);
      Serial.print(dt.timestamp(DateTime::TIMESTAMP_FULL));
      Serial.print("\t");
      Serial.print(temperature);
      Serial.print(" C\t\t");
      Serial.print(humidity);
      Serial.print(" %\t\t");
      Serial.print(luminosity);
      Serial.println(" %");
    }
  }
}

void TextMenu(){
          lcd.setCursor(0, 0);
        lcd.print("- Mar de Vinho -");
        lcd.setCursor(0, 1);
        lcd.print("    [MENU]");
}

void IconMenu() {

  byte name1x15[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  byte name0x6[] = { 0x00, 0x00, 0x00, 0x01, 0x03, 0x02, 0x02, 0x01 };
  byte name0x7[] = { 0x03, 0x01, 0x00, 0x1B, 0x1D, 0x1C, 0x0C, 0x19 };
  byte name0x8[] = { 0x00, 0x10, 0x00, 0x1B, 0x1D, 0x1D, 0x0C, 0x19 };
  byte name0x9[] = { 0x00, 0x00, 0x00, 0x10, 0x18, 0x18, 0x18, 0x10 };
  byte name1x7[] = { 0x00, 0x17, 0x13, 0x0E, 0x00, 0x00, 0x02, 0x01 };
  byte name1x8[] = { 0x00, 0x07, 0x03, 0x0E, 0x00, 0x1C, 0x0C, 0x18 };

  lcd.createChar(0, name1x15);
  lcd.setCursor(15, 1);
  lcd.write(0);

  lcd.createChar(1, name0x6);
  lcd.setCursor(6, 0);
  lcd.write(1);

  lcd.createChar(2, name0x7);
  lcd.setCursor(7, 0);
  lcd.write(2);

  lcd.createChar(3, name0x8);
  lcd.setCursor(8, 0);
  lcd.write(3);

  lcd.createChar(4, name0x9);
  lcd.setCursor(9, 0);
  lcd.write(4);

  lcd.createChar(5, name1x7);
  lcd.setCursor(7, 1);
  lcd.write(5);

  lcd.createChar(6, name1x8);
  lcd.setCursor(8, 1);
  lcd.write(6);
}

void shipGoing(){
  byte i = 36;
  ship();
  lcd.setCursor(27, 1);
  lcd.print("Wine Sea");
  while(i <= 64){ 
    delay(250);
    lcd.scrollDisplayRight();
    i++;
  }
}
void ship(){
  byte name0x0[] = { B01111, B11001, B11001, B01100, B00100, B00100, B00100, B01100 };
  byte name0x1[] = { B11111, B00100, B00100, B10010, B10010, B10010, B10010, B10010 };
  byte name0x2[] = { B11110, B10010, B10011, B01001, B01001, B01001, B01001, B01001 };
  byte name0x3[] = { B00000, B00000, B00000, B00000, B00000, B00110, B00011, B00011 };
  byte name1x0[] = { B11001, B11001, B00111, B00000, B10000, B11111, B11111, B01111 };
  byte name1x1[] = { B00100, B00100, B11111, B01010, B01010, B01010, B11111, B11111 };
  byte name1x2[] = { B10011, B10010, B11110, B00000, B00011, B11111, B11111, B11111 };
  byte name1x3[] = { B00011, B00111, B01111, B11110, B11110, B11100, B11000, B10000 };

  lcd.createChar(0, name0x0);
  lcd.setCursor(36, 0);
  lcd.write(byte(0));
  
  lcd.createChar(1, name0x1);
  lcd.setCursor(37, 0);
  lcd.write(byte(1));
  
  lcd.createChar(2, name0x2);
  lcd.setCursor(38, 0);
  lcd.write(byte(2));
  
  lcd.createChar(3, name0x3);
  lcd.setCursor(39, 0);
  lcd.write(byte(3));
  
  lcd.createChar(4, name1x0);
  lcd.setCursor(36, 1);
  lcd.write(byte(4));
  
  lcd.createChar(5, name1x1);
  lcd.setCursor(37, 1);
  lcd.write(byte(5));
  
  lcd.createChar(6, name1x2);
  lcd.setCursor(38, 1);
  lcd.write(byte(6));
  
  lcd.createChar(7, name1x3);
  lcd.setCursor(39, 1);
  lcd.write(byte(7));
}
