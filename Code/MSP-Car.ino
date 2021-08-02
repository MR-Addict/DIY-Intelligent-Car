#include <LiquidCrystal.h>

const uint8_t Right_A = 37;
const uint8_t Right_B = 38;
const uint8_t Left_A = 39;
const uint8_t Left_B = 40;
const uint8_t Left_Sensor = 23;
const uint8_t Center_Sensor = 24;
const uint8_t Right_Sensor = 25;
const uint8_t Buzzer = 26;
const uint8_t Rotary_Sensor = 27;
const uint8_t PWM_Pin = 11;

uint8_t message;
int Pre_Rotary_Value;
boolean Mode;

const int rs = 31, en = 32, d4 = 33, d5 = 34, d6 = 35, d7 = 36;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Init();
}

void loop() {
  Update_Speed();
  Get_Serial();
  if (Mode == 0)Route_Tracing();
  else if (Mode == 1)Mobile_Control();
  Show_Informatin();
}

void Init() {
  pinMode(Left_A, OUTPUT);
  pinMode(Left_B, OUTPUT);
  pinMode(Right_A, OUTPUT);
  pinMode(Right_B, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(PWM_Pin, OUTPUT);
  pinMode(Rotary_Sensor, INPUT);
  Serial1.begin(9600);
  lcd.begin(16, 2);
  Update_Speed();
  Stop();
  Buzzer_Ring();
}

void Update_Speed() {
  int Rotary_Value = analogRead(Rotary_Sensor);
  if (Pre_Rotary_Value - Rotary_Value > 2 || Pre_Rotary_Value - Rotary_Value < -2) {
    Pre_Rotary_Value = Rotary_Value;
    int val = map(Pre_Rotary_Value, 0, 1024, 0, 255);
    analogWrite(PWM_Pin, val);
  }
}

void Buzzer_Ring() {
  for (uint8_t i = 0; i < 3; i++) {
    digitalWrite(Buzzer, HIGH);
    delay(100);
    digitalWrite(Buzzer, LOW);
    delay(100);
  }
}

void Get_Serial() {
  if (Serial1.available() > 0) {
    message = Serial1.read();
    if (message == 'M' || message == 'm') {
      Mode = !Mode;
      Buzzer_Ring();
    }
  }
}

void Mobile_Control() {
  switch (message) {
    case 'r': Right(); break;
    case 'R': Right(); break;
    case 'b': Backword(); break;
    case 'B': Backword(); break;
    case 'l': Left(); break;
    case 'L': Left(); break;
    case 'f': Forword(); break;
    case 'F': Forword(); break;
    case 's': Stop(); break;
    case 'S': Stop(); break;
    default : Stop(); break;
  }
}

void Route_Tracing() {
  boolean Left_Value = digitalRead(Left_Sensor);
  boolean Center_Value = digitalRead(Center_Sensor);
  boolean Right_Value = digitalRead(Right_Sensor);
  if (Left_Value == 0 && Center_Value == 0 && Right_Value == 0) {
    Stop();
    message = 'S';
  }
  else if (Left_Value == 1 && Center_Value == 1 && Right_Value == 1) {
    message = 'S';
    Stop();
  }
  else if (Left_Value == 0 && Center_Value == 1 && Right_Value == 0) {
    message = 'F';
    Forword();
  }
  else if (Left_Value == 1 && Center_Value == 0 && Right_Value == 0) {
    message = 'R';
    Right();
  }
  else if (Left_Value == 0 && Center_Value == 0 && Right_Value == 1) {
    message = 'L';
    Left();
  }
  else if (Left_Value == 1 && Center_Value == 1 && Right_Value == 0) {
    message = 'L';
    Left();
  }
  else if (Left_Value == 0 && Center_Value == 1 && Right_Value == 1) {
    message = 'R';
    Right(); 
  }
  else {
    message = 'S';
    Stop();
  }
}

void Forword() {
  digitalWrite(Left_A, LOW);
  digitalWrite(Left_B, HIGH);
  digitalWrite(Right_A, HIGH);
  digitalWrite(Right_B, LOW);
}

void Backword() {
  digitalWrite(Left_A, HIGH);
  digitalWrite(Left_B, LOW);
  digitalWrite(Right_A, LOW);
  digitalWrite(Right_B, HIGH);
}

void Left() {
  digitalWrite(Left_A, HIGH);
  digitalWrite(Left_B, LOW);
  digitalWrite(Right_A, HIGH);
  digitalWrite(Right_B, LOW);
}

void Right() {
  digitalWrite(Left_A, LOW);
  digitalWrite(Left_B, HIGH);
  digitalWrite(Right_A, LOW);
  digitalWrite(Right_B, HIGH);
}

void Stop() {
  digitalWrite(Left_A, LOW);
  digitalWrite(Left_B, LOW);
  digitalWrite(Right_A, LOW);
  digitalWrite(Right_B, LOW);
}

void Show_Informatin() {
  lcd.setCursor(0, 0);
  lcd.print("Mode:");
  if (Mode == 0)lcd.print("Tracing");
  else if (Mode == 1)lcd.print("Control");
  lcd.setCursor(0, 1);
  lcd.print("Status:");
  switch (message) {
    case 'r': lcd.print("Right   "); break;
    case 'R': lcd.print("Right   "); break;
    case 'b': lcd.print("Backword"); break;
    case 'B': lcd.print("Backword"); break;
    case 'l': lcd.print("Left    "); break;
    case 'L': lcd.print("Left    "); break;
    case 'f': lcd.print("Forword "); break;
    case 'F': lcd.print("Forword "); break;
    case 's': lcd.print("Stop    "); break;
    case 'S': lcd.print("Stop    "); break;
    default : lcd.print("Stop    "); break;
  }
}
