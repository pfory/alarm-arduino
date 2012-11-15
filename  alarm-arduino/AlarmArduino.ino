byte state=0;
bool stateEntry=true;
bool oldStateArmPin=HIGH; //internal pullup set this pin to high
long lastTimeStateChanged;
const int timeToEliminateOscillation=50; //in ms
const int timeSampling=1000-timeToEliminateOscillation;
const byte pinArmDisarm=12;
bool isArmed=false;
bool isAlarm=isArmed;
bool isAnySensorActivated=false;
long timeForArmDisarm=0;
const int timeDelayForArm=5000; //in ms
const int timeDelayForDisArm=10000; //in ms

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(pinArmDisarm, OUTPUT);
  Serial.println("ArduAlarm");
}

void loop() {
  if (stateMachineForArmDisarm(pinArmDisarm, &isArmed)) {
    timeForArmDisarm=millis();
  }
  
  if (timeForArmDisarm>0) {
    if (isArmed) {
      if (millis()-timeForArmDisarm>timeDelayForArm) {
        isAlarm=true;
        timeForArmDisarm=0;
      }
    else
      if (millis()-timeForArmDisarm>timeDelayForDisArm) {
        isAlarm=false;
        timeForArmDisarm=0;
      }
    }
  }
  
  if (digitalRead(2)==HIGH) {
    isAnySensorActivated=true;
  }
  
  if (isAlarm && isAnySensorActivated) {
    digitalWrite(13, HIGH);
  }
  else {
    digitalWrite(13, LOW);
  }

  isAnySensorActivated=false;
}


bool stateMachineForArmDisarm(byte pin, bool* armStatus) {
  bool changeState=false;
  //state machine
  if (state==0) {
    if (stateEntry) {
      //Serial.println("Status 0 entry");
      stateEntry=false;
    }

    if (digitalRead(pin)==LOW && oldStateArmPin==HIGH) { //falling hrana
      state=1;
      stateEntry=true;
    }
  }
  
  if (state==1) {
    if (stateEntry) {
      //Serial.println("Status 1 entry");
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
      //Serial.println("Status 2 entry");
      stateEntry=false;
      lastTimeStateChanged = millis();
    }
    
    if (digitalRead(pin)==HIGH) {
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
      //Serial.println("Status 3 entry");
      stateEntry=false;
    }

    if (digitalRead(pin)==LOW) {
      Serial.println("Status changing");
      *armStatus=!*armStatus;
      changeState=true;
      Serial.println(*armStatus);
      if (*armStatus)
        Serial.println("ARMED");
      else
        Serial.println("DISARMED");
      state=4;
      stateEntry=true;
    }
  }

  if (state==4) {
    if (stateEntry) {
      //Serial.println("Status 4 entry");
      stateEntry=false;
    }

    if (digitalRead(pin)==HIGH) {
      state=5;
      stateEntry=true;
    }
  }

  if (state==5) {
    if (stateEntry) {
      //Serial.println("Status 5 entry");
      lastTimeStateChanged = millis();
      stateEntry=false;
    }
  
    if (millis()-lastTimeStateChanged>timeToEliminateOscillation) {
      if (digitalRead(pin)==HIGH) {
        state=0;
        stateEntry=true;
      }
    }
  }
  
  oldStateArmPin=digitalRead(pin);   
  return changeState;
}