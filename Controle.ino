#include <TimerOne.h>

const int PWM_Pin = 3;
const int IR_Pin = 4;  

/* Rotina da interrupção de tempo */
void timeInterrupt() {
  
}


void setup() {
  Serial.begin(9600);
  pinMode(PWM_Pin, OUTPUT);
  pinMode(IR_Pin, INPUT);
  
  // Configuração da interrupção de tempo
  Timer1.initialize(2000); 
  Timer1.attachInterrupt( timeInterrupt );
}

}

void loop() {
  analogWrite(PWM_Pin, 127);
  Serial.print(digitalRead(IR_Pin));
  Serial.print("\n");  
  delay(100);
  
}

