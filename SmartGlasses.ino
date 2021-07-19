#include "Arduino.h"

#define strlength 6
String data;
String str[strlength];
int rTime[3];
int rDate[3];
int rDTDelimit[4];
int dayWeek;
int delimit[strlength + 1];
int i = 0;
bool indexing = false;
bool reading = false;

String daysOfTheWeek[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
String Month_names[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "July", "Aug", "Sep", "Oct", "Nov", "Dec"};

void setup() { 
  Serial.begin(9600); 
}

void loop() {

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

      for(int j =0; j < 3; j++)
      Serial.println(rTime[j]);
      
      rDTDelimit[0] = -1;
      rDTDelimit[1] = str[5].indexOf(':');
      rDTDelimit[2] = str[5].indexOf(':', rDTDelimit[1]+1);
      rDTDelimit[3] = str[5].indexOf("*\n", rDTDelimit[2]+1);

      for(int j =0; j < 3; j++)
      rDate[j] = str[5].substring(rDTDelimit[j] + 1, rDTDelimit[j+1]).toInt();

      for(int j =0; j < 3; j++)
      Serial.println(rDate[j]);
     }
     else{
      for(int j =0; j < strlength; j++)
      Serial.println(str[j]);
     }
    indexing = false;
  }
}
