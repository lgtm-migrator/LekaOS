// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Singing.h"

namespace leka::led::animation {

static constexpr auto kNumberOfNotes = uint8_t {16};

void Singing::start()
{
	turnLedBlack();
}

void Singing::stop()
{
	turnLedBlack();
	_step		 = 0;
	_stage		 = 1;
	_note_number = 0;
}

void Singing::run()
{
	static constexpr auto kLastStage = kNumberOfNotes + 2;
	if (_stage == 1) {
		stage1();
	} else if (_stage <= kLastStage) {
		stage2();
	}
	_belt.show();
}

void Singing::stage1()
{
	static constexpr auto kMaxInputValueStage1 = uint8_t {20};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStage1); pos != 1.F) {
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Singing::stage2()
{
	static constexpr auto kMaxInputValueStage2 = uint8_t {25};

	static constexpr std::array<RGB, kNumberOfNotes> kNoteColors = {
		RGB::pure_blue, RGB::pure_blue, RGB::pure_green, RGB::pure_green, RGB::pure_red, RGB::pure_red,
		RGB::cyan,		RGB::cyan,		RGB::yellow,	 RGB::yellow,	  RGB::pure_red, RGB::pure_red,
		RGB::yellow,	RGB::yellow,	RGB::pure_green, RGB::pure_green,
	};

	static constexpr std::array<uint8_t, kNumberOfNotes> kNotePosition = {2,  17, 9,  5,  13, 19, 7,  1,
																		  18, 8,  16, 11, 4,  19, 14, 0};

	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStage2); pos != 1.F) {
		if (_note_number - 2 >= 0) {
			auto previous_note_index = kNotePosition.at(_note_number - 2);
			auto previous_note_color = kNoteColors.at(_note_number - 2);
			decreaseBrightnessAtIndex(previous_note_index, previous_note_color, pos);
		}
		if (_note_number - 1 >= 0) {
			auto previous_note_index = kNotePosition.at(_note_number - 1);
			auto previous_note_color = kNoteColors.at(_note_number - 1);
			decreaseBrightnessAtIndex(previous_note_index, previous_note_color, pos);
		}
		if (_note_number < kNumberOfNotes) {
			auto current_note_index = kNotePosition.at(_note_number);
			auto current_note_color = kNoteColors.at(_note_number);
			increaseBrightnessAtIndex(current_note_index, current_note_color, pos);
		}
		if (_note_number + 1 < kNumberOfNotes) {
			auto current_note_index = kNotePosition.at(_note_number + 1);
			auto current_note_color = kNoteColors.at(_note_number + 1);
			increaseBrightnessAtIndex(current_note_index, current_note_color, pos);
		}
		++_step;
	} else {
		_step = 0;
		_note_number += 2;
		++_stage;
	}
}

void Singing::increaseBrightnessAtIndex(uint8_t index, RGB color, float position)
{
	auto col = ColorKit::colorGradient(RGB::black, color, position);
	_belt.setColorAtIndex(index, col);
}

void Singing::decreaseBrightnessAtIndex(uint8_t index, RGB color, float position)
{
	auto col = ColorKit::colorGradient(color, RGB::black, position);
	_belt.setColorAtIndex(index, col);
}

void Singing::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
