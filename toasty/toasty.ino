/*
Application-level constants
*/
const boolean ON = true;
const boolean OFF = false;
const int ACTIVE_INTERVAL_MSECS = 500;
const int INACTIVE_INTERVAL_MSECS = 2000;
const int MIN_TOASTING_DURATION_MSECS = 50000;


/*
Application-level variables
*/
boolean currentState = OFF;
boolean lastState = OFF;
int onDuration = 0;
int checkingInterval = INACTIVE_INTERVAL_MSECS;

int onThreshold = 60;

/*
Hardware constants
*/
const int ledPin = 13;


/*
Hardware variables
*/

int lightSensorValue;
int sensorLow = 1023;
int sensorHigh = 0;

int piezoPin = 8;


void setup() {
	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, HIGH);
	Serial.begin(9600);

	while(millis() < 5000) {
	     lightSensorValue = analogRead(A0);
	     if(lightSensorValue > sensorHigh)
	     {
	       sensorHigh = lightSensorValue;
	     }
	     
	     if(lightSensorValue < sensorLow)
	     {
	       sensorLow = lightSensorValue;
	     }
	     
	     digitalWrite(ledPin, LOW);
	} 

	Serial.println("High: ");
	Serial.println(sensorHigh);
	Serial.println(" Low: ");
	Serial.println(sensorLow);
}


boolean getLightState() {
	lightSensorValue = analogRead(A0);
	int reading = map(lightSensorValue, sensorLow, sensorHigh, 0, 100);
	
        Serial.println("LightSensorValue: ");
	Serial.println(lightSensorValue);
	Serial.println("Reading: ");
	Serial.println(reading);  
        if(reading >= onThreshold) {
		return ON;
	}
  	return OFF;
}

void beginToasting() {
	onDuration = 1;
	checkingInterval = ACTIVE_INTERVAL_MSECS;
}

void stillToasting() {
	onDuration += 1;
	checkingInterval = ACTIVE_INTERVAL_MSECS;
}

void toastPopped() {
	onDuration = 0;
	checkingInterval = INACTIVE_INTERVAL_MSECS;
	letThereBeToast();
}

void letThereBeToast() {
	tone(piezoPin, 440, 1000);
	delay(1000);
        tone(piezoPin, 600, 1000);
	delay(1000);
        tone(piezoPin, 500, 1000);
        delay(1000);
}

void noToast() {
	onDuration = 0;
	checkingInterval = INACTIVE_INTERVAL_MSECS;
}

boolean lightTurnedOn() {
	return currentState && !lastState;
}

boolean lightRemainsOn() {
	return currentState && lastState;
}

boolean lightTurnedOff() {
	return !currentState && lastState;
}

boolean minDurationPassed() {
        Serial.println("OnDuration: ");
        Serial.println(onDuration);
	return onDuration * ACTIVE_INTERVAL_MSECS >= MIN_TOASTING_DURATION_MSECS; 
}

void loop() {
  currentState = getLightState();
  Serial.println(currentState ? "ON" : "OFF");
  if (lightTurnedOn()) {
  	Serial.println("Light Turned On!");
  	beginToasting();
  }
  else if (lightRemainsOn()) {
  	Serial.println("Light Still On!");
  	stillToasting();
  }
  else if (lightTurnedOff() && minDurationPassed()) {
  	Serial.println("Light turned off!");
  	toastPopped();
  } 
  else {
  	Serial.println("No light!");
  	noToast();
  }
  delay(checkingInterval);
  lastState = currentState;
}
