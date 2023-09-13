//======================================(LIB's)=========================================//

#include "dht.h" //necessario instalar essas libs
#include "RTClib.h" //necessario instalar essas libs
#include <EEPROM.h> //INCLUSÃO DA BIBLIOTECA
#include <LiquidCrystal.h> //INCLUSÃO DA BIBLIOTECA
#include <Wire.h> //INCLUSÃO DA BIBLIOTECA

//=====================================================================================//

//======================================(PORTAS)======================================//

LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2); // Display

const int LDR_PIN = A0; // Respectivas portas que os componentes estão associados (LDR, DHT, Alarme sonoro(buzzer), led vermelho e led verde)
const int pinoDHT11 = A2;
const int BUZZER_PIN = 8;
int ledVermelho = 6;
int ledVerde = 7;
bool inicio = true;

//=====================================================================================//

//====================================(VARIÁVEIS)======================================//

dht DHT; //OBJETO dht DO TIPO DHT (para conseguir chamar as funções do dht)

RTC_DS3231 rtc; //OBJETO rtc (chama as funções)

char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"}; //dias da semana para o rtc

int temperatura = 0; //declaração de variáveis
int umidade = 0;
int luminosidade = 0;

int temperaturaMedia = 0; //declaração de variáveis para as médias
int umidadeMedia = 0;
int luminosidadeMedia = 0;

int leituras = 0;

unsigned long intervalo = 60000; //intervalo de 1 minuto para cálculo das médias
unsigned long ultimaLeitura = 0; //variável para salvar ultima leitura

//=====================================================================================//

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0


//Deixa mais rápido ou lento
int tempo = 200;

//Porta do buzzer
int buzzer = 8;

// Melodia
int melody[] = {
    
  NOTE_AS4,8, NOTE_AS4,8, NOTE_AS4,8,//1
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  NOTE_AS5,8, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,2, NOTE_C5,8, NOTE_C5,8, NOTE_C5,8,
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4, //8  
  NOTE_AS5,8, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,2, NOTE_C5,-8, NOTE_C5,16, 
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_F5,8, NOTE_G5,8, NOTE_A5,8, NOTE_G5,4, NOTE_D5,8, NOTE_E5,4,NOTE_C5,-8, NOTE_C5,16,
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  
  NOTE_C6,-8, NOTE_G5,16, NOTE_G5,2, REST,8, NOTE_C5,8,//13
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_F5,8, NOTE_G5,8, NOTE_A5,8, NOTE_G5,4, NOTE_D5,8, NOTE_E5,4,NOTE_C6,-8, NOTE_C6,16,
  NOTE_F6,4, NOTE_DS6,8, NOTE_CS6,4, NOTE_C6,8, NOTE_AS5,4, NOTE_GS5,8, NOTE_G5,4, NOTE_F5,8,
  NOTE_C6,1
  
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;

int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;  


//=====================================FOGUETE========================================//

//montagem dos bytes para o desenho do foguete no início

byte fog[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00001,
  B00111,
  B11001
};

byte fog2[8] = {
  B01100,
  B01010,
  B01001,
  B01000,
  B01111,
  B11000,
  B01000,
  B01000
};

byte fog3[8] = {
  B00000,
  B00000,
  B00000,
  B10000,
  B11111,
  B00000,
  B00110,
  B01001
};

byte fog4[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11100,
  B00110,
  B00011,
  B00001
};

byte fog5[8] = {
  B00111,
  B00001,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte fog6[8] = {
  B01000,
  B11000,
  B01111,
  B01000,
  B01001,
  B01010,
  B01100,
  B00000
};

byte fog7[8] = {
  B00110,
  B00000,
  B11111,
  B10000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte fog8[8] = {
  B00011,
  B00110,
  B11100,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

//=====================================================================================//
void playMusic(){
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration*0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);
    
    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}


void setup()

{

//inicialização dos componentes (INPUT = Entrada de dados / OUTPUT = Saída de dados)

  rtc.begin();
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(LDR_PIN, INPUT);
  pinMode(pinoDHT11, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);

//Atrela um byte "desenhado" antes a um char (vai de 0 até 7, ou seja, 8 bytes

  lcd.createChar(0, fog);
  lcd.createChar(1, fog2);
  lcd.createChar(2, fog3);
  lcd.createChar(3, fog4);
  lcd.createChar(4, fog5);
  lcd.createChar(5, fog6);
  lcd.createChar(6, fog7);
  lcd.createChar(7, fog8);

  if(!rtc.begin()) { // SE O RTC NÃO FOR INICIALIZADO, FAZ

    Serial.println("DS3231 não encontrado"); //IMPRIME O TEXTO NO MONITOR SERIAL

    while(1); //SEMPRE ENTRE NO LOOP

  }

  if(rtc.lostPower()){ //SE RTC FOI LIGADO PELA PRIMEIRA VEZ / FICOU SEM ENERGIA / ESGOTOU A BATERIA, FAZ

    Serial.println("DS3231 OK!"); //IMPRIME O TEXTO NO MONITOR SERIAL

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //CAPTURA A DATA E HORA EM QUE O SKETCH É COMPILADO

  }

  delay(100); //INTERVALO DE 100 MILISSEGUNDOS

}

void loop()

{
  
  if (inicio) //loop para inicialização do foguete
  {
    lcd.setCursor(0, 0);  
    lcd.write(byte(0));
    lcd.setCursor(1, 0);  
    lcd.write(byte(1));
    lcd.setCursor(2, 0);  
    lcd.write(byte(2));
    lcd.setCursor(3, 0);
    lcd.write(byte(3));
    lcd.setCursor(0, 1);  
    lcd.write(byte(4));
    lcd.setCursor(1, 1);  
    lcd.write(byte(5));
    lcd.setCursor(2, 1);
    lcd.write(byte(6));
    lcd.setCursor(3, 1);
    lcd.write(byte(7));
    delay(1000);
    playMusic();
    lcd.clear();
    lcd.setCursor(4, 0);  
    lcd.write(byte(0));
    lcd.setCursor(5, 0);  
    lcd.write(byte(1));
    lcd.setCursor(6, 0);  
    lcd.write(byte(2));
    lcd.setCursor(7, 0);
    lcd.write(byte(3));
    lcd.setCursor(4, 1);  
    lcd.write(byte(4));
    lcd.setCursor(5, 1);  
    lcd.write(byte(5));
    lcd.setCursor(6, 1);  
    lcd.write(byte(6));
    lcd.setCursor(7, 1);
    lcd.write(byte(7));
    delay(1000);
    lcd.clear();
    lcd.setCursor(8, 0);  
    lcd.write(byte(0));
    lcd.setCursor(9, 0);  
    lcd.write(byte(1));
    lcd.setCursor(10, 0);  
    lcd.write(byte(2));
    lcd.setCursor(11, 0);
    lcd.write(byte(3));
    lcd.setCursor(8, 1);  
    lcd.write(byte(4));
    lcd.setCursor(9, 1);  
    lcd.write(byte(5));
    lcd.setCursor(10, 1);  
    lcd.write(byte(6));
    lcd.setCursor(11, 1);
    lcd.write(byte(7));
    delay(1000);
    lcd.clear();
    lcd.setCursor(12, 0);  
    lcd.write(byte(0));
    lcd.setCursor(13, 0);  
    lcd.write(byte(1));
    lcd.setCursor(14, 0);  
    lcd.write(byte(2));
    lcd.setCursor(15, 0);
    lcd.write(byte(3));
    lcd.setCursor(12, 1);  
    lcd.write(byte(4));
    lcd.setCursor(13, 1);  
    lcd.write(byte(5));
    lcd.setCursor(14, 1);  
    lcd.write(byte(6));
    lcd.setCursor(15, 1);
    lcd.write(byte(7));
    delay(1000);
    lcd.clear();
    delay(1000);
 
    inicio = false; //fim do loop
  }

//===========================DHT(Leituras e médias)===============================//

//faz as leituras das respectivas variáveis

  DHT.read11(pinoDHT11);
  int temperaturaAtual = DHT.temperature;
  int umidadeAtual = DHT.humidity;

//conversão da luminosidade para adquirir o valor correto

  int valor_luminosidade = analogRead(LDR_PIN);
  int luminosidadeAtual = map(valor_luminosidade, 0, 1023, 0 , 100);

//cálculo da média

  temperaturaMedia += temperaturaAtual;
  umidadeMedia += umidadeAtual;
  luminosidadeMedia += luminosidadeAtual;
  leituras++;

  unsigned long tempoAtual = millis();

  if(tempoAtual - ultimaLeitura >= intervalo){

    temperaturaMedia /= leituras;

    umidadeMedia /= leituras;

    luminosidadeMedia /= leituras;

//exibição na serial
   }
  Serial.println(" ");
  Serial.print("Temperatura: ");
  Serial.print(temperaturaMedia);
  Serial.println(" °C");
  Serial.print("Umidade: ");
  Serial.print(umidadeMedia);
  Serial.println(" %");
  Serial.print("Luminosidade: ");
  Serial.print(luminosidadeMedia);
  Serial.println(" %");
 
//=====================================================================================//

//=================================LEITURAS DO RTC====================================//

  DateTime now = rtc.now(); //CHAMADA DE FUNÇÃO
  Serial.print("Data: "); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.print(now.day(), DEC); //IMPRIME NO MONITOR SERIAL O DIA
  Serial.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
  Serial.print(now.month(), DEC); //IMPRIME NO MONITOR SERIAL O MÊS
  Serial.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
  Serial.print(now.year() % 100); //IMPRIME NO MONITOR SERIAL O ANO
  Serial.print(" / Dia: "); //IMPRIME O TEXTO NA SERIAL
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]); //IMPRIME NO MONITOR SERIAL O DIA
  Serial.print(" / Horas: "); //IMPRIME O TEXTO NA SERIAL
  Serial.print(now.hour(), DEC); //IMPRIME NO MONITOR SERIAL A HORA
  Serial.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
  Serial.print(now.minute(), DEC); //IMPRIME NO MONITOR SERIAL OS MINUTOS
  Serial.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
  Serial.println(now.second(), DEC); //IMPRIME NO MONITOR SERIAL OS SEGUNDOS
  Serial.println(); //QUEBRA DE LINHA NA SERIAL
  delay(1000); //INTERVALO DE 1 SEGUNDO

//=====================================================================================//

//=================================EXIBIÇÃO NO LCD====================================//
  lcd.setCursor(1,0);
  lcd.write("T");
  lcd.print(temperaturaMedia);
  //lcd.write(" ");
  lcd.write("C");
  lcd.setCursor(7,0);
  lcd.write("L");
  lcd.print(luminosidadeMedia);
  lcd.write("% ");
  lcd.setCursor(12,0);
  lcd.write("U");
  lcd.print(umidadeMedia);
  lcd.write("%");
  lcd.setCursor(0,2);
  lcd.print(now.day());
  lcd.write("/");
  lcd.print(now.month());
  lcd.write("/");
  lcd.print(now.year()%100);
  lcd.setCursor(10,2);
  lcd.print(now.hour());
  lcd.write(":");
  lcd.print(now.minute());
  //lcd.write(":");
  //lcd.print(now.second());

//=====================================================================================//

//=======================INDICADORES(LUZ, ALARME E DATA LOGGER)========================//

String mensagem1 = "Gravando as seguintes informações: ";
String mensagem2 = "Informações gravadas com sucesso!";

    if(luminosidadeMedia < 30 && luminosidadeMedia > 0 && temperaturaMedia > 15 && temperaturaMedia < 25 && umidadeMedia > 30 && umidadeMedia < 50 )

  {
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledVerde, HIGH);
  }  

  else

  {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, HIGH);
    EEPROM.write(0, temperaturaMedia);
    EEPROM.write(1, umidadeMedia);
    EEPROM.write(2, luminosidadeMedia);
    int endereco = 0;
    for (int endereco = 0; endereco < 3; endereco++)
      Serial.println(mensagem1 + EEPROM.read(endereco)+ " " + mensagem2);
    // Gere um som de frequência 2000Hz (tom agudo) por 500ms
   
    tone(BUZZER_PIN, 2000);
    delay(2000);

    // Pare o som por 100ms
   
    noTone(BUZZER_PIN);
    delay(100);
  }
 
//zera as variáveis e a leitura

    temperaturaMedia = 0;
    umidadeMedia = 0;
    luminosidadeMedia = 0;
    leituras = 0;

    ultimaLeitura = tempoAtual;
   
  delay(1000);  //iterate every 5 seconds
}
//================================================================================//
