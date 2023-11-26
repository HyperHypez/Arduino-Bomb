#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns
String scrambledWord = "";

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

const int rs = 2, en = 3, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte pin_rows[ROW_NUM] = {5, 6, 7, 8}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {A0, A1, A3, A2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);
String dataToSend = "";

void setup() {
  Wire.begin();

  pinMode(A4, INPUT_PULLUP); // Enable internal pull-up resistor for SDA
  pinMode(A5, INPUT_PULLUP); // Enable internal pull-up resistor for SCL
  pinMode(13, OUTPUT);

  lcd.begin(16, 2);
  analogWrite(4, 60);
  lcd.setCursor(0, 0);
  lcd.print("Enter code:");
}

void loop() {

  char key = keypad.getKey();
  
  if (key) {
    if (key == '#') {

      Wire.beginTransmission(9);  // Replace 9 with the address of the receiver Arduino
      Wire.write(1);
      int result = Wire.endTransmission();
      dataToSend = ""; // Clear the string variable
      lcd.clear();
    } else if (key == '*') {
      dataToSend = ""; // Clear the string variable
      lcd.clear();
    } else {
      dataToSend += key; // Append the key to the string
    }
  }

  lcd.setCursor(0, 0);
  lcd.print("Enter the code:");

  lcd.setCursor(0, 1);
  lcd.print(dataToSend);

}

