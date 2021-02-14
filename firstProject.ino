#include <EEPROM.h> 

#define led_red 8 // Led que sinaliza que a temperatura está acima do limite.
#define led_green 9 // Led que sinaliza que o sistema está funcionando.
#define led_green_2 3 // Led que sinaliza o armazenamento das informações de temperatura.
#define led_yellow 10 // Led que sinaliza que o está desligado.
#define motor_on 11 // Sinal que liga os motores do sistema.
#define temp_out 7 // Sinal que liga o equipamento responsável pela diminuição de temperatura.
#define on 6 // Sinal que liga ou desliga o sistema.
#define save 4 // Sinal para salvar as informações das temperaturas na memória.
#define temp A0 // Entrada do sistema que monitora a temperatura do sistema. 

int TempValue; // Variável que armazena os valores da entrada do temp A0;
int addr = 0;
String estados = "Start"; // Responsavel pelo controle dos estados.

void setup() {
  Serial.begin(9600);
  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_green_2, OUTPUT);
  pinMode(led_yellow, OUTPUT);
  pinMode(motor_on, OUTPUT);
  pinMode(temp_out, OUTPUT);
  pinMode(save, INPUT);
  pinMode(on, INPUT);
}

void loop() {
  TempValue = analogRead(temp);     // Variável que armazena as informações do sensor de temperatura.
  Serial.print("Valor da Temperatura");
  Serial.println(TempValue);        // Faz um print da tempereatura no seria.
  if(estados == "Start"){           // Estado inicial, esse estado é responsável por zarar as informações na memória.
    EEPROM.update(addr, 0); 
    estados = "On_Motor";           // Mudança de estado.
    digitalWrite(temp_out, LOW);
  }
  if(estados == "On_Motor"){
    if(TempValue <= 100){
      digitalWrite(motor_on, HIGH);
      digitalWrite(temp_out, LOW);
      digitalWrite(led_green, HIGH);
      digitalWrite(led_green_2, LOW);
      digitalWrite(led_yellow, LOW);
      digitalWrite(led_red, LOW);
    }
    else if(TempValue > 100){
      estados = "On_Termo";
    }
    else if(!digitalRead(on)){
      estados = "Off_Motor";
    }
    else if(digitalRead(save)){
      estados = "Save_info";
    }
  }
  if(estados == "On_Termo"){
    if(TempValue > 100){
      digitalWrite(motor_on, LOW);
      digitalWrite(led_green, LOW);
      digitalWrite(led_green_2, LOW);
      digitalWrite(led_yellow, LOW);
      digitalWrite(led_red, HIGH);
      digitalWrite(temp_out, HIGH);
    }
    else if(TempValue < 50){
      estados = "On_Motor";
    }
    else if(!digitalRead(on)){
      estados = "Off_Motor";
    }
    else if(digitalRead(save)){
      estados = "Save_info";
    }
  }
  if(estados == "Off_Motor"){
    if(!digitalRead(on)){
      digitalWrite(motor_on, LOW);
      digitalWrite(led_yellow, HIGH);
      digitalWrite(led_red, LOW);
      digitalWrite(led_green, LOW);
      digitalWrite(led_green_2, LOW);
    }
    else{
      estados = "On_Motor";
    }
  }
  if(estados == "Save_info"){
    digitalWrite(led_green, LOW);
    digitalWrite(led_yellow, LOW);
    digitalWrite(led_red, LOW);
    digitalWrite(led_green_2, HIGH);
    EEPROM.update(addr, TempValue);
    delay(500);
    estados = "On_Motor";
  }
}
