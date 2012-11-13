bool isArmed=false;
bool isAlarm=false;
bool oldStateArmPin=HIGH; //internal pullup set this pin to high
long lastTimeStateChanged;
long stateChangedDelay=1000; //1sec
bool signalIsMeassuring=false;
bool isStable=false;
bool isSignalStillLOW=false;
bool toHigh=false;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  
}

void loop() {
  bool isAnySensorActivated=false;

  if (!signalIsMeassuring) {
    if (digitalRead(12)==LOW && oldStateArmPin==HIGH) { //spousteci hrana
      Serial.println("rise edge, signal to LOW");
      signalIsMeassuring=true;
      lastTimeStateChanged=millis();
      isStable=false;
      isSignalStillLOW=true;
      oldStateArmPin=LOW;
      Serial.println("oldStateArmPin=LOW");
    }
    
    if (digitalRead(12)==HIGH && oldStateArmPin==LOW) { //dobezna hrana
      Serial.println("descending edge, signal to HIGH");
      lastTimeStateChanged=millis();
      isStable=false;
      toHigh=true;
    }
  }
  else {
    if (millis()-lastTimeStateChanged>50) { //50ms po zmene stavu na LOW kvuli zakmitum
      Serial.println("Signal sampling");
      isStable=true;
    }
    
    if (isStable) {
      if (toHigh) {
        oldStateArmPin=HIGH;
        Serial.println("oldStateArmPin=LOW");
      }
      else if (digitalRead(12)==HIGH) { //signal se po dobu mereni zmenil
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
}