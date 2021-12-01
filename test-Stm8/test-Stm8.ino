
#if defined(__AVR_ATmega328P__)
#include "atmega328p.h"
#elif defined (STM8S103)
#include "stm8s103f3.h"
#endif

//Advance: auto-configure these setting
#define steps_per_one_revo 20
#define steps_per_change (steps_per_one_revo/2)
//Changing output direction
#define change_direction false//use 1 pin, physically pull to vcc->true, to gnd->false, float->auto detect
#define millisecond_after_finish_to_disable_mosfet -1//avoiding overheat, -1 for disable this feature
//we should read ms1-3 settings, change in future

//notice short signed int limit
volatile int hall_pos=0;
volatile int hall_dir=-1;
int step_pos=0;
volatile int dest_pos=0;
unsigned int finish_time=0;//overflow after 2 month
bool finish_flag=true;//both INT0(input) or INT1(external force) would change the flag

//#define Pulse_Delay 2
#define Pulse_Delay 500

void on_STEP_INPUT_rised()
{
  digitalWrite(EN_PIN, LOW);
  finish_flag=false;
  //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  if(digitalRead(DIR_INPUT)) dest_pos++; else dest_pos--;
  //digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
}
//test-stm8
volatile int hall_sensor=0;
void on_HALL_PULSE_changed()
{
  if(digitalRead(HALL_DIR_PIN)) hall_sensor++; else hall_sensor--;
}
//problem: why stm8 entering EXTI repeatly without external votage change? we see change on hall_sensor counter, however iret was called in IMPLEMENT_ISR (WInterrupts.c)
//Solve: we add a capacitance and it works, and I think we need another one on STEP_INPUT/*EXTI3*/
/*void on_HALL_PULSE_changed()
{
  digitalWrite(EN_PIN, LOW);
  finish_flag=false;
  if(hall_dir==-1){//1st time to reach a limit
    hall_pos=step_pos;//to estimate the initial position
    hall_dir=digitalRead(HALL_DIR_PIN);
  }
  else if(hall_dir==digitalRead(HALL_DIR_PIN)){//forward
    if(digitalRead(HALL_DIR_PIN)) hall_pos+=steps_per_change; else hall_pos-=steps_per_change;
    step_pos=hall_pos;
  }
  else{//backward
    if(digitalRead(HALL_DIR_PIN)) hall_pos++; else hall_pos--;
    step_pos=hall_pos;
    hall_dir=digitalRead(HALL_DIR_PIN);
  }
}*/

void setup() {
  
  //Serial.begin(9600);
  Serial_begin(9600);
  //Serial_println_s("Setup()");
  
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  /*pinMode(SLP_PIN, OUTPUT);
  pinMode(RST_PIN, OUTPUT);*/
  pinMode(EN_PIN, OUTPUT);
  //pinMode(MS1_PIN, INPUT);
  //pinMode(MS2_PIN, INPUT);
  //pinMode(MS3_PIN, INPUT);
  pinMode(HALL_PULSE_PIN, INPUT_PULLUP);
  pinMode(HALL_DIR_PIN, INPUT_PULLUP);
  //pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(DIR_INPUT, INPUT);
  pinMode(STEP_INPUT, INPUT);
  
#if defined(STM8S103)
  //ugly walkaround
  EXTI_DeInit();
#define GPIO_Pin_3 ((uint16_t)0x0008)
  //attachInterrupt(digitalPinToInterrupt(STEP_INPUT), on_STEP_INPUT_rised, RISING);
  //GPIO_Init(GPIOD, GPIO_Pin_3, GPIO_MODE_IN_FL_IT);
  //EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_FALL_ONLY);
  attachInterrupt(digitalPinToInterrupt(HALL_PULSE_PIN), on_HALL_PULSE_changed, CHANGE);
  GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_MODE_IN_PU_IT);
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_RISE_FALL);
  enableInterrupts();
  
#else
  attachInterrupt(digitalPinToInterrupt(STEP_INPUT), on_STEP_INPUT_rised, RISING);
  attachInterrupt(digitalPinToInterrupt(HALL_PULSE_PIN), on_HALL_PULSE_changed, CHANGE);//one revolution=one RISING+one FALLING
#endif
  //Serial_println_s("end Setup()");
}

/*void loop() {
  if(step_pos==dest_pos)
  {
    if(finish_flag){
      if(-1!=millisecond_after_finish_to_disable_mosfet){
        if((millis()-finish_time)>=millisecond_after_finish_to_disable_mosfet) digitalWrite(EN_PIN, HIGH);
      }
    }
    else{
      finish_flag=true;
      finish_time=millis();
    }
    return;
  }
  else if(step_pos<dest_pos) digitalWrite(DIR_PIN, (change_direction?HIGH:LOW));
  else digitalWrite(DIR_PIN, (change_direction?LOW:HIGH));

  //calc step_pos before taking action, to avoid conflicting with INT1
  if(step_pos<dest_pos && step_pos<(hall_dir?hall_pos+steps_per_change-1:hall_pos)) step_pos++;
  if(step_pos>dest_pos && step_pos>(hall_dir?hall_pos:hall_pos-steps_per_change+1)) step_pos--;
  //reaching limit will trigger the INT1
  digitalWrite(STEP_PIN, HIGH);
  delay(Pulse_Delay);                       // wait for a second
  digitalWrite(STEP_PIN, LOW);
  delay(Pulse_Delay);                       // wait for a second
  
}*/
//test-stm8
int step_count=0;

#define Total_Pulse 40

void loop() {
  digitalWrite(DIR_PIN, LOW); 
  for(int i=0;i<Total_Pulse;i++)
  {
    digitalWrite(STEP_PIN, HIGH);
    //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(Pulse_Delay);                       // wait for a second
    digitalWrite(STEP_PIN, LOW);
    //digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(Pulse_Delay);                       // wait for a second
    
    step_count++;
    
    Serial_print_i(hall_sensor);
    Serial_print_s("\t");
    Serial_println_i(step_count);
    //hall_sensor=digitalRead(HALL_PULSE_PIN);
    //digitalWrite(LED_BUILTIN, hall_sensor);
    
  }
  
  digitalWrite(DIR_PIN, HIGH); 
  for(int i=0;i<Total_Pulse;i++)
  {
    digitalWrite(STEP_PIN, HIGH);
    //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(Pulse_Delay);                       // wait for a second
    digitalWrite(STEP_PIN, LOW);
    //digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(Pulse_Delay);                       // wait for a second
    
    step_count--;
    
    Serial_print_i(hall_sensor);
    Serial_print_s("\t");
    Serial_println_i(step_count);
    //hall_sensor=digitalRead(HALL_PULSE_PIN);
    //digitalWrite(LED_BUILTIN, hall_sensor);
    
  }
}
