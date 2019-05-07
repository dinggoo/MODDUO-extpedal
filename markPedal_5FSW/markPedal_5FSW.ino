#include <ControlChain.h>
#include <Arduino.h>
#include <Bounce2.h>

ControlChain cc;

// store the footswitch values in floats
float valFSW1, valFSW2, valFSW3, valFSW4;

bool somethingChanged = true;
 //int switchFlag = 0;

Bounce debounceFSW1 = Bounce();
Bounce debounceFSW2 = Bounce();
Bounce debounceFSW3 = Bounce();
Bounce debounceFSW4 = Bounce();
unsigned long debounceDelay = 10;    // the debounce time; increase if the output flickers

// define pinNumbers for switches and leds
int FSW1 = 7, FSW2 = 6, FSW3 = 5, FSW4 = 4;
int LED1 = 13, LED2 = 12, LED3 = 11, LED4 = 10;

void setup() {

  //#################  define pins and activate pullup-resistors   #####################
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  
  pinMode(FSW1, INPUT_PULLUP);
  pinMode(FSW2, INPUT_PULLUP);
  pinMode(FSW3, INPUT_PULLUP);
  pinMode(FSW4, INPUT_PULLUP);


  //###############################  set switch debouncers   ##########################
  debounceFSW1.attach(FSW1);
  debounceFSW1.interval(debounceDelay);
  
  debounceFSW2.attach(FSW2);
  debounceFSW2.interval(debounceDelay);
  
  debounceFSW3.attach(FSW3);
  debounceFSW3.interval(debounceDelay);

  debounceFSW4.attach(FSW4);
  debounceFSW4.interval(debounceDelay);
  


  //############################### create ControlChain device  #########################
  cc.begin();
  const char *uri = "https://github.com/dinggoo/MODDUO-extpedal";
  cc_device_t *device = cc.newDevice("MarkPedal", uri);

    //#############################   create switches     ###############################
    // create footswitch 1
    cc_actuator_config_t FSW1_config;
    FSW1_config.type = CC_ACTUATOR_MOMENTARY;
    FSW1_config.name = "FootSwitch1";
    FSW1_config.value = &valFSW1;
    FSW1_config.min = 0.0;
    FSW1_config.max = 1.0;
    FSW1_config.supported_modes = CC_MODE_TOGGLE | CC_MODE_TRIGGER;
    FSW1_config.max_assignments = 1;

    // create footswitch 2
    cc_actuator_config_t FSW2_config = FSW1_config;
    FSW2_config.name = "FootSwitch2";
    FSW2_config.value = &valFSW2;

    // create footswitch 3
    cc_actuator_config_t FSW3_config = FSW1_config;
    FSW3_config.name = "FootSwitch3";
    FSW3_config.value = &valFSW3;

    // create footswitch 4
    cc_actuator_config_t FSW4_config = FSW1_config;
    FSW4_config.name = "FootSwitch4";
    FSW4_config.value = &valFSW4;


    // add switches to device
    cc_actuator_t* actuator_FSW1;
    cc_actuator_t* actuator_FSW2;
    cc_actuator_t* actuator_FSW3;
    cc_actuator_t* actuator_FSW4;

    
    actuator_FSW1 = cc.newActuator(&FSW1_config);
    actuator_FSW2 = cc.newActuator(&FSW2_config);
    actuator_FSW3 = cc.newActuator(&FSW3_config);
    actuator_FSW4 = cc.newActuator(&FSW4_config);

    
    cc.addActuator(device, actuator_FSW1);
    cc.addActuator(device, actuator_FSW2);
    cc.addActuator(device, actuator_FSW3);
    cc.addActuator(device, actuator_FSW4);


    //############################# activate callback messages ###########################

    cc.setEventCallback(CC_EV_UPDATE, (void(*)(void* arg)) assignment_update);
    cc.setEventCallback(CC_EV_ASSIGNMENT, (void(*)(void* arg)) assignment_add);
    //cc.setEventCallback(CC_EV_UNASSIGNMENT, (void(*)(void* arg)) assignment_remove);
}

void toggleLED(int chosenLED, float assignmentVal) {
  if (assignmentVal > 0.0)
    digitalWrite(chosenLED, HIGH);
  else
    digitalWrite(chosenLED, LOW);
}

void assignment_update(cc_assignment_t *assignment) {

	if (assignment->mode & CC_MODE_TOGGLE) {
		//toggle LED based on assignment update
		switch (assignment->actuator_id) {
		case 0:
			toggleLED(LED1, assignment->value);
			break;
		case 1:
			toggleLED(LED2, assignment->value);
			break;
		case 2:
			toggleLED(LED3, assignment->value);
      break;
    case 3:
      toggleLED(LED4, assignment->value);
		default:
			break;
		}
	}
 
	somethingChanged = true;
}

void assignment_add(cc_assignment_t *assignment) {

	// make sure everything starts at the saved position
	switch (assignment->actuator_id) {
	case 0:
		valFSW1 = assignment->value;
		toggleLED(LED1, assignment->value);
		//switchFlag = 1;
		break;
	case 1:
		valFSW2 = assignment->value;
		toggleLED(LED2, assignment->value);
		//switchFlag = 2;
		break;
	case 2:
		valFSW3 = assignment->value;
		toggleLED(LED3, assignment->value);
		//switchFlag = 3;
    break;
  case 3:
    valFSW4 = assignment->value;
    toggleLED(LED4, assignment->value);
    //switchFlag = 4;
    default:
    break;
	}

	somethingChanged = true;
}

void assignment_remove(cc_assignment_t *assignment) {

	somethingChanged = true;
}

void loop() {

  debounceFSW1.update();
  debounceFSW2.update();
  debounceFSW3.update();
  debounceFSW4.update();


  valFSW1 = (float) debounceFSW1.read();
  valFSW2 = (float) debounceFSW2.read();
  valFSW3 = (float) debounceFSW3.read();
  valFSW4 = (float) debounceFSW4.read();
  
  cc.run();
}
