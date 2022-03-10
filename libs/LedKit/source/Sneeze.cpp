// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Sneeze.h"

#include "MathUtils.h"

namespace leka::led::animation {

void Sneeze::start()
{
	turnLedBlack();
}

void Sneeze::stop()
{
	turnLedBlack();
}

void Sneeze::run()
{
	switch (_stage) {
		case 1:
			stage1();
			break;
		case 2:
			stage2();
			break;
		case 3:
			stage3();
			break;
		case 4:
			stage4();
			break;
		case 5:
			stage5();
			break;
		default:
			break;
	}
	_belt.show();
}

auto Sneeze::mapStep(uint8_t step) const -> float
{
	static constexpr auto kInputMaxValue = uint8_t {20};
	return utils::math::map(step, uint8_t {0}, kInputMaxValue, 0.F, 1.F);
}

void Sneeze::stage1()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Sneeze::stage2()
{
	static constexpr auto green_sick = RGB {0x10, 0xF0, 0x30};

	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	_belt.setColorAtIndex(_sneeze_position, green_sick);
	_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_position + 1), green_sick);
	++_sneeze_position;
	if (_sneeze_position > kNumberOfLedsBelt / 6) {
		++_stage;
	}
}

void Sneeze::stage3()
{
	static constexpr auto green_sick = RGB {0x10, 0xF0, 0x30};

	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	if (auto pos = mapStep(_step); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, green_sick, pos);
		_belt.setColorAtIndex(_sneeze_position, color);
		_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_position + 1), color);
		++_step;
	} else {
		++_stage;
	}
}

void Sneeze::stage4()
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	_belt.setColorAtIndex(_sneeze_position, RGB::black);
	_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_position + 1), RGB::black);
	--_sneeze_position;
	if (_sneeze_position == 0) {
		++_stage;
	}
}

void Sneeze::stage5()
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	_belt.setColorAtIndex(_sneeze_position, RGB::black);
	_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_position + 1), RGB::black);
	++_stage;
}

void Sneeze::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report