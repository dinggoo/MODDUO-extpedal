#include <ControlChain.h>
//#include <Arduino.h>
#include <Bounce2.h>

ControlChain cc;

float valFSW1, valFSW2, valFSW3;

bool somethingChanged = true;
int switchFlag = 0;

Bounce debounceFSW1 = Bounce();
Bounce debounceFSW2 = Bounce();
Bounce debounceFSW3 = Bounce();

unsigned long debounceDelay = 1;    // the debounce time; increase if the output flickers

int FSW1 = 6, FSW2 = 5, FSW3 = 4;
int LED1 = 12, LED2 = 11, LED3 = 10;

void setup() {

  //#################  define pins and activate pullup-resistors   #####################
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);

  pinMode(FSW1, INPUT_PULLUP);
  pinMode(FSW2, INPUT_PULLUP);
  pinMode(FSW3, INPUT_PULLUP);

  //###############################  set switch debouncers   ##########################
  debounceFSW1.attach(FSW1);
  debounceFSW1.interval(debounceDelay);
  
  debounceFSW2.attach(FSW2);
  debounceFSW2.interval(debounceDelay);
  
  debounceFSW3.attach(FSW3);
  debounceFSW3.interval(debounceDelay);

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
    FSW1_config.max_assignments = 2;

  // create footswitch 2
    cc_actuator_config_t FSW2_config;
    FSW2_config.type = CC_ACTUATOR_MOMENTARY;
    FSW2_config.name = "FootSwitch2";
    FSW2_config.value = &valFSW2;
    FSW2_config.min = 0.0;
    FSW2_config.max = 1.0;
    FSW2_config.supported_modes = CC_MODE_TOGGLE | CC_MODE_TRIGGER;
    FSW2_config.max_assignments = 1;  

// create footswitch 3
    cc_actuator_config_t FSW3_config = FSW1_config;
    FSW3_config.name = "FootSwitch3";
    FSW3_config.value = &valFSW3;
   
 /*     
    // create footswitch 3
    cc_actuator_config_t FSW3_config;
    FSW3_config.type = CC_ACTUATOR_MOMENTARY;
    FSW3_config.name = "FootSwitch3";
    FSW3_config.value = &valFSW3;
    FSW3_config.min = 0.0;
    FSW3_config.max = 1.0;
    FSW3_config.supported_modes = CC_MODE_TOGGLE | CC_MODE_TRIGGER;
    FSW3_config.max_assignments = 1;
    
  
    // create footswitch 2
    cc_actuator_config_t FSW2_config = FSW1_config;
    FSW2_config.name = "FootSwitch2";
    FSW2_config.value = &valFSW2;

    // create footswitch 3
    cc_actuator_config_t FSW3_config = FSW1_config;
    FSW3_config.name = "FootSwitch3";
    FSW3_config.value = &valFSW3;


    // add switches to device
    cc_actuator_t* actuator_FSW1;
    cc_actuator_t* actuator_FSW2;
    cc_actuator_t* actuator_FSW3;
    actuator_FSW1 = cc.newActuator(&FSW1_config);
    actuator_FSW2 = cc.newActuator(&FSW2_config);
    actuator_FSW3 = cc.newActuator(&FSW3_config);
    cc.addActuator(device, actuator_FSW1);
    cc.addActuator(device, actuator_FSW2);
    cc.addActuator(device, actuator_FSW3);
*/

// add switches to device
    cc_actuator_t* actuator_FSW1;
    actuator_FSW1 = cc.newActuator(&FSW1_config);
    cc.addActuator(device, actuator_FSW1);
    
    cc_actuator_t* actuator_FSW2;
    actuator_FSW2 = cc.newActuator(&FSW2_config);
    cc.addActuator(device, actuator_FSW2);
    
    cc_actuator_t* actuator_FSW3;
    actuator_FSW3 = cc.newActuator(&FSW3_config);
    cc.addActuator(device, actuator_FSW3);


    
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
		switchFlag = 1;
		break;
	case 1:
		valFSW2 = assignment->value;
		toggleLED(LED2, assignment->value);
		switchFlag = 2;
		break;
	case 2:
		valFSW3 = assignment->value;
		toggleLED(LED3, assignment->value);
		switchFlag = 3;
    //break;
    default:
    break;
	/*
  case 3:
		encoderA.write(-(ENC_MIN + (assignment->value * ((ENC_MAX - ENC_MIN) / (assignment->max - assignment->min)))));
		break;
	case 4:
		encoderB.write(-(ENC_MIN + (assignment->value * ((ENC_MAX - ENC_MIN) / (assignment->max - assignment->min)))));
		break;
	default:
		break;
    */
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

  valFSW1 = (float) debounceFSW1.read();
  valFSW2 = (float) debounceFSW2.read();
  valFSW3 = (float) debounceFSW3.read();

  cc.run();
}
