// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "MemoryUtils.h"
#include "interface/drivers/RFIDReader.h"

namespace leka {

struct MagicCard {
	explicit constexpr MagicCard(uint16_t id)
	{
		_tag.data.at(id_high_byte_index) = utils::memory::getHighByte(id);
		_tag.data.at(id_low_byte_index)	 = utils::memory::getLowByte(id);
	}

	explicit constexpr MagicCard(const rfid::Tag &tag) : _tag(tag) {}

	[[nodiscard]] auto getId() const -> uint16_t
	{
		auto high = _tag.data.at(id_high_byte_index);
		auto low  = _tag.data.at(id_low_byte_index);

		auto both = utils::memory::combineBytes({.high = high, .low = low});

		return both;
	}

	constexpr auto operator==(MagicCard const &rhs) const -> bool
	{
		auto same_high = _tag.data.at(id_high_byte_index) == rhs._tag.data.at(id_high_byte_index);
		auto same_low  = _tag.data.at(id_low_byte_index) == rhs._tag.data.at(id_low_byte_index);

		auto same = same_high && same_low;

		return same;
	}

	static const MagicCard none;
	static const MagicCard emergency_stop;
	static const MagicCard dice_roll;

  private:
	static constexpr auto id_high_byte_index = 4;
	static constexpr auto id_low_byte_index	 = 5;

	rfid::Tag _tag {};
};

constexpr MagicCard MagicCard::none {0x00'00};
constexpr MagicCard MagicCard::emergency_stop {0x00'01};
constexpr MagicCard MagicCard::dice_roll {0x00'02};

}	// namespace leka
