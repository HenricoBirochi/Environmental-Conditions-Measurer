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

#define DHTPIN 7  // Pino do sensor DHT
#if IS_REAL_SYSTEM
#define DHTTYPE DHT11  // Tipo de sensor DHT para versão real
RTC_DS3231 rtc;        // RTC para sistema real
#else
#define DHTTYPE DHT22  // Tipo de sensor DHT para versão Wokwi
RTC_DS1307 rtc;                    // RTC para simulador Wokwi
#endif

#define SPEAKER_PIN 3  // Pino do Buzzer

DHT dht(DHTPIN, DHTTYPE);  // Inicializa o sensor DHT

// Array de dias da semana, embora não seja utilizado mais no código
char daysOfTheWeek[7][12] = { "Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado" };

LiquidCrystal_I2C lcd(ende, col, lin);  // Inicializa o display LCD

const int redPin = 9;        // Pino do LED vermelho
const int greenPin = 10;     // Pino do LED verde
const int bluePin = 11;      // Pino do LED azul
const int buttonScreen = 6;  // Pino do botão de tela
const int buttonConfig = 5;  // Pino do botão de configuração
const int pinLDR = A0;       // Pino do Photo-Resistor

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

// Símbolo de graus Celsius:
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

// Função para definir a cor do LED RGB
void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

// Função para simular a luminosidade
float fakeLuminosity() {
  int potValue = analogRead(A3);
  return map(potValue, 0, 1023, 0, 100);
}

int currentScreen;                        // Tela atual
int buttonScreenState = 0;                // Estado do botão de tela
int lastButtonScreenState = 0;            // Último estado do botão de tela
int buttonConfigState = 0;                // Estado do botão de configuração
int lastButtonConfigState = 0;            // Último estado do botão de configuração
bool configMode = false;                  // Modo de configuração
int tempUnit = 0;                         // Unidade de temperatura
int callLog = 0;                          // Chamada de log
unsigned long screenButtonPressTime = 0;  // Tempo de pressionamento do botão de tela
const int holdTime = 3000;                // Tempo necessário para ativar a tela 3 (3 segundos)

// Configuração do intervalo por millis()
unsigned long writeDelay = 0;
unsigned long logDelay = 0;
const long interval = 5000;  // Intervalo de 5 segundos

// Configurações da EEPROM
const int maxRecords = 64;                 // Máximo de registros
const int recordSize = 10;                 // Tamanho de cada registro em bytes
int startAddress = 0;                      // Endereço inicial
int endAddress = maxRecords * recordSize;  // Endereço final
int currentAddress = 0;                    // Endereço atual

int lastLoggedMinute = -1;  // Último minuto registrado

void setup() {
  currentScreen = 0;   // Inicializa a tela atual
  Serial.begin(9600);  // Inicializa a comunicação serial
  Serial.println("Sistema Inicializado!");

  pinMode(redPin, OUTPUT);                            // Define o pino do LED vermelho como saída
  pinMode(greenPin, OUTPUT);                          // Define o pino do LED verde como saída
  pinMode(bluePin, OUTPUT);                           // Define o pino do LED azul como saída
  pinMode(buttonScreen, INPUT_PULLUP);                // Define o pino do botão de tela como entrada com pull-up
  pinMode(buttonConfig, INPUT_PULLUP);                // Define o pino do botão de configuração como entrada com pull-up
  lastButtonScreenState = digitalRead(buttonScreen);  // Lê o estado inicial do botão de tela
  lastButtonConfigState = digitalRead(buttonConfig);  // Lê o estado inicial do botão de configuração

  dht.begin();  // Inicializa o sensor DHT

  if (!rtc.begin()) {  // Verifica se o RTC está presente
    Serial.println("RTC não encontrado");
    while (1);  // Se não encontrado, entra em loop infinito
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // Ajusta o RTC com a data e hora de compilação

  lcd.init();                      // Inicializa o display LCD
  lcd.backlight();                 // Liga a luz de fundo do display
  lcd.clear();                     // Limpa o display
  playMelodyPirateAndShipGoing();  // Toca a melodia inicial
  lcd.clear();                     // Limpa o display novamente
  EEPROM.begin();                  // Inicializa a EEPROM
}

void loop() {
  unsigned long currentMillis = millis();  // Obtém o tempo atual em milissegundos

  buttonScreenState = digitalRead(buttonScreen);  // Lê o estado do botão de tela
  buttonConfigState = digitalRead(buttonConfig);  // Lê o estado do botão de configuração

  float temp;                                  // Variável para armazenar a temperatura
  float temperatureC = dht.readTemperature();  // Lê a temperatura em Celsius do sensor DHT

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
      temp = NAN;  // Define como NaN (Not a Number) para indicar erro
      break;
  }

  float humid = dht.readHumidity();       // Lê a umidade do sensor DHT
  float luminosity = analogRead(pinLDR);  // Lê a luminosidade do Photo-Resistor

#if IS_REAL_SYSTEM
  float lumen = map(luminosity, 0, 512, 0, 100);  // Mapeia a luminosidade para um valor entre 0 e 100
#else
  float lumen = fakeLuminosity();  // Usa a função de luminosidade falsa no simulador
#endif

  // Definir a faixa de temperatura
  float minTemp = 15.0;      // Temperatura mínima
  float maxTemp = 25.0;      // Temperatura máxima
  float maxHumidity = 50;    // Umidade máxima
  float minHumidity = 30;    // Umidade mínima
  float maxLuminosity = 30;  // Luminosidade máxima

  DateTime now = rtc.now();  // Obtém a data e hora atual do RTC

  // Verifica se os valores estão fora dos limites definidos
  if (temperatureC < minTemp || temperatureC > maxTemp || humid < minHumidity || humid > maxHumidity || lumen > maxLuminosity) {
    if (currentMillis - writeDelay >= interval) {
      writeDelay = currentMillis;
      int tempCInt = (int)(temperatureC * 100);  // Converte a temperatura para inteiro
      int humidInt = (int)(humid * 100);         // Converte a umidade para inteiro
      int lumenInt = (int)(lumen * 100);         // Converte a luminosidade para inteiro

      EEPROM.put(currentAddress, now.unixtime());  // Armazena o timestamp na EEPROM
      EEPROM.put(currentAddress + 4, tempCInt);    // Armazena a temperatura na EEPROM
      EEPROM.put(currentAddress + 6, humidInt);    // Armazena a umidade na EEPROM
      EEPROM.put(currentAddress + 8, lumenInt);    // Armazena a luminosidade na EEPROM
      getNextAddress();                            // Obtém o próximo endereço para armazenamento
    }
    tone(SPEAKER_PIN, 1000, 500);  // Emite um som no buzzer
  }

  const int debounceDelay = 50;        // Tempo de debounce para os botões
  unsigned long lastDebounceTime = 0;  // Último tempo de debounce
  unsigned long configStartTime;       // Tempo de início da configuração
  int configEndTime = 2000;            // Tempo de fim da configuração

  // Verifica se o botão de tela foi pressionado
  if (buttonScreenState != lastButtonScreenState && buttonScreenState == HIGH) {
    if (millis() - lastDebounceTime > debounceDelay) {
      configMode = false;                       // Sai do modo de configuração
      lcd.clear();                              // Limpa o display
      currentScreen = (currentScreen + 1) % 4;  // Alterna entre as telas
      lastDebounceTime = millis();              // Atualiza o tempo de debounce
    }
  }
  lastButtonScreenState = buttonScreenState;  // Atualiza o estado do botão de tela

  // Verifica se o botão de configuração foi pressionado
  if (buttonConfigState != lastButtonConfigState && buttonConfigState == HIGH) {
    if (millis() - lastDebounceTime > debounceDelay) {
      lcd.clear();                  // Limpa o display
      lastDebounceTime = millis();  // Atualiza o tempo de debounce
      if (!configMode) {
        configMode = true;           // Entra no modo de configuração
        configStartTime = millis();  // Atualiza o tempo de início da configuração
      } else {
        configStartTime = millis();  // Atualiza o tempo de início da configuração
        switch (currentScreen) {
          case 0:
            tempUnit = (tempUnit > 1) ? 0 : tempUnit + 1;  // Alterna entre as unidades de temperatura
            break;
          case 3:
            callLog = (callLog > 0) ? 0 : callLog + 1;  // Alterna entre as opções de log
            break;
        }
      }
    }
  }

  lastButtonConfigState = buttonConfigState;  // Atualiza o estado do botão de configuração

  // Exibir a tela correspondente
  if (configMode) {
    switch (currentScreen) {
      case 0:  // Tela de configuração para temperatura
        lcd.setCursor(0, 0);
        lcd.print("Escala:");
        lcd.setCursor(0, 1);
        switch (tempUnit) {  // Exibe a unidade de temperatura selecionada
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
      case 3:  // Tela de configuração para o Menu
        lcd.setCursor(0, 0);
        lcd.print("Ajustes:");
        lcd.setCursor(0, 1);
        switch (callLog) {
          case 0:  // Printar Log
            lcd.print(">Printar Log");
            if (millis() - configStartTime >= configEndTime) {
              lcd.setCursor(0, 1);
              lcd.print("Puxando Dados...");
              get_current(temperatureC, humid, lumen);  // Chama a função get_current() após 5 segundos
              configMode = false;                       // Sai do modo de configuração
              lcd.clear();                              // Limpa a tela
            }
            break;
          case 1:  // Chamar EEPROM
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
    switch (currentScreen) {  // Exibe a tela correspondente
      case 0:                 // Tela de temperatura
        lcd.createChar(5, degreesSymbol);
        lcd.setCursor(0, 0);
        lcd.print("Temperatura");
        lcd.setCursor(0, 1);
        lcd.print(temp);
        switch (tempUnit) {
          case 0:  // Celsius
            lcd.write(5);
            lcd.print("C  ");
            break;
          case 1:  // Fahrenheit
            lcd.write(5);
            lcd.print("F  ");
            break;
          case 2:  // Kelvin
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
      case 1:  // Tela de umidade
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
        IconMenu();  // Chama a função IconMenu() para exibir o menu com ícones, podendo ser trocado pelo TextMenu()
        break;
    }

    if (currentMillis - logDelay >= interval) {
      logDelay = currentMillis;
    }
  }
}

// Função para obter o próximo endereço de armazenamento na EEPROM
void getNextAddress() {
  currentAddress += recordSize;
  if (currentAddress >= endAddress) {
    currentAddress = startAddress;  // Volta para o começo se atingir o limite
  }
}

// Função para obter as medidas atuais
void get_current(float temperatureC, float humid, float luminosity) {
  // Imprime cabeçalho para as medidas atuais
  Serial.println("Current measures");
  Serial.println("Timestamp\t\tTemperature\tHumidity\tLuminosity");

  // Obtém a data e hora atual do RTC
  DateTime now = rtc.now();

  // Verifica se os dados são válidos antes de imprimir
  if (now.unixtime() != 0xFFFFFFFF) {  // 0xFFFFFFFF é o valor padrão de uma EEPROM não inicializada
    // Converte o timestamp para um objeto DateTime
    DateTime dt = DateTime(now.unixtime());
    // Imprime o timestamp completo
    Serial.print(dt.timestamp(DateTime::TIMESTAMP_FULL));
    Serial.print("\t");
    // Imprime a temperatura em Celsius
    Serial.print(temperatureC);
    Serial.print(" C\t\t");
    // Imprime a umidade
    Serial.print(humid);
    Serial.print(" %\t\t");
    // Imprime a luminosidade
    Serial.print(luminosity);
    Serial.println(" %");
  }
}

// Função para obter os registros armazenados na EEPROM
void get_log() {
  // Imprime cabeçalho para os dados armazenados na EEPROM
  Serial.println("Data stored in EEPROM:");
  Serial.println("Timestamp\t\tTemperature\tHumidity\tLuminosity");

  // Itera sobre os endereços da EEPROM para ler os registros
  for (int address = startAddress; address < endAddress; address += recordSize) {
    long timeStamp;
    int tempInt, humiInt, lumenInt;

    // Ler dados da EEPROM
    EEPROM.get(address, timeStamp);
    EEPROM.get(address + 4, tempInt);
    EEPROM.get(address + 6, humiInt);
    EEPROM.get(address + 8, lumenInt);

    // Converter valores para float
    float temperature = tempInt / 100.0;
    float humidity = humiInt / 100.0;
    float luminosity = lumenInt / 100.0;

    // Verificar se os dados são válidos antes de imprimir
    if (timeStamp != 0xFFFFFFFF) {  // 0xFFFFFFFF é o valor padrão de uma EEPROM não inicializada
      // Converte o timestamp para um objeto DateTime
      DateTime dt = DateTime(timeStamp);
      // Imprime o timestamp completo
      Serial.print(dt.timestamp(DateTime::TIMESTAMP_FULL));
      Serial.print("\t");
      // Imprime a temperatura em Celsius
      Serial.print(temperature);
      Serial.print(" C\t\t");
      // Imprime a umidade
      Serial.print(humidity);
      Serial.print(" %\t\t");
      // Imprime a luminosidade
      Serial.print(luminosity);
      Serial.println(" %");
    }
  }
}

//Versão do menu sem ícones
void TextMenu() {
  // Define o cursor na posição inicial da primeira linha do LCD
  lcd.setCursor(0, 0);
  // Imprime o título do menu
  lcd.print("- Mar de Vinho -");
  // Define o cursor na posição inicial da segunda linha do LCD
  lcd.setCursor(0, 1);
  // Imprime a opção de menu
  lcd.print("    [MENU]");
}

// Versão do menu com ícones
void IconMenu() {
  // Define os bytes para os caracteres personalizados
  byte name1x15[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  byte name0x6[] = { 0x00, 0x00, 0x00, 0x01, 0x03, 0x02, 0x02, 0x01 };
  byte name0x7[] = { 0x03, 0x01, 0x00, 0x1B, 0x1D, 0x1C, 0x0C, 0x19 };
  byte name0x8[] = { 0x00, 0x10, 0x00, 0x1B, 0x1D, 0x1D, 0x0C, 0x19 };
  byte name0x9[] = { 0x00, 0x00, 0x00, 0x10, 0x18, 0x18, 0x18, 0x10 };
  byte name1x7[] = { 0x00, 0x17, 0x13, 0x0E, 0x00, 0x00, 0x02, 0x01 };
  byte name1x8[] = { 0x00, 0x07, 0x03, 0x0E, 0x00, 0x1C, 0x0C, 0x18 };

  // Cria e exibe os caracteres personalizados no LCD
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

// Função para tocar a melodia inicial
void playMelodyPirateAndShipGoing() {
  // Chama a função para criar o navio no LCD
  ship();
  // Define o cursor na posição inicial da segunda linha do LCD
  lcd.setCursor(27, 1);
  // Imprime o texto "Wine Sea"
  lcd.print("Wine Sea");
  // Toca a melodia nota por nota
  for (int thisNote = 0; thisNote < 61; thisNote++) {
    // Move o display para a direita
    lcd.scrollDisplayRight();

    // Calcula a duração da nota
    float noteDuration = 250 / noteDurationsPirate[thisNote];
    // Toca a nota no buzzer
    tone(SPEAKER_PIN, melodyPirate[thisNote], noteDuration);

    // Calcula o tempo de pausa entre as notas
    int pauseBetweenNotes = noteDuration * 1.40;
    // Aguarda o tempo de pausa
    delay(pauseBetweenNotes);

    // Para de tocar a nota
    noTone(SPEAKER_PIN);
  }
}

// Função para criar o navio no LCD
void ship() {
  // Define os bytes para os caracteres personalizados do navio
  byte name0x0[] = { B01111, B11001, B11001, B01100, B00100, B00100, B00100, B01100 };
  byte name0x1[] = { B11111, B00100, B00100, B10010, B10010, B10010, B10010, B10010 };
  byte name0x2[] = { B11110, B10010, B10011, B01001, B01001, B01001, B01001, B01001 };
  byte name0x3[] = { B00000, B00000, B00000, B00000, B00000, B00110, B00011, B00011 };
  byte name1x0[] = { B11001, B11001, B00111, B00000, B10000, B11111, B11111, B01111 };
  byte name1x1[] = { B00100, B00100, B11111, B01010, B01010, B01010, B11111, B11111 };
  byte name1x2[] = { B10011, B10010, B11110, B00000, B00011, B11111, B11111, B11111 };
  byte name1x3[] = { B00011, B00111, B01111, B11110, B11110, B11100, B11000, B10000 };

  // Cria e exibe os caracteres personalizados do navio no LCD
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
