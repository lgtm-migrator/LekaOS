// Leka - LekaOS
// Copyright 2020 Adafruit Industries (MIT License)
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? Heavily inspired by (taken from) Adafruit' AHRS library
// ? https://github.com/adafruit/Adafruit_AHRS

// ? Original paper by Robert Mahony
// ? https://ieeexplore.ieee.org/document/4608934

#pragma once

#include <array>
#include <tuple>

#include "MathUtils.h"

namespace leka::ahrs {

class Mahony
{
  public:
	Mahony() = default;

	void begin(float sampleFrequency) { _invSampleFreq = 1.0F / sampleFrequency; }
	void update(std::tuple<float, float, float>, std::tuple<float, float, float>, std::tuple<float, float, float>);

	auto getRoll() -> float;
	auto getPitch() -> float;
	auto getYaw() -> float;

	void setOrigin();

  private:
	void computeAngles();

	const float kTwoKp = float {2.0F * 2.F};

	float _q0 {1.0F};
	float _q1 {0.0F};
	float _q2 {0.0F};
	float _q3 {0.0F};
	float _invSampleFreq {};
	float _roll {};
	float _pitch {};
	float _yaw {};
	std::array<float, 3> gravity_vector {};
	bool anglesComputed = false;
};

}	// namespace leka::ahrs
