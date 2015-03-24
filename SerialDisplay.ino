/*
  LiquidCrystal Library - Serial Input

 Demonstrates the use of a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch has three "loopn" subroutines. "loop" should call
 one of them.

 loop0 is from the original example. It displays text sent over the
 serial port (e.g. from the Serial Monitor) on an attached LCD.

 loop1 displays text supplied as a line at a time. The first
 line is displayed on line 1 of the LCD; the second line is
 displayed on line 2. Subsequent lines are displayed on line 2,
 and the previous contents of line 2 is moved up -- that is, the
 text scrolls up from the bottom.

 loop2 displays text scrolling to the left; it is filled in a
 character at a time, and when the first line is full, the text
 starts to scroll.

 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 Subsequent code developed by Dean Bandes, 22 Mar 2015

 This example code is in the public domain.

 http://arduino.cc/en/Tutorial/LiquidCrystalSerial
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

char line0[16];
char line1[16];

int first;
int theStr;
char ch;

int   j;
int   k;
char  myBuffer[17];

char* myStrings[] = {"The crock of gold", "belonging to",
                     "the leprechauns",   "Of Gort na",
                     "Clokka Morra",      "has been stolen.",
                     "Can a philosopher", "persuade Celtic",
                     "god Angus Og",      "to rescue the",
                     "thief's daughter",  "from Pan?"};
                     
char *myWords[] = {"The", "crock", "of", "gold", "belonging",
                   "to", "the", "leprechauns", "of", "Gort",
                   "Na", "Clokka", "Morra", "has", "been",
                   "stolen.", "Greek", "god", "Pan", "(How",
                   "did", "he", "get", "to", "Ireland?)",
                   "has", "seduced", "the", "thief's",
                   "daughter", "Caitilin.", "Can", "the",
                   "philosopher", "find", "Celtic", "god",
                   "Angus", "Og", "and", "persuade", "him",
                   "to", "rescue", "the", "young", "woman?",
                   "                "};

void setup() {
  int i;
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  // initialize the serial communications:
  Serial.begin(9600);
  
  // initialize the character arrays
  first = 1;
  theStr = 0;
  j = 0;
  k = 0;

  for (i = 0; i < 16; i++) {
    line0[i] = '\0';
    line1[i] = '\0';
  }
  
  for (i = 0; i < 17; i++) {
    myBuffer[i] = '\0';
  }
}

void loop()
{
  loop2();
}

void loop0()
{
  int i = 0;
  
  // when characters arrive over the serial port...
  if (Serial.available()) {
    if (!first) {
      for (i = 0; i < 16; i++) {
        line0[i] = line1[i];
        line1[i] = '\0';
      }
    }
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    lcd.setCursor(0, 0);
    if (!first) {
      lcd.print(line0);
      // Serial.print("printed line0: '");
      // Serial.print(line0);
      // Serial.println("'");
      lcd.setCursor(0, 1);
      i = 0;
    }
    // read all the available characters
    while (Serial.available() > 0) {
      // display each character to the LCD
      ch = Serial.read();
      if (i < 15) {
        line1[i++] = ch;
      } else {
        line1[15] = '\0';
      }
      // lcd.write(ch);
    }
    lcd.print(line1);
    // Serial.print("printed line1: '");
    // Serial.print(line1);
    // Serial.println("'");
    first = 0;
  }
}

void loop1()
{
  // Print a line, then print it on top and print the
  //  next below it, repeat
  lcd.clear();
  if (theStr == 0) {
    lcd.setCursor(0, 0);
    lcd.print(myStrings[theStr]);
  } else {
    lcd.setCursor(0, 0);
    lcd.print(myStrings[theStr - 1]);
    lcd.setCursor(0, 1);
    if (theStr < 12) {
      lcd.print(myStrings[theStr]);
    }
  }
  theStr++;
  theStr = theStr % 13;
  delay(1200);
}

void loop2()
{
  char *aStr;
  int i = 0;
  int tDelay = 400;

  aStr = myWords[theStr];
  for (j = 0; aStr[j] != '\0'; j++) {
    if (k > 15) {
      for (i = 0; i < 15; i++) {
        myBuffer[i] = myBuffer[i + 1];
      }
      k = 15;
    }
    myBuffer[k++] = aStr[j];
    lcd.setCursor(0, 0);
    lcd.print(myBuffer);
    delay(tDelay);
  }
  if (k > 15) {
    for (i = 0; i < 15; i++) {
      myBuffer[i] = myBuffer[i + 1];
    }
    k = 15;
  }
  myBuffer[k++] = ' ';

  lcd.setCursor(0, 0);
  lcd.print(myBuffer);
  delay(tDelay);

  theStr++;
  theStr = theStr % 48;
}
