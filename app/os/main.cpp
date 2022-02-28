// Leka - LekaOS
// Copyright 2020-2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"

#include "CoreBattery.h"
#include "CoreTimeout.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "RobotController.h"

using namespace leka;
using namespace std::chrono;

auto sleep_timeout = CoreTimeout {};

auto charge_input = mbed::InterruptIn {PinName::BATTERY_CHARGE_STATUS};
auto battery	  = leka::CoreBattery {PinName::BATTERY_VOLTAGE, charge_input};

auto rc = RobotController {sleep_timeout, battery};

auto main() -> int
{
	logger::init();

	rtos::ThisThread::sleep_for(1s);

	log_info("\n\n");
	log_info("Hello, LekaOS!\n");

	rtos::ThisThread::sleep_for(2s);

	auto hello = HelloWorld();
	hello.start();

	rc.initializeComponents();
	rc.registerEvents();

	while (true) {
		log_debug("A message from your board %s --> \"%s\" at %ims", MBED_CONF_APP_TARGET_NAME, hello.world,
				  int(rtos::Kernel::Clock::now().time_since_epoch().count()));
		rtos::ThisThread::sleep_for(1s);
	}
}
