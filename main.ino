#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(0,1);
#define Password_Lenght 5
#define BUTTON  2     // the number of the pushbutton pin
#define RELAY   3     // the number of the relay pin
int buttonState = 0;

char Master[Password_Lenght] = "9999";
char customKey;
byte data_count = 0, master_count = 0;
bool Pass_is_good;


LiquidCrystal_I2C lcd(0x27, 16, 2);

char Data[Password_Lenght];
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {11, 10, 9, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad

Keypad myKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); //setup keyboard
String inc = "";
bool flag = false;


void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(RELAY, OUTPUT);
  lcd.init();
  lcd.backlight();
  digitalWrite(RELAY, HIGH);
}

void loop() {

  while (mySerial.available()) {
    inc = mySerial.readString();
    delay(500);
  }
 
  if (inc.equals("opening")) {
    lcd.print("CORRECT");
    
      delay(300);
      buttonState = digitalRead(BUTTON);
      Serial.println(buttonState);
      buttonState = LOW;
    while(buttonState == LOW){
      digitalWrite(RELAY, LOW);
      delay(7000);
      buttonState = digitalRead(BUTTON);
      //Serial.println("I am here");
      //Serial.println(buttonState);
      //Serial.println("after400");
      digitalWrite(RELAY, HIGH);
    }
      
      delay(400);// added 1 second delay to make sure the password is completely shown on screen before it gets cleared.
      lcd.clear();
      clearData();
  }
  
  lcd.setCursor(0, 0); //1st line 
  lcd.print("Enter Password: "); //print

  customKey = myKeypad.getKey(); //read key pressed
  
  if (customKey) // makes sure a key is actually pressed, equal to (customKey != NO_KEY)
  {
    Data[data_count] = customKey; // store char into data array
    lcd.setCursor(data_count, 1); // move cursor to show each new char
    lcd.print("*"); // print char at said cursor
    data_count++; // increment data array by 1 to store new char, also keep track of the number of chars entered
  }

  if (data_count == Password_Lenght - 1) // if the array index is equal to the number of expected chars, compare data to master
  {
    delay(400);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pass ");

    if (!strcmp(Data, Master)) // equal to (strcmp(Data, Master) == 0)
    {
      lcd.print("CORRECT");
      delay(300);
      buttonState = digitalRead(BUTTON);
      buttonState = LOW;
    while(buttonState == LOW){
      digitalWrite(RELAY, LOW);
      delay(2000);
      buttonState = digitalRead(BUTTON);
    }
      delay(400);
      digitalWrite(RELAY, HIGH);
    }

    else {
      lcd.print("INCORRECT");
      //SendMessage();
      delay(1000);
    }
    delay(1000);// added 1 second delay to make sure the password is completely shown on screen before it gets cleared.
    lcd.clear();
    clearData();
  }
}

void clearData()
{
  while (data_count != 0)
  { // This can be used for any array size,
    Data[data_count--] = 0; //clear array for new data
  }
  return;
}
