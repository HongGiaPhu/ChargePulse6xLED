int LED[6] = {3,5,6,9,10,11}; //PWM pin
int button = 2; 
int alert = 13; //Green Led
int ready = 7; //Buzzer

void setup() {
  for(int i = 0; i < 6; i++) {
    pinMode(LED[i], OUTPUT); //Set LED pins as output
  }
  pinMode(button, INPUT_PULLUP); //Easier wiring, reads LOW when pressed
  pinMode(alert,OUTPUT); 
  pinMode(ready,OUTPUT);
}

int phase = 16; //when phase = 16, the LED brightness increases in 16 steps instead of 255
bool state = false; //Logical state indicating the "fully charged" condition

void charging() { 
  for(int k = 0; k <= 255; k += 256/phase) { //Increment brightness based on the selected phase
    if (digitalRead(button) == HIGH)  //Only charge when the button is pressed. If not pressed (HIGH STATE), turn off LEDs
    {
      for(int i = 0;i<6;i++)
        digitalWrite(LED[i],LOW);
      return; //return immediately
    }

    for(int i = 0; i < 6; i++) //Otherwise, increase LED brightness according to the current phase
      analogWrite(LED[i], k); 

    delay(100); // Delay between steps (adjust for faster or slower charging)
  }

  state = true; // Charging completed
  digitalWrite(alert, HIGH); //Buzzer beep
  digitalWrite(ready,HIGH); //Green LED light up
  delay(500);
  digitalWrite(alert, LOW); //Buzzer off after 0.5 secs
}


void pulse() {
  digitalWrite(alert,HIGH); // Activate buzzer
  delay(500);
  digitalWrite(alert, LOW); // Turn off buzzer after 0.5 seconds
  digitalWrite(ready,LOW); // Turn off green LED (no longer fully charged)
  for(int i = 0; i < 6; i++) {
    digitalWrite(LED[i], LOW); // Turn off LEDs one by one for a visual effect
    delay(50);
  }
  state = false; //Reset charging state
}

void loop() {
  if(digitalRead(button) == LOW && state == false) { // Start charging only if not fully charged and the button is pressed (LOW)
    charging();
    while(digitalRead(button) == LOW); // Wait until the button is released
  }

  if(digitalRead(button) == LOW && state == true) { // Trigger pulse only if fully charged and the button is pressed (LOW)
    pulse();
    while(digitalRead(button) == LOW);  // Wait until the button is released
  }
}

