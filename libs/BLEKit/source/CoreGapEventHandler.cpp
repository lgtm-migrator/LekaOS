#include "CoreGapEventHandler.h"

using namespace leka;
using namespace ble;

void CoreGapEventHandler::registerStartAdvertising(std::function<void()> const &function)
{
	_start_advertising = function;
}

void CoreGapEventHandler::onInitializationComplete(BLE::InitializationCompleteCallbackContext *params)
{
	if (params->error != BLE_ERROR_NONE) {
		return;
	}

	_event_queue.call(_start_advertising);
}

void CoreGapEventHandler::onConnectionComplete(ConnectionCompleteEvent const &event)
{
	if (event.getStatus() != BLE_ERROR_NONE) {
		_event_queue.call(_start_advertising);
	}
}

void CoreGapEventHandler::onDisconnectionComplete(DisconnectionCompleteEvent const &event)
{
	_event_queue.call(_start_advertising);
}

void CoreGapEventHandler::onAdvertisingEnd(AdvertisingEndEvent const &event)
{
	_event_queue.call(_start_advertising);
}