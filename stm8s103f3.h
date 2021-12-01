/*enum portpin {
	PA1, // OSC
	PA2, // OSC
	PA3, // EXTI0
	PB5, // I2C without pull-up
	PB4, // I2C without pull-up
	PC3, // EXTI2*
	PC4, //*
	PC5, // SPI*
	PC6, // SPI*
	PC7, // SPI*
	PD1, // SWIM 
	PD2, //*
	PD3, // EXTI3*
	PD4,
	PD5, // UART
	PD6  // UART 
};*/

//fix : sduino\hardware\stm8\0.5.0\variants\standard\pins_arduino.h
#ifndef digitalPinToInterrupt
const uint8_t digitalPinToInterruptMap[] = {
  0, // PA1
  0, // PA2
  0, // PA3
  1, // PB5
  1, // PB4
  2, // PC3
  2, // PC4
  2, // PC5
  2, // PC6
  2, // PC7
  3, // PD1
  3, // PD2
  3, // PD3
  3, // PD4
  3, // PD5
  3  // PD6
};
#define digitalPinToInterrupt(p)  (((uint16_t)p) < NUM_DIGITAL_PINS ? digitalPinToInterruptMap[p] : NOT_AN_INTERRUPT)
#endif//digitalPinToInterrupt
//end fix : sduino\hardware\stm8\0.5.0\variants\standard\pins_arduino.h

#define STEP_INPUT PD3/*EXTI3*/
#define DIR_INPUT PD2
#define DIR_PIN PC6
#define STEP_PIN PC5
#define HALL_PULSE_PIN PC3/*EXTI2*/ //PULLUP
#define HALL_DIR_PIN PC4 //PULLUP
#define EN_PIN PC7

/*
#define MS3_PIN 4
#define MS2_PIN 5
#define MS1_PIN 6
*/


//stm8 external interrupt quite different from arduino
/*
//stm8s003 have EXTI on every digital pin but have only 6 vector. Since we use only two, maybe we could limit EXTI to some pins to avoid problem.
#define EXTERNAL_NUM_INTERRUPTS 6
typedef void (*voidFuncPtr)(void);
static void nothing(void) {}
static volatile voidFuncPtr intFunc[EXTERNAL_NUM_INTERRUPTS] = {nothing,nothing,nothing,nothing,nothing,nothing}

uint8_t digitalPinToInterrupt(uint8_t pin){
  
}
//RISING,FALLING
void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode) {
  if(interruptNum < EXTERNAL_NUM_INTERRUPTS) {
    intFunc[interruptNum] = userFunc;
  }
}
void detachInterrupt(uint8_t interruptNum) {
  if(interruptNum < EXTERNAL_NUM_INTERRUPTS) {
  }
}
*/
