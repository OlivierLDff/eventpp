// eventpp library
// Copyright (C) 2018 Wang Qi (wqking)
// Github: https://github.com/wqking/eventpp
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Include the head
#include "eventpp/eventqueue.h"

#include "test.h"

#include <iostream>
#include <thread>

TEST_CASE("EventQueue tutorial 1, basic")
{
	std::cout << "EventQueue tutorial 1, basic" << std::endl;

	eventpp::EventQueue<int, void (const std::string &, std::unique_ptr<int> &)> queue;

	queue.appendListener(3, [](const std::string & s, std::unique_ptr<int> & n) {
		std::cout << "Got event 3, s is " << s << " n is " << *n << std::endl;
	});
	// The listener prototype doesn't need to be exactly same as the dispatcher.
	// It would be find as long as the arguments is compatible with the dispatcher.
	queue.appendListener(5, [](std::string s, const std::unique_ptr<int> & n) {
		std::cout << "Got event 5, s is " << s << " n is " << *n << std::endl;
	});
	queue.appendListener(5, [](const std::string & s, std::unique_ptr<int> & n) {
		std::cout << "Got another event 5, s is " << s << " n is " << *n << std::endl;
	});

	// Enqueue the events, the first argument is always the event type.
	// The listeners are not triggered during enqueue.
	queue.enqueue(3, "Hello", std::unique_ptr<int>(new int(38)));
	queue.enqueue(5, "World", std::unique_ptr<int>(new int(58)));

	// Process the event queue, dispatch all queued events.
	queue.process();
}

TEST_CASE("EventQueue tutorial 2, multiple threading")
{
	std::cout << "EventQueue tutorial 2, multiple threading" << std::endl;

	using EQ = eventpp::EventQueue<int, void (int)>;
	EQ queue;

	constexpr int stopEvent = 1;
	constexpr int otherEvent = 2;

	// Start a thread to process the event queue.
	// All listeners are invoked in that thread.
	std::thread thread([stopEvent, otherEvent, &queue]() {
		volatile bool shouldStop = false;
		queue.appendListener(stopEvent, [&shouldStop](int) {
			shouldStop = true;
		});
		queue.appendListener(otherEvent, [](const int index) {
			std::cout << "Got event, index is " << index << std::endl;
		});

		while(! shouldStop) {
			queue.wait();

			queue.process();
		}
	});

	// Enqueue an event from the main thread. After sleeping for 10 milliseconds,
	// the event should have be processed by the other thread.
	queue.enqueue(otherEvent, 1);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	std::cout << "Should have triggered event with index = 1" << std::endl;

	queue.enqueue(otherEvent, 2);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	std::cout << "Should have triggered event with index = 2" << std::endl;

	{
		// EventQueue::DisableQueueNotify is a RAII class that
		// disables waking up any waiting threads.
		// So no events should be triggered in this code block.
		// DisableQueueNotify is useful when adding lots of events at the same time
		// and only want to wake up the waiting threads after all events are added.
		EQ::DisableQueueNotify disableNotify(&queue);

		queue.enqueue(otherEvent, 10);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		std::cout << "Should NOT trigger event with index = 10" << std::endl;
		
		queue.enqueue(otherEvent, 11);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		std::cout << "Should NOT trigger event with index = 11" << std::endl;
	}
	// The DisableQueueNotify object is destroyed here, and has resumed
	// waking up waiting threads. So the events should be triggered.
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	std::cout << "Should have triggered events with index = 10 and 11" << std::endl;

	queue.enqueue(stopEvent, 1);
	thread.join();
}

