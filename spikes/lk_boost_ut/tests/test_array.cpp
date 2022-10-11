// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./tests/config.h"
#include "boost/ut.hpp"

using namespace boost::ut;

suite suite_array = [] {
	"std::array size"_test = [] {
		const auto size = size_t {5};
		auto tab		= std::array<uint8_t, size> {};
		expect((size == std::size(tab)) >> fatal) << "fatal";
	};
};
