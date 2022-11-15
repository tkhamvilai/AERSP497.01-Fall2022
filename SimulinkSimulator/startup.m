% clear; clc;
% sldrtkernel -install

%% Initial Condition
Init.x = 0; % m
Init.y = 0; % m
Init.z = 0; % m
Init.u = 0; % m/s
Init.v = 0; % m/s
Init.w = 0; % m/s
Init.phi = 0; % rad
Init.theta = 0; % rad
Init.psi = 0; % rad
Init.p = 0; % rad/s
Init.q = 0; % rad/s
Init.r = 0; % rad/s

%% Vehicle Parameters
Vehicle.mass = 1.0; % kg
Vehicle.inertia = eye(3); % kgm^2
Vehicle.cg = [0,0,0]; % m
% pwm to forces and thrust
Vehicle.allocation_matrix = [ 0  0  0  0; % force-body-x
                              0  0  0  0; % force-body-y
                             -1 -1 -1 -1; % force-body-z
                             -1 -1  1  1; % rolling-moment
                              1  1 -1 -1; % pitching-moment
                              1 -1  1 -1];% yawing-moment  
Vehicle.arm_length = sqrt(2); % m

%% Sensor Parameters
IMU.location = [0,0,0];

%% Actuator Parameters
Motor.minPWM = 1000;
Motor.maxPWM = 2000;
Motor.maxThrust = 1; % N

%% Test
PWM = [2000;2000;2000;2000];
