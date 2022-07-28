// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RFIDKit.h"

namespace leka {

void RFIDKit::init()
{
	using namespace rfid::sm;

	static auto tag_detected_callback = [this]() { state_machine.process_event(event::tag_response_available {}); };

	_rfid_reader.registerOnTagDetectedCallback(tag_detected_callback);
	registerMagicCard();

	_rfid_reader.init();
	_rfid_reader.setModeTagDetection();
}

void RFIDKit::registerMagicCard()
{
	auto on_magic_card_readable_callback = [this](rfid::Tag &tag) {
		if (isTagSignatureValid(tag)) {
			_card = MagicCard {static_cast<uint16_t>((tag.data[4] << 8) + tag.data[5])};

			if (_on_tag_available_callback != nullptr) {
				_on_tag_available_callback(_card);
			}
		}
	};
	_rfid_reader.registerOnTagReadableCallback(on_magic_card_readable_callback);
}

auto RFIDKit::isTagSignatureValid(rfid::Tag tag) const -> bool
{
	return std::equal(std::begin(leka_tag_header), std::end(leka_tag_header), std::begin(tag.data));
}

void RFIDKit::onTagActivated(std::function<void(MagicCard &_card)> const &callback)
{
	_on_tag_available_callback = callback;
}

}	// namespace leka
