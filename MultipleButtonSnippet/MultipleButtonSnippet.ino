cc_actuator_t *getActuatorConfig(char* name, float* value){

    cc_actuator_config_t actuator_config;
    actuator_config.type = CC_ACTUATOR_MOMENTARY;
    actuator_config.name = name;

    actuator_config.min = 0.0;
    actuator_config.max = 1.0;
    actuator_config.value = value;
    actuator_config.supported_modes = CC_MODE_TOGGLE | CC_MODE_TRIGGER;
    actuator_config.max_assignments = 1;

    cc_actuator_t *actuator;
    actuator = cc.newActuator(&actuator_config);
    return actuator;
}

float switchState[] = {0,0,0,0,0,0};

void setup() {
    ...
    ...

    cc_device_t *device = cc.newDevice("DeviceName", uri);

    cc.addActuator(device, getActuatorConfig("Button-0", &switchState[0])); 
    cc.addActuator(device, getActuatorConfig("Button-1", &switchState[1])); 
    cc.addActuator(device, getActuatorConfig("Button-2", &switchState[2])); 
    cc.addActuator(device, getActuatorConfig("Button-3", &switchState[3])); 
    cc.addActuator(device, getActuatorConfig("Button-4", &switchState[4])); 
    cc.addActuator(device, getActuatorConfig("Button-5", &switchState[5])); 
 }
