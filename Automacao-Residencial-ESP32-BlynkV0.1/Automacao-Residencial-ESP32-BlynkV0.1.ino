//Projeto simples de Automação residencial utilizando o chip ESP32, em conjunto com a plataforma Blynk
//Bibliotecas
#include <Arduino.h>
#include <IRremote.hpp>
#include <BlynkSimpleEsp32.h>

BlynkTimer timer1;

//Auth Token da Plataforma Blynk e nome/senha rede WiFi.
#define AUTH "AUTH-TOKEN"
#define NOME_WIFI "NOME-WIFI"
#define SENHA_WIFI "SENHA-WIFI"

//Definir os pinos de saídas para os relés.
#define RELE_1 18       //Iluminação1
#define RELE_2 19       //Iluminação2
#define RELE_3 23       //Ventilador
#define RELE_4 05       //Tomadas

//Definir os Pinos de Entrada para os Botões.
#define BOTAO_1 13 
#define BOTAO_2 12
#define BOTAO_3 14
#define BOTAO_4 27

//Pino do Sensor IR, usado para receber os sinais do controle remoto.
#define RECV_IR 16

//Pinos virtuais da plataforma Blynk.
#define PINO_VIRTUAL_1  V1
#define PINO_VIRTUAL_2  V2
#define PINO_VIRTUAL_3  V3
#define PINO_VIRTUAL_4  V4
#define PINO_VIRTUAL_5  V5 

//Definir um inteiro para lembrar o estado de alternância do relé.
int estadoRele_1 =1;
int estadoRele_2 =1;
int estadoRele_3 =1;
int estadoRele_4 =1;

int WiFiFlag = 0;

void ligaDesligaRele(int rele){
    switch (rele){
    case 1:
       if(estadoRele_1 == 1){
        digitalWrite(RELE_1, LOW);
        estadoRele_1 = 0;
        Serial.println("Relé 1 Ligado");
       }
       else{
        digitalWrite(RELE_1, HIGH);
        estadoRele_1 = 1;
        Serial.println("Relé 1 Desligado");
       }
       delay(100);
    break;
    case 2:
       if(estadoRele_2 == 1){
        digitalWrite(RELE_2, LOW);
        estadoRele_2 = 0;
        Serial.println("Relé 2 Ligado");
       }
       else{
        digitalWrite(RELE_2, HIGH);
        estadoRele_2 = 1;
        Serial.println("Relé 2 Desligado");
       }
       delay(100);
    break;
     case 3:
       if(estadoRele_3 == 1){
        digitalWrite(RELE_3, LOW);
        estadoRele_3 = 0;
        Serial.println("Relé 3 Ligado");
       }
       else{
        digitalWrite(RELE_3, HIGH);
        estadoRele_3 = 1;
        Serial.println("Relé 3 Desligado");
       }
       delay(100);
    break;
     case 4:
       if(estadoRele_4 == 1){
        digitalWrite(RELE_4, LOW);
        estadoRele_4 = 0;
        Serial.println("Relé 4 Ligado");
       }
       else{
        digitalWrite(RELE_4, HIGH);
        estadoRele_4 = 1;
        Serial.println("Relé 4 Desligado");
       }
       delay(100);
    break;
    default: break;
    }
}

void desligarTodos(){
    estadoRele_1 = 1; digitalWrite(RELE_1, estadoRele_1); delay(100);
    estadoRele_2 = 1; digitalWrite(RELE_2, estadoRele_2); delay(100);
    estadoRele_3 = 1; digitalWrite(RELE_3, estadoRele_3); delay(100);
    estadoRele_4 = 1; digitalWrite(RELE_4, estadoRele_4); delay(100);

}

void controleManual(){
    if(digitalRead(BOTAO_1) == LOW){
        delay(100);
        ligaDesligaRele(1);
    }
    else if(digitalRead(BOTAO_2) == LOW){
        delay(100);
        ligaDesligaRele(2);
    }
    else if(digitalRead(BOTAO_3) == LOW){
        delay(100);
        ligaDesligaRele(3);
    }
    else if(digitalRead(BOTAO_4) == LOW){
        delay(100);
        ligaDesligaRele(4);
    }
}

void controleRemoto(){
    if(IrReceiver.decode()){
        switch (IrReceiver.decodedIRData.decodedRawData)
        {
        case 0xEA15BD00: ligaDesligaRele(1);  break;
        case 0xE916BD00: ligaDesligaRele(2);  break;
        case 0xE817BD00: ligaDesligaRele(3);  break;
        case 0xE619BD00: ligaDesligaRele(4);  break;
        case 0xFE01BD00: desligarTodos();     break;
        default: break;
        }
        Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
        IrReceiver.resume();
    }
}

//Ações para o aplicativo
BLYNK_WRITE(PINO_VIRTUAL_1) {
  estadoRele_1 = param.asInt();
  digitalWrite(RELE_1, estadoRele_1);
}
BLYNK_WRITE(PINO_VIRTUAL_2) {
  estadoRele_2 = param.asInt();
  digitalWrite(RELE_2, estadoRele_2);
}
BLYNK_WRITE(PINO_VIRTUAL_3) {
  estadoRele_3 = param.asInt();
  digitalWrite(RELE_3, estadoRele_3);
}
BLYNK_WRITE(PINO_VIRTUAL_4) {
  estadoRele_4 = param.asInt();
  digitalWrite(RELE_4, estadoRele_4);
}
BLYNK_WRITE(PINO_VIRTUAL_5) {
  desligarTodos();
}

//Chamado a cada 3 segundos pelo SimpleTimer
void checkBlynkStatus() { 

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    WiFiFlag = 1;
    digitalWrite(LED_BUILTIN, LOW); //Desliaga o led quando não existir rede wifi.
  }
  if (isconnected == true) {
    WiFiFlag = 0;
    Blynk.virtualWrite(PINO_VIRTUAL_1, estadoRele_1);
    Blynk.virtualWrite(PINO_VIRTUAL_2, estadoRele_2);
    Blynk.virtualWrite(PINO_VIRTUAL_3, estadoRele_3);
    Blynk.virtualWrite(PINO_VIRTUAL_4, estadoRele_4);

    digitalWrite(LED_BUILTIN, HIGH); //Liga o led quando existir rede wifi.
  }
}

void setup()
{
  Serial.begin(9600);
  
  IrReceiver.begin(RECV_IR, ENABLE_LED_FEEDBACK); 
  
  pinMode(RELE_1, OUTPUT);
  pinMode(RELE_2, OUTPUT);
  pinMode(RELE_3, OUTPUT);
  pinMode(RELE_4, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(BOTAO_1, INPUT_PULLUP);
  pinMode(BOTAO_2, INPUT_PULLUP);
  pinMode(BOTAO_3, INPUT_PULLUP);
  pinMode(BOTAO_4, INPUT_PULLUP);
    
  //Durante a partida todos os relés devem DESLIGAR
  digitalWrite(RELE_1, estadoRele_1);
  digitalWrite(RELE_2, estadoRele_2);
  digitalWrite(RELE_3, estadoRele_3);
  digitalWrite(RELE_4, estadoRele_4);
 
  WiFi.begin(NOME_WIFI, SENHA_WIFI);
  timer1.setInterval(2000L, checkBlynkStatus); //Verifica se o servidor Blynk está conectado a cada 3 segundos
  Blynk.config(AUTH);
  delay(1000);


  Blynk.virtualWrite(PINO_VIRTUAL_1, estadoRele_1);
  Blynk.virtualWrite(PINO_VIRTUAL_2, estadoRele_2);
  Blynk.virtualWrite(PINO_VIRTUAL_3, estadoRele_3);
  Blynk.virtualWrite(PINO_VIRTUAL_4, estadoRele_4);
}

void loop()
{      
  controleManual();
  controleRemoto();
  
  if (WiFi.status() == WL_CONNECTED)
  {
    Blynk.run();
  }
  
  timer1.run();
}
