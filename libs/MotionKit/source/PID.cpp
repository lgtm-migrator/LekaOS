// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PID.h"
#include <algorithm>

using namespace leka;

auto PID::processPID([[maybe_unused]] float pitch, [[maybe_unused]] float roll, float yaw)
	-> std::tuple<float, Rotation>
{
	Rotation direction {};
	_current_position_error = kTargetAngle - yaw;

	if (std::abs(_current_position_error) < kStaticBound) {
		_position_error_total += _current_position_error;
		_position_error_total = std::min(_position_error_total, 50.F / _Ki);
	} else {
		_position_error_total = 0.F;
	}
	if (std::abs(_current_position_error) < kStaticBound) {
		_derivative = 0.F;
	}

	_proportion = _current_position_error * _Kp;
	_integral	= _position_error_total * _Ki;
	_derivative = (_current_position_error - _last_position_error) * _Kd;

	_last_position_error = _current_position_error;

	auto speed = (_proportion + _integral + _derivative) / kDeltaT;

	if (speed < 0) {
		speed	  = -speed;
		direction = Rotation::counterClockwise;
	} else {
		direction = Rotation::clockwise;
	}

	return {speed, direction};
}
