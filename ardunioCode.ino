#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int dotPin = 2;    
const int dashPin = 3;   
const int buzzerPin = 8; 

const char* letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* morse[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};

int currentIndex = 0;
String currentInput = "";

void setup() {
  pinMode(dotPin, INPUT_PULLUP);
  pinMode(dashPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  startNewRound();
}

void loop() {
  // Read dot button
  if (digitalRead(dotPin) == LOW) {
    processInput(".");
    delay(250); 
  }
  
  // Read dash button
  if (digitalRead(dashPin) == LOW) {
    processInput("-");
    delay(350);
  }
}

void startNewRound() {
  currentInput = "";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Letter: ");
  lcd.print(letters[currentIndex]);
  lcd.print(" ");
  lcd.print(morse[currentIndex]); // Show the Morse code first
  lcd.setCursor(0, 1);
  lcd.print("Memorize...");
  
  delay(2000); // Wait 2 seconds for the user to see the answer
  
  // Clear the answer from screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Letter: ");
  lcd.print(letters[currentIndex]);
  lcd.setCursor(0, 1);
  lcd.print("Type: ");
}

void processInput(String type) {
  if (type == ".") {
    tone(buzzerPin, 1000, 100);
    currentInput += ".";
  } else {
    tone(buzzerPin, 800, 300);
    currentInput += "-";
  }
  
  lcd.setCursor(6, 1);
  lcd.print(currentInput);
  
  // Check if input length is equal to target Morse code length
  if (currentInput.length() >= String(morse[currentIndex]).length()) {
    delay(400);
    if (currentInput == morse[currentIndex]) {
      lcd.setCursor(12, 1);
      lcd.print("OK!");
      tone(buzzerPin, 1200, 100); delay(100);
      tone(buzzerPin, 1500, 200);
      delay(1000);
      currentIndex = (currentIndex + 1) % 26; // Next letter in alphabet
    } else {
      lcd.setCursor(11, 1);
      lcd.print("WRONG");
      tone(buzzerPin, 200, 500); // Error sound
      delay(1000);
    }
    startNewRound(); // Refresh and show the same or next letter
  }
}
