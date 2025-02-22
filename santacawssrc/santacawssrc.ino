// santa caws 

// this is super-duper messy because I've been busy and then sick

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include <pins_arduino.h>

// #define PIN_OUTPUT_COUNT 10
#define PIN_BUTTON PIN_PA4

#undef MILLIS_USE_TIMERB0 

#define DEBOUNCE_TIME 75

#define CORRECTED_MILLIS (millis())
#define CORRECTED_DELAY(ms) (delay(ms))

void blinkPattern();
void fhqwhgadsPattern();
void fadeoutPattern();
void secretMessagePattern();
void spinPattern();
void clockPattern();

typedef void (*PatternFunction)();

PatternFunction patternList[] = {
  spinPattern,
  
  fhqwhgadsPattern,
  blinkPattern,
  clockPattern,
  
  fadeoutPattern,
  secretMessagePattern,
};

#define NUM_PATTERNS (sizeof(patternList) / sizeof(PatternFunction))

int mainboardPins[] = {
  // 1, 3, 2, 6
  PIN_PA5, PIN_PA7, PIN_PA6, PIN_PB1
};

int swPwmPins[] = {
  PIN_PA5, PIN_PA7, PIN_PA6, PIN_PB1,
  PIN_PA3, PIN_PA1, PIN_PA2,
};

#define COUNT_PWM_PINS 7

#define MAINBOARD_PINS_LENGTH 4

int childboardCommon = PIN_PA3; // 10
int childboardLeft = PIN_PA1; // 8
int childboardRight = PIN_PA2; // 9

unsigned long lastDebounceTime = 0;
int pattern = 0;

uint8_t pwmTimers[COUNT_PWM_PINS] = {0};
bool pwmTimersEnabled[COUNT_PWM_PINS] = {0};
int pwmTimer = 0;

void setupIsr();

// the setup function runs once when you press reset or power the board
void setup() {
  
  for (int i = 0; i < COUNT_PWM_PINS; i++)
  {
    pinMode(swPwmPins[i], OUTPUT);
  }
  // pinMode(childboardCommon, OUTPUT);
  // pinMode(childboardLeft, OUTPUT);
  // pinMode(childboardRight, OUTPUT);

  pinMode(PIN_BUTTON, INPUT_PULLUP);

  setupIsr();
}

int lastButtonState = 0;
int buttonState = 0;

#define TCB0_CTRLA_ENABLE 1
#define TCB0_CTRLA_CLKSEL_1 0 // CLK_PER
#define TCB0_CTRLA_CLKSEL_half 1 // CLK_PER / 2
#define TCB0_CTRLB_ASYNC 0b01000000
#define TCB0_CTRLB_PERIODIC_INTERRUPT_MODE 0b00

void setupIsr() {
  TCB0.CCMP = 0xfff;
  TCB0.CTRLB = TCB0_CTRLB_ASYNC | TCB0_CTRLB_PERIODIC_INTERRUPT_MODE;
  TCB0.INTCTRL = 1 ; // no capture interrupt
  TCB0.CTRLA = TCB0_CTRLA_CLKSEL_1 | TCB0_CTRLA_ENABLE;
  
  // pin 2 interrupt
  // bit 7 invert I/O
  // pull up enabled (bit 3)
  // bits 2:0 - sense falling edge
  // PORTA_PIN2CTRL = 0b1000 | 0x3;

  // button is on pin a4
  // TODO TODO check these flags
  PORTA.INTFLAGS = (1 << 4);// 0b100;
  PORTA.PIN4CTRL = 0b10000000 | 0b1000 | 0b11;
  VPORTA.INTFLAGS = (1 << 4);
  // PORTA.INTCTRL = 1;
  // PORTA.PIN2CTRL = 0b1011;
}
void setPwm(int pin, bool pwmEnabled, int pwmRate)
{
  pwmTimersEnabled[pin] = pwmEnabled;
  if (pwmEnabled)
  {
    pwmTimers[pin] = pwmRate & 255;
  }
  else {
    pwmTimers[pin] = 0;
  }
}

bool isporta(int pin) {

  // only pin PB1 is used 
  // return pin != PIN_PB0
  // if (pin < PIN_PB0 && pin > PIN_PB3) {
  if (pin == PIN_PB1) {
    return false;
  }
  return true;
}

ISR(TCB0_INT_vect) {

  pwmTimer = (pwmTimer + 1) % 255;
  uint8_t portASet = 0, portAClear = 0;
  uint8_t portBSet = 0, portBClear = 0;
  
  for (int i = 0; i < COUNT_PWM_PINS; i++) {
    // preserve whatevers already in the register
    if (pwmTimersEnabled[i] == false) continue;

    // not sure this will work with the common childboard, dunno
    int swPwmPin = swPwmPins[i];
    
    if (pwmTimer < pwmTimers[i]) {

      // this was annoying
      if (isporta(swPwmPin)) {
        portASet |= digital_pin_to_bit_mask[swPwmPin];
      } else {
        portBSet |= digital_pin_to_bit_mask[swPwmPin];
      }
    }
    else {
      if (isporta(swPwmPin)) {
        portAClear |= digital_pin_to_bit_mask[swPwmPin];
      } else {
        portBClear |= digital_pin_to_bit_mask[swPwmPin];
      }
    }
  }

  PORTA.OUTSET = portASet;
  PORTA.OUTCLR = portAClear;
  PORTB.OUTSET = portBSet;
  PORTB.OUTCLR = portBClear;

  TCB0.INTFLAGS = 0; // clear interrupt
}

ISR(PORTA_PORT_vect) {
  byte flags = PORTA.INTFLAGS;
  PORTA.INTFLAGS |= flags;
  
  if ((CORRECTED_MILLIS - lastDebounceTime) > DEBOUNCE_TIME) {

    pattern = (pattern + 1) % NUM_PATTERNS;
    clear();

    lastDebounceTime = CORRECTED_MILLIS;
  }
}

// anything beyond this is too dang bright
// #define MAX_PWM_BRIGHTNESS 75

void clearPwm() {
  for (int i = 0; i < COUNT_PWM_PINS; i++) {
    setPwm(i, false, 0);
  }
}

void clear()
{
  clearPwm();
  PORTA.OUT = 0;
  PORTB.OUT = 0;
}

void loop() {
  int lastPattern = pattern;
  (*patternList[lastPattern])();

  // pattern has changed
  if (pattern != lastPattern) {
    clear();
  }

  delay(50); // doesn't need to spin super duper fast
}

// void clockPattern() {
//   // clear();
//   int time = ((int)(CORRECTED_MILLIS / 1000));

//   for (int i = 0; i < MAINBOARD_PINS_LENGTH; i++) {
//     bool b = time & (1 << i);
//     digitalWrite(mainboardPins[i], b > 0);
//   }
// }

void clockPattern() {
  // clear();
  int time = ((int)(CORRECTED_MILLIS / 1000));

  for (int i = 0; i < MAINBOARD_PINS_LENGTH; i++) {
    bool b = time & (1 << i);
    digitalWrite(mainboardPins[i], b > 0);
  }
}

int vSpinPatternLUT[] = {0, 50, 255, 128 };

void spinPattern() {

   int offset = (int)(CORRECTED_MILLIS / 75.0) % MAINBOARD_PINS_LENGTH;

  for (int i = 0; i < MAINBOARD_PINS_LENGTH; i++) {
    setPwm(i, true, vSpinPatternLUT[(i + offset) % MAINBOARD_PINS_LENGTH]);
  }
}

void fadeoutPattern() {
  int r = rand() % MAINBOARD_PINS_LENGTH;

  setPwm(swPwmPins[r], true, 255);

  for (int i = 0; i < COUNT_PWM_PINS; i++) {
    pwmTimers[i] *= 0.75;
  }

  delay(rand() % 100);
}

void allOff() {
  for (int i = 0; i < MAINBOARD_PINS_LENGTH; i++) {
    digitalWrite(mainboardPins[i], LOW);
  }
}

void allOn() {
  for (int i = 0; i < MAINBOARD_PINS_LENGTH; i++) {
    digitalWrite(mainboardPins[i], HIGH);
  }
}

bool blinkState = false;
void blinkPattern() {
  for (int i = 0; i < MAINBOARD_PINS_LENGTH; i++) {
    // mainboardPins
    // PIN_PA5, PIN_PA7, PIN_PA6, PIN_PB1
    digitalWrite(mainboardPins[i], blinkState);
  }
  digitalWrite(childboardCommon, blinkState);
  digitalWrite(childboardLeft, !blinkState);
  digitalWrite(childboardRight, !blinkState);
  delay(500);
  blinkState = !blinkState;
}

void fhqwhgadsPattern() {

  allOff();
  delay(50 + rand() % 100);

  int r = rand() % MAINBOARD_PINS_LENGTH;
  digitalWrite(mainboardPins[r], HIGH);
  delay(rand() % 300);
}

void showNibble(int nibble) {
  allOff();
  for (int i = 0; i < MAINBOARD_PINS_LENGTH; i++) {
    bool b = nibble & (1 << i);
    digitalWrite(mainboardPins[i], b);
  }
}

void secretMessagePattern() {
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < MAINBOARD_PINS_LENGTH; i++) {
      allOff();
      digitalWrite(mainboardPins[i], HIGH);
      delay(100);
      allOff();
      delay(100);
    }
    delay(200);
  }

  const char message[] = "secret=Y'all ever just yeet?? Lmao";
  const int len = 28;

  for (int i = 0; i < len; i++) {
    // should alternate the daughterboard so that timing is more clear
    // upper nibble
    showNibble(message[i] >> 4);
    delay(100);

    // lower nibble
    showNibble(message[i]);
    delay(100);
  }

  for (int i = 0; i < 5; i++) {
    allOff();
    delay(100);
    allOn();
    delay(100);
  }
}
