#define DIR_INPUT 2/*INT 0*/
#define STEP_INPUT 5
#define DIR_PIN 7
#define STEP_PIN 6
#define HALL_PULSE_PIN 3/*INT 1*/
#define HALL_DIR_PIN 4

/*
#define SLP_PIN 2
#define RST_PIN 3
#define MS3_PIN 4
#define MS2_PIN 5
#define MS1_PIN 6
#define EN_PIN 7
*/

void setup() {
  
  Serial.begin(9600);
  
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  /*pinMode(SLP_PIN, OUTPUT);
  pinMode(RST_PIN, OUTPUT);
  pinMode(MS3_PIN, OUTPUT);
  pinMode(MS2_PIN, OUTPUT);
  pinMode(MS1_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);*/
  pinMode(HALL_PULSE_PIN, INPUT_PULLUP);//fix: connnect to open-drain pin
  pinMode(HALL_DIR_PIN, INPUT_PULLUP);//fix: connnect to open-drain pin
  pinMode(LED_BUILTIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(HALL_PULSE_PIN),on_HALL_PULSE_changed,CHANGE);//one revolution=one RISING+one FALLING
}

volatile int hall_sensor=0;
int step_count=0;

void on_HALL_PULSE_changed()
{
  if(digitalRead(HALL_DIR_PIN)) hall_sensor++; else hall_sensor--;
}
/*void on_HALL_PULSE_RISING()
{
  //Serial.print(hall_sensor);
  //Serial.println(" on_HALL_PULSE_RISING()");
  if(digitalRead(HALL_DIR_PIN)) hall_sensor++; else hall_sensor--;
}*/

void loop() {
  digitalWrite(DIR_PIN, LOW); 
  for(int i=0;i<200;i++)
  {
    digitalWrite(STEP_PIN, HIGH);
    //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(10);                       // wait for a second
    digitalWrite(STEP_PIN, LOW);
    //digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(490);                       // wait for a second
    
    step_count++;
    
    Serial.print(hall_sensor);
    Serial.print('\t');
    Serial.println(step_count);
    //hall_sensor=digitalRead(HALL_PULSE_PIN);
    //digitalWrite(LED_BUILTIN, hall_sensor);
    
  }
  
  digitalWrite(DIR_PIN, HIGH); 
  for(int i=0;i<200;i++)
  {
    digitalWrite(STEP_PIN, HIGH);
    //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(10);                       // wait for a second
    digitalWrite(STEP_PIN, LOW);
    //digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(490);                       // wait for a second
    
    step_count--;
    
    Serial.print(hall_sensor);
    Serial.print('\t');
    Serial.println(step_count);
    //hall_sensor=digitalRead(HALL_PULSE_PIN);
    //digitalWrite(LED_BUILTIN, hall_sensor);
    
  }
}
