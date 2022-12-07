// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "MotionKit.h"

#include "IMUKit.h"
#include "gtest/gtest.h"
#include "mocks/leka/Accelerometer.h"
#include "mocks/leka/CoreMotor.h"
#include "mocks/leka/Gyroscope.h"
#include "mocks/leka/Timeout.h"
#include "stubs/leka/EventLoopKit.h"

using namespace leka;

using ::testing::MockFunction;

class MotionKitTest : public ::testing::Test
{
  protected:
	MotionKitTest() = default;

	void SetUp() override
	{
		imukit.init();
		motion.init();
	}
	// void TearDown() override {}

	stub::EventLoopKit stub_event_loop_imu {};
	stub::EventLoopKit stub_event_loop_motion {};
	mock::Timeout timeout {};

	mock::CoreMotor mock_motor_left {};
	mock::CoreMotor mock_motor_right {};

	mock::Accelerometer accel {};
	mock::Gyroscope gyro {};

	std::tuple<float, float, float> accel_data = {0.F, 0.F, 0.F};
	std::tuple<float, float, float> gyro_data  = {0.F, 0.F, 0.F};

	IMUKit imukit {stub_event_loop_imu, accel, gyro};

	MotionKit motion {mock_motor_left, mock_motor_right, imukit, stub_event_loop_motion, timeout};
};

TEST_F(MotionKitTest, initialization)
{
	ASSERT_NE(&motion, nullptr);
}

TEST_F(MotionKitTest, registerMockCallbackAndControlledSpin)
{
	auto mock_function_imu = MockFunction<void(void)> {};
	auto loop_imu		   = [&] { mock_function_imu.Call(); };

	auto mock_function_motion = MockFunction<void(void)> {};
	auto loop_motion		  = [&] { mock_function_motion.Call(); };

	EXPECT_CALL(mock_function_imu, Call()).Times(1);
	EXPECT_CALL(mock_function_motion, Call()).Times(1);

	EXPECT_CALL(mock_motor_left, stop).Times(1);
	EXPECT_CALL(mock_motor_right, stop).Times(1);

	EXPECT_CALL(mock_motor_left, spin(Rotation::clockwise, 1)).Times(1);
	EXPECT_CALL(mock_motor_right, spin(Rotation::clockwise, 1)).Times(1);

	stub_event_loop_imu.registerCallback(loop_imu);
	stub_event_loop_motion.registerCallback(loop_motion);
	motion.controlledSpin(1, Rotation::clockwise);
}

TEST_F(MotionKitTest, registerMockCallbackAndResponsive)
{
	auto mock_function_imu = MockFunction<void(void)> {};
	auto loop_imu		   = [&] { mock_function_imu.Call(); };

	auto mock_function_motion = MockFunction<void(void)> {};
	auto loop_motion		  = [&] { mock_function_motion.Call(); };

	EXPECT_CALL(mock_function_imu, Call()).Times(1);
	EXPECT_CALL(mock_function_motion, Call()).Times(1);

	EXPECT_CALL(mock_motor_left, stop).Times(1);
	EXPECT_CALL(mock_motor_right, stop).Times(1);

	EXPECT_CALL(timeout, onTimeout).Times(1);
	EXPECT_CALL(timeout, start).Times(1);

	stub_event_loop_imu.registerCallback(loop_imu);
	stub_event_loop_motion.registerCallback(loop_motion);
	motion.responsive(std::chrono::seconds {10});
}

TEST_F(MotionKitTest, controlledSpinAndstop)
{
	auto mock_function_imu = MockFunction<void(void)> {};
	auto loop_imu		   = [&] { mock_function_imu.Call(); };

	auto mock_function_motion = MockFunction<void(void)> {};
	auto loop_motion		  = [&] {
		 mock_function_motion.Call();
		 motion.stop();
	};

	stub_event_loop_motion.spy_setNumberOfLoops(2);
	EXPECT_CALL(mock_function_motion, Call()).Times(1);

	stub_event_loop_imu.registerCallback(loop_imu);
	stub_event_loop_motion.registerCallback(loop_motion);
	motion.controlledSpin(1, Rotation::clockwise);
}

TEST_F(MotionKitTest, ResponsiveAndstop)
{
	auto mock_function_imu = MockFunction<void(void)> {};
	auto loop_imu		   = [&] { mock_function_imu.Call(); };

	auto mock_function_motion = MockFunction<void(void)> {};
	auto loop_motion		  = [&] {
		 mock_function_motion.Call();
		 motion.stop();
	};

	stub_event_loop_motion.spy_setNumberOfLoops(2);
	EXPECT_CALL(mock_function_motion, Call()).Times(1);

	stub_event_loop_imu.registerCallback(loop_imu);
	stub_event_loop_motion.registerCallback(loop_motion);
	motion.responsive(std::chrono::seconds {10});
}
