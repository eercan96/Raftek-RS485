#define SLAVE_EN  8
String recInput;  
String recInput2;// Variable to store Receive string 
const char* parsedString;
void setup() {
  pinMode(SLAVE_EN , OUTPUT);                   // Declare Enable pin as output
  Serial.begin(9600);                           // set serial communication baudrate 
  digitalWrite(SLAVE_EN , LOW);                 // Make Enable pin low, Receiving mode ON 
}
 
void loop() {
  while(Serial.available())                     // If serial data is available then enter into while loop
  {
    recInput = Serial.readStringUntil('*');     // Receive Serial data in Variable
    parsedString = strtok(recInput.c_str(),"_");

    while(parsedString != NULL)
    {
        Serial.println(parsedString);
        parsedString = strtok(NULL, "_");
    }
  }
}
