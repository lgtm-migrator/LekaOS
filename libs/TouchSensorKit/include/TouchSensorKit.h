// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "Position.h"
#include "interface/drivers/TouchSensor.h"
#include "interface/libs/EventLoop.h"

namespace leka {

class TouchSensorKit
{
  public:
	static constexpr auto kNumberOfSensors = uint8_t {6};

	explicit TouchSensorKit(interface::EventLoop &event_loop, interface::TouchSensor &ear_left,
							interface::TouchSensor &ear_right, interface::TouchSensor &belt_left_back,
							interface::TouchSensor &belt_left_front, interface::TouchSensor &belt_right_back,
							interface::TouchSensor &belt_right_front)
		: _event_loop(event_loop),
		  _ear_left(ear_left),
		  _ear_right(ear_right),
		  _belt_left_back(belt_left_back),
		  _belt_left_front(belt_left_front),
		  _belt_right_back(belt_right_back),
		  _belt_right_front(belt_right_front) {};

	void init();
	void start();
	void stop();
	auto isRunning() -> bool;

	void registerOnStartRead(std::function<void()> const &on_start_read_callback);
	void registerOnSensorTouched(std::function<void(const Position)> const &on_sensor_touched_callback);
	void registerOnSensorReleased(std::function<void(const Position)> const &on_sensor_released_callback);
	void registerOnEndRead(std::function<void()> const &on_end_read_callback);

  private:
	void run();

	auto readAtPosition(Position position) -> bool;
	void resetAtPosition(Position position);
	void setSensitivityAtPosition(Position position, uint16_t value);

	interface::EventLoop &_event_loop;
	bool _running = false;

	interface::TouchSensor &_ear_left;
	interface::TouchSensor &_ear_right;
	interface::TouchSensor &_belt_left_back;
	interface::TouchSensor &_belt_left_front;
	interface::TouchSensor &_belt_right_back;
	interface::TouchSensor &_belt_right_front;

	static constexpr auto default_max_sensitivity_value = float {1.F};
	static constexpr auto default_min_sensitivity_value = float {0.6F};

	std::function<void()> _on_start_read_callback {};
	std::function<void(const Position)> _on_sensor_touched_callback {};
	std::function<void(const Position)> _on_sensor_released_callback {};
	std::function<void()> _on_end_read_callback {};
};
}	// namespace leka
