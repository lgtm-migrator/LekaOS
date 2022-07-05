// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <interface/drivers/MCU.h>

namespace leka {

class SerialNumberKit
{
	static constexpr size_t SN_SIZE		  = 33;
	static constexpr size_t SHORT_SN_SIZE = 16 + 1;

  public:
	explicit SerialNumberKit(interface::MCU &mcu) : _mcu(mcu) {}

	auto getSerialNumber() -> std::array<uint8_t, SN_SIZE> &;
	auto getShortSerialNumber() -> std::array<uint8_t, SHORT_SN_SIZE> &;

  private:
	interface::MCU &_mcu;

	std::array<uint8_t, SN_SIZE> serial_number {};
	std::array<uint8_t, SHORT_SN_SIZE> short_serial_number {};
};

}	// namespace leka
