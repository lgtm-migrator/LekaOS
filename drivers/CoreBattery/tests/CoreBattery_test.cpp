// Leka - LekaOS
// Copyright 2021 APF rance handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreBattery.h"

#include "gtest/gtest.h"
#include "mocks/mbed/DigitalIn.h"
#include "stubs/mbed/AnalogIn.h"

using namespace leka;

using ::testing::Return;

class CoreBatteryTest : public ::testing::Test
{
  protected:
	CoreBatteryTest() : battery(PinName::BATTERY_VOLTAGE, charge_input) {}

	// void SetUp() override {}
	// void TearDown() override {}

	mbed::mock::DigitalIn charge_input = {};

	CoreBattery battery;
};

TEST_F(CoreBatteryTest, initialization)
{
	ASSERT_NE(&battery, nullptr);
}

TEST_F(CoreBatteryTest, voltageAboveFull)
{
	auto AnalogIn_voltage_value	  = float {2.208};
	auto expected_battery_voltage = float {12.6};
	spy_AnalogIn_setVoltageValue(AnalogIn_voltage_value);

	ASSERT_NEAR(battery.voltage(), expected_battery_voltage, 0.1);
	ASSERT_GT(battery.voltage(), CoreBattery::Capacity::full);
}

TEST_F(CoreBatteryTest, voltageBetweenThreeQuarterAndFull)
{
	auto AnalogIn_voltage_value	  = float {2.166};
	auto expected_battery_voltage = float {11.8};
	spy_AnalogIn_setVoltageValue(AnalogIn_voltage_value);

	ASSERT_NEAR(battery.voltage(), expected_battery_voltage, 0.1);
	ASSERT_LT(battery.voltage(), CoreBattery::Capacity::full);
	ASSERT_GT(battery.voltage(), CoreBattery::Capacity::three_quarter);
}

TEST_F(CoreBatteryTest, voltageBetweenHalfAndThreeQuarter)
{
	auto AnalogIn_voltage_value	  = float {2.144};
	auto expected_battery_voltage = float {11.4};
	spy_AnalogIn_setVoltageValue(AnalogIn_voltage_value);

	ASSERT_NEAR(battery.voltage(), expected_battery_voltage, 0.1);
	ASSERT_LT(battery.voltage(), CoreBattery::Capacity::three_quarter);
	ASSERT_GT(battery.voltage(), CoreBattery::Capacity::half);
}

TEST_F(CoreBatteryTest, voltageBetweenQuarterAndHalf)
{
	auto AnalogIn_voltage_value	  = float {2.108};
	auto expected_battery_voltage = float {10.8};
	spy_AnalogIn_setVoltageValue(AnalogIn_voltage_value);

	ASSERT_NEAR(battery.voltage(), expected_battery_voltage, 0.1);
	ASSERT_LT(battery.voltage(), CoreBattery::Capacity::half);
	ASSERT_GT(battery.voltage(), CoreBattery::Capacity::quarter);
}

TEST_F(CoreBatteryTest, voltageBetweenEmptyAndQuarter)
{
	auto AnalogIn_voltage_value	  = float {2.033};
	auto expected_battery_voltage = float {9.7};
	spy_AnalogIn_setVoltageValue(AnalogIn_voltage_value);

	ASSERT_NEAR(battery.voltage(), expected_battery_voltage, 0.1);
	ASSERT_LT(battery.voltage(), CoreBattery::Capacity::quarter);
	ASSERT_GT(battery.voltage(), CoreBattery::Capacity::empty);
}

TEST_F(CoreBatteryTest, voltageBelowEmpty)
{
	auto AnalogIn_voltage_value	  = float {1.82};
	auto expected_battery_voltage = float {7.5};
	spy_AnalogIn_setVoltageValue(AnalogIn_voltage_value);

	ASSERT_NEAR(battery.voltage(), expected_battery_voltage, 0.1);
	ASSERT_LT(battery.voltage(), CoreBattery::Capacity::empty);
}

TEST_F(CoreBatteryTest, isCharging)
{
	EXPECT_CALL(charge_input, read).WillOnce(Return(1));

	ASSERT_TRUE(battery.isCharging());
}

TEST_F(CoreBatteryTest, isNotCharging)
{
	EXPECT_CALL(charge_input, read).WillOnce(Return(0));

	ASSERT_FALSE(battery.isCharging());
}
