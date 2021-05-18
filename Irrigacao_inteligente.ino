#include <LiquidCrystal.h>
#include <Servo.h>

#define COLS 16
#define ROWS 2
 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int const PINO_SGAS = A1;
int valueLight;
Servo servo1;
int pinLedVerm = 7;
short int irrigando = 0;
short int light = 0;

//tipo e frequencia em minutos e tempo da irrigação min
//aqui vai ser usado segundos somente para simular
//os valores sao ficticios
char cultura1[] = "tomate,10,15";
char cultura2[] = "soja,5,10";
char cultura3[] = "milho,3,15";

//mudar cultura
const int botao1 = 6;
int estadoBotao1 = 0;

//selecionar cultura
const int botao2 = 8;
int estadoBotao2 = 0;

short int freq;
unsigned long tempoIrri;
unsigned long countTempoIrri;

unsigned long esperaTela;

short int min=0, hora=0;

void setup() {
  pinMode(A0, INPUT);
  pinMode(A2, INPUT);
  pinMode(13, OUTPUT);
  pinMode(pinLedVerm, OUTPUT);
  servo1.attach(9);
  
  Serial.begin(9600);
 
  lcd.begin(COLS, ROWS);
  pinMode(botao1, INPUT);
  pinMode(botao2, INPUT);
  
  selecionarCultura();
  
  countTempoIrri=millis();
  
  min+=tempoIrri/1000;
}

void loop() {
  
  //leitura nível do gás
  int valorGas = analogRead(PINO_SGAS);
  valorGas = map(valorGas, 300, 750, 0, 100);
  //Serial.println(valorGas);
  
  //leitura temperatura
  int tmp = analogRead(A0);
  float voltage = (tmp * 5.0)/1024;
  float milliVolt = voltage * 1000;
  float tmpCel = (milliVolt-500)/10;
  
  //chega se eh pra irrigar
  if(millis()-countTempoIrri > freq && tmpCel>25){
    countTempoIrri=millis();
    servo1.write(180);
    
    lcd.clear();
  
    lcd.setCursor(0, 0);
    lcd.print("irrigando por:");
    lcd.setCursor(0, 1);
    lcd.print(tempoIrri/1000);
    lcd.print(" Min");
    
    min += tempoIrri/1000;
    if(min>60){
      min -= 60;
        hora++;
    }
    if(hora>=24)
      hora -= 24;
    
    delay(tempoIrri);
    
    servo1.write(0);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("prox irrig as:");
    lcd.setCursor(0, 1);
    lcd.print(hora);lcd.print(" ");lcd.print(min);
    delay(2000);
  }
  
  //incêndio
  if(tmpCel > 60 || valorGas >= 50){
    servo1.write(180);
    digitalWrite(pinLedVerm, HIGH);
    irrigando=1;
  }
  else{
    servo1.write(0);
    digitalWrite(pinLedVerm, LOW);
    irrigando=0;
  }
  
  //leitura da claridade
  valueLight = analogRead(A2);
  
  //luz sobre a plantação
  if(valueLight < 250){
    digitalWrite(13,HIGH);
    light=1;
  }
  else{
    digitalWrite(13,LOW);
    light=0;
  }
  
  //exbibe temperatura e claridade
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(tmpCel);
  lcd.print(" C");
  
  lcd.setCursor(0, 1);
  lcd.print("Claridade: ");
  lcd.print(valueLight);
  esperaTela=millis();
  estadoBotao1=HIGH;
  while(millis()-esperaTela < 500){
    estadoBotao1 = digitalRead(botao1);
    if(estadoBotao1==LOW){
      selecionarCultura();
    }
  } 
  lcd.clear();
  
  //exibe nível do gás e se há incêncio ou não
  lcd.setCursor(0, 0);
  lcd.print("nivel gas: ");
  lcd.print(valorGas);
  
  lcd.setCursor(0, 1);
  if(tmpCel > 60 || valorGas >= 50){
    lcd.print("incendio");
  }
  else{
    lcd.print("Niv gas normal");
  }
  esperaTela=millis();
  estadoBotao1=HIGH;
  while(millis()-esperaTela < 500){
    estadoBotao1 = digitalRead(botao1);
    if(estadoBotao1==LOW){
      selecionarCultura();
    }
  }
  lcd.clear();
  
  //exibe se esta irrigando e se a luz esta acesa
  lcd.setCursor(0, 0);
  if(irrigando==1)
    lcd.print("Irrigando");
  else
    lcd.print("agua fechada");
  lcd.setCursor(0, 1);
  if(light==1)
    lcd.print("luz acesa");
  else
    lcd.print("luz apagada");
  esperaTela=millis();
  estadoBotao1=HIGH;
  while(millis()-esperaTela < 500){
    estadoBotao1 = digitalRead(botao1);
    if(estadoBotao1==LOW){
      selecionarCultura();
    }
  }
  lcd.clear();
  
  //exibe momento da prox irrigacao
  lcd.setCursor(0, 0);
  lcd.print("Prox Irri");
  lcd.setCursor(0, 1);
  lcd.print(hora);
  lcd.print(" ");
  lcd.print(min);
  
  esperaTela=millis();
  estadoBotao1=HIGH;
  while(millis()-esperaTela < 500){
    estadoBotao1 = digitalRead(botao1);
    if(estadoBotao1==LOW){
      selecionarCultura();
    }
  }
  lcd.clear();
}


void selecionarCultura(){
    estadoBotao1 = digitalRead(botao1);
    estadoBotao2 = digitalRead(botao2);
    unsigned long espera=millis();
    short int click=0; 
  lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Selec a cultura");
    lcd.setCursor(0, 1);
    lcd.print("Tomate");
    while(estadoBotao2==HIGH){
      estadoBotao1 = digitalRead(botao1);
      if(estadoBotao1==LOW){
        estadoBotao1=HIGH;
        click++;
        if(click>3)
          click=0;
        
        if(click==0){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Selec a cultura");
          lcd.setCursor(0, 1);
          lcd.print("Tomate");
        }
        else if(click==1){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Selec a cultura");
          lcd.setCursor(0, 1);
          lcd.print("Soja");
        }
        else{
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Selec a cultura");
          lcd.setCursor(0, 1);
          lcd.print("Milho"); 
        }
        delay(30);
      }
      estadoBotao2 = digitalRead(botao2);
    }
    if(click==0){
      tokenizar(cultura1);
    }
    else if(click==1){
      tokenizar(cultura2);
    }
    else{
      tokenizar(cultura3);
    }
}


void tokenizar(char cultura[]){
  //Serial.println(cultura);
    char *token = strtok(cultura,",");
    short int param=0;
    while(token != NULL){
      if(param==1){
            freq = (unsigned long)atoi(token);
            freq *= 100;
        }
        else if(param==2){
          tempoIrri = (unsigned long)atoi(token);
            tempoIrri *= 1000;
        }
        param++;
        token = strtok(NULL,",");
    }
    //Serial.println(freq);
    //Serial.println(tempoIrri);
    lcd.clear();
}
