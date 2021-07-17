#include "Arduino.h"

#define strlength 6
String data;
String str[strlength];
int delimit[strlength + 1];
int i = 0;
bool indexing = false;
bool reading = false;

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
    delimit[2] = data.indexOf('\n', delimit[1]+1);
    delimit[3] = data.indexOf('\n', delimit[2]+1);
    delimit[4] = data.indexOf('\n', delimit[3]+1);
    delimit[5] = data.indexOf('\n', delimit[4]+1);
    delimit[6] = data.indexOf("*\n", delimit[5]+1);
        
//    Serial.println(delimit[0]);
//    Serial.println(delimit[1]);
//    Serial.println(delimit[2]);
//    Serial.println(delimit[3]);
//    Serial.println(delimit[4]);
//    Serial.println(delimit[5]);
//    Serial.println(delimit[6]);
    
    for(int j =0; j < strlength; j++){
      str[j] = data.substring(delimit[j] + 1, delimit[j+1]);
    }
    for(int k = 0; k<6; k++){
      Serial.print(str[k]);
      Serial.println(":");
      k++;
      Serial.println(str[k]);
    }
    indexing = false;
  }
  
}
