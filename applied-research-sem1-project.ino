
int START_PIN = 3;
int REACTION_PIN = 4;
int LED_RED_PIN = 9;
int LED_BLUE_PIN = 10;
int LED_GREEN_PIN = 11;

int RED = 0;
int BLUE = 150;
int GREEN = 200;

boolean ledOn = false;
boolean activated = false;
long expectedPressTime = -1;
int counterIndex = -1;
int counter = 0;

int counterValues[] = {0, 1, 2, 3};
int numCounterValues = 4;

void shuffle() {
  Serial.print("Counter values before shuffle: [");
  for (int i = 0; i < numCounterValues; i++) {
    Serial.print(counterValues[i]);
    Serial.print(", ");
  }
  Serial.println("]");
  for (int i = 0; i < numCounterValues; i++) {
    int index = random(0, numCounterValues);
    int temp = counterValues[index];
    counterValues[index] = counterValues[i];
    counterValues[i] = temp;
  }
  Serial.print("Counter values after shuffle: [");
  for (int i = 0; i < numCounterValues; i++) {
    Serial.print(counterValues[i]);
    Serial.print(", ");
  }
  Serial.println("]");
}

void activate() {
  activated = true;
  long randNumMs = random(2000, 7000);
  expectedPressTime = millis() + randNumMs;
  counterIndex += 1;
  if (counterIndex > numCounterValues) {
    shuffle();
    counterIndex = -1;
  }
  counter = counterValues[counterIndex];
  Serial.print("Counter index: ");
  Serial.println(counterIndex);
  if (counter == 0) {
    RED = 0;
    BLUE = 128;
    GREEN = 128;
  } else if (counter == 1) {
    RED = 85;
    BLUE = 85;
    GREEN = 85;
  } else if (counter == 2) {
    RED = 170;
    BLUE = 43;
    GREEN = 43;
  } else if (counter == 3) {
    RED = 255;
    BLUE = 0;
    GREEN = 0;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  randomSeed(analogRead(0));
  shuffle();
}

void loop() {
  // put your main code here, to run repeatedly:
  long currTime = millis();
  if (activated) {
    if (digitalRead(REACTION_PIN) == HIGH && currTime > expectedPressTime) {
      long differential = currTime - expectedPressTime;
      digitalWrite(LED_RED_PIN, 0);
      digitalWrite(LED_BLUE_PIN, 0);
      digitalWrite(LED_GREEN_PIN, 0);
      activated = false;
      ledOn = false;
      Serial.print("Differential: ");
      Serial.print(differential);
      Serial.println("ms");
    } else if (currTime > expectedPressTime && !ledOn) {
      Serial.print("RGB: (");
      Serial.print(RED);
      Serial.print(", ");
      Serial.print(BLUE);
      Serial.print(", ");
      Serial.print(GREEN);
      Serial.println(")");
      analogWrite(LED_RED_PIN, RED);
      analogWrite(LED_BLUE_PIN, BLUE);
      analogWrite(LED_GREEN_PIN, GREEN);
      ledOn = true;
      Serial.println("Turning on light");
    }
  } else if (digitalRead(START_PIN) == HIGH) {
    activate();
    Serial.println("Activated");
  }
}
