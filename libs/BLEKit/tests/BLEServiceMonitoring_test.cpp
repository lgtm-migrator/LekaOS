// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEServiceMonitoring.h"

#include "gtest/gtest.h"

using namespace leka;

TEST(BLEServiceMonitoringTest, initialisation)
{
	auto service_monitoring = BLEServiceMonitoring {};

	EXPECT_NE(&service_monitoring, nullptr);
}

TEST(BLEServiceMonitoringTest, setChargingStatus)
{
	auto service_monitoring = BLEServiceMonitoring {};

	uint8_t actual_charging_status {};

	auto spy_callback = [&actual_charging_status](const BLEServiceMonitoring::data_to_send_handle_t &handle) {
		actual_charging_status = std::get<1>(handle)[0];
	};
	service_monitoring.onDataReadyToSend(spy_callback);

	service_monitoring.setChargingStatus(true);
	EXPECT_TRUE(actual_charging_status);

	service_monitoring.setChargingStatus(false);
	EXPECT_FALSE(actual_charging_status);
}

TEST(BLEServiceMonitoringTest, onDataReceived)
{
	auto service_monitoring = BLEServiceMonitoring {};

	auto dummy_params = BLEServiceMonitoring::data_received_handle_t {};
	service_monitoring.onDataReceived(dummy_params);

	// nothing expected
}