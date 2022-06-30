//Projeto simples de Automação residencial utilizando o chip ESP32, em conjunto com a plataforma Blynk v0.1
//Bibliotecas
#include <Arduino.h>
#include <IRremote.hpp>
#include <BlynkSimpleEsp32.h>

BlynkTimer timer1;

//Auth Token da Plataforma Blynk e nome/senha rede WiFi.
#define AUTH "AUTH-TOKEN"
#define NOME_WIFI "NOME-WIFI"
#define SENHA_WIFI "SENHA-WIFI"

//Definir os pinos de saídas e entrada da placa.
#define RELAY_PIN_1      18       //Relé Iluminação 1.
#define RELAY_PIN_2      19       //Relé Iluminação 2.
#define RELAY_PIN_3      23       //Relé Ventilador.
#define BUTTON_PIN_1     13       //Botão Iluminação 1.
#define BUTTON_PIN_2     12       //Botão Iluminação 2.
#define BUTTON_PIN_3     14       //Botão Ventilador.
#define WIFI_LED         17       //Led indicador de rede.
#define RECV_IR          27       //Receptor Infravermelho.

#define BAUD_RATE  9600          

//Código Hexadecimal teclas do controle remoto.
#define IR_BUTTON_1 0xEA15BD00 //Relé Iluminação 1.
#define IR_BUTTON_2 0xE916BD00 //Relé Iluminação 2.
#define IR_BUTTON_3 0xE817BD00 //Relé Ventilador.
#define IR_OFF_ALL 0xFE01BD00 //Desliga todos.

//Pinos virtuais da plataforma Blynk.
#define PINO_VIRTUAL_1  V1
#define PINO_VIRTUAL_2  V2
#define PINO_VIRTUAL_3  V3
#define PINO_VIRTUAL_4  V4

//Estado de energia das saídas
bool POWER_STATE_1 = LOW;
bool POWER_STATE_2 = LOW;
bool POWER_STATE_3 = LOW;

int WiFiFlag = 0;

//Ações para o controle através do aplicativo blynk v0.1
BLYNK_WRITE(PINO_VIRTUAL_1) {
  POWER_STATE_1 = param.asInt();
  digitalWrite(RELAY_PIN_1, POWER_STATE_1);
}
BLYNK_WRITE(PINO_VIRTUAL_2) {
  POWER_STATE_2 = param.asInt();
  digitalWrite(RELAY_PIN_2, POWER_STATE_2);
}
BLYNK_WRITE(PINO_VIRTUAL_3) {
  POWER_STATE_3 = param.asInt();
  digitalWrite(RELAY_PIN_3, POWER_STATE_3);
}
BLYNK_WRITE(PINO_VIRTUAL_4) {
  desligarTodos();
}

//Função para controle através de botões físicos e controle remoto IR
void ligaDesligaRele(int rele){
    switch (rele){
    case 1:
       if(POWER_STATE_1 == LOW){
        digitalWrite(RELAY_PIN_1, LOW);
        POWER_STATE_1 = !POWER_STATE_1;
        Serial.println("Relé 1 Ligado");
       }
       else{
        digitalWrite(RELAY_PIN_1, HIGH);
        POWER_STATE_1 = !POWER_STATE_1;
        Serial.println("Relé 1 Desligado");
       }
       delay(100);
    break;
    case 2:
       if(POWER_STATE_2 == LOW){
        digitalWrite(RELAY_PIN_2, LOW);
        POWER_STATE_2 = !POWER_STATE_2;
        Serial.println("Relé 2 Ligado");
       }
       else{
        digitalWrite(RELAY_PIN_2, HIGH);
        POWER_STATE_2 = !POWER_STATE_2;
        Serial.println("Relé 2 Desligado");
       }
       delay(100);
    break;
    case 3:
       if(POWER_STATE_3 == LOW){
        digitalWrite(RELAY_PIN_3, LOW);
        POWER_STATE_3 = !POWER_STATE_3;
        Serial.println("Relé 3 Ligado");
       }
       else{
        digitalWrite(RELAY_PIN_3, HIGH);
        POWER_STATE_3 = !POWER_STATE_3;
        Serial.println("Relé 3 Desligado");
       }
       delay(100);
    break;
    default: break;
    }
}

//Função desligar todos, desliga todos os equipamentos ao precionar o botão off-all.
void desligarTodos(){
    POWER_STATE_1 = HIGH; digitalWrite(RELAY_PIN_1, POWER_STATE_1); delay(100);
    POWER_STATE_2 = HIGH; digitalWrite(RELAY_PIN_2, POWER_STATE_2); delay(100);
    POWER_STATE_3 = HIGH; digitalWrite(RELAY_PIN_3, POWER_STATE_3); delay(100);
}

//Função para controle manual através de botões físicos
void controleManual(){
    if(digitalRead(BUTTON_PIN_1) == LOW){
        delay(100);
        ligaDesligaRele(1);
    }
    else if(digitalRead(BUTTON_PIN_2) == LOW){
        delay(100);
        ligaDesligaRele(2);
    }
    else if(digitalRead(BUTTON_PIN_3) == LOW){
        delay(100);
        ligaDesligaRele(3);
    }
}

//Função para controle através do controle remoto infravermelho.
void controleRemoto(){
    if(IrReceiver.decode()){
        switch (IrReceiver.decodedIRData.decodedRawData)
        {
        case 0xEA15BD00: ligaDesligaRele(1);  break;
        case 0xE916BD00: ligaDesligaRele(2);  break;
        case 0xE817BD00: ligaDesligaRele(3);  break;
        case 0xFE01BD00: desligarTodos();     break;
        default: break;
        }
        Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
        IrReceiver.resume();
    }
}

//Chamado a cada 3 segundos pelo SimpleTimer
void checkBlynkStatus() { 

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    WiFiFlag = 1;
    digitalWrite(WIFI_LED, LOW); //Desligado o led quando não existir rede wifi.
  }
  if (isconnected == true) {
    WiFiFlag = 0;
    Blynk.virtualWrite(PINO_VIRTUAL_1, POWER_STATE_1);
    Blynk.virtualWrite(PINO_VIRTUAL_2, POWER_STATE_2);
    Blynk.virtualWrite(PINO_VIRTUAL_3, POWER_STATE_3);

    digitalWrite(WIFI_LED, HIGH); //Liga o led quando existir rede wifi.
  }
}

void setup()
{
  Serial.begin(BAUD_RATE);
  IrReceiver.begin(RECV_IR, ENABLE_LED_FEEDBACK); 
  
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(RELAY_PIN_3, OUTPUT);

  pinMode(WIFI_LED, OUTPUT);

  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);
    
  //Durante a partida todos os relés devem DESLIGAR
  digitalWrite(RELAY_PIN_1, !POWER_STATE_1);
  digitalWrite(RELAY_PIN_2, !POWER_STATE_2);
  digitalWrite(RELAY_PIN_3, !POWER_STATE_3);
 
  WiFi.begin(NOME_WIFI, SENHA_WIFI);
  timer1.setInterval(2000L, checkBlynkStatus); //Verifica se o servidor Blynk está conectado a cada 3 segundos
  Blynk.config(AUTH);
  delay(1000);

  Blynk.virtualWrite(PINO_VIRTUAL_1, POWER_STATE_1);
  Blynk.virtualWrite(PINO_VIRTUAL_2, POWER_STATE_2);
  Blynk.virtualWrite(PINO_VIRTUAL_3, POWER_STATE_3);
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
