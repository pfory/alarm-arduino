/*bool isArmed=false;
bool isAlarm=false;
bool oldStateArmPin=HIGH; //internal pullup set this pin to high
long lastTimeStateChanged;
long stateChangedDelay=1000; //1sec
bool signalIsMeassuring=false;
bool isStable=false;
bool isSignalStillLOW=false;
*/
byte state=0;
bool stateEntry;
bool oldStateArmPin=HIGH; //internal pullup set this pin to high
long lastTimeStateChanged;
const int timeToEliminateOscillation=50; //in ms
const int timeSampling=1000-timeToEliminateOscillation;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  Serial.println("ArduAlarm");
}

void loop() {
  //state machine
  if (state==0) {
    if (stateEntry) {
      stateEntry=false;
    }

    if (digitalRead(12)==LOW && oldStateArmPin==HIGH) { //falling hrana
      state=1;
      stateEntry=true;
    }
  }
  
  if (state==1) {
    if (stateEntry) {
      Serial.println("Status 1 entry");
      stateEntry=false;
      lastTimeStateChanged = millis();
    }
    
    if (millis()-lastTimeStateChanged>timeToEliminateOscillation) {
      state=2;
      stateEntry=true;
    }
  }
  
  if (state==2) {
    if (stateEntry) {
      Serial.println("Status 2 entry");
      stateEntry=false;
      lastTimeStateChanged = millis();
    }
    
    if (digitalRead(12)==HIGH) {
      state=0;
      stateEntry=true;
    }
    else
    {
      if (millis()-lastTimeStateChanged>timeSampling) {
        state=3;
        stateEntry=true;
      }
    }
  }  
  
  if (state==3) {
    if (stateEntry) {
      Serial.println("Status 3 entry");
      stateEntry=false;
      if (digitalRead(12)==LOW) {
        Serial.println("Status changing");
      }
    }
    
    if (digitalRead(12)==HIGH) {
      state=0;
      stateEntry=true;
    }
  }
    
  oldStateArmPin=digitalRead(12);
  
/*
  bool isAnySensorActivated=false;

  if (!signalIsMeassuring) { //signal neni meren
    if (digitalRead(12)==LOW && oldStateArmPin==HIGH) { //spousteci hrana
      Serial.println("rise edge, signal to LOW");
      signalIsMeassuring=true;
      lastTimeStateChanged=millis();
      isStable=false;
      isSignalStillLOW=true;
    }
    if (digitalRead(12)==HIGH && oldStateArmPin==LOW) { //dobezna hrana
      Serial.println("faling edge, signal to HIGH");
      oldStateArmPin=HIGH;
      delay(50);
    }
  }
  else { //mereni signalu
    if (millis()-lastTimeStateChanged>50) { //50ms po zmene stavu na LOW kvuli zakmitum
      Serial.println("Signal sampling");
      isStable=true;
      oldStateArmPin=LOW;
      //Serial.println("oldStateArmPin=LOW");
    }
    
    if (isStable) {
      if (digitalRead(12)==HIGH) { //signal se po dobu mereni zmenil
        Serial.println("Signal changed during meassurement");
        isSignalStillLOW=false;
      }
    }
    
    if (millis()-lastTimeStateChanged>stateChangedDelay ) { //timeout mereni
      if (isSignalStillLOW) {
        Serial.print("Status changing to ");
        isArmed=!isArmed;
        if (isArmed)
          Serial.println("ARMED");
        else
          Serial.println("NOT ARMED");
      }
      //oldStateArmPin=digitalRead(12);
      signalIsMeassuring=false;
      Serial.print("oldStateArmPin=");
      Serial.println(oldStateArmPin);
        
    }
  }
  if (digitalRead(2)==HIGH) {
    isAnySensorActivated=true;
  }

  
  if (isArmed && isAnySensorActivated) {
    digitalWrite(13, HIGH);
  }
  else {
    digitalWrite(13, LOW);
  }


*/
}