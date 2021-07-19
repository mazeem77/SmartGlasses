#include "Arduino.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

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
int AMPM = 0;
bool indexing = false;
bool reading = false;
unsigned int faceClock = 2;
String daysOfTheWeek[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
String MonthNames[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "July", "Aug", "Sep", "Oct", "Nov", "Dec"};
String sAMPM[2] = {"AM", "PM"};

// All Components definitions
Adafruit_SSD1306 display(128, 64);

// All Functions

void showTimePin(int x, int y, double initial_point, double end_point, double recievedInMin);
void updateTime(unsigned long currentTime);
void setFaceTime(unsigned int face);

void setup() { 
  Serial.begin(9600); 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  {
    Serial.println("OLED connection Failed");
    for(;;);
  }
}

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
    if (str[0] == "It's Clock:"){
      Serial.println(str[0].indexOf("gg"));    // when it is not present it gives -1 value
      
      dayWeek = str[1].toInt()-1;
      
      rDTDelimit[0] = -1;
      rDTDelimit[1] = str[3].indexOf(':');
      rDTDelimit[2] = str[3].indexOf(':', rDTDelimit[1]+1);
      rDTDelimit[3] = str[3].indexOf('\n', rDTDelimit[2]+1);

      for(int j =0; j < 3; j++)
      rTime[j] = str[3].substring(rDTDelimit[j] + 1, rDTDelimit[j+1]).toInt();

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
      
      rDTDelimit[0] = -1;
      rDTDelimit[1] = str[5].indexOf(':');
      rDTDelimit[2] = str[5].indexOf(':', rDTDelimit[1]+1);
      rDTDelimit[3] = str[5].indexOf("*\n", rDTDelimit[2]+1);

      for(int j =0; j < 3; j++)
      rDate[j] = str[5].substring(rDTDelimit[j] + 1, rDTDelimit[j+1]).toInt();
   }
     else{
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.println(str[1]);
      display.setCursor(0,10);
      display.println(str[3]);
      display.setCursor(0,20);
      display.println(str[5]);
      display.display();
     }
    indexing = false;
    delay[5000];
  }
}

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

void setFaceTime(unsigned int face){      // Mix Clock
  if(face == 1){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawCircle(28, 32, 28, WHITE);
    if(rTime[0]<=12)
    showTimePin(28, 32, 0.1, 0.5, ((rTime[0]*60)/12) + (rTime[1]/10));
    else showTimePin(35, 32, 0.1, 0.5, (((rTime[0]-12)*60)/12) + (rTime[1]/10));
    showTimePin(28, 32, 0.1, 0.78, rTime[1]);
    display.setCursor(55,0);
    display.setTextSize(2);
    display.print(rTime[0]);
    display.print(":");
    display.print(rTime[1]);
    display.setTextSize(1);
    display.println(sAMPM[AMPM]);
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
    if(rTime[0]<=12)
    showTimePin(64, 32, 0.1, 0.5, ((rTime[0]*60)/12) + (rTime[1]/10));
    else showTimePin(35, 32, 0.1, 0.5, (((rTime[0]-12)*60)/12) + (rTime[1]/10));
    showTimePin(64, 32, 0.1, 0.78, rTime[1]);
    display.display();
  }

  else if(face == 3){             // Just Digital
   
  }
}
