// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "DACTimer.h"

namespace leka {

DACTimer::DACTimer(LKCoreSTM32HalBase &hal, HardwareBasicTimer tim) : _hal(hal), _hardwareTim(tim)
{
	if (_hardwareTim == HardwareBasicTimer::BasicTimer6) {
		_htim.Instance = TIM6;
	} else if (_hardwareTim == HardwareBasicTimer::BasicTimer7) {
		_htim.Instance = TIM7;
	}
}

void DACTimer::initialize(uint32_t frequency)
{
	_htim.Init.Prescaler		 = 0;	// no need of prescaler for high frequencies
	_htim.Init.CounterMode		 = TIM_COUNTERMODE_UP;
	_htim.Init.Period			 = _calculatePeriod(frequency);
	_htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	_registerMspCallbacks();

	_hal.HAL_TIM_Base_Init(&_htim);

	TIM_MasterConfigTypeDef sMasterConfig = {0};

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;   // use hardware trigger on DAC
	sMasterConfig.MasterSlaveMode	  = TIM_MASTERSLAVEMODE_DISABLE;
	_hal.HAL_TIMEx_MasterConfigSynchronization(&_htim, &sMasterConfig);
}

void DACTimer::terminate()
{
	_hal.HAL_TIM_Base_DeInit(&_htim);
}

void DACTimer::start()
{
	_hal.HAL_TIM_Base_Start(&_htim);
}

void DACTimer::stop()
{
	_hal.HAL_TIM_Base_Stop(&_htim);
}

auto DACTimer::getHandle() const -> const TIM_HandleTypeDef &
{
	return this->_htim;
}

auto DACTimer::getHardWareBasicTimer() const -> const HardwareBasicTimer &
{
	return _hardwareTim;
}

auto DACTimer::_calculatePeriod(uint32_t frequency) -> uint32_t
{
	uint32_t clockFreq = _hal.HAL_RCC_GetPCLK1Freq();

	/* Get PCLK1 prescaler */
	if ((RCC->CFGR & RCC_CFGR_PPRE1) != 0) {
		clockFreq *= 2;
	}

	if (frequency < (clockFreq >> 16) || frequency > clockFreq) {
		log_error("Chosen freq out of bounds\n");
	}

	return (clockFreq / frequency);
}

void DACTimer::_registerMspCallbacks()
{
	static auto *self = this;
	if (self != this) {
		self = this;
	}
	auto initCbLambda	= []([[maybe_unused]] TIM_HandleTypeDef *htim) { self->_msp_onInitializationCb(); };
	auto deInitCbLambda = []([[maybe_unused]] TIM_HandleTypeDef *htim) { self->_msp_onTerminationCb(); };
	_hal.HAL_TIM_RegisterCallback(&_htim, HAL_TIM_BASE_MSPINIT_CB_ID, initCbLambda);
	_hal.HAL_TIM_RegisterCallback(&_htim, HAL_TIM_BASE_MSPDEINIT_CB_ID, deInitCbLambda);
}

void DACTimer::_msp_onInitializationCb()
{
	if (_hardwareTim == HardwareBasicTimer::BasicTimer6) {
		_hal.HAL_RCC_TIM6_CLK_ENABLE();
	} else if (_hardwareTim == HardwareBasicTimer::BasicTimer7) {
		_hal.HAL_RCC_TIM7_CLK_ENABLE();
	}
}

void DACTimer::_msp_onTerminationCb()
{
	if (_hardwareTim == HardwareBasicTimer::BasicTimer6) {
		_hal.HAL_RCC_TIM6_CLK_DISABLE();
	} else if (_hardwareTim == HardwareBasicTimer::BasicTimer7) {
		_hal.HAL_RCC_TIM7_CLK_DISABLE();
	}
}

}	// namespace leka