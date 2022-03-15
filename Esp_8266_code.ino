#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPLpo8fqEm6"
#define BLYNK_DEVICE_NAME "CCIOT"
#define BLYNK_AUTH_TOKEN "egj9CyzPB33gr0GtmZGnyhXMCNZyToCb"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define TRIGGER_PIN D0
#define ECHO_PIN D1
#define MAX_DISTANCE 200
#define PwmPin D3


// Hardware specific
#define NUM_TURN 10
#define pi_constant 3.14
//#define RST_HT 12
#define RADI 2.1
#define AREA 13.8474
#define MAX_I 2.0
#define DBDX 50
#define MASS 4.75
#define GRAV 9.8
#define R_EFF 6.0
int pwmVal;
int count=0;
int timeS=0;
int timeE=0;
double sum = 0;
bool First_f=true;
float initH=12;
float pwmValP=0;
float En=0,Eff=0;

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char ssid[]="Disconnected";
char pass[]="This is a very long password more than 8 chars";
char auth[]="egj9CyzPB33gr0GtmZGnyhXMCNZyToCb";
void setup()
{
  pwmVal=0;
  Serial.begin(9600);
  Blynk.begin(auth,ssid,pass,"blynk.cloud",8080); //Connects the ESP8266 to theWiFi and then to the server
  while(!Blynk.connect()){
    //wait till connection
  }
  pinMode(PwmPin,OUTPUT);

}
long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

void loop() {
  Blynk.run();// Starts Blynk services
  
  if(First_f)
  {
    Blynk.virtualWrite(V1,0);
    First_f=false;
  }
  
  int pwmValT=(pwmVal*255)/100; //The pmmVal in percentage is scaled
  analogWrite(PwmPin,pwmValT);
  

    float x = 0.0172 * readUltrasonicDistance(TRIGGER_PIN, ECHO_PIN);
    sum = sum + x;
    count++;
    
  
//  Serial.println(x,8);
//  Serial.println(pwmValT);
  if(count==3){   //Averages distance over 3 tries
  
  double avg = (double)(sum)/count ;
  count=0;
  sum=0;
  if(pwmValT==0)
  {
    initH=avg;
  }
  
  float height=initH-avg;
  float I=MAX_I*pwmVal/100.0;
  float F=force(NUM_TURN,I,AREA,height);
  if(pwmValT!=pwmValP)
  {
      timeE=(millis()-timeS);
      timeS=millis();
      En=MASS*GRAV*height/100.00;
      Eff=100000*(En/(I*I*R_EFF*(timeE)));
      Serial.println(Eff);   
  
  }
  
//  Serial.println(avg);
//  Serial.println(initH);
  Blynk.virtualWrite(V0,height);  
  Blynk.virtualWrite(V2,F);
  F=field(NUM_TURN,I,RADI);
  Blynk.virtualWrite(V3,F);
  Blynk.virtualWrite(V4,En);
  Blynk.virtualWrite(V5,Eff);
  pwmValP=pwmValT;
  }
  
  delay(100);
  
}

BLYNK_WRITE(V1)
{
  pwmVal = param.asInt(); // assigning incoming value from pin V1 to a variable
  
  // process received value
}

// Added functions for force and magnetic field calculations:
float force(int N, float i, float A,float h){
      //height in cm
   float F = 4*pi_constant*((N*i*N*i)*A)/(2*h*h*10000.0);

    return F;
}

float field(int N, float I, float R)
{

   float B = (4*pi_constant*N*I)/(2*R*100.0);

   return B;
}
