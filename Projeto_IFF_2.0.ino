#include <SoftwareSerial.h>
#include <Ultrasonic.h>
#include "DHT.h"

#define DHTPIN A5
#define DHTTYPE DHT11
#define TRIGGER_PIN  A0
#define ECHO_PIN     A1

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial bluetooth(10,11);
String comando;
float t,h;
int manual = 0;
//Variáveis do motor:
int velocidadeEsq = 5;
int velocidadeDir = 3;
int CtrlEsq1 = 6;
int CtrlEsq2 = 7;
int CtrlDir1 = 8;
int CtrlDir2 = 9;

void setup() {
  //Bluetooth:
   Serial.begin(9600);
   bluetooth.begin(9600);
  
  //Motor:
   pinMode(CtrlEsq1, OUTPUT);
   pinMode(CtrlEsq2, OUTPUT);
   pinMode(CtrlDir1, OUTPUT);
   pinMode(CtrlDir2, OUTPUT);
   pinMode(velocidadeEsq, OUTPUT);
   pinMode(velocidadeDir, OUTPUT);
}

// ******************************************************************** FUNÇÕES DE MOVIMENTO *********************************************

void andarParaFrente(){
   //Gira o Motor Esquerdo no sentido horario
    digitalWrite(CtrlEsq1, HIGH);
    digitalWrite(CtrlEsq2, LOW);
   //Gira o Motor Direito no sentido horario
    digitalWrite(CtrlDir1, HIGH);
   digitalWrite(CtrlDir2, LOW);
}

void andarParaTras(){
     //Gira o Motor Esquerdo no sentido anti-horario
    digitalWrite(CtrlEsq1, LOW);
    digitalWrite(CtrlEsq2, HIGH);
   //Gira o Motor Direito no sentido anti-horario
    digitalWrite(CtrlDir1, LOW);
    digitalWrite(CtrlDir2, HIGH);
}

void virarParaEsquerda(){
    //Gira o Motor Esquerdo no sentido anti-horario
    digitalWrite(CtrlEsq1, LOW);
    digitalWrite(CtrlEsq2, HIGH);
    //Gira o Motor Direito no sentido horario
    digitalWrite(CtrlDir1, HIGH);
    digitalWrite(CtrlDir2, LOW);
    //delay(50);
}

void virarParaDireita(){
    //Gira o Motor Esquerdo no sentido horario
    digitalWrite(CtrlEsq1, HIGH);
    digitalWrite(CtrlEsq2, LOW);
    //Gira o Motor Direito no sentido anti-horario
    digitalWrite(CtrlDir1, LOW);
    digitalWrite(CtrlDir2, HIGH);
    //delay(50);
}

void parar(){
    //Parar motor direito
    digitalWrite(CtrlDir1, HIGH);
    digitalWrite(CtrlDir2, HIGH);
    //Parar motor esquerdo
    digitalWrite(CtrlEsq1, HIGH);
    digitalWrite(CtrlEsq2, HIGH);
}
//***************************************************************** FUNÇÕES DO DHT 11****************************************

float medirTemperatura(){
  return dht.readTemperature();;
}

float medirUmidade(){
  return dht.readHumidity();;
}

//***************************************************************** FUNÇÕES DO HCSR ****************************************
float medirDistancia(){
  long microsec = ultrasonic.timing();
  return ultrasonic.convert(microsec, Ultrasonic::CM);
}

//*************************************************************************************************************************

void loop() {

  analogWrite(velocidadeEsq, 50);
  analogWrite(velocidadeDir, 50);
  
  comando = "";
  if(bluetooth.available()){
    while(bluetooth.available()){
      char caracter = (char) bluetooth.read();
      comando += caracter;
      delay(10);
    }
    Serial.println("Comando recebido: ");
    Serial.print(comando); 

     if(!manual){//Modo automatico
        if(medirDistancia() < 20){
          parar();
          delay(1000);
          andarParaTras();
          delay(1000);
          parar();
          delay(1000);
          virarParaDireita();
          delay(500);
          parar();
          delay(1000);
        }

        else
          andarParaFrente();        
    }
    else{//Modo manual
      if(comando.indexOf("acelera") > 0)
        andarParaFrente();
      else if(comando.indexOf("re") > 0)
        andarParaTras();
      else if(comando.indexOf("direita") > 0)
         virarParaDireita();
      else if(comando.indexOf("esquerda") > 0)
        virarParaEsquerda();
      else
        parar();
    }
    
    //Modo de pilotagem
    if(comando.indexOf("manual") > 0)
        manual = 1;
    else if(comando.indexOf("automatico")>0)
        manual = 0;    
  
  }
  t = medirTemperatura();
  h = medirUmidade();
  if(!isnan(t) || !isnan(h)){//Envia dados
    bluetooth.println("{");
    bluetooth.print(t);
    bluetooth.print("H");
    bluetooth.print(h);
    bluetooth.print("}");
  }
}
