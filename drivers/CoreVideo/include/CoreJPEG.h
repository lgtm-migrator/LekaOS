// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_JPEG_H_
#define _LEKA_OS_DRIVER_JPEG_H_

#include <cstdint>
#include <memory>

#include "CoreJPEGMode.h"
#include "LKCoreFatFsBase.h"
#include "LKCoreSTM32HalBase.h"
#include "interface/CoreJPEG.h"
#include "st_jpeg_utils.h"

namespace leka {

class CoreJPEG : public interface::CoreJPEG
{
  public:
	explicit CoreJPEG(LKCoreSTM32HalBase &hal);

	void initialize() final;

	void registerCallbacks() final;

	auto decodeImage(LKCoreFatFsBase &file) -> uint32_t final;

	struct Config : JPEG_ConfTypeDef {
		bool initialized = false;
		auto getWidthOffset() -> uint32_t;
	};
	auto getConfig() -> CoreJPEG::Config;

	static auto findFrameOffset(LKCoreFatFsBase &file, uint32_t offset) -> uint32_t;

  private:
	LKCoreSTM32HalBase &_hal;

	CoreJPEGModeDMA _mode;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_JPEG_H_