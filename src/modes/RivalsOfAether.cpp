#include "modes/RivalsOfAether.hpp"

#define ANALOG_STICK_MIN 1
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

RivalsOfAether::RivalsOfAether(socd::SocdType socd_type) {
    _socd_pair_count = 5;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{ &InputState::left,   &InputState::right,   socd_type},
        socd::SocdPair{ &InputState::down,   &InputState::up,      socd_type},
        socd::SocdPair{ &InputState::down,   &InputState::up2,     socd_type},
        socd::SocdPair{ &InputState::c_left, &InputState::c_right, socd_type},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up,    socd_type},
    };
}

void RivalsOfAether::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.a;
    outputs.b = inputs.b;
    outputs.x = inputs.x;
    outputs.y = inputs.y;
    outputs.buttonL = inputs.lightshield;
    outputs.buttonR = inputs.z;
    outputs.triggerLDigital = inputs.l;
    outputs.triggerRDigital = inputs.r;
    outputs.start = inputs.start;
    outputs.select = inputs.select;
    outputs.home = inputs.home;

    // Activate D-Pad layer by holding Mod X + Mod Y.
    if (inputs.mod_x && inputs.mod_y) {
        outputs.dpadUp = inputs.c_up;
        outputs.dpadDown = inputs.c_down;
        outputs.dpadLeft = inputs.c_left;
        outputs.dpadRight = inputs.c_right;
    }

    // Activate select by holding Mod X
    if ((inputs.mod_x)) outputs.select = inputs.start;

    // Activate home by holding Mod Y
    if ((inputs.mod_y)) outputs.home = inputs.start;

    // Don't override dpad up if it's already pressed using the MX + MY dpad
    // layer.
    outputs.dpadUp = outputs.dpadUp || inputs.midshield;
}

void RivalsOfAether::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
    // Coordinate calculations to make modifier handling simpler.
    UpdateDirections(
        inputs.left,
        inputs.right,
        inputs.down,
        inputs.up || inputs.up2,
        inputs.c_left,
        inputs.c_right,
        inputs.c_down,
        inputs.c_up,
        ANALOG_STICK_MIN,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_MAX,
        outputs
    );

    bool shield_button_pressed = inputs.l || inputs.r;

    if (inputs.mod_x) {
        if (directions.horizontal) {
            outputs.leftStickX = 128 + (directions.x * 76);
            // MX Horizontal Tilts
            if (inputs.a) {
                outputs.leftStickX = 128 + (directions.x * 44);
            }
        }

        if(directions.vertical) {
            outputs.leftStickY = 128 + (directions.y * 73);
        }

        if (directions.diagonal) {
            outputs.leftStickX = 128 + (directions.x * 70);
            outputs.leftStickY = 128 + (directions.y * 34);

            // Angled F-tilts
            if (inputs.a) {
                outputs.leftStickX = 128 + (directions.x * 81);
                outputs.leftStickY = 128 + (directions.y * 46);
            }

            /* Extra DI, Air Dodge, and Up B angles */

            if (inputs.b) {
                outputs.leftStickX = 128 + (directions.x * 85);
                outputs.leftStickY = 128 + (directions.y * 31);
            }

            if (inputs.c_down) {
                outputs.leftStickX = 128 + (directions.x * 75);
                outputs.leftStickY = 128 + (directions.y * 33);
            }

            if (inputs.c_left) {
                outputs.leftStickX = 128 + (directions.x * 109);
                outputs.leftStickY = 128 + (directions.y * 65);
            }
      
            if (inputs.c_up) {
                outputs.leftStickX = 128 + (directions.x * 100);
                outputs.leftStickY = 128 + (directions.y * 72);
            }
     
            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 94);
                outputs.leftStickY = 128 + (directions.y * 79);
            }
        }
    }

    if (inputs.mod_y) {
        if (directions.horizontal) {
            outputs.leftStickX = 128 + (directions.x * 41);
        }

        if(directions.vertical) {
            outputs.leftStickY = 128 + (directions.y * 78);
        }

        if (directions.diagonal) {
            outputs.leftStickX = 128 + (directions.x * 41);
            outputs.leftStickY = 128 + (directions.y * 76);

            /* Extra DI, Air Dodge, and Up B angles */

            if (inputs.b) {
                outputs.leftStickX = 128 + (directions.x * 28);
                outputs.leftStickY = 128 + (directions.y * 85);
            }

            // Angles just for DI and Up B
            if (inputs.c_down) {
                outputs.leftStickX = 128 + (directions.x * 44);
                outputs.leftStickY = 128 + (directions.y * 106);
            }

            if (inputs.c_left) {
                outputs.leftStickX = 128 + (directions.x * 52);
                outputs.leftStickY = 128 + (directions.y * 109);
            }
      
            if (inputs.c_up) {
                outputs.leftStickX = 128 + (directions.x * 72);
                outputs.leftStickY = 128 + (directions.y * 100);
            }
     
            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 81);
                outputs.leftStickY = 128 + (directions.y * 94);
            }
        }
    }

    // Shut off C-stick when using D-Pad layer.
    if (inputs.mod_x && inputs.mod_y) {
        outputs.rightStickX = 128;
        outputs.rightStickY = 128;
    }

    // Turns off Start when holding Mod X or Mod Y
    if ((inputs.mod_x || inputs.mod_y)) {
        outputs.start = false;
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}
