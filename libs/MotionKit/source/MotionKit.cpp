// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "MotionKit.h"

#include "MathUtils.h"
#include "ThisThread.h"

using namespace leka;
using namespace std::chrono_literals;

void MotionKit::init()
{
	_event_loop.registerCallback([this] { run(); });
}

void MotionKit::stop()
{
	_left_motor.stop();
	_right_motor.stop();
	_is_not_delay_over		  = false;
	_target_not_reached		  = false;
	_rotate_x_turns_requested = false;
	_event_loop.stop();
}

void MotionKit::controlledSpin(uint8_t number_of_turn, Rotation direction)
{
	stop();

	_imu_kit.start();
	_imu_kit.reset();
	_target_not_reached		  = true;
	_is_not_delay_over		  = false;
	_rotate_x_turns_requested = true;

	_turn_to_execute = number_of_turn;
	_turn_executed	 = 0;

	_left_motor.spin(direction, kPwmMax);
	_right_motor.spin(direction, kPwmMax);

	_event_loop.start();
}

void MotionKit::responsive(std::chrono::seconds duration)
{
	stop();

	_imu_kit.start();
	_imu_kit.reset();
	_target_not_reached		  = true;
	_is_not_delay_over		  = true;
	_rotate_x_turns_requested = false;

	auto on_timeout = [this] { _is_not_delay_over = false; };
	_timeout.onTimeout(on_timeout);
	_timeout.start(duration);

	_event_loop.start();
}

// LCOV_EXCL_START - Dynamic behavior, involving motors and time.
void MotionKit::run()
{
	auto last_yaw = kReferenceAngle;
	while (_rotate_x_turns_requested) {
		auto [current_pitch, current_roll, current_yaw] = _imu_kit.getAngles();

		if (std::abs(last_yaw - current_yaw) >= 300.F) {
			++_turn_executed;
		}
		rtos::ThisThread::sleep_for(70ms);

		last_yaw = current_yaw;
		if (_turn_executed == _turn_to_execute) {
			_rotate_x_turns_requested = false;
			_turn_executed			  = 0;
			_turn_to_execute		  = 0;
		}
	}

	while (_is_not_delay_over || _target_not_reached) {
		auto [pitch, roll, yaw] = _imu_kit.getAngles();
		auto [speed, rotation]	= _pid.processPID(pitch, roll, yaw);

		executeSpeed(speed, rotation);

		rtos::ThisThread::sleep_for(70ms);
	}
	_imu_kit.stop();
}

auto MotionKit::mapSpeed(float speed) const -> float
{
	return utils::math::map(speed, 0.F, 1.F, kMinimalViableRobotPwm, kPwmMax);
}

void MotionKit::executeSpeed(float speed, Rotation direction)
{
	auto speed_bounded = mapSpeed(speed);
	if (speed_bounded <= kPwmBound) {
		_left_motor.stop();
		_right_motor.stop();
		_target_not_reached = false;
	} else {
		_left_motor.spin(direction, speed_bounded);
		_right_motor.spin(direction, speed_bounded);
		_target_not_reached = true;
	}
}
// LCOV_EXCL_STOP
