// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "airsim.h"
#include "navigation.h"
#include "controller.h"

int main()
{
  AirSim airsim; // contains sensor and actuator drivers
  Navigation nav;
  Controller ctrl;

  nav.init();
  airsim.arm();

  while ( true )
  {
    airsim.sensor_read();
    airsim.get_truth(); // only available in the sim

    nav.use_truth( airsim.true_states );
    ctrl.control_algorithm( nav.s, airsim.sens );

    airsim.set_pwm( ctrl.pwm_out );
    airsim.drive_motors();
  };

  airsim.disarm();

  return 0;
}
