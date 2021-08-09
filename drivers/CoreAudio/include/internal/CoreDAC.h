// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DAC_H_
#define _LEKA_OS_LIB_DAC_H_

#include "LKCoreSTM32HalBase.h"
#include "lstd_span"

namespace leka {

class CoreDAC
{
  public:
	explicit CoreDAC(LKCoreSTM32HalBase &hal);

	void initialize();
	void terminate();
	void start(lstd::span<uint16_t> &outBuffer);
	void stop();
	auto getHandle() -> DAC_HandleTypeDef &;
	auto getDMAHandle() -> DMA_HandleTypeDef &;

	template <class ReadBuffCb>
	void setOnHalfBufferReadPtr(ReadBuffCb cb)
	{
		this->_pOnHalfBufferRead = static_cast<pDAC_CallbackTypeDef>(cb);
	}

	template <class ReadBuffCb>
	void setOnFullBufferReadPtr(ReadBuffCb cb)
	{
		this->_pOnFullBufferRead = static_cast<pDAC_CallbackTypeDef>(cb);
	}

  private:
	LKCoreSTM32HalBase &_hal;
	DAC_HandleTypeDef _hdac;
	DMA_HandleTypeDef _hdma;

	pDAC_CallbackTypeDef _pOnHalfBufferRead = nullptr;
	pDAC_CallbackTypeDef _pOnFullBufferRead = nullptr;

	void _registerCallbacks();

	void _registerMspCallbacks();
	void _mspInitCallback();
	void _mspDeInitCallback();
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_DAC_H_
