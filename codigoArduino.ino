//por Lucas de Aguiar Gontijo

#include <EEPROM.h>
#include <Wire.h>
#include <DS1307new.h>

//declaração de variáveis globais:
int button1State;
int button2State;
int button3State;
int button4State;
int lastButton1State = LOW;
int lastButton2State = LOW;
int lastButton3State = LOW;
int lastButton4State = LOW;
unsigned long lastDebounce1Time = 0;
unsigned long lastDebounce2Time = 0;
unsigned long lastDebounce3Time = 0;
unsigned long lastDebounce4Time = 0;
unsigned long lastTimeClock = 0;
unsigned long debounceDelay = 50;
byte doses;
byte doseSize;

void setup() {
  pinMode(2, OUTPUT);   //define pino 2 como saída
  pinMode(3, OUTPUT);   //define pino 3 como saída
  pinMode(4, OUTPUT);   //define pino 4 como saída
  pinMode(5, OUTPUT);   //define pino 5 como saída
  pinMode(6, INPUT);   //define pino 6 como entrada
  pinMode(7, OUTPUT);   //define pino 11 como saída
  pinMode(8, INPUT);   //define pino 8 como entrada
  pinMode(9, INPUT);   //define pino 9 como entrada
  pinMode(10, INPUT);   //define pino 10 como entrada
  carregaDados();   //carrega os dados da EEPROM
  digitalWrite(7, LOW);
  lightLeds();
}

void carregaDados(){
  doses = EEPROM.read(0);  //carrega a informação de quantidade de doses, da EEPROM
  doseSize = EEPROM.read(1);  //carrega a informação de tamanho de dose, da EEPROM
  if (doses < 1){
    doses = 1;
  }
  if (doses > 4){
    doses = 4;
  }
  if (doseSize < 1){
    doseSize = 1;
  }
  if (doseSize > 10){
    doseSize = 10;
  }
}

void lightLeds(){  //rotina para acender os leds indicativos de quantidade de doses diárias
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  switch (doses) {
  case 1:
    digitalWrite(2,HIGH);
    break;
  case 2:
    digitalWrite(3,HIGH);
    break;
  case 3:
    digitalWrite(4,HIGH);
    break;
  case 4:
    digitalWrite(5,HIGH);
    break;
  default:
    // comando(s)
    break;
  }
}

void libera(){   //rotina para liberar ração
  for (int i = 0; i <= (doseSize*10); i++){
    digitalWrite(7, HIGH);
    delay(50);
    digitalWrite(7, LOW);
    delay(50);
  }
  delay(1000);
}

void compara(){  //compara o tempo real com os horarios de alimentação
  if (doses == 1){
    if ((RTC.hour == 8) && (RTC.minute == 0) && (RTC.second == 0)){ libera(); }
  } //caso 1 dose selecionada, libera ração às 8h
  if (doses == 2){
    if ((RTC.hour == 8) && (RTC.minute == 0) && (RTC.second == 0)){ libera(); }
    if ((RTC.hour == 14) && (RTC.minute == 0) && (RTC.second == 0)){ libera(); }
  } //caso 2 doses selecionada, libera ração às 8h e 14h
  if (doses == 3){
    if ((RTC.hour == 8) && (RTC.minute == 0) && (RTC.second == 0)){ libera(); }
    if ((RTC.hour == 12) && (RTC.minute == 0) && (RTC.second == 0)){ libera(); }
    if ((RTC.hour == 16) && (RTC.minute == 0) && (RTC.second == 0)){ libera(); }
  } //caso 3 doses selecionada, libera ração às 8h, 12h e 16h
  if (doses == 4){
    if ((RTC.hour == 8) && (RTC.minute == 0) && (RTC.second == 0)){ libera(); }
    if ((RTC.hour == 11) && (RTC.minute == 0) && (RTC.second == 0)){ libera(); }
    if ((RTC.hour == 14) && (RTC.minute == 0) && (RTC.second == 0)){ libera(); }
    if ((RTC.hour == 17) && (RTC.minute == 0) && (RTC.second == 0)){ libera(); }
  } //caso 4 doses selecionada, libera ração às 8h, 11h, 14h e 17h
}

void loop() {

  if ((millis() - lastTimeClock) > 100){
    RTC.getTime();
    compara();
    lastTimeClock = millis();
  }
  
  int reading1 = digitalRead(6);   //leitura da entrada digital 6
  int reading2 = digitalRead(8);   //leitura da entrada digital 8
  int reading3 = digitalRead(9);   //leitura da entrada digital 9
  int reading4 = digitalRead(10);   //leitura da entrada digital 10
    if (reading1 != lastButton1State) {   //caso entrada digital 6 mudar
      lastDebounce1Time = millis();   //reseta timer do debounce
    }
  if ((millis() - lastDebounce1Time) > debounceDelay) {   //checa se timer do debounce estourou
    if (reading1 != button1State) {   //checa se estado do botao se alterou
      button1State = reading1;   //muda variavel de ultimo estado do botao
      if (button1State == HIGH) {   //checa se botao está pressionado
        if (doses < 4){doses++;} else if (doses == 4){doses = 1;}  //altera quantidade de doses
        lightLeds();
        EEPROM.write(0,doses);   //salva quantidade de doses na EEPROM
      }
    }
  }

    if (reading2 != lastButton2State) {   //caso entrada digital 8 mudar
      lastDebounce2Time = millis();   //reseta timer do debounce
    }
  if ((millis() - lastDebounce2Time) > debounceDelay) {   //checa se timer do debounce estourou
    if (reading2 != button2State) {   //checa se estado do botao se alterou
      button2State = reading2;   //muda variavel de ultimo estado do botao
      if (button2State == HIGH) {   //checa se botao está pressionado
        if (doseSize < 10){doseSize++;}   //precionando botao 2 incrementa a quantidade de ração por dose
        EEPROM.write(1,doseSize);   //salva quantidade por dose na EEPROM
      }
    }
  }

    if (reading3 != lastButton3State) {   //caso entrada digital 9 mudar
      lastDebounce3Time = millis();   //reseta timer do debounce
    }
  if ((millis() - lastDebounce3Time) > debounceDelay) {   //checa se timer do debounce estourou
    if (reading3 != button3State) {   //checa se estado do botao se alterou
      button3State = reading3;   //muda variavel de ultimo estado do botao
      if (button3State == HIGH) {   //checa se botao está pressionado
        if (doseSize > 1){doseSize--;}   //precionando botao 3 decrementa a quantidade de ração por dose
        EEPROM.write(1,doseSize);   //salva quantidade por dose na EEPROM
      }
    }
  }
  
    if (reading4 != lastButton4State) {   //caso entrada digital 8 mudar
      lastDebounce4Time = millis();   //reseta timer do debounce
    }
  if ((millis() - lastDebounce4Time) > debounceDelay) {   //checa se timer do debounce estourou
    if (reading4 != button4State) {   //checa se estado do botao se alterou
      button4State = reading4;   //muda variavel de ultimo estado do botao
      if (button4State == HIGH) {   //checa se botao está pressionado
        libera();   //precionando botao 4 libera ração
      }
    }
  }

  lastButton1State = reading1;
  lastButton2State = reading2;
  lastButton3State = reading3;
  lastButton4State = reading4;
  
}
