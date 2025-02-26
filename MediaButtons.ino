/* Media Buttons plus Rotary Encoder
   Started with the code from this page: 
   https://www.instructables.com/Multimedia-Keypad/
   and tweaked a bit. Note it is mostly code from a Teensy 
   example, with rotary encoder handling added.

   Buttons to USB Keyboard Example - Special Media Player Keys
   You must select Keyboard from the "Tools > USB Type" menu
   This example code is in the public domain.
*/

#include <Bounce.h>
#include <Encoder.h>

// Create Bounce objects for each button.  The Bounce object automatically 
// deals with contact chatter or "bounce", and it makes detecting changes
// very simple. 10 ms debounce time is appropriate for most mechanical 
// pushbuttons if a button is too "sensitive"  you can increase this time
Bounce button0 = Bounce(0, 10); // Switch to left virtual desktop
Bounce button1 = Bounce(1, 10); // Switch to right virtual desktop
Bounce button2 = Bounce(2, 10); // Play/Pause
Bounce button3 = Bounce(3, 10); // Previous Track
Bounce button4 = Bounce(4, 10); // Next Track
Bounce button5 = Bounce(5, 10); // Mute
Bounce button6 = Bounce(6, 10); // Rotary Encoder pushbutton Play/Pause

Encoder myEnc(7,8); // initialize the rotary encoder on pins 7 and 8

int led = 13; // initialize the LED on pin 13 to know that the code is working

void setup() {
  // Configure the pins for input mode with pullup resistors.
  // The pushbuttons connect from each pin to ground.  When
  // the button is pressed, the pin reads LOW because the button
  // shorts it to ground.  When released, the pin reads HIGH
  // because the pullup resistor connects to +5 volts inside
  // the chip.
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
	pinMode(5, INPUT_PULLUP);
	pinMode(6, INPUT_PULLUP);

  // Flash the on board LED three times as the last setup step
	pinMode(led, OUTPUT);
	digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
  delay(100);
	digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
  delay(100);
	digitalWrite(led, HIGH);
  delay(400);
  digitalWrite(led, LOW);
  
	Serial.begin(9600);
}

// TODO: remove the serial print commands once happy with how it is performing
// get rid of the Serial.begin command too? do i need that for anything other than printing? 

long oldPosition = -999;
int vollevel = 0;

void loop() {
  // Update all the buttons.  There should not be any long
  // delays in loop(), so this runs repetitively at a rate
  // faster than the buttons could be pressed and released.
  button0.update();
  button1.update();
  button2.update();
  button3.update();
  button4.update();
	button5.update();
	button6.update();

  // Check each button for "falling" edge.
  // falling = high (not pressed - voltage from pullup resistor)
  //           to low (pressed - button connects pin to ground)
  if (button0.fallingEdge()) { 
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_LEFT);
    delay(100);
    Keyboard.releaseAll();
    delay(500);
  }
  if (button1.fallingEdge()) { 
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_RIGHT);
    delay(100);
    Keyboard.releaseAll();
    delay(500);
  }
  if (button2.fallingEdge()) { 
    Keyboard.set_media(KEY_MEDIA_PLAY_PAUSE);
    Keyboard.send_now();
    Keyboard.set_media(0);
    Keyboard.send_now();
  }
  if (button3.fallingEdge()) { 
    Keyboard.set_media(KEY_MEDIA_PREV_TRACK);
    Keyboard.send_now();
    Keyboard.set_media(0);
    Keyboard.send_now();
  }
	if (button4.fallingEdge()) { 
    Keyboard.set_media(KEY_MEDIA_NEXT_TRACK);
    Keyboard.send_now();
    Keyboard.set_media(0);
    Keyboard.send_now();
  }
  if (button5.fallingEdge()) {
    Keyboard.set_media(KEY_MEDIA_MUTE);
    Keyboard.send_now();
    Keyboard.set_media(0);
    Keyboard.send_now();
  }
	if (button6.fallingEdge()) {
    Keyboard.set_media(KEY_MEDIA_PLAY_PAUSE);
    Keyboard.send_now();
    Keyboard.set_media(0);
    Keyboard.send_now();
  }

	// Rotary Encoder 
	long newPosition = myEnc.read();
	if (newPosition != oldPosition) {
		int mod = newPosition % 4;
		if(mod == 0) { // four positions per detent, only want one, hence modulo
			if(newPosition < vollevel) { // "bounce" preventer
				Serial.println("Volume up!");
				Keyboard.set_media(KEY_MEDIA_VOLUME_INC);
				Keyboard.send_now();
				Keyboard.set_media(0);
				Keyboard.send_now();
			} else if(newPosition > vollevel) {
				Serial.println("Volume down!");
				Keyboard.set_media(KEY_MEDIA_VOLUME_DEC);
				Keyboard.send_now();
				Keyboard.set_media(0);
				Keyboard.send_now();
			}
			vollevel = newPosition;
		}
		oldPosition = newPosition;
	}
}
