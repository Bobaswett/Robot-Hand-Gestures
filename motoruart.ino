#include <Ps3Controller.h>


int ENA = 13;
int IN1 = 15;
int IN2 = 2;

int ENB = 12;
int IN3 = 14;
int IN4 = 27;

int ENC = 26;
int IN5 = 25;
int IN6 = 33;

int END = 0;
int IN7 = 4;
int IN8 = 16;

const int frequency = 500;
const int pwm_channel = 0;
const int resolution = 8;
const char TERMINATOR = '|';
void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT); 
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT); 
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  
  pinMode(ENC, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT); 
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, LOW);

  pinMode(END, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT); 
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, LOW);
  
  
  Serial.begin(115200);
 
  Ps3.begin("00:13:a9:df:29:14");

  
  ledcAttachChannel(ENA, frequency, resolution, pwm_channel);
  ledcAttachChannel(ENB, frequency, resolution, pwm_channel);
  ledcAttachChannel(ENC, frequency, resolution, pwm_channel);
  ledcAttachChannel(END, frequency, resolution, pwm_channel);
}

int val = 0;


void backward()
{
  //back right
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
    
    //front right
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
 
    //back left
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
    
    //front left
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);
    
  ledcWrite(pwm_channel,175);
  delay(850);
}

void forward()
{
 //back right
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
    
    //front right
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);


    //back left
  digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);
    
    //front left
  digitalWrite(IN7, HIGH);
  digitalWrite(IN8, LOW);

    
  ledcWrite(pwm_channel,175);
  delay(850);



  
}

void turnRight()
{

  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
    
    //front left
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);

    //back right
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
    
    //front right
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  ledcWrite(pwm_channel,175);
  delay(850);


  
}

void turnLeft()
{

 //back left
  digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);
    
    //front left
  digitalWrite(IN7, HIGH);
  digitalWrite(IN8, LOW);

    //back right
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
    
    //front right
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  ledcWrite(pwm_channel,175);
  delay(850);
  
}
void sleep()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, LOW);
}


void loop() {
  //forwards
  if (Serial.available()>0){
    String commandFromJetson = Serial.readStringUntil(TERMINATOR);

      //Forward
      if (commandFromJetson == "forward") {
        forward();
        backward();
        sleep();
        
       }
    
      //Backward
      else if (commandFromJetson == "back") {
         backward();
         forward();
         sleep();
       }
      
      //Turn RIGHT
      else if (commandFromJetson == "right") {
        turnRight();
        turnLeft();
        sleep();
      }
    
        //Turn LEFT
      else if (commandFromJetson == "left") {
        turnLeft();
        turnRight();
       
        sleep();     
      }
      
      else {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        digitalWrite(IN5, LOW);
        digitalWrite(IN6, LOW);
        digitalWrite(IN7, LOW);
        digitalWrite(IN8, LOW);
      }
  }
  delay(500);

}
