int analogPin1 = 1;     // potentiometer wiper (middle terminal) connected to analog pin 3
int analogPin0 = 0;
                      // outside leads to ground and +5V


int val0 = 0;           // variable to store the value read
int val1 = 0; 
void setup()
{
  Serial.begin(9600);          //  setup serial
}
void loop()
{
  val0 = analogRead(analogPin0);    // read the input pin
  val1 = analogRead(analogPin1);
  Serial.println(val0);             // debug value
  Serial.println(val1); 
  Serial.println('*'); 
  delay(500);
}

