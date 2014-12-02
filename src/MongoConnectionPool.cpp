/*
 * MongoConnectionPool.cpp
 *
 *  Created on: 01 Dec 2014
 *      Author: pieter
 */

#include "MongoConnectionPool.h"

namespace mongopool {

MongoConnectionPool::MongoConnectionPool(): m_IO(), m_TaskTimer(m_IO), m_MaxPoolSize(50), m_TaskRunner([this](){
	m_TaskTimer.async_wait(bind(&MongoConnectionPool::PeriodicTask, this, asio::placeholders::error));
	m_IO.run();}) {
}

MongoConnectionPool::~MongoConnectionPool() {
	m_IO.stop();
	m_TaskRunner.join();
}

DBClientBase* MongoConnectionPool::get(const ConnectionString& host) {
}

void MongoConnectionPool::clear() {
}

void MongoConnectionPool::PeriodicTask(const system::error_code& e) {
	// Do work here TODO
	m_TaskTimer.expires_at(m_TaskTimer.expires_at() + posix_time::seconds(MCP_PERIODIC_TASK_PERIOD));
	m_TaskTimer.async_wait(bind(&MongoConnectionPool::PeriodicTask, this, asio::placeholders::error));
}

int MongoConnectionPool::getMaxPoolSize() const {
	return m_MaxPoolSize;
}

void MongoConnectionPool::setMaxPoolSize(int maxPoolSize) {
	m_MaxPoolSize = maxPoolSize;
}
} /* namespace mongopool */
