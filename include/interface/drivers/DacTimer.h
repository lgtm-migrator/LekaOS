// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_INTERFACE_DRIVER_DACTIMER_H_
#define _LEKA_OS_INTERFACE_DRIVER_DACTIMER_H_

#include "stm32f7xx_hal.h"

namespace leka::interface {

class DacTimer
{
  public:
    ~DacTimer() = default;

    virtual void initialize(float frequency) =0;
	virtual void deInitialize() =0;
	virtual void start() =0;
	virtual void stop() =0;

	virtual auto getHandle() -> TIM_HandleTypeDef =0;

  protected:

	virtual auto _calculatePeriod(float freq) -> uint32_t =0;
    virtual void _registerMspCallbacks() =0;
	virtual void _mspInitCallback() =0;
	virtual void _mspDeInitCallback() =0;
};


} // namespace leka


#endif