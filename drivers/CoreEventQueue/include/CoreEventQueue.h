// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_EVENT_QUEUE_H_
#define _LEKA_OS_DRIVER_CORE_EVENT_QUEUE_H_

#include "events/EventQueue.h"
#include "rtos/Thread.h"

#include "interface/drivers/EventQueue.h"

namespace leka {

class CoreEventQueue : public interface::EventQueue
{
  public:
	explicit CoreEventQueue() = default;

	void dispatch_forever() final;

	void call(std::function<void()> const &f) final;
	void call(mbed::Callback<void()> const &f);

	template <typename F>
	void call(F f)
	{
		call(std::function<void()>(f));
	}

  private:
	rtos::Thread _event_queue_thread {};
	events::EventQueue _event_queue {};
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_CORE_EVENT_QUEUE_H_
