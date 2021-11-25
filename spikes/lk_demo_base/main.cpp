// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "events/EventQueue.h"
#include "rtos/EventFlags.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "BLEUtils.h"

#include "BatteryUtils.h"
#include "DisplayUtils.h"
#include "Flags.h"
#include "HelloWorld.h"
#include "LedsUtils.h"
#include "LogKit.h"
#include "MotorsUtils.h"
#include "RFIDUtils.h"
#include "WatchdogUtils.h"

using namespace leka;
using namespace std::chrono_literals;

auto thread_watchdog		  = rtos::Thread {osPriorityNormal};
auto thread_event_queue		  = rtos::Thread {osPriorityNormal};
auto thread_ble				  = rtos::Thread {osPriorityNormal};
auto thread_ble_notifications = rtos::Thread {osPriorityNormal};
auto thread_video			  = rtos::Thread {osPriorityNormal};

auto event_queue					  = events::EventQueue {};
auto event_flags_external_interaction = rtos::EventFlags {};

auto hello = HelloWorld {};

auto battery_utils = BatteryUtils {};

auto leds_utils = LedsUtils {};

auto motor_left_dir_1  = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_1};
auto motor_left_dir_2  = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_2};
auto motor_left_speed  = CorePwm {MOTOR_LEFT_PWM};
auto motor_right_dir_1 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_1};
auto motor_right_dir_2 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_2};
auto motor_right_speed = CorePwm {MOTOR_RIGHT_PWM};
auto motor_left		   = CoreMotor {motor_left_dir_1, motor_left_dir_2, motor_left_speed};
auto motor_right	   = CoreMotor {motor_right_dir_1, motor_right_dir_2, motor_right_speed};
auto motors_utils	   = MotorsUtils {motor_left, motor_right, motor_left_speed, motor_right_speed};

auto ble_utils = BLEUtils {event_flags_external_interaction};

auto hal	   = LKCoreSTM32Hal {};
auto coresdram = CoreSDRAM {hal};
auto display   = VideoKit {hal};
VideoKit_DeclareIRQHandlers(display);
auto display_utils = DisplayUtils {thread_video, event_flags_external_interaction, hal, coresdram, display};

auto rfid_utils = RFIDUtils {event_flags_external_interaction};

void useLeds()
{
	leds_utils.setBrightness(0x08);
	leds_utils.runReinforcer(LedsReinforcer::rainbow);
	leds_utils.turnOff(LedsRange::all);
}

void useDisplay()
{
	display_utils.setOn();

	display_utils.displayImage("leka-logo");
	rtos::ThisThread::sleep_for(5s);
	display_utils.displayVideo("animation-idle");
	rtos::ThisThread::sleep_for(10s);
	display_utils.setOff();
}

void useRFID()
{
	event_flags_external_interaction.wait_any(NEW_RFID_TAG_FLAG);
	log_info("Data: %x", rfid_utils.getTag());
}

void updateBLENotifications()
{
	auto ping_flag_is_set = [&]() { return (event_flags_external_interaction.get() & BLE_PING_FLAG) == BLE_PING_FLAG; };
	auto reboot_instruction_flag_is_set = [&]() {
		return (event_flags_external_interaction.get() & BLE_REBOOT_INSTRUCTION_FLAG) == BLE_REBOOT_INSTRUCTION_FLAG;
	};
	auto leds_intensity_flag_is_set = [&]() {
		return (event_flags_external_interaction.get() & BLE_LEDS_INTENSITY_FLAG) == BLE_LEDS_INTENSITY_FLAG;
	};
	auto lcd_intensity_flag_is_set = [&]() {
		return (event_flags_external_interaction.get() & BLE_LCD_INTENSITY_FLAG) == BLE_LCD_INTENSITY_FLAG;
	};

	auto update_battery_level = [&]() { ble_utils.setBatteryLevel(battery_utils.getBatteryLevel()); };
	event_queue.call_every(1s, update_battery_level);

	while (true) {
		event_flags_external_interaction.clear(BLE_PING_FLAG | BLE_REBOOT_INSTRUCTION_FLAG | BLE_LEDS_INTENSITY_FLAG |
											   BLE_LCD_INTENSITY_FLAG);
		event_flags_external_interaction.wait_any(NEW_BLE_MESSAGE_FLAG);

		if (ping_flag_is_set()) {
			for (int i = 0; i < 3; i++) {
				leds_utils.turnOn(LedsRange::belt, CRGB::RoyalBlue);
				rtos::ThisThread::sleep_for(400ms);
				leds_utils.turnOff(LedsRange::all);
				rtos::ThisThread::sleep_for(400ms);
			}
		}
		if (reboot_instruction_flag_is_set()) {
			if (auto has_to_reboot = ble_utils.getRebootInstruction(); has_to_reboot) {
				event_flags_external_interaction.set(STOP_VIDEO_FLAG);
				leds_utils.turnOff(LedsRange::all);
				display_utils.setOff();

				rtos::ThisThread::sleep_for(500ms);

				NVIC_SystemReset();
			}
		}
		if (leds_intensity_flag_is_set()) {
			auto new_value = ble_utils.getLedsIntensity();
			leds_utils.setBrightness(new_value);
		}
		if (lcd_intensity_flag_is_set()) {
			auto new_value = (float(ble_utils.getLCDIntensity()) / 255.F);
			display_utils.setBrightness(new_value);
		}
	}
}

auto main() -> int
{
	startWatchdog(thread_watchdog);

	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	auto start = rtos::Kernel::Clock::now();

	thread_event_queue.start({&event_queue, &events::EventQueue::dispatch_forever});

	log_info("Hello, World!\n\n");
	hello.start();

	leds_utils.initialize();
	motors_utils.stop();

	battery_utils.registerEventQueue(event_queue);

	motors_utils.setSpeed(1.0F, 1.0F);

	ble_utils.setDeviceName("LekaDemo");
	thread_ble.start({&ble_utils, &BLEUtils::startAdvertising});
	thread_ble_notifications.start(updateBLENotifications);

	display_utils.initializeSD();
	display_utils.initializeScreen();

	rfid_utils.initialize();
	rfid_utils.registerEventQueue(event_queue);

	leds_utils.initializationAnimation();

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));

		rtos::ThisThread::sleep_for(1s);

		useLeds();
		useDisplay();
		useRFID();
	}
}
