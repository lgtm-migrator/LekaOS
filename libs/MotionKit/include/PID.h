// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <tuple>

#include "interface/drivers/Motor.h"

namespace leka {

class PID
{
  public:
	PID() = default;

	auto processPID([[maybe_unused]] float pitch, [[maybe_unused]] float roll, float yaw)
		-> std::tuple<float, Rotation>;

  private:
	const float _Kp			 = 0.3F;
	const float _Ki			 = 0.0001F;
	const float _Kd			 = 0.4F;
	const float kStaticBound = 5.F;
	const float kDeltaT		 = 70.F;
	const float kTargetAngle = 180.F;

	float _position_error_total	  = 0.F;
	float _current_position_error = 0.F;
	float _last_position_error	  = 0.F;
	float _proportion			  = 0.F;
	float _integral				  = 0.F;
	float _derivative			  = 0.F;
};

}	// namespace leka
