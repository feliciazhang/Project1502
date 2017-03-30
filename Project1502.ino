#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);

// Declare variables
const int buttonPin[] = {0,1,2,3,4,5,6,7,8,9}; // Holds pin locations of buttons
const int buttonNumber[] = {0,1,2,3,4,5,6,7,8,9}; // Representative of actual digits to be printed
int buttonPressed = 10; // Necessary for multiple arduino communication
int op; // Representative of four basic arithmetic operations
int diff; // Representative of user-inputted difficulty setting
int i = 0;
int randNumber1, randNumber2;
int rightAnswer;
int inputAnswer; // Stores user-inputted answer
int refresh = 0;
int score = 0;

void setup() {
  Serial.begin(115200);
  // Initialize pushbutton pins as an input
  for (int count=0;count<10;count++) {
    pinMode(buttonPin[count],INPUT_PULLUP);
  }
  // Set up the LCD's number of columns and rows:
  lcd.begin(16,2);
  // Turn on display
  lcd.display();
  // Welcome message
  lcd.print("Welcome to [ma+h i+]!");
  delay(1000);
  lcd.clear();
}

void loop() {
  if (refresh == 0){
    op = chooseOperation();
    diff = chooseDifficulty();
    rightAnswer = getProblem(diff, op);
    refresh = evalAnswer(rightAnswer);
    lcd.clear();
  }
  else {
    rightAnswer = getProblem(diff, op);
    refresh = evalAnswer(rightAnswer);
    lcd.clear();
  }
}

int chooseDifficulty() {
  int difficulty;
  // Ask for difficulty
  lcd.print("What difficulty level(1-3)?");
  // Check and assign user difficulty input
  if (buttonPressed == buttonNumber[1]) { // Easy
       difficulty = 1;
       lcd.clear();
       lcd.print("Easy");
       delay(300);
  }
  else if (buttonPressed == buttonNumber[2]) { // Medium
       difficulty = 2;
       lcd.clear();
       lcd.print("Medium");
       delay(300);
  }
  else if (buttonPressed == buttonNumber[3]) { // Hard
       difficulty = 3;
       lcd.clear();
       lcd.print("Hard");
       delay(300);
  }
  lcd.clear();
  delay(500);
  return difficulty;
}

int chooseOperation() {
  int operation;
  // Ask for operation
  lcd.print("What operation do you ");
  lcd.setCursor(0,1);
  lcd.print("want to study?");
  // Check and assign user operation input
  if (buttonPressed == buttonNumber[0]) { // Addition
    operation = 1;
    lcd.clear();
    lcd.print("Addition");
  }
  else if (buttonPressed == buttonNumber[1]) { // Subtraction
    operation = 2;
    lcd.clear();
    lcd.print ("Subtraction");
  }
  else if (buttonPressed == buttonNumber[2]) { // Multiplication
    operation = 3;
    lcd.clear();
    lcd.print ("Multiplication");
  }
  else if (buttonPressed == buttonNumber[3]) { // Division
    operation = 4;
    lcd.clear();
    lcd.print ("Division");
  }
  delay(300);
  lcd.clear();
  delay(500);
  return operation;
}

int getProblem(int difficulty, int operation) {
  // Adjusts difficulty according to user input
  randomSeed(millis());
  
  switch (difficulty) {
    case 1:
       randNumber1 = random(1,11);
       randNumber2 = random(1,11);
       break;
  
    case 2:
    if (operation == 3||4){ // Special cases for multiplication and division
      randNumber1 = random(1,11);
      randNumber2 = random(1,21);
    }
    else {
      randNumber1 = random(1,21);
      randNumber2 = random(1,21);
    }
    break;
    
  case 3:
    if (operation = 3||4) { // Special cases for multiplication and division
      randNumber1 = random(1,11);
      randNumber2 = random(1,51);
    }
    else {
      randNumber1 = random(1,51);
      randNumber2 = random(1,51);
    }
    break;
  }

  int correctAnswer;
  // Saves correct answer from random numbers, according to operation
  switch (operation) {
  case 1: 
       correctAnswer = randNumber1+randNumber2;
       lcd.print(randNumber1);
       lcd.print("+");
       lcd.print(randNumber2); 
       lcd.print("=");
       break;
       
  case 2:
       correctAnswer = randNumber1;
       lcd.print(randNumber1+randNumber2);
       lcd.print("-");
       lcd.print(randNumber2); 
       lcd.print("=");
       break;
       
  case 3:
       correctAnswer = randNumber1*randNumber2;
       lcd.print(randNumber1);
       lcd.print("×");
       lcd.print(randNumber2); 
       lcd.print("=");
       break;
       
  case 4:
       correctAnswer = randNumber2;
       lcd.print(randNumber1*randNumber2);
       lcd.print("÷");
       lcd.print(randNumber1); 
       lcd.print("=");
       break;
   }
  return correctAnswer;
}

int evalAnswer(int correctAnswer) {
  // Converts correct answer to a string
  String answerString = String(correctAnswer);
  // Number of digits in the correct answer
  int arraySize = answerString.length();
  int playerAnswer[arraySize];
  
  // Shorten timer as score gets higher
  int limit = 5000 - 100 * score;
  long currentTime = millis();
  long endTime = currentTime + limit;

// Timer loop: Continues as long as time elapsed is less than endTime
while(millis() < endTime){

  // Fills array of given size with number corresponding to buttons pushed
  // Also prints to LCD the user-calculated sum
  for (i = 0; i < arraySize; i++) {
    // get integer of button pressed from serial
    while(buttonPressed > 9)
    {
      if(Serial.available())
      {
        buttonPressed = Serial.parseInt();
      }
      else{}
     }

    // Add digits to player answer matrix and print on LCD
    if (buttonPressed <= 9) {
      lcd.print(buttonPressed);
      playerAnswer[i] = buttonPressed;
      }
     else {}
     
     buttonPressed = 10;
  }

  // Convert array to integer
  for (int j = 0; j <= arraySize - 1; j++){
     inputAnswer = (playerAnswer[j] * pow(10,arraySize - j - 1)) + inputAnswer;
    }
  delay(200);
  
  // Compare inputted and correct answer and exit timer loop
  if (inputAnswer == correctAnswer){
    endTime = 0;
  }    
  else {
    endTime = 1;
  }
}

  // Clear LCD in preparation for next question 
  lcd.clear();
  
  // Print correct, incorrect, or too slow
  if (endTime == 0){
    score++;
    lcd.print("Correct!");
    delay(400);
    lcd.clear();
    lcd.print("Your score ");
    lcd.setCursor(0,1);
    lcd.print("is ");
    lcd.print(score);
    refresh = 1;
    delay(400);
  }
  else if (endTime == 1){
    lcd.print("Incorrect.");
    delay(400);
    lcd.clear();
    lcd.print("Answer: ");
    lcd.setCursor(0,1);
    lcd.print(correctAnswer);
    delay(500);
    lcd.clear();
    refresh = 0;
    score = 0;
  }
  else {
    lcd.print("Too slow!");
    lcd.clear();
    lcd.print("Answer: ");
    lcd.setCursor(0,1);
    lcd.print(correctAnswer);
    delay(500);
    lcd.clear();
    refresh = 0;
    score = 0;
  }
  return refresh;
}
