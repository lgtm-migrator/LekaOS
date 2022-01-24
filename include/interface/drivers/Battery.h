// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_INTERFACE_BATTERY_H_
#define _LEKA_OS_DRIVER_INTERFACE_BATTERY_H_

#include <cstdint>

#include "platform/Callback.h"

namespace leka::interface {

class Battery
{
  public:
	virtual ~Battery() = default;

	virtual void onChargeDidStart(mbed::Callback<void()> const &callback) = 0;
	virtual void onChargeDidStop(mbed::Callback<void()> const &callback)  = 0;

	virtual auto voltage() -> float	  = 0;
	virtual auto level() -> uint8_t	  = 0;
	virtual auto isCharging() -> bool = 0;
};

}	// namespace leka::interface

#endif	 //_LEKA_OS_DRIVER_INTERFACE_BATTERY_H_
