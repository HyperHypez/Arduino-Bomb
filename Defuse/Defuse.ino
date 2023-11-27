#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

// Keypad row/column declaration
const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns

// Variable to hold the scrambled word
String scrambledWord = "";

// 5 Letter Word List
String wordList[] = {"apple", "bacon", "child", "happy", "metal"}; // Word list (unscrambled)
String scrambledWordList[] = {"paelp", "noabc", "idclh", "payhp", "elmta"}; // Word list (scrambled)
String solvedScrambledWordList[] = {"27753", "22266", "24453", "42779", "63825"}; // Word list (number encodings of unscrambled words)

// Variable indicating the index in the word lists to grab the word/elements from
int listIndex;

// Keypad array
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

// LCD Display Set Up
const int rs = 2, en = 3, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Set pin numbers for green LED
const int gLEDPin = 13;  // green LED pin

// Keypad connections
byte pin_rows[ROW_NUM] = {5, 6, 7, 8}; // connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {A0, A1, A3, A2}; // connect to the column pinouts of the keypad

// Keypad construction
Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

// Variable to hold data to send to bomb
String dataToSend = "";

void setup() {
  // Set up I2C Communication
  Wire.begin();

  pinMode(A4, INPUT_PULLUP); // Enable internal pull-up resistor for SDA
  pinMode(A5, INPUT_PULLUP); // Enable internal pull-up resistor for SCL

  // Set up green LED
  pinMode(gLEDPin, OUTPUT);

  // Set up LCD display
  lcd.begin(16, 2);
  analogWrite(4, 60);
  lcd.setCursor(0, 0);
  lcd.print("Enter code:");

  // Generate random index
  randomSeed(analogRead(0));
  listIndex = random(5);
}

void loop() {
  // Get key from keypad
  char key = keypad.getKey();
  
  if (key) { // if keypress was detected
    if (key == '#') { // # means submit the guess
      if(dataToSend == solvedScrambledWordList[listIndex]) { // if the guess is correct
        Wire.beginTransmission(9); // begin data transmission with address of the receiver Arduino
        Wire.write(1); // send over a 1 to the bomb microcontroller
        int result = Wire.endTransmission(); // end the data transmission
        digitalWrite(gLEDPin, HIGH); // turn on green LED
      }
      dataToSend = ""; // clear the string variable
      lcd.clear(); // clear the LCD
    } else if (key == '*') { // * clears user input
      dataToSend = ""; // clear the string variable
      lcd.clear(); // clear the LCD
    } else { // user pressed a key
      dataToSend += key; // append the key to the string
    }
  }

  // Show the scrambled word to unscramble
  lcd.setCursor(0, 0);
  lcd.print("Unscramble ");
  lcd.print(scrambledWordList[listIndex]);

  // Update LCD to reflect user keypresses
  lcd.setCursor(0, 1);
  lcd.print(dataToSend);
}