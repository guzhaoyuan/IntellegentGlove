#include <Wire.h>
#include <SeeedOLED.h>

#define F1 A0
#define F2 A1
#define MENU_MODE 1 //menu selection mode
#define FUNC_MODE 2 // working mode

const int UU=0;
const int UD=1;
const int DU=2;
const int DD=3;

int THRES_F1=200;
int THRES_F2=200;
int lastHand=0;
int debounce=150;
long lstHandTime=0;

//////////////////////////////////////////////////////////
//global vars
//////////////////////////////////////////////////////////
int CURRENT_MODE=MENU_MODE;

//for menu items--------------------------------------------
char* menus[]={"Console","Speed","Gyroscope","Altitude","Temperature"};
int menuListCount=6;//menus items per page
int menuCounts=5;
int menuStartRow=2;//which row starts the menu
int titleSplitRow=1;
int currentMenuPage=-1;
int menuSelected=0;
int bottomSelected=0;
int bottomInit=false;

void setup(){
  
  Wire.begin();
  SeeedOled.init();
  //cls();
  //drawEmpty();
  initUI();
  //Serial.begin(9600);
}

void loop(){
 int rd=readHand();
  long thistime=millis();
  long interval=thistime-lstHandTime;
  if(rd!=lastHand && interval>debounce &&rd>0){
   switch(rd){
     case DU:
     if(CURRENT_MODE==MENU_MODE){
        if(menuSelected<menuCounts-1){
          menuSelected++;
        }
        else{
          menuSelected=0;
        }
        mainMenu(menuSelected);
     }
     if(CURRENT_MODE==FUNC_MODE){
       if(bottomSelected<1){
          bottomSelected++;
        }
        else{
          bottomSelected=0;
        }
        bottomMenu(bottomSelected);
     }
     break;
     
     case UD:
      modeChange();
     break;
   }
    lstHandTime=thistime;
  }
}

//-----read hand------
int readHand(){
  int fr_1=0;
  int fr_2=0;
  for(int i=0;i<20;i++){
    fr_1+=analogRead(F1);
    fr_2+=analogRead(F2);
  }
  fr_1=fr_1/20;
  fr_2=fr_2/20;
  
  if(fr_1<THRES_F1 && fr_2>THRES_F2){
    return DU;
  }
  if(fr_1<THRES_F1 && fr_2<THRES_F2){
    return DD;
  }
  if(fr_1>THRES_F1 && fr_2>THRES_F2){
    return UU;
  }
  if(fr_1>THRES_F1 && fr_2<THRES_F2){
    return UD;
  }
  return 0;
}

//clean the screen
void cls(){
  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();  //Set addressing mode to Page Mode
  bottomInit=false;
  currentMenuPage=-1;
}

//////////////////////////////////////////////////////////
//drawing functions
//////////////////////////////////////////////////////////
//draw txt 
void setTXT(int row,int col,char* txt,boolean line=false){
  if(line){
    SeeedOled.setTextXY(row,0);
    SeeedOled.putString("               ");
  }
  else{
    SeeedOled.setTextXY(row,col);
    SeeedOled.putString(" ");
  }
  SeeedOled.setTextXY(row,col);
  SeeedOled.putString(txt);
}
//draw numbers
void setNUM(int row,int col,int txt,boolean line=false){
  if(line){
    SeeedOled.setTextXY(row,0);
    SeeedOled.putString("               ");
  }
  else{
    SeeedOled.setTextXY(row,col);
    SeeedOled.putString(" ");
  }
  SeeedOled.setTextXY(row,col);
  SeeedOled.putNumber(txt);
}

//draw Main menu
void mainMenu(int cur){
  int tempPage=(cur-cur%menuListCount)/menuListCount;
  if(tempPage!=currentMenuPage){
    currentMenuPage=tempPage;
    int item=currentMenuPage*menuListCount;
    for(int i=0;i<menuListCount;i++){
       if(item<menuCounts){
         setTXT(i+menuStartRow,1,menus[item],true);
         item++;
       }
       else{
         setTXT(i+menuStartRow,0," ",true);
       }
     }
  }
  setNUM(1,14,menuSelected,false);
  for(int i=0;i<menuListCount;i++){
    setTXT(i+menuStartRow,0," ",false);
  }
  setTXT(cur%menuListCount+menuStartRow,0,">",false);
}

//draw bottom menu
void bottomMenu(int cur){
      switch(menuSelected){
        case 0:
          if(!bottomInit){setTXT(7,0," Save    Retry",true);}
          setTXT(7,0," ",false);
          setTXT(7,8," ",false);
          setTXT(7,cur*8,">",false);
        break;
        case 1:
          if(!bottomInit){setTXT(7,0," mid fingure esc",true);}
        break;
      }
 }
 
 //change system mode
void modeChange(){
  
  switch(CURRENT_MODE){
    case MENU_MODE:
      CURRENT_MODE=FUNC_MODE;
      bottomSelected=0;
    break;
    case FUNC_MODE:
      funcButtonHandler();
    break;
  }
  initUI();
}

void funcButtonHandler(){
  switch(menuSelected){
    case 0:
      if(bottomSelected==0){bottomSelected=-1;CURRENT_MODE=MENU_MODE;}
    break;
    case 1:
      bottomSelected=-1;CURRENT_MODE=MENU_MODE;
    break;
  }
}

//init UI
void initUI(){
  cls();
  drawTitle();
  if(CURRENT_MODE==MENU_MODE){mainMenu(menuSelected);}
  if(CURRENT_MODE==FUNC_MODE){
    bottomMenu(0);
    bottomInit=true;
    //switch(menuSelected){
    //}
  } 
}

// draw ui title
void drawTitle(){
  SeeedOled.setTextXY(0,0);
  switch(CURRENT_MODE){
    case MENU_MODE:
       SeeedOled.putString("Main Menu");
    break;
    case FUNC_MODE:
      SeeedOled.putString(menus[menuSelected]);
    break;
  }
  SeeedOled.setTextXY(titleSplitRow,0);
  SeeedOled.putString("---------------");
}

void drawEmpty(){
  SeeedOled.setTextXY(0,0);
  SeeedOled.putString("GloveKit V1.0");
  SeeedOled.setTextXY(titleSplitRow,0);
  SeeedOled.putString("---------------");
  SeeedOled.setTextXY(2,0);
  SeeedOled.putString("00km/h  0.0xG");
  SeeedOled.setTextXY(3,0);
  SeeedOled.putString("TMP:24C  ALT:50m");
  SeeedOled.setTextXY(4,0);
  SeeedOled.putString("Heartrate:75");
  SeeedOled.setTextXY(5,0);
  SeeedOled.putString("Phone:connected");
}
