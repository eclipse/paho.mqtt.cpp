/////////////////////////////////////////////////////////////////////////////
/// @file thread_queue.h
/// Implementation of the template class 'thread_queue', a thread-safe,
/// blocking queue for passing data between threads, safe for use with smart
/// pointers.
/// @date 09-Jan-2017
/////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Copyright (c) 2017 Frank Pagliughi <fpagliughi@mindspring.com>
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Frank Pagliughi - initial implementation and documentation
 *******************************************************************************/

#ifndef __mqtt_thread_queue_h
#define __mqtt_thread_queue_h

#include <thread>
#include <mutex>
#include <condition_variable>
#include <limits>
#include <deque>
#include <queue>
#include <algorithm>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/**
 * A thread-safe queue for inter-thread communication.
 * This is a lockinq queue with blocking operations. The get() operations
 * can always block on an empty queue, but have variations for non-blocking
 * (try_get) and bounded-time blocking (try_get_for, try_get_until).
 * @par
 * The default queue has a capacity that is unbounded in the practical
 * sense, limited by available memory. In this mode the object will not
 * block when placing values into the queue. A capacity can bet set with the
 * construtor or, at any time later by calling the @ref capacity(size_type)
 * method. Using this latter method, the capacity can be set to an amount
 * smaller than the current size of the queue. In that case all put's to the
 * queue will block until the number of items are removed from the queue to
 * bring the size below the new capacity.
 * @par
 * Note that the queue uses move semantics to place items into the queue and
 * remove items from the queue. This means that the type, T, of the data
 * held by the queue only needs to follow move semantics; not copy
 * semantics. In addition, this means that copies of the value will @em not
 * be left in the queue. This is especially useful when creating queues of
 * shared pointers, as the "dead" part of the queue will not hold onto a
 * reference count after the item has been removed from the queue.
 *
 * @param T The type of the items to be held in the queue.
 * @param Container The type of the underlying container to use. It must
 * support back(), front(), push_back(), pop_front().
 */
template <typename T, class Container=std::deque<T>>
class thread_queue
{
public:
	/** The underlying container type to use for the queue. */
	using container_type = Container;
	/** The type of items to be held in the queue. */
	using value_type = T;
	/** The type used to specify number of items in the container. */
	using size_type = typename Container::size_type;

	/** The maximum capacity of the queue. */
	static constexpr size_type MAX_CAPACITY = std::numeric_limits<size_type>::max();

private:
	/** Object lock */
	mutable std::mutex lock_;
	/** Condition get signaled when item added to empty queue */
	std::condition_variable notEmptyCond_;
	/** Condition gets signaled then item removed from full queue */
	std::condition_variable notFullCond_;
	/** The capacity of the queue */
	size_type cap_;
	/** The actual STL container to hold data */
	std::queue<T,Container> que_;

	/** Simple, scope-based lock guard */
	using guard = std::lock_guard<std::mutex>;
	/** General purpose guard */
	using unique_guard = std::unique_lock<std::mutex>;

public:
	/**
	 * Constructs a queue with the maximum capacity.
	 */
	thread_queue() : cap_(MAX_CAPACITY) {}
	/**
	 * Constructs a queue with the specified capacity.
	 * @param cap The maximum number of items that can be placed in the
	 *  		  queue. The minimum capacity is 1.
	 */
	explicit thread_queue(size_t cap) : cap_(std::max<size_type>(cap, 1)) {}
	/**
	 * Determine if the queue is empty.
	 * @return @em true if there are no elements in the queue, @em false if
	 *  	   there are any items in the queue.
	 */
	bool empty() const {
		guard g(lock_);
		return que_.empty();
	}
	/**
	 * Gets the capacity of the queue.
	 * @return The maximum number of elements before the queue is full.
	 */
	size_type capacity() const {
		guard g(lock_);
		return cap_;
	}
	/**
	 * Sets the capacity of the queue.
	 * Note that the capacity can be set to a value smaller than the current
	 * size of the queue. In that event, all calls to put() will block until
	 * a suffucuent number
	 */
	void capacity(size_type cap) {
		guard g(lock_);
		cap_ = cap;
	}
	/**
	 * Gets the number of items in the queue.
	 * @return The number of items in the queue.
	 */
	size_type size() const {
		guard g(lock_);
		return que_.size();
	}
	/**
	 * Put an item into the queue.
	 * If the queue is full, this will block the caller until items are
	 * removed bringing the size less than the capacity.
	 * @param val The value to add to the queue.
	 */
	void put(value_type val) {
		unique_guard g(lock_);
		if (que_.size() >= cap_)
			notFullCond_.wait(g, [=]{return que_.size() < cap_;});
        bool wasEmpty = que_.empty();
		que_.emplace(std::move(val));
		if (wasEmpty) {
			g.unlock();
			notEmptyCond_.notify_one();
		}
	}
	/**
	 * Non-blocking attempt to place an item into the queue.
	 * @param val The value to add to the queue.
	 * @return @em true if the item was added to the queue, @em false if the
	 *  	   item was not added because the queue is currently full.
	 */
	bool try_put(value_type val) {
		unique_guard g(lock_);
		size_type n = que_.size();
		if (n >= cap_)
			return false;
		que_.emplace(std::move(val));
		if (n == 0) {
			g.unlock();
			notEmptyCond_.notify_one();
		}
		return true;
	}
	/**
	 * Attempt to place an item in the queue with a bounded wait.
	 * This will attempt to place the value in the queue, but if it is full,
	 * it will wait up to the specified time duration before timing out.
	 * @param val The value to add to the queue.
	 * @param relTime The amount of time to wait until timing out.
	 * @return @em true if the value was added to the queue, @em false if a
	 *  	   timeout occurred.
	 */
	template <typename Rep, class Period>
	bool try_put_for(value_type* val, const std::chrono::duration<Rep, Period>& relTime) {
		unique_guard g(lock_);
		if (que_.size() >= cap_ && !notFullCond_.wait_for(g, relTime, [=]{return que_.size() < cap_;}))
			return false;
        bool wasEmpty = que_.empty();
		que_.emplace(std::move(val));
		if (wasEmpty) {
			g.unlock();
			notEmptyCond_.notify_one();
		}
		return true;
	}
	/**
	 * Attempt to place an item in the queue with a bounded wait to an
	 * absolute time point.
	 * This will attempt to place the value in the queue, but if it is full,
	 * it will wait up until the specified time before timing out.
	 * @param val The value to add to the queue.
	 * @param absTime The absolute time to wait to before timing out.
	 * @return @em true if the value was added to the queue, @em false if a
	 *  	   timeout occurred.
	 */
	template <class Clock, class Duration>
	bool try_put_until(value_type* val, const std::chrono::time_point<Clock,Duration>& absTime) {
		unique_guard g(lock_);
		if (que_.size() >= cap_ && !notFullCond_.wait_until(g, absTime, [=]{return que_.size() < cap_;}))
			return false;
        bool wasEmpty = que_.empty();
		que_.emplace(std::move(val));
		if (wasEmpty) {
			g.unlock();
			notEmptyCond_.notify_one();
		}
		return true;
	}
	/**
	 * Retrieve a value from the queue.
	 * If the queue is empty, this will block indefinitely until a value is
	 * added to the queue by another thread,
	 * @param val Pointer to a variable to receive the value.
	 */
	void get(value_type* val) {
		unique_guard g(lock_);
		if (que_.empty())
			notEmptyCond_.wait(g, [=]{return !que_.empty();});
		*val = std::move(que_.front());
		que_.pop();
		if (que_.size() == cap_-1) {
			g.unlock();
			notFullCond_.notify_one();
		}
	}
	/**
	 * Retrieve a value from the queue.
	 * If the queue is empty, this will block indefinitely until a value is
	 * added to the queue by another thread,
	 * @return The value removed from the queue
	 */
	value_type get() {
		unique_guard g(lock_);
		if (que_.empty())
			notEmptyCond_.wait(g, [=]{return !que_.empty();});
		value_type val = std::move(que_.front());
		que_.pop();
		if (que_.size() == cap_-1) {
			g.unlock();
			notFullCond_.notify_one();
		}
		return val;
	}
	/**
	 * Attempts to remove a value from the queue without blocking.
	 * If the queue is currently empty, this will return immediately with a
	 * failure, otherwise it will get the next value and return it.
	 * @param val Pointer to a variable to receive the value.
	 * @return @em true if a value was removed from the queue, @em false if
	 *  	   the queue is empty.
	 */
	bool try_get(value_type* val) {
		unique_guard g(lock_);
		if (que_.empty())
			return false;
		*val = std::move(que_.front());
		que_.pop();
		if (que_.size() == cap_-1) {
			g.unlock();
			notFullCond_.notify_one();
		}
		return true;
	}
	/**
	 * Attempt to remove an item from the queue for a bounded amout of time.
	 * This will retrieve the next item from the queue. If the queue is
	 * empty, it will wait the specified amout of time for an item to arive
	 * before timing out.
	 * @param val Pointer to a variable to receive the value.
	 * @param relTime The amount of time to wait until timing out.
	 * @return @em true if the value was removed the queue, @em false if a
	 *  	   timeout occurred.
	 */
	template <typename Rep, class Period>
	bool try_get_for(value_type* val, const std::chrono::duration<Rep, Period>& relTime) {
		unique_guard g(lock_);
		if (que_.empty() && !notEmptyCond_.wait_for(g, relTime, [=]{return !que_.empty();}))
			return false;
		*val = std::move(que_.front());
		que_.pop();
		if (que_.size() == cap_-1) {
			g.unlock();
			notFullCond_.notify_one();
		}
		return true;
	}
	/**
	 * Attempt to remove an item from the queue for a bounded amout of time.
	 * This will retrieve the next item from the queue. If the queue is
	 * empty, it will wait until the specified time for an item to arive
	 * before timing out.
	 * @param val Pointer to a variable to receive the value.
	 * @param absTime The absolute time to wait to before timing out.
	 * @return @em true if the value was removed from the queue, @em false
	 *  	   if a timeout occurred.
	 */
	template <class Clock, class Duration>
	bool try_get_until(value_type* val, const std::chrono::time_point<Clock,Duration>& absTime) {
		unique_guard g(lock_);
		if (que_.empty() && !notEmptyCond_.wait_until(g, absTime, [=]{return !que_.empty();}))
			return false;
		*val = std::move(que_.front());
		que_.pop();
		if (que_.size() == cap_-1) {
			g.unlock();
			notFullCond_.notify_one();
		}
		return true;
	}
};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_thread_queue_h

