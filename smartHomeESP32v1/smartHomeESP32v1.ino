#include <Arduino.h>
#include <IRremote.h>
#include <BlynkSimpleEsp32.h>

BlynkTimer timer1;

//Auth Token da plataforma Blynk e nome e senha da rede WiFi local.
#define AUTH "-BTvuCe74-dSR8RMtK8TMxg7fCwqr4_Q"    
#define WIFI_SSID "Motoe"                         
#define WIFI_PASS "1A2b3c4d"                     

//Definir os pinos de saidas para os relés
#define pinoRele1 18  //Iluminação 1
#define pinoRele2 19  //Iluminação 2
#define pinoRele3 23  //Ventilador
#define pinoRele4 05  //Tomada

//Definir os pinos de entrada dos botões
#define pinoBotao1 13  //Iluminação 1
#define pinoBotao2 12  //Iluminação 2
#define pinoBotao3 14  //Ventilador
#define pinoBotao4 27  //Tomada

#define PINO_IR_RECV 16 //Sensor IR usado para receber os sinais do controle remoto.

#define PINO_VIRTUAL1    V1 
#define PINO_VIRTUAL2    V2
#define PINO_VIRTUAL3    V3 
#define PINO_VIRTUAL4    V4
#define PINO_VIRTUAL5    V5

int estadoRele_1 = 1; //Defina um inteiro para lembrar o estado de alternância para o relé 1
int estadoRele_2 = 1; //Defina um inteiro para lembrar o estado de alternância para o relé 2
int estadoRele_3 = 1; //Defina um inteiro para lembrar o estado de alternância para o relé 3
int estadoRele_4 = 1; //Defina um inteiro para lembrar o estado de alternância para o relé 4

int wifiFlag = 0;

IRrecv irrecv(PINO_IR_RECV);
decode_results results;

void ligaDesligaRele(int relay ){
    switch(relay){
      case 1: 
             if(estadoRele_1 == 1){
              digitalWrite(pinoRele1, LOW); // Liga o relé 1
              estadoRele_1 = 0;
              Serial.println("\nRelé 1 ligado");
              }
             else{
              digitalWrite(pinoRele1, HIGH); // Desliga o relé 1
              estadoRele_1 = 1;
              Serial.println("\nRelé 1 desligado");
              } 
             delay(100);
      break;
      case 2: 
             if(estadoRele_2 == 1){
              digitalWrite(pinoRele2, LOW); // Liga o relé 2
              estadoRele_2 = 0;
              Serial.println("\nRelé 2 ligado");
              }
             else{
              digitalWrite(pinoRele2, HIGH); // Desliga o relé 2
              estadoRele_2 = 1;
              Serial.println("\nRelé 2 desligado");
              }
             delay(100);
      break;
       case 3: 
             if(estadoRele_3 == 1){
              digitalWrite(pinoRele3, LOW); // Liga o relé 3
              estadoRele_3 = 0;
              Serial.println("\nRelé 3 ligado");
              }
             else{
              digitalWrite(pinoRele3, HIGH); // Desliga o relé 3
              estadoRele_3 = 1;
              Serial.println("\nRelé 3 desligado");
              }
             delay(100);
      break;
      case 4: 
             if(estadoRele_4 == 1){
              digitalWrite(pinoRele4, LOW); // Liga o relé 1
              estadoRele_4 = 0;
              Serial.println("Relé 4 ligado");
              }
             else{
              digitalWrite(pinoRele4, HIGH); // Desliga o relé 1
              estadoRele_4 = 1;
              Serial.println("Relé 4 desligado");
              }
             delay(100);
      break;
      default : break;      
      }  
}

void desligarTodos()
{
  estadoRele_1 = 1; digitalWrite(pinoRele1, estadoRele_1);  delay(100);
  estadoRele_2 = 1; digitalWrite(pinoRele2, estadoRele_2);  delay(100);
  estadoRele_3 = 1; digitalWrite(pinoRele3, estadoRele_3);  delay(100);
  estadoRele_4 = 1; digitalWrite(pinoRele4, estadoRele_4);  delay(100);
}

void controleManual()
{
    if (digitalRead(pinoBotao1) == LOW){
      delay(200);
      ligaDesligaRele(1);      
    }
    else if (digitalRead(pinoBotao2) == LOW){
      delay(200);
      ligaDesligaRele(2);
    }
    else if (digitalRead(pinoBotao3) == LOW){
      delay(200);
      ligaDesligaRele(3);
    }
    else if (digitalRead(pinoBotao4) == LOW){
      delay(200);
      ligaDesligaRele(4);
    }
}

void controleRemoto(){
  if (irrecv.decode(&results)) {
      switch(results.value){
          case 0xBDA857:  ligaDesligaRele(1);  break;
          case 0xBD6897:  ligaDesligaRele(2);  break;
          case 0xBDE817:  ligaDesligaRele(3);  break;
          case 0xBD9867:  ligaDesligaRele(4);  break;
          case 0xBD807F:  desligarTodos(); break;
          default : break;         
        }   
        Serial.println(results.value, HEX);    
        irrecv.resume();   
  } 
}

// Quando o botão do app é pressionado - muda o estado
BLYNK_WRITE(PINO_VIRTUAL1) {
  estadoRele_1 = param.asInt();
  digitalWrite(pinoRele1, estadoRele_1);
}

BLYNK_WRITE(PINO_VIRTUAL2) {
  estadoRele_2 = param.asInt();
  digitalWrite(pinoRele2, estadoRele_2);
}

BLYNK_WRITE(PINO_VIRTUAL3) {
  estadoRele_3 = param.asInt();
  digitalWrite(pinoRele3, estadoRele_3);
}

BLYNK_WRITE(PINO_VIRTUAL4) {
  estadoRele_4 = param.asInt();
  digitalWrite(pinoRele4, estadoRele_4);
}

BLYNK_WRITE(PINO_VIRTUAL5) {
  desligarTodos();
}

//Chamado a cada 3 segundos pelo SimpleTimer
void checkBlynkStatus() { 

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    digitalWrite(LED_BUILTIN, LOW); //Desliaga o led quando não existir rede wifi.
  }
  if (isconnected == true) {
    wifiFlag = 0;
    Blynk.virtualWrite(PINO_VIRTUAL1, estadoRele_1);
    Blynk.virtualWrite(PINO_VIRTUAL2, estadoRele_2);
    Blynk.virtualWrite(PINO_VIRTUAL3, estadoRele_3);
    Blynk.virtualWrite(PINO_VIRTUAL4, estadoRele_4);

    digitalWrite(LED_BUILTIN, HIGH); //Liga o led quando existir rede wifi.
  }
}
void setup()
{
  Serial.begin(9600);
  
  irrecv.enableIRIn(); // Start the receiver
  
  pinMode(pinoRele1, OUTPUT);
  pinMode(pinoRele2, OUTPUT);
  pinMode(pinoRele3, OUTPUT);
  pinMode(pinoRele4, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(pinoBotao1, INPUT_PULLUP);
  pinMode(pinoBotao2, INPUT_PULLUP);
  pinMode(pinoBotao3, INPUT_PULLUP);
  pinMode(pinoBotao4, INPUT_PULLUP);
    
  //Durante a partida todos os relés devem DESLIGAR
  digitalWrite(pinoRele1, estadoRele_1);
  digitalWrite(pinoRele2, estadoRele_2);
  digitalWrite(pinoRele3, estadoRele_3);
  digitalWrite(pinoRele4, estadoRele_4);
 
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer1.setInterval(2000L, checkBlynkStatus); //Verifica se o servidor Blynk está conectado a cada 3 segundos
  Blynk.config(AUTH);
  delay(1000);


  Blynk.virtualWrite(PINO_VIRTUAL1, estadoRele_1);
  Blynk.virtualWrite(PINO_VIRTUAL2, estadoRele_2);
  Blynk.virtualWrite(PINO_VIRTUAL3, estadoRele_3);
  Blynk.virtualWrite(PINO_VIRTUAL4, estadoRele_4);
}

void loop()
{      
  controleRemoto();
  controleManual();
  
  if (WiFi.status() == WL_CONNECTED)
  {
    Blynk.run();
  }
  
  timer1.run();
}
