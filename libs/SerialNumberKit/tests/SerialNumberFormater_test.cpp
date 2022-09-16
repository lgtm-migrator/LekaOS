// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstring>
#include <fstream>

#include "SerialNumberFormater.h"
#include "gtest/gtest.h"
#include "mocks/leka/MCU.h"

using namespace leka;

using ::testing::Return;

class SerialNumberFormaterTest : public ::testing::Test
{
  protected:
	SerialNumberFormaterTest() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	mock::MCU mock_mcu;
	SerialNumberFormater serial_number_formater {mock_mcu};

	MCUID mcu_id_returned {0x2A2B2C2D, 0x3A3B3C3D, 0x4A4B4C4D};
};

TEST_F(SerialNumberFormaterTest, initialization)
{
	EXPECT_NE(&serial_number_formater, nullptr);
}

TEST_F(SerialNumberFormaterTest, setPrefix)
{
	auto expected_serial_number = std::array<uint8_t, 32> {"LK-"};
	auto actual_serial_number	= std::array<uint8_t, 32> {};

	auto actual_prefix_number_size = serial_number_formater.setPrefix(actual_serial_number);
	EXPECT_EQ(actual_serial_number, expected_serial_number);
	EXPECT_EQ(actual_prefix_number_size, std::size("LK-") - sizeof('\0'));
}

TEST_F(SerialNumberFormaterTest, setDateOfTest)
{
	auto expected_serial_number = std::array<uint8_t, 32> {"LK-XXXX"};
	auto actual_serial_number	= std::array<uint8_t, 32> {"LK-"};

	auto prefix_size = std::size("LK-") - sizeof('\0');

	auto actual_prefix_number_size = serial_number_formater.setDateOfTest(actual_serial_number, prefix_size);
	EXPECT_EQ(actual_serial_number, expected_serial_number);
	EXPECT_EQ(actual_prefix_number_size, std::size("XXXX") - sizeof('\0'));
}

TEST_F(SerialNumberFormaterTest, setMCUID)
{
	auto expected_serial_number = std::array<uint8_t, 32> {};
	auto actual_serial_number	= std::array<uint8_t, 32> {};

	auto prefix_plus_date_of_test_size = std::size("LK-XXXX") - sizeof('\0');
	auto number_of_digits			   = 0;

	EXPECT_CALL(mock_mcu, getID).WillRepeatedly(Return(mcu_id_returned));

	actual_serial_number   = std::array<uint8_t, 32> {"LK-XXXX"};
	number_of_digits	   = 10;
	expected_serial_number = std::array<uint8_t, 32> {"LK-XXXX2A2B2C2D3A"};
	serial_number_formater.setMCUID(actual_serial_number, prefix_plus_date_of_test_size, number_of_digits);
	EXPECT_EQ(actual_serial_number, expected_serial_number);

	actual_serial_number   = std::array<uint8_t, 32> {"LK-XXXX"};
	number_of_digits	   = 19;
	expected_serial_number = std::array<uint8_t, 32> {"LK-XXXX2A2B2C2D3A3B3C3D4A4"};
	serial_number_formater.setMCUID(actual_serial_number, prefix_plus_date_of_test_size, number_of_digits);
	EXPECT_EQ(actual_serial_number, expected_serial_number);

	actual_serial_number   = std::array<uint8_t, 32> {"LK-XXXX"};
	number_of_digits	   = 24;
	expected_serial_number = std::array<uint8_t, 32> {"LK-XXXX2A2B2C2D3A3B3C3D4A4B4C4D"};
	serial_number_formater.setMCUID(actual_serial_number, prefix_plus_date_of_test_size, number_of_digits);
	EXPECT_EQ(actual_serial_number, expected_serial_number);
}

TEST_F(SerialNumberFormaterTest, setMCUIDExceedingNumberOfDigits)
{
	auto expected_serial_number = std::array<uint8_t, 32> {"LK-XXXX2A2B2C2D3A3B3C3D4A4B4C4D"};
	auto actual_serial_number	= std::array<uint8_t, 32> {"LK-XXXX"};

	auto prefix_plus_date_of_test_size = std::size("LK-XXXX") - sizeof('\0');

	EXPECT_CALL(mock_mcu, getID).WillRepeatedly(Return(mcu_id_returned));

	serial_number_formater.setMCUID(actual_serial_number, prefix_plus_date_of_test_size, 32);
	EXPECT_EQ(actual_serial_number, expected_serial_number);
}
