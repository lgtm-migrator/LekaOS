// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_MOCK_DRIVERS_IO_EXPANDER_H_
#define _LEKA_OS_MOCK_DRIVERS_IO_EXPANDER_H_

#include "gmock/gmock.h"
#include "interface/drivers/IOExpander.h"

namespace leka::mock {

class IOExpander : public interface::IOExpander
{
  public:
	MOCK_METHOD(void, init, (uint16_t), (override));

	MOCK_METHOD(void, setRegisterMapping, (bool), (override));

	MOCK_METHOD(void, setInputPins, (uint16_t), (override));
	MOCK_METHOD(void, setOutputPins, (uint16_t), (override));

	MOCK_METHOD(void, writeOutputs, (uint16_t), (override));

	MOCK_METHOD(uint16_t, readOutputs, (), (override));
	MOCK_METHOD(uint16_t, readInputs, (), (override));

	MOCK_METHOD(void, setInputPolarity, (uint16_t), (override));
	MOCK_METHOD(uint16_t, getInputPolarity, (), (override));

	MOCK_METHOD(void, setPullups, (uint16_t), (override));
	MOCK_METHOD(uint16_t, getPullups, (), (override));

	MOCK_METHOD(void, interruptOnChanges, (uint16_t), (override));
	MOCK_METHOD(void, disableInterrupts, (uint16_t), (override));
	MOCK_METHOD(void, acknowledgeInterrupt, (uint16_t &, uint16_t &), (override));
};

}	// namespace leka::mock

#endif	 // _LEKA_OS_MOCK_DRIVERS_IO_EXPANDER_H_
