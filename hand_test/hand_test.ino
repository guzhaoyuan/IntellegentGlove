
int flexSensorPin[5] = {A0, A1, A2, A3, A4};
int THRES_F1 = 260;      //A0 voltage change value
//int debounce = 150;    //
long thisTime = 0;       //
int interval = 800;      //time interval between click and double-click
int hand[5];

class Finger {
  public:
    //4 status for a finger
    static String clicked ;
    static String isBent ;
    static String freed ;
    static String double_clicked ;
    static String relax ;

    int status;               //analog voltage in
    String fingerInfo;
    long lastClickedTime = 0 - interval;
    bool isFirst = true;     //flag judging whether click is firstly bent
    bool isPresed = false;   //flag judging whether sensor is bent or straight
    Finger(int stat, String info) {
      this->status = stat;
      this->fingerInfo = info;
    }
    String toString() {
      return fingerInfo;
    }
};

String Finger::clicked = "is clicked";
String Finger::isBent = "is Bent";
String Finger::freed = "is freed";
String Finger::double_clicked = "is double-clicked";
String Finger::relax = "is relaxed";

Finger *finger[5] = {
  new Finger(0, "Finger 0"), 
  new Finger(0, "Finger 1"), 
  new Finger(0, "Finger 2"),
  new Finger(0, "Finger 3"), 
  new Finger(0, "Finger 4")
};

void setup() {
  Serial.begin(9600);
}

void loop() {
  //readHand();
  //
  for (int j = 0; j < 2; j++) {
    readFinger(j);
    thisTime = millis();//get the time of click
    if (hand[j] >= THRES_F1) {
      if (finger[j]->isPresed == true) {
        Serial.print(finger[j]->toString());
        Serial.println(Finger::freed);
      }
      finger[j]->isPresed = false;
    }
    if (hand[j] < THRES_F1 && finger[j]->isPresed == false) {
      if (finger[j]->isFirst) {
        Serial.print(finger[j]->fingerInfo);
        Serial.println(Finger::isBent);
        finger[j]->isFirst = false;
      } else if ((thisTime - finger[j]->lastClickedTime) > interval) {
        Serial.print(finger[j]->fingerInfo);
        Serial.println(Finger::clicked);
        finger[j]->isFirst = false;
      } else {
        Serial.print(finger[j]->fingerInfo);
        Serial.println(Finger::double_clicked);
        finger[j]->isFirst = true;
        finger[j]->lastClickedTime = thisTime;
      }
      finger[j]->isPresed = true;
    }
  }
  delay(100);
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

int readFinger(int finNum){
    
    int fr = 0;
    for (int i = 0; i < 20; i++) {
      fr += analogRead(flexSensorPin[finNum]);
    }
    fr /= 20;
    hand[finNum] = fr;
}
