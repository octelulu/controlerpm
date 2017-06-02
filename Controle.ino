/* EA076: Laboratório de Sistemas Embarcados
 *
 * Projeto 3: Controle Realimentado de RPM
 * Moteres DC são sistemas que consomem uma corrente maior do que o Arduino é capaz de
 * fornecer. Para conseguir a corrente necessário é necessário uma fonte externa, que
 * tem o nome de atuador. Os parâmetros do sistema podem variar muito dependendo da
 * fabricação do atuador, do motor e a degradação com o tempo. Isso faz com que o controle
 * velocidade em malha aberta seja difícil. Uma solução para esse problema é usar um sensor
 * para fazer um controle em malha fechada, que consiste em medir a velocidade e comparar com
 * uma referência e variar a alimentação do motor baseado nessa diferença. Medindo a
 * velocidade de fato que o motor está girando torna o sistema mais robusto à variações
 * nos parâmetros do sistema.
 * O programa a seguir implementa a lógica de controle de velocidade em malha fechada.
 */


#include <TimerOne.h> //Biblioteca para interrupção de tempo

const int PWM_Pin = 3; //Sinal PWM no pino 3
const int IR_Pin = 2; //Sinal do sensor IR
int half_turns = 0; //Contador de meias-voltas do eixo do motor
float RPM = 0; //Velocidade medida pelo sensor
float turns = 0; //Contador de voltas completas do eixo do motor
byte PWM_input = 0; //Duty-cycle do sinal PWM
float RPM_Ref = 2000; //Sinal de referência do controlador
float Kp = (255.0/4200.0)*0.1; //Ganho proporcianal do controlador

/* Rotina de interrupção de borda:
 * Cada vez que a hélice do motor interrompe o sensor IR há um descocamento de 180º,
 * assim, incrementa-se o número de meias volstas para o cáculo da velocidade.
 */
void changeInterrupt() {
  half_turns++;
}

/* Rotina da interrupção de tempo:
 * A cada 2 segundos, calcula-se a velocidade do motor basedo no número
 * de meias-voltas do contador e atualiza a entrada do sinal PWM no atuador
 */
void timeInterrupt() {
  turns = half_turns/2.0;
  RPM = (turns*60)/(2.0); //Cálculo da velocidade em rpm
  PWM_input = controller(); //Ajuste do duty-cycle do PWM
  analogWrite(PWM_Pin, PWM_input);
  Serial.print(RPM);
  Serial.print('\n');
  half_turns = 0;
}

/* Lógica do controlador para ajuste do duty-cycle baseado no erro entre a velocidade
 * de referência e a medida pelo sensor
 */
byte controller(){
  float error = RPM_Ref - RPM;
  //Serial.print(error);
  return PWM_input = PWM_input + Kp*error;
}


void setup() {
  Serial.begin(9600);
  pinMode(PWM_Pin, OUTPUT);
  pinMode(IR_Pin, INPUT);

  // Configuração da interrupção de borda de subida
  attachInterrupt(digitalPinToInterrupt(IR_Pin), changeInterrupt , RISING);

  // Configuração da interrupção de tempo
  Timer1.initialize(2000000); // 2 segundos
  Timer1.attachInterrupt( timeInterrupt );
}

void loop() {
  // toda a lógica acontece por interrupções
}
