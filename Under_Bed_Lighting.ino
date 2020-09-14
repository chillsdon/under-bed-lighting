const int sensorPin = A2; // Analogue input pin for LDR
const int analogOutPin = 1; // Analogue (PWM) output to boost converter 0v in 0v out, 5v in 12v out
const int pirInputPin = 0;
const int lightsOnDuration = 180000; // Three minutes
const int fadeStep = 5;
const int sensorReadCounterLimit = 5;

const int testingThreshold = 800;
const int darkThreshold = 200;
int effectiveThreshold = darkThreshold;

const int darkInterval = 100; //250;
const int fadeInterval = 5;
int currentInterval = darkInterval;

int outputValue = 0; // Variable to hold output to LED strip: 0 = 0v, 255 = 5v (boosted to 12v)
int pirState = LOW;  // Variable to hold the incoming state from the PIR
int sensorValue = 0; // Variable to store the value coming from the LDR sensor

int sensorReadCounter = 1; // Used to reduce how often the sensor is read

unsigned int previousMillis = 0;
unsigned int currentMillis = 0;
unsigned int lightsOnMillis;

bool isDark = false;
bool isMovement = false;

enum lightState {
  On,
  Off,
  FadingUp,
  FadingDown
};

lightState currentLightState = Off;

//========================================================
void setup()
{
  //Serial.begin(9600); // Sets serial port for communication
  
  pinMode(pirInputPin, INPUT);

  previousMillis = millis();
}

//========================================================
void loop()
{
  pirState = digitalRead(pirInputPin);
  isMovement = pirState == HIGH;
  
  if (intervalUp())
  {
    outputCurrentState();
    
    if (currentLightState == Off || currentLightState == FadingDown)
    {
      // Only read the LDR every 10 intervals as it's slow
      sensorReadCounter += 1;

      if (sensorReadCounter == sensorReadCounterLimit)
      {
        outputMovement();
        
        // Check if it's dark
        //Serial.println("Reading LDR...");
        sensorValue = analogRead(sensorPin); // Read the value from the LDR sensor

        //Serial.print("LDR Sensor Reading: ");
        //Serial.println(sensorValue);
   
        isDark = sensorValue < effectiveThreshold;

        outputIsDark();

        if (isDark && isMovement)
        {
          currentLightState = FadingUp;
        }

        // Reset counter
        sensorReadCounter = 1;
      }
    }

    switch (currentLightState)
    {
      case On:
        if (currentMillis - lightsOnMillis > lightsOnDuration)
        {
          currentLightState = FadingDown;
        }
        break;

      case Off:
        if (isDark && isMovement)
        {
          currentLightState = FadingUp;
        }
        break;

      case FadingUp:
        fadeUp();
        break;

      case FadingDown:
        fadeDown();
        break;
    }
  }
}

//========================================================
void fadeUp()
{
  currentInterval = fadeInterval;
  
  if (outputValue < 255)
  {
    outputValue += fadeStep; 
    analogWrite(analogOutPin, outputValue);
  }
  else
  {
    currentInterval = darkInterval;
    currentLightState = On;
    lightsOnMillis = millis();
  }
}

//========================================================
void fadeDown()
{
  currentInterval = fadeInterval;
  
  if (outputValue > 0)
  {
    outputValue -= fadeStep;
    analogWrite(analogOutPin, outputValue);
  }
  else
  {
    currentInterval = darkInterval;
    currentLightState = Off;
  }
}

//========================================================
bool intervalUp()
{
  currentMillis = millis();

  // Handle millis() rollover
  if (currentMillis < previousMillis)
  {
    previousMillis = 0;
  }

  bool result = currentMillis - previousMillis > currentInterval;

  if (result)
  {
    previousMillis = currentMillis;  
  }
  
  return
    result;
}

//========================================================
void outputIsDark()
{
  //Serial.print("Dark: ");

  if (isDark)
  {
    //Serial.println("Yes");
  }
  else
  {
    //Serial.println("No");
  } 
}

//========================================================
void outputMovement()
{
  //Serial.print("Movement: ");

  if (isMovement)
  {
    //Serial.println("Yes");
  }
  else
  {
    //Serial.println("No");
  }  
}

//========================================================
void outputCurrentState()
{
  switch (currentLightState)
  {
    case On:
      //Serial.println("currentLightState = On");
      break;

    case Off:
      //Serial.println("currentLightState = Off");
      break;

    case FadingUp:
      //Serial.println("currentLightState = FadingUp");
      break;

    case FadingDown:
      //Serial.println("currentLightState = FadingDown");
      break;
  }
}
