#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);


  
  int LCD_power = 8;            // Power for LCD
  
  int signal_pin = 10;          // buzzer + red light
  int green_pin = 13;           // green light
  int main_state = 0;           // initial state
  
  const int button_1 = 3;       // button 1 pin
  boolean button_1_state = 1;   // button 1 state
  const int button_2 = 2;       // button 2 pin
  boolean button_2_state = 1;   // button 2 staet
  int reading1;                 // current state bt1
  int reading2;                 // current state bt2
  int lastButton1State = LOW;   // the previous reading from the btn1 pin
  int lastButton2State = LOW;   // the previous reading from the btn2 pin
  
  boolean enter_2stars = false; // green lighting on first correct move
  boolean enter_3stars = false; // green lighting on second correct move

  unsigned long lastDebounceTime1 = 0;   // the last time the output pin was toggled
  unsigned long lastDebounceTime2 = 0;   // the last time the output pin was toggled
  unsigned long debounceDelay = 50;      // debounce time

  int timer = 1;                // stage 1 counter
  int timer_sec;                // countdown seconds
  float timer_fast;             // stage 2 counter
  float timer_faster;           // stage 3 counter
  int timer_sec_remainder;      // display ":0" sec if counter < 10
  int timer_min_remainder;      // display ":0" min if counter < 10
  
  int SEQ = 300;                // target defuse sequence
  
  const int Red_wire = 12;      // bomb red wire
  boolean R = 0;                // red state
  const int Blue_wire = 11;     // bomb blue wire
  boolean B = 0;                // blue state
  const int Green_wire = 7;     // bomb green wire
  boolean G = 0;                // green state
  const int Yellow_wire = 8;    // bomb yellow wire
  boolean Y = 0;                // yellow state
  
  int D_current[] = {R,B,G,Y};  // current defuse sequence
  int D_State = 0;              // current defuse state
  int D0[] = {0,0,0,0};         // full armed
  int D1[] = {0,0,0,0};         // 1 step defused
  int D2[] = {0,0,0,0};         // 2 steps defused
  int D3[] = {0,0,0,0};         // full defused

void intro() {
  tone(signal_pin, 600); 
    delay(100);
    noTone(signal_pin);
    lcd.setCursor(1,0);
    lcd.print("PM");
    delay(500);
    lcd.setCursor(1,0);
    lcd.print("PM 2022");
    delay(500);
    lcd.setCursor(1,0);
    lcd.print("PM 2022 Project");
    delay(500);
    lcd.setCursor(0,1);
    lcd.print("");
    delay(1500);
    
    lcd.setCursor(0,1);
    lcd.print("Defuse in time");
    delay(800);
    
    lcd.setCursor(0,1);
    lcd.print("efuse in time o");
    delay(800);
    
    lcd.setCursor(0,1);
    lcd.print("fuse in time or");
    delay(800);
    
    lcd.setCursor(0,1);
    lcd.print("se in time or BO");
    delay(800);
    
    lcd.setCursor(0,1);
    lcd.print(" in time or BOOM");
    
    delay(2000);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Set timer [min]:");
    lcd.setCursor(0,1);
    lcd.print("timer = ");
    lcd.print(timer);
}

void setup() {
  pinMode(LCD_power, OUTPUT); 
  digitalWrite(LCD_power, HIGH);
  
  pinMode(signal_pin, OUTPUT);
  pinMode(green_pin, OUTPUT); 
  
  pinMode(button_1, INPUT_PULLUP); 
  pinMode(button_2, INPUT_PULLUP); 
  
  lcd.init();                      // initialize the lcd 
  Serial.begin(9600);              // initialize serial
  
  pinMode(Red_wire, INPUT_PULLUP);
  pinMode(Blue_wire, INPUT_PULLUP); 
  pinMode(Green_wire, INPUT_PULLUP); 
  pinMode(Yellow_wire, INPUT_PULLUP); 

  digitalWrite(signal_pin, 1);
  delay(50);
  digitalWrite(signal_pin, 0);
  delay(50);
  digitalWrite(signal_pin, 1);
  delay(50);
  digitalWrite(signal_pin, 0);
  delay(50);
    
  lcd.backlight();
  lcd.clear();
  digitalWrite(green_pin, 1);
    
  intro();
    
}

void incrementTimer() {
        timer = timer + 1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Set timer [min]:");
        lcd.setCursor(0,1);
        lcd.print("timer = ");
        lcd.print(timer);
}

void nextState() {
    main_state = (main_state + 1) % 5;
}

void displayTime() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ARMED:");
  lcd.setCursor(7,0);
  timer_min_remainder = int(timer_sec / 60);
  timer_sec_remainder = timer_sec % 60;
  if (timer_min_remainder < 10){ 
    lcd.print("0"); 
  }
  lcd.print(timer_min_remainder);
  if (timer_sec_remainder < 10){
    lcd.print(":0");
  }
  else {
    lcd.print(":");
  }
  lcd.print(timer_sec_remainder);
  
}
void checkButton(const int button,
                 boolean& button_state, 
                 int reading, 
                 int& lastButtonState, 
                 unsigned long& lastDebounceTime,
                 void (*action_1)(),
                 void (*action_2)()) {

  reading = digitalRead(button);
  if (reading != lastButtonState) {
      lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != button_state) {
      button_state = reading;
      if (button_state == LOW) {
        action_1();
        if (action_2)
          action_2();
      }
    }
  }
  lastButtonState = reading;
}

void plantingMessage() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Planting");
    digitalWrite(green_pin, 0);
    delay(500);
    lcd.setCursor(0,0);
    lcd.print("Planting.");
    digitalWrite(green_pin, 1);
    delay(500);
    lcd.setCursor(0,0);
    lcd.print("Planting..");
    digitalWrite(green_pin, 0);
    delay(500);
    lcd.setCursor(0,0);
    lcd.print("Planting...");
    digitalWrite(green_pin, 1);
    delay(500);
    lcd.setCursor(0,1);
    digitalWrite(green_pin, 0);
    delay(500);
    lcd.print("Done!");
    digitalWrite(green_pin, 1);
    delay(50);
    digitalWrite(green_pin, 0);
    delay(50);
    digitalWrite(green_pin, 1);
    delay(1000);
}

void beep() {
  digitalWrite(signal_pin, 1);
  tone(signal_pin, 600); 
  delay(50);
  timer_sec = timer_sec - 1;
  noTone(signal_pin);
  digitalWrite(signal_pin, 0);
  delay(950);
}

void beepFast() {
  digitalWrite(signal_pin, 1);
  tone(signal_pin, 600);
  delay(50);
  timer_sec = timer_sec - 1;
  noTone(signal_pin);
  digitalWrite(signal_pin, 0);
  delay(400);
  digitalWrite(signal_pin, 1);
  tone(signal_pin, 600);
  delay(50);
  noTone(signal_pin);
  digitalWrite(signal_pin, 0);
  delay(400);
}

void beepFaster() {
  digitalWrite(signal_pin, 1);
  tone(signal_pin, 600);
  delay(50);
  timer_sec = timer_sec - 1;
  noTone(signal_pin);
  digitalWrite(signal_pin, 0);
  delay(200);
  digitalWrite(signal_pin, 1);
  tone(signal_pin, 600);
  delay(50);
  noTone(signal_pin);
  digitalWrite(signal_pin, 0);
  delay(200);
  digitalWrite(signal_pin, 1);
  tone(signal_pin, 600);
  delay(50);
  noTone(signal_pin);
  digitalWrite(signal_pin, 0);
  delay(200);   
  digitalWrite(signal_pin, 1);
  tone(signal_pin, 600);
  delay(50);
  noTone(signal_pin);
  digitalWrite(signal_pin, 0);
  delay(200);
}

void boom() {
  lcd.clear();
  lcd.setCursor(0,0);
  digitalWrite(signal_pin, 1);
  tone(signal_pin, 100);
  delay(10);
  noTone(signal_pin);
  tone(signal_pin, 120);
  delay(10);
  noTone(signal_pin);
  tone(signal_pin, 150);
  delay(10);
  noTone(signal_pin);    
  tone(signal_pin, 180); 
  lcd.print(" TERRORISTS WIN");
  delay(3500);
  noTone(signal_pin);
  delay(1000000);
}
void loop() {
  if (main_state == 0){             // set timer
    checkButton(button_1, button_1_state, reading1, lastButton1State, lastDebounceTime1, incrementTimer, NULL);
    checkButton(button_2, button_2_state, reading2, lastButton2State, lastDebounceTime2, nextState, NULL);
    Serial.println(main_state);
  }
  else if (main_state == 1){        // generate defuse key
    plantingMessage();
    randomSeed(analogRead(0));
    int SEQ = random(1, 25);  
 
    if (SEQ == 1){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: R,B,G");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {1,0,0,0}; // *
      D1[0] = 1;
      //int D2[] = {1,1,0,0}; // **
      D2[0] = 1;
      D2[1] = 1;
      //int D3[] = {1,1,1,0}; // Disarmed
      D3[0] = 1;
      D3[1] = 1;
      D3[2] = 1;
      delay(1000); 
    }
    else if (SEQ == 2){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: R,B,Y");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {1,0,0,0}; // *
      D1[0] = 1;
      //int D2[] = {1,1,0,0}; // **
      D2[0] = 1;
      D2[1] = 1;
      //int D3[] = {1,1,0,1}; // Disarmed
      D3[0] = 1;
      D3[1] = 1;
      D3[3] = 1;
      delay(1000);
    }
    else if (SEQ == 3){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: R,G,B");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {1,0,0,0}; // *
      D1[0] = 1;
      //int D2[] = {1,0,1,0}; // **
      D2[0] = 1;
      D2[2] = 1;
      //int D3[] = {1,1,1,0}; // Disarmed
      D3[0] = 1;
      D3[1] = 1;
      D3[2] = 1;
      delay(1000);
    }
    else if (SEQ == 4){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: R,G,Y");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {1,0,0,0}; // *
      D1[0] = 1;
      //int D2[] = {1,0,1,0}; // **
      D2[0] = 1;
      D2[2] = 1;
      //int D3[] = {1,0,1,1}; // Disarmed
      D3[0] = 1;
      D3[2] = 1;
      D3[3] = 1;
      delay(1000);
    }
    else if (SEQ == 5){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: R,Y,B");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {1,0,0,0}; // *
      D1[0] = 1;
      //int D2[] = {1,0,0,1}; // **
      D2[0] = 1;
      D2[3] = 1;
      //int D3[] = {1,1,0,1}; // Disarmed
      D3[0] = 1;
      D3[1] = 1;
      D3[3] = 1;
      delay(1000);
    }
    else if (SEQ == 6){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: R,Y,G");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {1,0,0,0}; // *
      D1[0] = 1;
      //int D2[] = {1,0,0,1}; // **
      D2[0] = 1;
      D2[3] = 1;
      //int D3[] = {1,0,1,1}; // Disarmed
      D3[0] = 1;
      D3[2] = 1;
      D3[3] = 1;
      delay(1000);
    }
    else if (SEQ == 7){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: B,R,G");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {0,1,0,0}; // *
      D1[1] = 1;
      //int D2[] = {1,1,0,0}; // **
      D2[0] = 1;
      D2[1] = 1;
      //int D3[] = {1,1,1,0}; // Disarmed
      D3[0] = 1;
      D3[1] = 1;
      D3[2] = 1;
      delay(1000);
    }
    else if (SEQ == 8){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: B,R,Y");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {0,1,0,0}; // *
      D1[1] = 1;
      //int D2[] = {1,1,0,0}; // **
      D2[0] = 1; 
      D2[1] = 1;
      //int D3[] = {1,1,0,1}; // Disarmed
      D3[0] = 1;
      D3[1] = 1;
      D3[3] = 1;
      delay(1000);
    }
    else if (SEQ == 9){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: B,G,R");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {0,1,0,0}; // *
      D1[1] = 1;
      //int D2[] = {0,1,1,0}; // **
      D2[1] = 1;
      D2[2] = 1;
      //int D3[] = {1,1,1,0}; // Disarmed
      D3[0] = 1;
      D3[1] = 1;
      D3[2] = 1;
      delay(1000);
    }
    else if (SEQ == 10){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: B,G,Y");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {0,1,0,0}; // *
      D1[1] = 1;
      //int D2[] = {0,1,1,0}; // **
      D2[1] = 1;
      D2[2] = 1;
      //int D3[] = {0,1,1,1}; // Disarmed
      D3[1] = 1;
      D3[2] = 1;
      D3[3] = 1;
      delay(1000);
    }
    else if (SEQ == 11){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: B,Y,R");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {0,1,0,0}; // *
      D1[1] = 1;
      //int D2[] = {0,1,0,1}; // **
      D2[1] = 1;
      D2[3] = 1;
      //int D3[] = {1,1,0,1}; // Disarmed
      D3[0] = 1;
      D3[1] = 1;
      D3[3] = 1;
      delay(1000);
    }
    else if (SEQ == 12){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: B,Y,G");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {0,1,0,0}; // *
      D1[1] = 1;
      //int D2[] = {0,1,0,1}; // **
      D2[1] = 1;
      D2[3] = 1;
      //int D3[] = {0,1,1,1}; // Disarmed
      D3[1] = 1;
      D3[2] = 1;
      D3[3] = 1;
      delay(1000);
    }
    else if (SEQ == 13){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: G,R,B");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {0,0,1,0}; // *
      D1[2] = 1;
      //int D2[] = {1,0,1,0}; // **
      D2[0] = 1;
      D2[2] = 1;
      //int D3[] = {1,1,1,0}; // Disarmed
      D3[0] = 1;
      D3[1] = 1;
      D3[2] = 1;
      delay(1000);
    }
    else if (SEQ == 14){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: G,R,Y");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {0,0,1,0}; // *
      D1[2] = 1;
      //int D2[] = {1,0,1,0}; // **
      D2[0] = 1;
      D2[2] = 1;
      //int D3[] = {1,0,1,1}; // Disarmed
      D3[0] = 1;
      D3[2] = 1;
      D3[3] = 1;
      delay(1000);
    }
    else if (SEQ == 15){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: G,B,R");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {0,0,1,0}; // *
      D1[2] = 1;
      //int D2[] = {0,1,1,0}; // **
      D2[1] = 1;
      D2[2] = 1;
      //int D3[] = {1,1,1,0}; // Disarmed
      D3[0] = 1;
      D3[1] = 1;
      D3[2] = 1;
      delay(1000);
    }
    else if (SEQ == 16){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: G,B,Y");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {0,0,1,0}; // *
      D1[2] = 1;
      //int D2[] = {0,1,1,0}; // **
      D2[1] = 1;
      D2[2] = 1;
      //int D3[] = {0,1,1,1}; // Disarmed
      D3[1] = 1;
      D3[2] = 1;
      D3[3] = 1;
      delay(1000);
    }
    else if (SEQ == 17){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: G,Y,R");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {0,0,1,0}; // *
      D1[2] = 1;
      //int D2[] = {0,0,1,1}; // **
      D2[2] = 1;
      D2[3] = 1;
      //int D3[] = {1,0,1,1}; // Disarmed
      D3[0] = 1;
      D3[2] = 1;
      D3[3] = 1;
      delay(1000);
    }
    else if (SEQ == 18){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: G,Y,B");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {0,0,1,0}; // *
      D1[2] = 1;
      //int D2[] = {0,0,1,1}; // **
      D2[2] = 1;
      D2[3] = 1;
      //int D3[] = {0,1,1,1}; // Disarmed
      D3[1] = 1;
      D3[2] = 1;
      D3[3] = 1;
      delay(1000);
    }
    else if (SEQ == 19){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: Y,R,B");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {0,0,0,1}; // *
      D1[3] = 1;
      //int D2[] = {1,0,0,1}; // **
      D2[0] = 1;
      D2[3] = 1;
      //int D3[] = {1,1,0,1}; // Disarmed
      D3[0] = 1;
      D3[1] = 1;
      D3[3] = 1;
      delay(1000);
    }
    else if (SEQ == 20){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: Y,R,G");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {0,0,0,1}; // *
      D1[3] = 1;
      //int D2[] = {1,0,0,1}; // **
      D2[0] = 1;
      D2[3] = 1;
      //int D3[] = {1,0,1,1}; // Disarmed
      D3[0] = 1;
      D3[2] = 1;
      D3[3] = 1;
      delay(1000);
    }
    else if (SEQ == 21){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: Y,B,R");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {0,0,0,1}; // *
      D1[3] = 1;
      //int D2[] = {0,1,0,1}; // **
      D2[1] = 1;
      D2[3] = 1;
      //int D3[] = {1,1,0,1}; // Disarmed
      D3[0] = 1;
      D3[1] = 1;
      D3[3] = 1;
      delay(1000);
    }
    else if (SEQ == 22){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: Y,B,G");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {0,0,0,1}; // *
      D1[3] = 1;
      //int D2[] = {0,1,0,1}; // **
      D2[1] = 1;
      D2[3] = 1;
      //int D3[] = {0,1,1,1}; // Disarmed
      D3[1] = 1;
      D3[2] = 1;
      D3[3] = 1;
      delay(1000);
    }
    else if (SEQ == 23){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: Y,G,R");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {0,0,0,1}; // *
      D1[3] = 1;
      //int D2[] = {0,0,1,1}; // **
      D2[2] = 1;
      D2[3] = 1;
      //int D3[] = {1,0,1,1}; // Disarmed
      D3[0] = 1;
      D3[2] = 1; 
      D3[3] = 1;
      delay(1000);
    }
    else if (SEQ == 24){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("KEY: Y,G,B");
      // Set the Disarm steps {R,B,G,Y}:
      //int D0[] = {0,0,0,0}; // Fully armed
      //int D1[] = {0,0,0,1}; // *
      D1[3] = 1;
      //int D2[] = {0,0,1,1}; // **
      D2[2] = 1;
      D2[3] = 1;
      //int D3[] = {0,1,1,1}; // Disarmed
      D3[1] = 1;
      D3[2] = 1;
      D3[3] = 1;
      delay(1000);
    }
    nextState();
 }
  else if (main_state == 2){        // arm bomb
    lcd.setCursor(0,1);
    lcd.print("Press B2 to ARM");
    checkButton(button_2, button_2_state, reading2, lastButton2State, lastDebounceTime2, displayTime, nextState);
    timer_sec = timer * 60;
    timer_fast = timer_sec / 6;
    timer_faster = timer_fast / 3;
 }
 else if (main_state == 3){         // time to defuse
  digitalWrite(green_pin, 0);
  displayTime();
  
  if (D_State == 0){
    lcd.setCursor(0,1);
    lcd.print("***");
  }
  else if (D_State == 1){
      if (!enter_2stars) {
          tone(signal_pin, 1000);
          digitalWrite(signal_pin, 0);
          digitalWrite(green_pin, 1);
          delay(500);
          digitalWrite(green_pin, 0);
          noTone(signal_pin);
          enter_2stars = true;
      }
      lcd.setCursor(0,1);
      lcd.print("**");
  }
  else if (D_State == 2){
      if (!enter_3stars) {
          tone(signal_pin, 1000);
          digitalWrite(signal_pin, 0);
          digitalWrite(green_pin, 1);
          delay(500);
          digitalWrite(green_pin, 0);
          noTone(signal_pin);
          enter_3stars = true;
      }
      lcd.setCursor(0,1);
      lcd.print("*");
  }

  if (timer_sec <= 1){
      nextState();
  }
  else if (timer_sec < timer_faster){
      beepFaster();
  }
  else if (timer_sec < timer_fast){
      beepFast();
  }
  else if (timer_sec > 1){
      beep();
  }
    
  check_D_current();
    
  if (D_State == 0){
    if (
      D_current[0] == D0[0] and
      D_current[1] == D0[1] and
      D_current[2] == D0[2] and
      D_current[3] == D0[3]
      ){
        Serial.println(" ");
      }
    else {
        if (
          D_current[0] == D1[0] and
          D_current[1] == D1[1] and
          D_current[2] == D1[2] and
          D_current[3] == D1[3]
          ){
            digitalWrite(signal_pin, 1);
            delay(1000);
            D_State = 1;
            digitalWrite(signal_pin, 0);
            delay(50);
            digitalWrite(signal_pin, 1);
            delay(50);
            digitalWrite(signal_pin, 0);
            delay(50);
            digitalWrite(signal_pin, 1);
            delay(50);
            digitalWrite(signal_pin, 0);
          }
        //----- if D_current != D1 -> BOOM
        else {
        digitalWrite(signal_pin, 1);
        main_state = 4;
        delay(1000);
        digitalWrite(signal_pin, 1);
        }
    }
  } // end D_State == 0
  
  
  
  if (D_State == 1){
    if (
      D_current[0] == D1[0] and
      D_current[1] == D1[1] and
      D_current[2] == D1[2] and
      D_current[3] == D1[3]
      ){
        Serial.println(" ");
      }
    else {
        Serial.println(D2[0]);
        Serial.println(D2[1]);
        Serial.println(D2[2]);
        Serial.println(D2[3]);
        Serial.println(" ");
        if (
          D_current[0] == D2[0] and
          D_current[1] == D2[1] and
          D_current[2] == D2[2] and
          D_current[3] == D2[3]
          ){
            digitalWrite(signal_pin, 1);
            delay(1000);
            D_State = 2;
            digitalWrite(signal_pin, 0);
            delay(50);
            digitalWrite(signal_pin, 1);
            delay(50);
            digitalWrite(signal_pin, 0);
            delay(50);
            digitalWrite(signal_pin, 1);
            delay(50);
            digitalWrite(signal_pin, 0);
          }
        else {
        digitalWrite(signal_pin, 1);
        main_state = 4;
        delay(1000);
        digitalWrite(signal_pin, 1);
        }
    }
  } // end D_State == 1
  
  
  
  if (D_State == 2){
  //--- if D_current == D0 : pass
    if (
      D_current[0] == D2[0] and
      D_current[1] == D2[1] and
      D_current[2] == D2[2] and
      D_current[3] == D2[3]
      ){
        Serial.println(" ");
      }
  //--- else if D_current != D0:
    else {
        //----- else if D_current == D1 -> D_state = 1
        if (
          D_current[0] == D3[0] and
          D_current[1] == D3[1] and
          D_current[2] == D3[2] and
          D_current[3] == D3[3]
          ){
            digitalWrite(signal_pin, 1);
            delay(1000);
            D_State = 3;
            digitalWrite(signal_pin, 0);
            delay(50);
            digitalWrite(signal_pin, 1);
            delay(50);
            digitalWrite(signal_pin, 0);
            delay(50);
            digitalWrite(signal_pin, 1);
            delay(50);
            digitalWrite(signal_pin, 0);
          }
        //----- if D_current != D1 -> BOOM
        else {
        digitalWrite(signal_pin, 1);
        main_state = 4;
        delay(1000);
        digitalWrite(signal_pin, 1);
        }
    }
  } // end D_State == 2
  
  
  
  if (D_State == 3){
    lcd.clear();
    lcd.setCursor(0,0);
      tone(signal_pin, 700); 
      digitalWrite(signal_pin, 1);
      delay(50);
      noTone(signal_pin);
      digitalWrite(signal_pin, 0);
      delay(50);
      tone(signal_pin, 700); 
      digitalWrite(signal_pin, 1);
      delay(50);
      noTone(signal_pin);
      digitalWrite(signal_pin, 0);
      delay(50);
      tone(signal_pin, 700); 
      digitalWrite(signal_pin, 1);
      delay(50);
      noTone(signal_pin);
      digitalWrite(signal_pin, 0);
      delay(50);
    lcd.print("  BOMB DEFUSED");
    lcd.setCursor(0,1);
    lcd.print(" YOUR TIME:");
    timer_min_remainder = int(timer_sec / 60);
    timer_sec_remainder = timer_sec % 60;
    if (timer_min_remainder < 10){ 
      lcd.print("0"); 
    }
    lcd.print(timer_min_remainder);
    if (timer_sec_remainder < 10){
      lcd.print(":0");
    }
    else {
      lcd.print(":");
    }
    lcd.print(timer_sec_remainder);
    digitalWrite(green_pin, 1);
    delay(1000000);
  } // end D_State == 3 
 } // END STATE 3
 else if (main_state == 4){         // detonated
    boom();
 }
}









// Check current bomb wires config:
void check_D_current(){
  R = digitalRead(Red_wire);
  B = digitalRead(Blue_wire);
  G = digitalRead(Green_wire);
  Y = digitalRead(Yellow_wire);
  // Print for debug:
  Serial.println("D_current:");
  D_current[0] = R;
  D_current[1] = B;
  D_current[2] = G;
  D_current[3] = Y;
  Serial.println(D_current[0]);
  Serial.println(D_current[1]);
  Serial.println(D_current[2]);
  Serial.println(D_current[3]);
  Serial.println(" ");
}





/* DISAMR SEQUENCE REF:
R,B,G 1
R,B,Y 2
R,G,B 3
R,G,Y 4
R,Y,B 5
R,Y,G 6

B,R,G 7
B,R,Y 8
B,G,R 9
B,G,Y 10
B,Y,R 11
B,Y,G 12

G,R,B 13
G,R,Y 14
G,B,R 15
G,B,Y 16
G,Y,R 17
G,Y,B 18

Y,R,B 19
Y,R,G 20
Y,B,R 21
Y,B,G 22
Y,G,R 23
Y,G,B 24
*/
