// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "TouchSensorKit.h"

#include "gtest/gtest.h"
#include "mocks/leka/CoreTouchSensor.h"
#include "stubs/leka/EventLoopKit.h"

using namespace leka;

class TouchSensorTest : public ::testing::Test
{
  protected:
	TouchSensorTest() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	mock::CoreTouchSensor mock_ear_left;
	mock::CoreTouchSensor mock_ear_right;
	mock::CoreTouchSensor mock_belt_left_front;
	mock::CoreTouchSensor mock_belt_left_back;
	mock::CoreTouchSensor mock_belt_right_front;
	mock::CoreTouchSensor mock_belt_right_back;

	stub::EventLoopKit stub_event_loop {};

	TouchSensorKit touch_sensor_kit {stub_event_loop,	   mock_ear_left,		mock_ear_right,
									 mock_belt_left_front, mock_belt_left_back, mock_belt_right_front,
									 mock_belt_right_back};

	static void spy_onStartRead_func() {};
	std::function<void()> OnStartReadCallback = spy_onStartRead_func;
	static void spy_onSensorTouched_func(Position position) {};
	std::function<void(Position)> OnSensorTouchedCallback = spy_onSensorTouched_func;
	static void spy_onSensorReleased_func(Position position) {};
	std::function<void(Position)> OnSensorReleasedCallback = spy_onSensorReleased_func;
	static void spy_onEndRead_func() {};
	std::function<void()> OnEndReadCallback = spy_onEndRead_func;

	static void func(Position position) {};
	std::function<void(Position)> callbackFunction = func;
};

TEST_F(TouchSensorTest, initializationDefault)
{
	ASSERT_NE(&touch_sensor_kit, nullptr);
}

TEST_F(TouchSensorTest, init)
{
	EXPECT_CALL(mock_ear_left, init).Times(1);
	EXPECT_CALL(mock_ear_right, init).Times(1);
	EXPECT_CALL(mock_belt_left_front, init).Times(1);
	EXPECT_CALL(mock_belt_left_back, init).Times(1);
	EXPECT_CALL(mock_belt_right_front, init).Times(1);
	EXPECT_CALL(mock_belt_right_back, init).Times(1);

	EXPECT_CALL(mock_ear_left, setSensitivity).Times(1);
	EXPECT_CALL(mock_ear_right, setSensitivity).Times(1);
	EXPECT_CALL(mock_belt_left_front, setSensitivity).Times(1);
	EXPECT_CALL(mock_belt_left_back, setSensitivity).Times(1);
	EXPECT_CALL(mock_belt_right_front, setSensitivity).Times(1);
	EXPECT_CALL(mock_belt_right_back, setSensitivity).Times(1);

	touch_sensor_kit.init();
}

TEST_F(TouchSensorTest, start)
{
	EXPECT_FALSE(touch_sensor_kit.isRunning());
	touch_sensor_kit.start();
	EXPECT_TRUE(touch_sensor_kit.isRunning());
}

TEST_F(TouchSensorTest, run)
{
	touch_sensor_kit.run();
}

TEST_F(TouchSensorTest, stop)
{
	touch_sensor_kit.stop();
}

TEST_F(TouchSensorTest, registerOnStartRead)
{
	touch_sensor_kit.registerOnStartRead(OnStartReadCallback);
}

TEST_F(TouchSensorTest, registerOnSensorTouched)
{
	touch_sensor_kit.registerOnSensorTouched(OnSensorTouchedCallback);
}

TEST_F(TouchSensorTest, registerOnSensorReleased)
{
	touch_sensor_kit.registerOnSensorReleased(OnSensorReleasedCallback);
}

TEST_F(TouchSensorTest, registerOnEndRead)
{
	touch_sensor_kit.registerOnEndRead(OnEndReadCallback);
}
