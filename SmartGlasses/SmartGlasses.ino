#include "Arduino.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "icons.h"

// All definitions
#define strlength 6

// All Global Variables
String data;
String str[strlength];
int rTime[3];
int rDate[3];
int rDTDelimit[4];
int dayWeek;
int delimit[strlength + 1];
int i = 0;
double rad=3.141592/180;
double LR = 89.99;
unsigned long currentTime = 0;
unsigned long previousTime = 0;
unsigned long cTime = 0;
unsigned long pTime = 0;
int setAMPM = 0;
int AMPM = 0;
int check = 0;
bool indexing = false;
bool reading = false;
unsigned int faceClock = 1;
String daysOfTheWeek[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
String MonthNames[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "July", "Aug", "Sep", "Oct", "Nov", "Dec"};
String sAMPM[2] = {"AM", "PM"};

// All Components definitions
Adafruit_SSD1306 display(128, 64);

// All Functions prototyping

void showTimePin(int x, int y, double initial_point, double end_point, double recievedInMin);
void updateTime(unsigned long currentTime);
void setFaceTime(unsigned int face);
void setTimeRecieved();
void displayNotifications();
void mapDisplay();

// Setup
void setup() { 
  Serial.begin(9600); 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  {
    Serial.println("OLED connection Failed");
    for(;;);
  }
  // Company logo
  display.clearDisplay();
  display.drawBitmap(0, 0, Logo, 128, 64, 1);
  display.display();
  delay(3000);
}

// Loop
void loop() {
  currentTime = millis();
  updateTime(currentTime);
 
  setFaceTime(faceClock);
    
  if(Serial.available()) {
       data = Serial.readString();
       indexing = true;
  }

  if(!Serial.available() && indexing == true){
    delimit[0] = -1;
    delimit[1] = data.indexOf('\n');
    
    for (int j = 1; j < 5; j++)
      delimit[j + 1] = data.indexOf('\n', delimit[j]+1);
      
    delimit[6] = data.indexOf("*\n", delimit[5]+1);
    
    for(int j =0; j < strlength; j++){
      str[j] = data.substring(delimit[j] + 1, delimit[j+1]);
    }
    if (str[0] == "It's Clock")
      setTimeRecieved();
    else  displayNotifications();
    indexing = false;
    delay(5000);
  }
}

// Function Definitions

void showTimePin(int x, int y, double initial_point, double end_point, double recievedInMin) {
  double x1, x2, y1, y2;
  x1 = x + (28 * initial_point) * cos((6 * recievedInMin + LR) * rad);
  y1 = y + (28 * initial_point) * sin((6 * recievedInMin + LR) * rad);
  x2 = x + (28 * end_point) * cos((6 * recievedInMin - LR) * rad);
  y2 = y + (28 * end_point) * sin((6 * recievedInMin - LR) * rad);
  
  display.drawLine((int)x1, (int)y1, (int)x2, (int)y2, WHITE);
}

void updateTime(unsigned long currentTime){
  if (currentTime - previousTime > 60000){
    rTime[1]++;               // increment minute

    if(rTime[1] >= 60){
      rTime[0]++;
      rTime[1] = 0;
      if(rTime[0] >= 12){
        if(rTime[0] == 12)
          (AMPM == 0)? AMPM = 1 : AMPM = 0;
           else if(rTime[0] > 12)
              rTime[0] = 1;            
           if(AMPM == 0){
            dayWeek++;
            if(dayWeek > 7)
              dayWeek = 1;
            rDate[0]++;
            if(rDate[0] > 30){
              rDate[0] = 1;
              rDate[1]++;
              if(rDate[1] > 12){
                rDate[1] = 1;
                rDate[2]++;
              }
            }
          }
        }
      }
    previousTime = currentTime;
  }
}

void setTimeRecieved(){
      Serial.println(str[0].indexOf("gg"));    // when it is not present it gives -1 value
      setAMPM = 0;
      faceClock = str[4].toInt();
      setAMPM = str[2].toInt();
      dayWeek = str[1].toInt()-1;
      
      rDTDelimit[0] = -1;
      rDTDelimit[1] = str[3].indexOf(':');
      rDTDelimit[2] = str[3].indexOf(':', rDTDelimit[1]+1);
      rDTDelimit[3] = str[3].indexOf('\n', rDTDelimit[2]+1);

      for(int j =0; j < 3; j++)
        rTime[j] = str[3].substring(rDTDelimit[j] + 1, rDTDelimit[j+1]).toInt();

      if(setAMPM == 0){
        if(rTime[0] < 12 && rTime[0] != 0)
          AMPM = 0; 
        else if(rTime[0] == 12){
          rTime[0] = 12;
          AMPM = 1;
        }
        else if(rTime[0] >= 12 && rTime[0] < 24){
          AMPM = 1;
          rTime[0] = rTime[0] - 12;
        }
        else if(rTime[0] == 0){
          rTime[0] = 12;
          AMPM = 0;
        }
      }
      
      rDTDelimit[0] = -1;
      rDTDelimit[1] = str[5].indexOf(':');
      rDTDelimit[2] = str[5].indexOf(':', rDTDelimit[1]+1);
      rDTDelimit[3] = str[5].indexOf("*\n", rDTDelimit[2]+1);

      for(int j =0; j < 3; j++)
      rDate[j] = str[5].substring(rDTDelimit[j] + 1, rDTDelimit[j+1]).toInt();
   }

void displayNotifications(){
  display.clearDisplay();
  if(str[1] == "com.whatsapp"){                                                     // whatsapp messages and calls
    display.drawBitmap(0, 0, Whatsapp, 128, 64, 1);
    if(str[5] == "Incoming voice call"){
      check = 2;
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,46);
      display.print(str[5]);
      display.print(" : ");
      display.println(str[3]);
      display.display();
    }      
    else if(str[5] == "Missed voice call") {
      check = 2;
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,50);
      display.print("Missed call: ");
      display.println(str[3]);
      display.display();
    }
    else {
      check = 2;
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,50);
      display.print("Text from: ");
      display.println(str[3]);
      display.display();
    }
  }
  else if(str[1] == "android"){                                                    // android notifications
    display.drawBitmap(0, 0, Android, 128, 64, 1);
    check = 0;
  }
  else if(str[1] == "com.google.android.dialer"){                                 // Phone Call
    display.drawBitmap(0, 0, Phone, 128, 64, 1);
    if(str[5] == "Incoming call"){
      check = 2;
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,46);
      display.print(str[5]);
      display.print(" : ");
      display.println(str[3]);
      display.display();
    }
    else {
      check = 2;
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,45);
      display.print("Missed call: ");
      display.println(str[5]);
      display.display();
    }
  }
  else if(str[1] == "com.google.android.gm"){                                    // Gmail
    display.drawBitmap(0, 0, Gmail, 128, 64, 1);
    check = 1;
  }
  else if(str[1] == "com.google.android.apps.messaging"){                        // Messages
    display.drawBitmap(0, 0, Message, 128, 64, 1);
    check = 2;
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.print(str[3]);
    display.print(" : ");
    display.println(str[5]);
    display.display();
  }
  else if(str[1] == "com.google.android.calendar"){                            // Google Calender 
    display.drawBitmap(0, 0, Calender, 128, 64, 1);
    check = 1;
  }
  else if(str[1] == "com.google.android.apps.maps"){                           // Google Maps
    mapDisplay();
    check = 0;
  }

  else if(str[3] == "App" || str[3] == "Text" || str[3] == "Title" || str[3].indexOf("Home") != -1){                           // Google Maps
    cTime = millis();
    if (cTime - pTime > 10000){
      display.drawBitmap(0, 0, Maps, 128, 64, 1);
      check = 2;
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,40);
      display.println(str[4]);
      display.println(str[5].substring(6));
      pTime = cTime;
      display.display();
    }
  }
  else {
    display.drawBitmap(0, 0, Notifications, 128, 64, 1);
    check = 2;
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,44);
    display.println("Notification Recieved");
    display.display();
  }

  if (check == 0){
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(0,50);
   display.println(str[5]);
   display.display();
  }
  else if(check == 1){
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(0,50);
   display.println(str[3]);
   display.display();
  }
}

void setFaceTime(unsigned int face){      // Mix Clock
  if(face == 1){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawCircle(28, 32, 28, WHITE);
    if(setAMPM == 0)
      showTimePin(28, 32, 0.1, 0.5, ((rTime[0]*60)/12) + (rTime[1]/10));
    else if(setAMPM == 1) showTimePin(28, 32, 0.1, 0.5, (((rTime[0]-12)*60)/12) + (rTime[1]/10));
    showTimePin(28, 32, 0.1, 0.78, rTime[1]);
    display.setCursor(55,0);
    display.setTextSize(2);
    display.print(rTime[0]);
    display.print(":");
    display.print(rTime[1]);
    display.setTextSize(1);
    if(setAMPM == 0)
     display.print(sAMPM[AMPM]);
    display.setCursor(50,55);
    display.setTextSize(1);
    display.print(rDate[0]);
    display.print(" ");
    display.print(MonthNames[rDate[1]-1]);
    display.print(", ");
    display.println(rDate[2]);
    display.setCursor(70,28);
    display.setTextSize(2);
    display.println(daysOfTheWeek[dayWeek]);
    display.display();
}
  else if(face == 2){             // only Analog
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawCircle(64, 32, 28, WHITE);
    if(setAMPM == 0)
    showTimePin(64, 32, 0.1, 0.5, ((rTime[0]*60)/12) + (rTime[1]/10));
    else if(setAMPM == 1) showTimePin(64, 32, 0.1, 0.5, (((rTime[0]-12)*60)/12) + (rTime[1]/10));
    showTimePin(64, 32, 0.1, 0.78, rTime[1]);
    display.display();
}

  else if(face == 3){             // Just Digital
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.drawBitmap(0, 0, frame, 128, 64, 1);
  display.setCursor(25,0);
  display.print("Clock");
  display.setTextSize(3);
  display.setCursor(0, 35);
  display.print(rTime[0]);
  display.print(":");
  display.print(rTime[1]); 
  display.setTextSize(1);
  if(setAMPM == 0)
    display.print(sAMPM[AMPM]);
  display.setCursor(110, 35);
  display.print(daysOfTheWeek[dayWeek]);
  display.setCursor(110, 45);
  display.print(rDate[0]);
  display.setCursor(110, 55);
  display.print(MonthNames[rDate[1]-1]);
  display.display();
  }
}

void mapDisplay(){
  if(str[3].indexOf("Head northwest") != -1 || str[3].indexOf("Head southeast") != -1 || str[3].indexOf("Head south") != -1){
    display.drawBitmap(0, 0, headNorthWest, 128, 64, 1);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,40);
    display.println(str[3]);
    display.display();
  }
  else if(str[3].indexOf("Turn right") != -1){
    display.drawBitmap(0, 0, turnRight, 128, 64, 1);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,40);
    display.println(str[3]);
    display.display();
  }
  else if(str[3].indexOf("Turn left") != -1){
    display.drawBitmap(0, 0, turnLeft, 128, 64, 1);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,40);
    display.println(str[3]);
    display.display();
  }
  else{
    display.drawBitmap(0, 0, Maps, 128, 64, 1);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,40);
    display.println(str[3]);
    display.display();
  }
}
