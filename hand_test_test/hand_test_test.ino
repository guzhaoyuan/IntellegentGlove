
int flexSensorPin[5] = {A0, A1, A2, A3, A4};
int THRES_F1 = 400;      //A0 voltage change value
//int debounce = 150;    //
long thisTime = 0;       //
int interval = 800;      //time interval between click and double-click
int hand[5];

class Finger {
  public:
    String fingerInfo;
    //4 status for a finger
    static String onpress ;
    static String downpress ;
    static String free ;
    static String double_clicked ;
    static String relax ;

    int status;               //analog voltage in
    long lastClickedTime = 0 - interval;
    bool isFirst = true;     //flag judging whether click is firstly bent
    bool isPresed = false;   //flag judging whether sensor is bent or straight
    Finger(int stat) {
      this->status = stat;
    }
    String toString() {
      return fingerInfo;
    }
};
String Finger::onpress = "is onPressing";
String Finger::downpress = "is clicked";
String Finger::free = "is clicked";
String Finger::double_clicked = "is clicked";
String Finger::relax = "is relaxing";

Finger *finger[5] = {new Finger(0), new Finger(0), new Finger(0),
         new Finger(0), new Finger(0)
};


void setup() {
  Serial.begin(9600);
}

void loop() {
  readHand();
  //
  //for (int j = 0; j < 5; j++) {
    Serial.println(hand[0]);
  //}
  delay(500);
  for (int j = 0; j < 5; j++) {
    thisTime = millis();//get the time of click
    if (hand[j] >= THRES_F1) {
      if(finger[j]->isPresed==true){
        Serial.println(finger[j]->toString());
        //Serial.println(Finger.free);
      }
      
      finger[j]->isPresed = false;
    }
    if (hand[j] < THRES_F1 && finger[j]->isPresed == false) {
      if (finger[j]->isFirst) {
        Serial.println(finger[j]->fingerInfo);
        Serial.println("click");
        finger[j]->isFirst = false;
      } else if (thisTime - finger[j]->lastClickedTime > interval) {
        Serial.println(finger[j]->fingerInfo);
        Serial.println("click");
        finger[j]->isFirst = false;
      } else {
        Serial.println(finger[j]->fingerInfo);
        Serial.println("double-click");
        finger[j]->isFirst = true;
        finger[j]->lastClickedTime = thisTime;
      }
      finger[j]->isPresed = true;
    }
    

  }

}


int readHand() {
  for (int j = 0; j < 5; j++) {
    int fr = 0;
    for (int i = 0; i < 20; i++) {
      fr += analogRead(flexSensorPin[j]);
    }
    fr /= 20;
    hand[j] = fr;
  }
}
