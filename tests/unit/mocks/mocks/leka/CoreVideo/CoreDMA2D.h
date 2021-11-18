// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DMA2D_MOCK_H_
#define _LEKA_OS_DMA2D_MOCK_H_

#include "gmock/gmock.h"
#include "interface/DMA2D.hpp"

namespace leka {

class CoreDMA2DMock : public interface::DMA2DBase
{
  public:
	MOCK_METHOD(void, initialize, (), (override));

	MOCK_METHOD(void, setFrameBufferAddress, (uintptr_t address), (override));

	MOCK_METHOD(void, transferData, (uintptr_t src, uintptr_t dst, uint32_t width, uint32_t height), (override));
	MOCK_METHOD(void, transferImage, (uint32_t width, uint32_t height, uint32_t width_offset), (override));

	MOCK_METHOD(void, fillRect, (uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color), (override));
	MOCK_METHOD(void, setPixel, (uint32_t x, uint32_t y, uint32_t color), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_DMA2D_MOCK_H_