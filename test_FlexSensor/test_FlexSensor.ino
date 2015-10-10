/**
5V ------ flex-sensor
flex-sensor ----A0---- 22K ohm 
22K ohm ------- GND


**/


int flexSensorPin[5] = {A0,A1,A2,A3,A4};
int THRES_F1 = 260;      //A0 voltage change value
long lastTime = 0;       //
//int debounce = 150;    //
long thisTime = 0;       //
bool isFirst = true;     //flag judging whether click is firstly bent
bool isPresed = false;   //flag judging whether sensor is bent or straight
int interval = 800;      //time interval between click and double-click
int hand[5];

class Finger{
  public:
  int status;
  long lastTime;
  bool isFirst;
  bool isBent;
  Finger(){}
};

  //Finger (*finger)[5] ={new Finger(),new Finger(),new Finger(),new Finger(),new Finger()};
  //Finger finger1 ;
  //finger[2] =new Finger();
  //Finger finger3 ;
  //Finger finger4 ;
  
void setup() {
  Serial.begin(9600);
}

void loop() {
  //int rd = 401;
  readHand();
  //int flexSensorReading = analogRead(flexSensorPin);
  //  long thistime=millis();
  //  long interval=thistime-lstHandTime;
  int rd = hand[0];
  if(rd>=THRES_F1){
    isPresed=false;
  }
  if (rd < THRES_F1 && isPresed==false) {
    thisTime = millis();//get the time of click
    if(isFirst){
      Serial.println("click");
//      Serial.println(lastTime);
//      Serial.println(thisTime);
      isFirst=false;
    }else if(thisTime-lastTime>interval){
      Serial.println("click");
//      Serial.println(lastTime);
//      Serial.println(thisTime);
      isFirst=false;
    }else{
      Serial.println("double-click");
//      Serial.println(lastTime);
//      Serial.println(thisTime);
      isFirst=true;
    }
    lastTime=thisTime;
    isPresed =true;
  }
  //Serial.println(flexSensorReading);

  //In my tests I was getting a reading on the arduino between 512, and 614.
  //Using map(), you can convert that to a larger range like 0-100.
  //  int flex0to100 = map(flexSensorReading, 512, 614, 0, 100);
  //  Serial.println(flex0to100);

  //delay(500); //just here to slow down the output for easier reading
  //lstHandTime=thistime;
}


int readHand() {
  for(int j=0;j<5;j++){
    int fr = 0;
    for (int i = 0; i < 20; i++) {
    fr += analogRead(flexSensorPin[j]);
    }
    fr /= 20;
    hand[j]=fr;
  }
}
