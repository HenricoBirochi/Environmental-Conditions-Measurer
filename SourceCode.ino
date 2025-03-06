#include <Wire.h> // Biblioteca utilizada para fazer a comunicação com o I2C
#include <LiquidCrystal_I2C.h> // Biblioteca utilizada para fazer a comunicação com o display 20x4 
#include <DHT.h>

#define col 16 // Serve para definir o numero de colunas do display utilizado
#define lin  2 // Serve para definir o numero de linhas do display utilizado
#define ende  0x27 // Serve para definir o endereço do display.

LiquidCrystal_I2C lcd(ende,col,lin); // Chamada da funcação LiquidCrystal para ser usada com o I2C

#define DHTPIN 9 // Pino do DHT
#define DHTTYPE DHT11  // Tipo de sensor DHT

DHT dht(DHTPIN, DHTTYPE); // DHT

float lastAvgTemp; // Armazena a última média de temperatura
float lastAvgHumd; // Armazena a última média de umidade

float tempReadings[10]; // Array para armazenar as últimas 10 leituras de temperatura
float humdReadings[10]; // Array para armazenar as últimas 10 leituras de umidade
int currentIndex = 0; // Índice para rastrear a posição atual no array de leituras

int lightLevel; //Nível de luz

const int pinLDR = A0;  // Pino do LED do LDR
const int ledRed = 8;   // Pino do LED vermelho
const int ledYel = 7;   // Pino do LED amarelo
const int ledGre = 6;   // Pino do LED verde
const int pinBUZ = 13;  // Pino do buzzer

void setup() //Incia o display
{  
  lcd.init(); // Serve para iniciar a comunicação com o display já conectado
  lcd.backlight(); // Serve para ligar a luz do display
  lcd.clear(); // Serve para limpar a tela do display
  dht.begin(); // Inicializa o sensor DHT

  analogWrite(pinBUZ, OUTPUT); // Inicializa buzzer

  // Inicializa LEDs
  pinMode(ledRed, OUTPUT);
  pinMode(ledYel, OUTPUT);
  pinMode(ledGre, OUTPUT);

  // Inicializa LDR
  pinMode(pinLDR, INPUT);

  booting(); // Exibe uma mensagem inicial no LCD
  delay(1000); // Aguarda 1 segundo
  lcd.clear(); // Limpa o LCD
  lcd.setCursor(4, 0);
  lcd.print("Loading"); // Exibe uma mensagem de loading para as primeiras 10 leituras
}

void loop() {
  int valorLDR = analogRead(pinLDR); 
  lightLevel = map(valorLDR, 500, 969, 0, 100); //Utiliza a função map para interpretar os valores do LDR

  float temp = dht.readTemperature(); // Lê a temperatura do sensor DHT
  float humid = dht.readHumidity();   // Lê a umidade do sensor DHT

  verifyLDR(); // Verifica o nível de luz


    //Verifica se há algum LED de 'atenção' ligado e se o ambiente está escuro
  if (digitalRead(ledYel) == HIGH || digitalRead(ledRed) == HIGH && valorLDR < 40) {

    // Verifica se o LED amarelo pode ser desligado
    if (10 < lastAvgTemp && lastAvgTemp < 15) {
      digitalWrite(ledYel, LOW);
    }
    // Verifica se o LED vermelho pode ser desligado
    if (50 < lastAvgHumd && lastAvgHumd < 70) {
      digitalWrite(ledRed, LOW);
    }
  }
  

  // Verifica se o buzzer pode ser desligado. Exije ambiente escuro
  if (digitalRead(ledYel) == LOW && digitalRead(ledRed) == LOW && valorLDR < 40) {
    noTone(pinBUZ);
  }

  tempReadings[currentIndex] = temp;  // Armazena a leitura atual da temperatura
  humdReadings[currentIndex] = humid;  // Armazena a leitura atual da umidade

  currentIndex = (currentIndex + 1) % 10; // Atualiza o índice para a próxima leitura

  if (currentIndex == 9) {
    tenthRead();
  }

  // Exibe informações no monitor serial
  serialLog(temp, humid, valorLDR);

  delay(500); // Aguarda 500 milissegundos
}

// Exibe uma mensagem de inicialização no LCD
void booting() {
  lcd.setCursor(3, 1);
  tone(pinBUZ, 400);
  lcd.print("NEXT FUTURE "); // Nome da empresa
  noTone(pinBUZ);
  delay(1000);
  move_pac(); // Exibe animação do pacman
  lcd.clear(); // Limpa o LCD
  delay(200);
  slogan(); // Exibe um slogan animado no LCD
}

// Aniamção do "Pac-Man" comendo a logo da empresa no LCD
void move_pac() {
  String line = "NEXT FUTURE";
  ghost(); // Exibe uma imagem de fantasma no LCD
  move_image(14, 1, 10); // Move e exibe uma imagem no LCD
  for (int pos = 0; pos < 15; pos++) {
    move(pos, line); // Move uma imagem no LCD
    line.remove(0, 1);
    tone(pinBUZ, 500);
    delay(150);
    noTone(pinBUZ);
    lcd.clear(); // Limpa o LCD
  }
}

// Move a imagem de "Pac-Man" e alterna entre boca aberta e fechada
void move(int pos, String texto) {
  if (pos % 2 == 0) {
    fechado(); // Se a posição for par, a boca estará fechada

  } else {
    aberto();  // Se a posição for ímpar, a boca estará aberta
  }
  if (texto != "") { // Verifica se a string não está vazia
    lcd.setCursor(pos + 3, 1); // Define a posição do cursor no LCD
    lcd.print(texto); // Imprime texto no LCD
  }

  ghost_move(pos);

  move_image(pos + 0, 0, 4);
  move_image(pos + 1, 0, 5);
  move_image(pos + 2, 0, 6);
  move_image(pos + 0, 1, 7);
  move_image(pos + 1, 1, 8);
  move_image(pos + 2, 1, 9);

  delay(300); // Aguarda 300 milissegundos antes de continuar a animação
}

// Define a imagem de "Pac-Man" com a boca fechada
void fechado() {
  // Define a matriz de bits para a imagem de "Pac-Man" com a boca fechada
  byte i4[8] = {B00000, B00000, B00000, B00001, B00010, B00010, B00100, B00100};
  byte i5[8] = {B00000, B00000, B11111, B00000, B00000, B00110, B00110, B00000};
  byte i6[8] = {B00000, B00000, B00000, B10000, B01000, B01000, B00100, B00100};
  byte i7[8] = {B00100, B00100, B00010, B00010, B00001, B00000, B00000, B00000};
  byte i8[8] = {B00000, B00000, B00000, B00000, B00000, B11111, B00000, B00000};
  byte i9[8] = {B00100, B00100, B01000, B01000, B10000, B00000, B00000, B00000};

  // Cria os caracteres personalizados para a boca fechada de "Pac-Man"
  lcd.createChar(4, i4);
  lcd.createChar(5, i5);
  lcd.createChar(6, i6);
  lcd.createChar(7, i7);
  lcd.createChar(10, i8);
  lcd.createChar(9, i9);
}

// Define a imagem de "Pac-Man" com a boca aberta
void aberto() {
  // Define a matriz de bits para a imagem de "Pac-Man" com a boca aberta
  byte i4[8] = {B00000, B00000, B00000, B00001, B00010, B00010, B00100, B00100};
  byte i5[8] = {B00000, B00000, B11111, B00000, B00000, B00110, B00110, B00000};
  byte i6[8] = {B00000, B00000, B00000, B10000, B01000, B00100, B01000, B10000};
  byte i7[8] = {B00100, B00100, B00010, B00010, B00001, B00000, B00000, B00000};
  byte i8[8] = {B00000, B00000, B00000, B00000, B00000, B11111, B00000, B00000};
  byte i9[8] = {B10000, B01000, B00100, B01000, B10000, B00000, B00000, B00000};

  // Cria os caracteres personalizados para a boca aberta de "Pac-Man"
  lcd.createChar(4, i4);
  lcd.createChar(5, i5);
  lcd.createChar(6, i6);
  lcd.createChar(7, i7);
  lcd.createChar(8, i8);
  lcd.createChar(9, i9);
}

// Define a imagem de um fantasma
void ghost() {
  byte ghost[8] = {B01110, B11111, B10101, B11011, B11111, B11111, B10101, B00000};
  lcd.createChar(10, ghost);
  move_image(14, 1, 10);
}

// Move a imagem de um fantasma no LCD
void ghost_move(int pos) {
  byte ghost[8] = {B01110, B11111, B10101, B11011, B11111, B11111, B10101, B00000};
  lcd.createChar(10, ghost);
  if (pos % 2 != 0) {
    move_image(15, 1, 10);
  } else {
    move_image(14, 1, 10);
  }
}

// Exibe um slogan animado no LCD
void slogan() {
  String line = "FUTURE IS NOW!";
  for (int i = 0; i < line.length(); i++) {

    //Animação de letras caindo
    lcd.setCursor(i + 1, 0);
    lcd.print(line[i]);
    delay(150);
    lcd.setCursor(i + 1, 0);
    lcd.print(" ");
    lcd.setCursor(i + 1, 1);
    lcd.print(line[i]);

    // Caso o char não seja um blankspace, ativa o buzzer
    if (!(isWhitespace(line[i]))) {
      tone(pinBUZ, 250);
      delay(150);
      noTone(pinBUZ);
    }
  }
}

// Exibe informações no monitor serial
void serialLog(float temp, float humid, int valorLDR) {
  Serial.println("Leitura: " + String(currentIndex + 1));
  Serial.println("Temp: " + String(temp) + "°C");
  Serial.println("Last Avarage Temp: " + String(lastAvgTemp) + "°C");
  Serial.println("Humidity: " + String(humid) + " %");
  Serial.println("Last Avarage Humidity: " + String(lastAvgHumd) + " %");
  Serial.println("Luminosidade: " + String(lightLevel));
  Serial.println("ValorLDR: " + String(valorLDR));
  Serial.println("---");
}

// Calcula a média das últimas 10 leituras e exibe as telas de cada campo
void tenthRead() {
  float sumTemp = 0;
  float sumHumd = 0;

  for (int i = 0; i < 10; i++) {
    sumTemp += tempReadings[i];
    sumHumd += humdReadings[i];
  }

  lastAvgTemp = sumTemp / 10;
  lastAvgHumd = sumHumd / 10;

  show_data();
}

// Exibe dados no LCD
void show_data() {
  show_temp();
  delay(5000);
  show_humid();
  delay(5000);
  show_light();
}

// Exibe a temperatura no LCD
void show_temp() {
  if ((lastAvgTemp < 10 || lastAvgTemp > 15) && lastAvgTemp != 0) {
    digitalWrite(ledYel, HIGH);
    digitalWrite(ledGre, LOW);
    tone(pinBUZ, 500);
  }
  lcd.clear(); // Limpa o LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: " + String(lastAvgTemp, 1) + " C");
  term_image(lastAvgTemp); // Exibe uma imagem correspondente à temperatura no LCD
}

// Exibe a umidade no LCD
void show_humid() {
  if ((lastAvgHumd < 50 || lastAvgHumd > 70) && lastAvgHumd != 0) {
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGre, LOW);
    tone(pinBUZ, 400);
  }
  lcd.clear(); // Limpa o LCD
  lcd.setCursor(0, 0);
  lcd.print("Humid: " + String(lastAvgHumd, 1) + " %");
  humid_image(lastAvgHumd);
}

// Exibe a luminosidade no LCD
void show_light() {
  lcd.clear(); // Limpa o LCD
  lcd.setCursor(0, 0);
  lcd.print("Light Lvl: " + String(lightLevel));
  light_image();
}

// Define e exibe uma imagem correspondente à temperatura no LCD
void term_image(float temp) {
  byte termLow2[8] = {B01010, B01010, B01010, B01010, B10001, B10001, B10001, B01110};
  byte termLow1[8] = {B00100, B01010, B01010, B01010, B01010, B01010, B01010, B01010};

  byte termMid2[8] = {B01010, B01010, B01110, B01110, B11111, B11111, B11111, B01110};
  byte termMid1[8] = {B00100, B01010, B01010, B01010, B01010, B01010, B01010, B01010};

  byte termHig2[8] = {B01110, B01110, B01110, B01110, B11111, B11111, B11111, B01110};
  byte termHig1[8] = {B00100, B01110, B01110, B01110, B01110, B01110, B01110, B01110};

  // Cria os caracteres personalizados e os exibe no LCD, dependendo da temperatura
  if (temp < 10) {
    lcd.createChar(9, termLow1);
    lcd.createChar(10, termLow2);
    messageTemplate("Temp. Baixa");

  } else if (temp > 15) {
    lcd.createChar(9, termHig1);
    lcd.createChar(10, termHig2);
    messageTemplate("Temp. Alta");

  } else { // Temperatura ideal
    lcd.createChar(9, termMid1);
    lcd.createChar(10, termMid2);
    messageTemplate("Temp. OK");
  }
  move_image(14, 0, 9); // Move e exibe a primeira imagem no LCD
  move_image(14, 1, 10); // Move e exibe a segunda imagem no LCD
}

// Define e exibe uma imagem correspondente à umidade no LCD
void humid_image(float humid) {
  byte humidLow_1[8] = {B00111, B00011, B00001, B00000, B00000, B00000, B00000, B00000};
  byte humidLow_2[8] = {B00000, B00000, B00000, B00000, B00001, B00001, B00011, B00111};
  byte humidLow_3[8] = {B00000, B00000, B00000, B00000, B10000, B10000, B01000, B11100};
  byte humidLow_4[8] = {B11100, B11000, B10000, B00000, B00000, B00000, B00000, B00000};

  byte humidMid_1[8] = {B01111, B01111, B00111, B00011, B00000, B00000, B00000, B00000};
  byte humidMid_2[8] = {B00000, B00000, B00001, B00001, B00011, B00111, B00111, B01111};
  byte humidMid_3[8] = {B00000, B00000, B10000, B10000, B01000, B00100, B10100, B11110};
  byte humidMid_4[8] = {B11110, B11110, B11100, B11000, B00000, B00000, B00000, B00000};


  byte humidHigh_1[8] = {B11111, B11111, B11111, B01111, B00111, B00011, B00000, B00000};
  byte humidHigh_2[8] = {B00011, B00111, B01111, B01111, B11111, B11111, B11111, B11111};
  byte humidHigh_3[8] = {B11000, B10100, B10010, B10001, B10001, B11001, B11111, B11111};
  byte humidHigh_4[8] = {B11111, B11111, B11111, B11110, B11100, B11000, B00000, B00000};

  if (humid < 50) {
    lcd.createChar(11, humidLow_1);
    lcd.createChar(12, humidLow_2);
    lcd.createChar(13, humidLow_3);
    lcd.createChar(14, humidLow_4);

    messageTemplate("Humid. Baixa");

  } else if (humid > 70) {
    lcd.createChar(11, humidHigh_1);
    lcd.createChar(12, humidHigh_2);
    lcd.createChar(13, humidHigh_3);
    lcd.createChar(14, humidHigh_4);

    messageTemplate("Humid. Alta");

  } else {
    lcd.createChar(11, humidMid_1);
    lcd.createChar(12, humidMid_2);
    lcd.createChar(13, humidMid_3);
    lcd.createChar(14, humidMid_4);

    messageTemplate("Humid. OK");
  }

  move_image(14, 1, 11);
  move_image(14, 0, 12);
  move_image(15, 0, 13);
  move_image(15, 1, 14);
}

//Define e exibe uma imagem correspondente ao nível de luz no LDR
void light_image() {
  byte lampLow1[8] = {B00100, B00100, B00100, B00100, B01110, B01110, B11111, B10001};
  byte lampLow2[8] = {B10001, B10001, B10001, B01110, B00000, B00000, B00000, B00000};

  byte lampMid1[8] = {B00100, B00100, B00100, B00100, B01110, B01110, B11111, B10001};
  byte lampMid2[8] = {B10001, B11111, B11111, B01110, B00000, B00000, B00000, B00000};

  byte lampHigh1[8] = {B00100, B00100, B00100, B00100, B01110, B01110, B11111, B11111};
  byte lampHigh2[8] = {B11111, B11111, B11111, B01110, B00000, B00000, B00000, B00000};

  if (lightLevel >= 40 && lightLevel < 90) {
    lcd.createChar(10, lampLow1);
    lcd.createChar(11, lampLow2);
    messageTemplate("Ambiente Meia-Luz");

  } else if (lightLevel >= 90) {
    lcd.createChar(10, lampHigh1);
    lcd.createChar(11, lampHigh2);
    messageTemplate("Ambiente Muito Claro");

  } else {
    lcd.createChar(10, lampMid1);
    lcd.createChar(11, lampMid2);
     messageTemplate("Ambiente Escuro");
  }

  move_image(15, 0, 10);
  move_image(15, 1, 11);
}

// Verifica o nível de luz do ambiente
void verifyLDR() {
  // Em um ambiente muito claro, liga a luz vermelha
  if (lightLevel >= 80) {
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGre, LOW);

    //Caso a temperatura tenha sido normalizada, desliga a luz de alerta da temperatura e seu som no buzzer
    if (lastAvgTemp >= 10 && lastAvgTemp <= 15){
      digitalWrite(ledYel, LOW);
      noTone(pinBUZ);
    }
    tone(pinBUZ, 600);

  // Em um ambiente um pouco claro, liga a luz amarela
  } else if (lightLevel < 80 && lightLevel >= 40) {
    digitalWrite(ledYel, HIGH);
    digitalWrite(ledGre, LOW);

    //Caso a umidade tenha sido normalizada, desliga a luz de alerta da umidade e seu som no buzzer
    if (lastAvgHumd >= 50 && lastAvgHumd <= 70 ){
      digitalWrite(ledRed, LOW);
      noTone(pinBUZ);
    }
    tone(pinBUZ, 400);
  } else {
    // Caso todos os dados estejam normalizados e o ambiente esteja escuro, desliga todos os LEDs acesos, desliga o buzzer e liga o LED verde
    if (lastAvgHumd >= 50 && lastAvgHumd <= 70 && lastAvgTemp >= 10 && lastAvgTemp <= 30){
      digitalWrite(ledGre, HIGH);
      digitalWrite(ledRed, LOW);
      digitalWrite(ledYel, LOW);
      noTone(pinBUZ);
    }
    
  }
}

// Move uma imagem no LCD
void move_image(int x, int y, int byt) {
  lcd.setCursor(x, y); // Define a posição do cursor no LCD
  lcd.write(byte(byt)); // Exibe o caractere personalizado no LCD
}

void messageTemplate(String msg){
  lcd.setCursor(0, 1);
  lcd.print(msg);
}

