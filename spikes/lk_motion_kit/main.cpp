// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "CoreAccelerometer.h"
#include "CoreGyroscope.h"
#include "CoreI2C.h"
#include "CoreLSM6DSOX.h"
#include "CoreMotor.h"
#include "CorePwm.h"
#include "CoreTimeout.h"
#include "EventLoopKit.h"
#include "HelloWorld.h"
#include "IMUKit.h"
#include "LogKit.h"
#include "MotionKit.h"

using namespace std::chrono;
using namespace leka;

namespace {

namespace motors {

	namespace left {

		namespace internal {

			auto dir_1 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_1};
			auto dir_2 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_2};
			auto speed = CorePwm {MOTOR_LEFT_PWM};

		}	// namespace internal

		auto motor = CoreMotor {internal::dir_1, internal::dir_2, internal::speed};

	}	// namespace left

	namespace right {

		namespace internal {

			auto dir_1 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_1};
			auto dir_2 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_2};
			auto speed = CorePwm {MOTOR_RIGHT_PWM};

		}	// namespace internal

		auto motor = CoreMotor {internal::dir_1, internal::dir_2, internal::speed};

	}	// namespace right

}	// namespace motors

namespace imu {

	namespace internal {

		CoreI2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
		EventLoopKit event_loop {};

	}	// namespace internal

	CoreLSM6DSOX lsm6dsox(internal::i2c);
	CoreAccelerometer accel(lsm6dsox);
	CoreGyroscope gyro(lsm6dsox);

	IMUKit kit(internal::event_loop, accel, gyro);

}	// namespace imu

namespace motion {

	namespace internal {

		EventLoopKit event_loop {};
		CoreTimeout timeout {};

	}	// namespace internal

	MotionKit kit(motors::left::motor, motors::right::motor, imu::kit, internal::event_loop, internal::timeout);

}	// namespace motion

}	// namespace

auto main() -> int
{
	logger::init();

	HelloWorld hello;
	hello.start();

	imu::lsm6dsox.init();
	imu::kit.init();
	motion::kit.init();

	while (true) {
		motion::kit.controlledSpin(3, Rotation::counterClockwise);
		rtos::ThisThread::sleep_for(1s);
		motion::kit.stop();
		rtos::ThisThread::sleep_for(10s);

		motion::kit.controlledSpin(3, Rotation::clockwise);
		rtos::ThisThread::sleep_for(10s);

		motion::kit.responsive(10s);

		rtos::ThisThread::sleep_for(15s);
	}
}
