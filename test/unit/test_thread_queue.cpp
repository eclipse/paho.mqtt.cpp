// test_thread_queue.cpp
//
// Unit tests for the thread_queue class in the Paho MQTT C++ library.
//

/*******************************************************************************
 * Copyright (c) 2022 Frank Pagliughi <fpagliughi@mindspring.com>
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v20.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Frank Pagliughi - Initial implementation
 *******************************************************************************/

#define UNIT_TESTS

#include "catch2_version.h"
#include "mqtt/types.h"
#include "mqtt/thread_queue.h"

#include <thread>
#include <future>
#include <chrono>
#include <vector>

using namespace mqtt;
using namespace std::chrono;

TEST_CASE("que put/get", "[thread_queue]")
{
	thread_queue<int> que;

	que.put(1);
	que.put(2);
	REQUIRE(que.get() == 1);

	que.put(3);
	REQUIRE(que.get() == 2);
	REQUIRE(que.get() == 3);
}

TEST_CASE("que mt put/get", "[thread_queue]")
{
	thread_queue<string> que;
	const size_t N = 1000000;
	const size_t N_THR = 2;

	auto producer = [&que, &N]() {
		string s;
		for (size_t i=0; i<512; ++i)
			s.push_back('a' + i%26);

		for (size_t i=0; i<N; ++i)
			que.put(s);
	};

	auto consumer = [&que, &N]() {
		string s;
		bool ok = true;
		for (size_t i=0; i<N && ok; ++i) {
			ok = que.try_get_for(&s, seconds{1});
		}
		return ok;
	};

	std::vector<std::thread> producers;
	std::vector<std::future<bool>> consumers;

	for (size_t i=0; i<N_THR; ++i)
		producers.push_back(std::thread(producer));

	for (size_t i=0; i<N_THR; ++i)
		consumers.push_back(std::async(consumer));

	for (size_t i=0; i<N_THR; ++i)
		producers[i].join();

	for (size_t i=0; i<N_THR; ++i) {
		REQUIRE(consumers[i].get());
	}
}

