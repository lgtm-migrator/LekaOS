// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "VideoKit.h"

#include "rtos/ThisThread.h"

#include "FileManagerKit.h"

using namespace leka;
using namespace std::chrono_literals;

void VideoKit::initializeScreen()
{
	_video.initialize();

	_video.setBrightness(1.F);
	_video.clearScreen();

	_thread.start(mbed::Callback(this, &VideoKit::run));
}

void VideoKit::displayImage(const std::filesystem::path &path)
{
	if (path == _current_path) {
		return;
	}

	if (auto file = FileManagerKit::File {path}; file.is_open()) {
		_event_flags.set(flags::STOP_VIDEO_FLAG);

		_current_path = path;

		rtos::ThisThread::sleep_for(100ms);

		_video.displayImage(file);

		file.close();
	}
}

void VideoKit::playVideo(const std::filesystem::path &path, bool must_loop)
{
	if (auto file = FileManagerKit::File {path}; file.is_open()) {
		file.close();

		_event_flags.set(flags::STOP_VIDEO_FLAG);

		_current_path = path;
		_must_loop	  = must_loop;

		rtos::ThisThread::sleep_for(100ms);
		_event_flags.set(flags::START_VIDEO_FLAG);

		_lcd.turnOn();
	}
}

void VideoKit::stopVideo()
{
	_event_flags.set(flags::STOP_VIDEO_FLAG);
}

void VideoKit::run()
{
	auto file = FileManagerKit::File {};

	auto keep_running = [this] {
		auto must_not_stop	  = !((_event_flags.get() & flags::STOP_VIDEO_FLAG) == flags::STOP_VIDEO_FLAG);
		auto is_still_playing = !_video.isLastFrame();

		return must_not_stop && (_must_loop || is_still_playing);
	};

	while (true) {
		_event_flags.wait_any(flags::START_VIDEO_FLAG);
		_event_flags.clear(flags::STOP_VIDEO_FLAG);

		file.open(_current_path);
		_video.setVideo(file);

		while (keep_running()) {
			_video.displayNextFrameVideo(file);

			rtos::ThisThread::sleep_for(1ms);
		}

		file.close();
	}
}
