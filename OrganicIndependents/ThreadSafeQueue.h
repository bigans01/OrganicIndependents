#pragma once

#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <queue>
#include <mutex>

/*

Description:

Templated class built around the built-in Queue class. Designed with lock guard wrappers,
to allow multiple threads to simulatenously access this container safely.

*/

template <typename QueueObjectType> class ThreadSafeQueue {
	public:
		bool empty()
		{
			std::lock_guard<std::mutex> guard(tsQueueMutex);
			return tsQueue.empty();
		}

		void push(QueueObjectType in_objectToPush)
		{
			std::lock_guard<std::mutex> guard(tsQueueMutex);
			tsQueue.push(in_objectToPush);
		}

		typename QueueObjectType front()
		{
			std::lock_guard<std::mutex> guard(tsQueueMutex);
			return tsQueue.front();
		}

		void pop()
		{
			std::lock_guard<std::mutex> guard(tsQueueMutex);
			tsQueue.pop();
		}

	private:
		std::mutex tsQueueMutex;
		std::queue<QueueObjectType> tsQueue;
};

#endif