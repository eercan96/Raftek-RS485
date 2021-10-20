#define MASTER_EN   8                 // connected to RS485 Enable pin
#define RPM_PIN 7
uint8_t hall_Thresh = 20; // "10 value +25rpm" set number of hall trips for RPM reading (higher improves accuracy)
uint8_t hall_Count; // Counter for each spin

float lastSendMillis = 0;
float sendIntervalMillis = 3000;
float rpmValue = 0;

void setup() {
  pinMode(MASTER_EN , OUTPUT);        // Declare Enable pin as output
  pinMode(RPM_PIN, INPUT);
  digitalWrite(MASTER_EN , LOW);      // Make Enable pin low, Receiving mode ON
  Serial.begin(9600);                 // set serial communication baudrate 
}
//encoder -> ayrı yollancak int
void loop()
{
  // Initialize values
  hall_Count = 1;
  float start = millis();
  bool on_state = false;

  // Calculation Spin Time
  while (1) {
    if (digitalRead(RPM_PIN) == 0) 
    {
      if (on_state == false) 
      {
        on_state = true;
        hall_Count++;                   // Increase counter in each spin 
      }
    }
    else 
    {
      on_state = false;
    }

    if (hall_Count >= hall_Thresh)      // Control counter overflow as threshold 
    {
      break;
    }
  } 

  // Calculation Passed Time 
  float end_Time = millis();
  float time_Passed = (end_Time - start) / 1000;
  rpmValue = calc_Rpm(time_Passed);
  sendInput();
  delay(1);// delay in between reads for stability
}

void sendInput()
{
    if(lastSendMillis != 0 && lastSendMillis + sendIntervalMillis > millis())
    {
        return;
    }
    String inputString = "RPM Value=" + String(rpmValue);
    digitalWrite(MASTER_EN , HIGH);     // Make Enable pin high to send Data
    delay(5);                           // required minimum delay of 5ms
    Serial.print(inputString + "*\n");       // Send String serially, End String with *
    Serial.flush();                     // wait for transmission of data
    digitalWrite(MASTER_EN , LOW);      // Receiving mode ON
    lastSendMillis = millis();
}

float calc_Rpm(float passed_Time)
{
  return (hall_Count/passed_Time) * 60;
}
