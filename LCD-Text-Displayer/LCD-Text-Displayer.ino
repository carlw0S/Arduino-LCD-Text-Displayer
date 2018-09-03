// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int pos;    // position of next write
int count;  // number of characters on the LCD

void setup() {
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    // initialize the serial communications:
    Serial.begin(9600);
    
    lcd.print("Escribe algo ;)");
    lcd.blink();
  
}

void loop() {
  // when characters arrive over the serial port...
  if (Serial.available()) {

    delay(50);   // wait for the whole message to be received

    char c, buff[1024] = {0};
    pos = 0;
    
    // store the string in a buffer (fricking 64 bytes limit)
    while (Serial.available() && pos < 1024 && c != '\n'){
      
        delay(1);   // this delay allows long messages to be received correctly for some reason...
        
        c = Serial.read();

        if (c == '\n' || pos == 1023) {     // if the string ends or if it is too long, write the NUL terminator
          buff[pos++] = '\0';
          Serial.println(buff);   // echo
        }
        else
          buff[pos++] = c;
          
     }

     

    // reset the character count
    count = 0;
     
    // clear the screen
    lcd.clear();
    
    // print the characters stored in the buffer
    for (pos = 0; buff[pos] != '\0'; pos++) {

        // if 15 (or 15 + 40) characters have been displayed, we need to scroll to display the rest of them
        if(count == 15 || count == 55)
          lcd.autoscroll();
          
        // 40 is the max length when scrolling (I guess the "inner resolution" is 40x2) -- if the right border is reached, change to the other row
        if(count == 40 || count == 80){
  
          lcd.noBlink();
          delay(1000);
          
          // we need to unscroll in order to get to the initial position
          int i;
          for(i=0; i<24; i++){
            lcd.scrollDisplayRight();
            delay(75);
          }
  
          if(count == 40)
            lcd.setCursor(0,1);
          else{
            lcd.setCursor(0,0);
            lcd.clear();          // if running out of space, clear the screen and repeat
            count = 0;
          }
  
          lcd.blink();
          
        }

        // wait a bit for each character to be displayed
        delay(250);
        
        // display each character to the LCD and the Serial Monitor
        lcd.write(buff[pos]);
        Serial.print(buff[pos]);
  
        // add one to the character count
        count++;
        
        // before the last character of the row is displayed, autoscroll needs to be disabled in order not to reach the start of it
        if(count == 39 | count == 79)
          lcd.noAutoscroll();
      
    }

    // wait before showing the "keep going" message
    delay(1500);
    
    // disable autoscroll in order to display the message correctly
    lcd.noAutoscroll();
    
    // if the screen has been scrolled, we have to calculate the relative first column of the screen
    if(count >= 16 && count <= 39)
      lcd.setCursor(count-15, 1);
    else if(count == 40)
      lcd.setCursor(count-16, 1);
    else if(count >= 41 && count <= 55){
      lcd.setCursor(13,0);
      lcd.print("   ");
      lcd.setCursor(0,0);
    }
    else if(count >= 56 && count <= 79){
      lcd.setCursor(count-55+13,0);
      lcd.print("   ");
      lcd.setCursor(count-55, 0);
    }
    else if(count == 80){
      lcd.setCursor(count-56+13,0);
      lcd.print("   ");
      lcd.setCursor(count-56, 0);
    }
    else
      lcd.setCursor(0,1);
      
    lcd.print("Enga sigue :D");
    
    Serial.print("\n\n");   // just for aesthetics
  }
}