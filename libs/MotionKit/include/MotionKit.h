// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <interface/drivers/Motor.h>

#include "IMUKit.h"
#include "PID.h"
#include "interface/drivers/Timeout.h"

namespace leka {

class MotionKit
{
  public:
	MotionKit(interface::Motor &left_motor, interface::Motor &right_motor, IMUKit &imu_kit,
			  interface::EventLoop &event_loop, interface::Timeout &timeout)
		: _left_motor(left_motor),
		  _right_motor(right_motor),
		  _imu_kit(imu_kit),
		  _event_loop(event_loop),
		  _timeout(timeout) {};

	void init();

	void controlledSpin(uint8_t number_of_turn, Rotation direction);
	void responsive(std::chrono::seconds duration);

	void stop();

  private:
	void run();

	[[nodiscard]] auto mapSpeed(float speed) const -> float;
	void executeSpeed(float speed, Rotation direction);

	interface::Motor &_left_motor;
	interface::Motor &_right_motor;
	IMUKit &_imu_kit;
	interface::EventLoop &_event_loop;
	interface::Timeout &_timeout;
	PID _pid;

	uint8_t _turn_executed	 = 0;
	uint8_t _turn_to_execute = 0;

	bool _target_not_reached	   = true;
	bool _is_not_delay_over		   = true;
	bool _rotate_x_turns_requested = false;

	const float kReferenceAngle		   = 180.F;
	const float kMinimalViableRobotPwm = 0.25F;
	const float kPwmMax				   = 1.F;
	const float kPwmBound			   = 0.28F;
};

}	// namespace leka
