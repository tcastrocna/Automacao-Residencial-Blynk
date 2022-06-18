#include <Arduino.h>
#include <IRremote.h>
#include <BlynkSimpleEsp32.h>

BlynkTimer timer1;

//Definir os pinos de saidas para os relés
#define Relay1 18  //Iluminação 1
#define Relay2 19  //Iluminação 2
#define Relay3 23  //Ventilador
#define Relay4 05  //Tomada

//Definir os pinos de entrada dos botões
#define Switch1 13  //Iluminação 1
#define Switch2 12  //Iluminação 2
#define Switch3 14  //Ventilador
#define Switch4 27  //Tomada

#define IR_RECV_PIN 16 //Sensor IR usado para receber os sinais do controle remoto.

#define PINO_VIRTUAL1    V1 
#define PINO_VIRTUAL2    V2
#define PINO_VIRTUAL3    V3 
#define PINO_VIRTUAL4    V4
#define PINO_VIRTUAL5    V5

int toggleState_1 = 1; //Defina um inteiro para lembrar o estado de alternância para o relé 1
int toggleState_2 = 1; //Defina um inteiro para lembrar o estado de alternância para o relé 2
int toggleState_3 = 1; //Defina um inteiro para lembrar o estado de alternância para o relé 3
int toggleState_4 = 1; //Defina um inteiro para lembrar o estado de alternância para o relé 4

int wifiFlag = 0;

IRrecv irrecv(IR_RECV_PIN);
decode_results results;

#define AUTH "-BTvuCe74-dSR8RMtK8TMxg7fCwqr4_Q"   //Você deve obter o Auth Token no aplicativo Blynk e colar aqui. 
#define WIFI_SSID "Motoe"                         //Nome da rede Wifi
#define WIFI_PASS "1A2b3c4d"                      //Senha da rede Wifi

void relayOnOff(int relay){
    switch(relay){
      case 1: 
             if(toggleState_1 == 1){
              digitalWrite(Relay1, LOW); // Liga o relé 1
              toggleState_1 = 0;
              Serial.println("\nRelé 1 ligado");
              }
             else{
              digitalWrite(Relay1, HIGH); // Desliga o relé 1
              toggleState_1 = 1;
              Serial.println("\nRelé 1 desligado");
              } 
             delay(100);
      break;
      case 2: 
             if(toggleState_2 == 1){
              digitalWrite(Relay2, LOW); // Liga o relé 2
              toggleState_2 = 0;
              Serial.println("\nRelé 2 ligado");
              }
             else{
              digitalWrite(Relay2, HIGH); // Desliga o relé 2
              toggleState_2 = 1;
              Serial.println("\nRelé 2 desligado");
              }
             delay(100);
      break;
       case 3: 
             if(toggleState_3 == 1){
              digitalWrite(Relay3, LOW); // Liga o relé 3
              toggleState_3 = 0;
              Serial.println("\nRelé 3 ligado");
              }
             else{
              digitalWrite(Relay3, HIGH); // Desliga o relé 3
              toggleState_3 = 1;
              Serial.println("\nRelé 3 desligado");
              }
             delay(100);
      break;
      case 4: 
             if(toggleState_4 == 1){
              digitalWrite(Relay4, LOW); // Liga o relé 1
              toggleState_4 = 0;
              Serial.println("Relé 4 ligado");
              }
             else{
              digitalWrite(Relay4, HIGH); // Desliga o relé 1
              toggleState_4 = 1;
              Serial.println("Relé 4 desligado");
              }
             delay(100);
      break;
      default : break;      
      }  
}

void all_SwitchOff()
{
  toggleState_1 = 1; digitalWrite(Relay1, toggleState_1);  delay(100);
  toggleState_2 = 1; digitalWrite(Relay2, toggleState_2);  delay(100);
  toggleState_3 = 1; digitalWrite(Relay3, toggleState_3);  delay(100);
  toggleState_4 = 1; digitalWrite(Relay4, toggleState_4);  delay(100);
}

void manual_button()
{
    if (digitalRead(Switch1) == LOW){
      delay(200);
      relayOnOff(1);      
    }
    else if (digitalRead(Switch2) == LOW){
      delay(200);
      relayOnOff(2);
    }
    else if (digitalRead(Switch3) == LOW){
      delay(200);
      relayOnOff(3);
    }
    else if (digitalRead(Switch4) == LOW){
      delay(200);
      relayOnOff(4);
    }
}

void ir_remote(){
  if (irrecv.decode(&results)) {
      switch(results.value){
          case 0xBDA857:  relayOnOff(1);  break;
          case 0xBD6897:  relayOnOff(2);  break;
          case 0xBDE817:  relayOnOff(3);  break;
          case 0xBD9867:  relayOnOff(4);  break;
          case 0xBD807F:  all_SwitchOff(); break;
          default : break;         
        }   
        Serial.println(results.value, HEX);    
        irrecv.resume();   
  } 
}

// Quando o botão do app é pressionado - muda o estado
BLYNK_WRITE(PINO_VIRTUAL1) {
  toggleState_1 = param.asInt();
  digitalWrite(Relay1, toggleState_1);
}

BLYNK_WRITE(PINO_VIRTUAL2) {
  toggleState_2 = param.asInt();
  digitalWrite(Relay2, toggleState_2);
}

BLYNK_WRITE(PINO_VIRTUAL3) {
  toggleState_3 = param.asInt();
  digitalWrite(Relay3, toggleState_3);
}

BLYNK_WRITE(PINO_VIRTUAL4) {
  toggleState_4 = param.asInt();
  digitalWrite(Relay4, toggleState_4);
}

BLYNK_WRITE(PINO_VIRTUAL5) {
  all_SwitchOff();
}

void checkBlynkStatus() { //Chamado a cada 3 segundos pelo SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    digitalWrite(LED_BUILTIN, LOW); //Desliaga o led quando não existir rede wifi.
  }
  if (isconnected == true) {
    wifiFlag = 0;
    Blynk.virtualWrite(PINO_VIRTUAL1, toggleState_1);
    Blynk.virtualWrite(PINO_VIRTUAL2, toggleState_2);
    Blynk.virtualWrite(PINO_VIRTUAL3, toggleState_3);
    Blynk.virtualWrite(PINO_VIRTUAL4, toggleState_4);

    digitalWrite(LED_BUILTIN, HIGH); //Liga o led quando existir rede wifi.
  }
}
void setup()
{
  Serial.begin(9600);
  
  irrecv.enableIRIn(); // Start the receiver
  
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(Switch1, INPUT_PULLUP);
  pinMode(Switch2, INPUT_PULLUP);
  pinMode(Switch3, INPUT_PULLUP);
  pinMode(Switch4, INPUT_PULLUP);
    
  //Durante a partida todos os relés devem DESLIGAR
  digitalWrite(Relay1, toggleState_1);
  digitalWrite(Relay2, toggleState_2);
  digitalWrite(Relay3, toggleState_3);
  digitalWrite(Relay4, toggleState_4);
 
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer1.setInterval(2000L, checkBlynkStatus); //Verifica se o servidor Blynk está conectado a cada 3 segundos
  Blynk.config(AUTH);
  delay(1000);


  Blynk.virtualWrite(PINO_VIRTUAL1, toggleState_1);
  Blynk.virtualWrite(PINO_VIRTUAL2, toggleState_2);
  Blynk.virtualWrite(PINO_VIRTUAL3, toggleState_3);
  Blynk.virtualWrite(PINO_VIRTUAL4, toggleState_4);
}

void loop()
{      
  ir_remote();
  manual_button();
  
  if (WiFi.status() == WL_CONNECTED)
  {
    Blynk.run();
  }
  
  timer1.run(); // Inicia o SimpleTimer
}
