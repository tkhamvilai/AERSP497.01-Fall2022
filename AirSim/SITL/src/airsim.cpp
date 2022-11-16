#include "airsim.h"

AirSim::AirSim()
{
	this->client.confirmConnection();
	this->client.enableApiControl( true );
	this->disarm();

	zero_t zero = { 0.0,0.0,0.0 };
	this->client.setAngleLevelControllerGains( zero, zero, zero );
	this->client.setAngleRateControllerGains( zero, zero, zero );
	this->client.setVelocityControllerGains( zero, zero, zero );
	this->client.setPositionControllerGains( zero, zero, zero );
}

AirSim::~AirSim()
{
}

void AirSim::arm()
{
	this->client.armDisarm( true );
}

void AirSim::disarm()
{
	this->client.armDisarm( false );
}

void AirSim::sensor_read()
{
	auto barometer_data = this->client.getBarometerData();
	auto imu_data = this->client.getImuData();
	auto magnetometer_data = this->client.getMagnetometerData();

	this->sens.baro_pressure = barometer_data.pressure;
	this->sens.gyr[0] = imu_data.angular_velocity.x();
	this->sens.gyr[1] = imu_data.angular_velocity.y();
	this->sens.gyr[2] = imu_data.angular_velocity.z();
	this->sens.acc[0] = imu_data.linear_acceleration.x();
	this->sens.acc[1] = imu_data.linear_acceleration.y();
	this->sens.acc[2] = imu_data.linear_acceleration.z();
	this->sens.mag[0] = magnetometer_data.magnetic_field_body.x();
	this->sens.mag[1] = magnetometer_data.magnetic_field_body.y();
	this->sens.mag[2] = magnetometer_data.magnetic_field_body.z();
}

void AirSim::get_truth()
{
	auto truth = this->client.simGetGroundTruthKinematics();
	this->true_states.position = truth.pose.position;
	this->true_states.orientation(0) = truth.pose.orientation.w();
	this->true_states.orientation(1) = truth.pose.orientation.x();
	this->true_states.orientation(2) = truth.pose.orientation.y();
	this->true_states.orientation(3) = truth.pose.orientation.z();
	this->true_states.linear_velocity = truth.twist.linear;
	this->true_states.angular_velocity = truth.twist.angular;
	this->true_states.linear_acceleration = truth.accelerations.linear;
	this->true_states.angular_acceleration = truth.accelerations.angular;
}

void AirSim::drive_motors()
{
	// motor mapping
	auto front_right = this->motor_cmd[0];
	auto front_left  = this->motor_cmd[1];
	auto rear_left   = this->motor_cmd[2];
	auto rear_right  = this->motor_cmd[3];

	this->client.moveByMotorPWMsAsync( front_right, rear_left, front_left, rear_right, 10.0 );
}
