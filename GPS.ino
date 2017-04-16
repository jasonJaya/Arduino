#include <SoftwareSerial.h>
//declaring the RX and TX pins
SoftwareSerial serialMod(2,3); 
#define DEBUG true

//declaring variables
int count =0;
String coord = "";  
char txt[400];


void setup() {
//this code is run once to setup
Serial.begin(9600);
while(!Serial);

//the method of communicating between the arduino and the GPS/GSM module
serialMod.begin(9600);
delay(1000);

Serial.println("Module has been set up. Retrieving GPS coordinates: ");
Gps();
}

void loop() {
//to get the current location
   command( "AT+CGNSINF",1000,DEBUG);
}

void Gps(void){
  //turning on the GPS module
   command( "AT+CGNSPWR=1",1000,DEBUG); 
   command( "AT+CGPSINF=0",1000,DEBUG); 
}

void command(String ATrule, const int timer, boolean debug){
      
    serialMod.println(ATrule); 
    delay(5);
    if(debug){
    long int time = millis();   
    while( (time+timer) > millis()){
      while(serialMod.available()){       
        coord += char(serialMod.read());
      }  
    }    
    if (count==2){
      //print the GPS coordinates
      Serial.print(coord);
      Serial.print("\n");
      //breaking the result into latitude and longitude
      String lat=coord.substring(176, 184);
      String longi=coord.substring(185, 194);
      String every="I'm in danger! My location: \nlatitude: "+lat+" \nlongitude: "+longi+"\n"+"Location: http://maps.google.com/?q="+lat+","+longi;
      Serial.print(every);
      //parsing the string to char array
      every.toCharArray(txt, 400);

    //setting the SMS to text format
    cleanSerial();
    serialMod.println("AT+CMGF=1");
    
    //declaring the mobile number to send the SMS to
    cleanSerial();
    serialMod.print("AT+CMGS=\"");
    serialMod.print("0775951590");
    serialMod.println("\"");
    
    //sending the SMS
    cleanSerial();
    serialMod.print(txt);
    serialMod.println((char)26);
    
      
    }
    count=count+1;
    }    
}

void cleanSerial()
{
  //cleaning the serial input
    delay( 250 );
    while ( serialMod.available() > 0) 
    {
        serialMod.read();
        delay(50);
    }
}

