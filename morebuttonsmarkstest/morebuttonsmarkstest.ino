// 
// V0.1	30/12/17	ISR	Basic control from a single knob
// V0.2				ISR	Display value of a single label (after enabling strings in config.h)
// V0.5 13/10/18	ISR	Multi-page display

#include <ControlChain.h>
#define buttons 5

// the following variables are unsigned long's because the time, measured in miliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime[buttons];	// the last time the output pin was toggled
unsigned long debounceDelay = 20;			// the debounce time; increase if the output flickers
int lastButtonState[buttons];				// the previous reading from the input pin
int buttonState[buttons];					// the current reading from the input pin
float buttonValue;
int buttonPin = 2;
int displayloop = 0;
int ledPin = 13;
int displaypage = 0;
int displaybuttonstate;
int displaybuttonlaststate=0;



//amount of actuators connected 
#define amountOfPorts 7
#define amountOfPotentiometers 2
float actuatorValues[amountOfPorts], maxValues[amountOfPorts], minValues[amountOfPorts], potValues[amountOfPorts];
 
//2D array for saving the actuator labels
char  actuatorNames[amountOfPorts][7];
//String testactname = "name unassigned";

ControlChain cc;


void setup() {	
	// configure button pin as input and enable internal pullup
	for (int i = 0; i < buttons; i++)
	{
		lastButtonState[i] = 0;
		buttonState[i] = 0;
		lastButtonState[i] = 0;
	}

	// initialize control chain
	cc.begin();

	// define device name (1st parameter) and its URI (2nd parameter).  The URI must be an unique identifier for your device. A good practice is to use a URL pointing to your project's code or documentation
	const char *uri = "https://www.rogersons.net/ModControl";
	cc_device_t *device = cc.newDevice("ISR", uri);

	for (int i = 0; i < amountOfPorts; i++) {
		cc_actuator_config_t actuator_config;
		switch (i) {
		case 0: actuator_config.name = "Button 1"; break; //actuator_config.type = CC_ACTUATOR_MOMENTARY; actuator_config.max = 1; break;
		case 1: actuator_config.name = "Button 2"; break; //actuator_config.type = CC_ACTUATOR_MOMENTARY; break;
		case 2: actuator_config.name = "Button 3"; break; //actuator_config.type = CC_ACTUATOR_MOMENTARY; break;
		case 3: actuator_config.name = "Button 4"; break; //actuator_config.type = CC_ACTUATOR_MOMENTARY; break;	
		case 4: actuator_config.name = "Button 5"; break; //actuator_config.type = CC_ACTUATOR_MOMENTARY; break;
		case 5:	actuator_config.name = "Knobswitch 2";  break;
		case 6:	actuator_config.name = "Knob 1";   break;
		case 7: actuator_config.name = "Knob 2";   break;
		}

		actuator_config.value = &actuatorValues[i];
	//	actuator_config.min = 0.0;
		if (i < 8)
		{
		// Setup switches here					
			actuator_config.type = CC_ACTUATOR_MOMENTARY;
			actuator_config.name = "Btn " + i - 5;
			actuator_config.value = &buttonValue;
			actuator_config.min = 0.0;
			actuator_config.max = 1.0;
			actuator_config.supported_modes = CC_MODE_TOGGLE | CC_MODE_TRIGGER;
		  actuator_config.max_assignments = 1;
		
		// create and add actuator to device
		cc_actuator_t *actuator;
		actuator = cc.newActuator(&actuator_config);
		cc.addActuator(device, actuator);


	cc.setEventCallback(CC_EV_UPDATE, updateValues);
	cc.setEventCallback(CC_EV_ASSIGNMENT, updateNames);
	}
	
	int null;
	for (int i = 0; i < buttons; i++)
	{
		null = ReadButton[0];
	}
}

void loop() {
	if (displayloop == 500)
	{
	
	displayloop++;

	actuatorValues[0] = ReadButton(0);
	actuatorValues[1] = ReadButton(1);
	actuatorValues[2] = ReadButton(2);
	actuatorValues[3] = ReadButton(3);


	cc.run();
}

int ReadButton(int buttonnum) {
	int bpin = 40 + (buttonnum * 2);	// Pins 40, 42 etc are used for the buttons
	int reading = digitalRead(bpin);	// read the state of the switch into a local variable:
										// check to see if you just pressed the button (i.e. the input went from LOW to HIGH),  and you've waited long enough since the last press to ignore any noise:	
	if (reading != lastButtonState[buttonnum]) lastDebounceTime[buttonnum] = millis();	// If the switch changed, due to noise or pressing: reset the debouncing timer

	if ((millis() - lastDebounceTime[buttonnum]) > debounceDelay) {  // whatever the reading is at, it's been there for longer than the debounce delay, so take it as the actual current state:
		if (reading != buttonState[buttonnum]) { 				// if the button state has changed:
			buttonState[buttonnum] = reading;					// save button last state			
			lastButtonState[buttonnum] = reading;
			if (buttonState[buttonnum] == LOW) return 1;		// button pressed
			else return -1;							// button released
		}
	}
	lastButtonState[buttonnum] = reading;
	return 0;
}




}

}

void updateLED(cc_assignment_t *assignment) {
	// check if assignment mode is toggle
	// turn led on/off according the assignment value
	if (assignment->mode & CC_MODE_TOGGLE) {
		if (assignment->value > 0.0)
			digitalWrite(ledPin, HIGH);
		else
			digitalWrite(ledPin, LOW);
	}
}

//updates actuator value and calls the write function
void updateValues(void *ass) {
	cc_assignment_t* assignment = (cc_assignment_t*)ass;
	if (assignment->actuator_id < 5)
	{
		actuatorValues[assignment->actuator_id] = assignment->value;
		digitalWrite(11 - assignment->actuator_id, assignment->value);
	}
	//	  writeValues(assignment->actuator_id);
}

void writeValues(cc_assignment_t *assignment)
{

}

void SetupPins()
{
	// Encoders
	pinMode(Encoder1APin, INPUT);
	pinMode(Encoder1BPin, INPUT);

	// Inputs
	// Buttons
	pinMode(40, INPUT);
	pinMode(42, INPUT);
	pinMode(44, INPUT);
	pinMode(46, INPUT);
	pinMode(48, INPUT);

	// LEDs
	pinMode(8, OUTPUT);
	pinMode(9, OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(11, OUTPUT);
}




void clearlcd(void *ass)
{
	cc_assignment_t* assignment = (cc_assignment_t*)ass;
	//lcd.clear();
	//for (int i=0;i < assignment->label.size;i++)
	//{ 
	//	//actuatorNames[assignment->actuator_id][i] = (char)"";
	//}
}

//when assigned to a actuator, gets the needed information and calls the write names function
//void updateNames(cc_assignment_t *assignment) {
void updateNames(void *ass) {
	cc_assignment_t*  assignment = (cc_assignment_t*)ass;
	minValues[assignment->actuator_id] = assignment->min;
	maxValues[assignment->actuator_id] = assignment->max;
//	int len = assignment->label.size;
//	if (len >= characters) len = characters - 1;

//	testactname = (String)assignment->label.text;

//	for (int i = 0; i < assignment->label.size; i++) {
//	for (int i = 0; i < len; i++) {
//		actuatorNames[assignment->actuator_id][i] = assignment->label.text[i];
	}
//	writeNames(assignment->actuator_id, assignment->label.size, 0);
//	displayInfo();
//}
	}
}
