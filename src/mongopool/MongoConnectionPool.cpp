/*
 * MongoConnectionPool.cpp
 *
 *  Created on: 01 Dec 2014
 *      Author: pieter
 */

#include "mongopool/MongoConnectionPool.h"
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

namespace mongopool {

MongoConnectionPool::MongoConnectionPool(): m_IO(), m_TaskTimer(m_IO), m_MaxPoolSize(50), m_TaskRunner([this](){
	m_TaskTimer.async_wait(bind(&MongoConnectionPool::PeriodicTask, this, asio::placeholders::error));
	m_IO.run();}), m_Mutex() {
}

MongoConnectionPool::~MongoConnectionPool() {
	m_IO.stop();
	m_TaskRunner.join();
}

DBClientBase* MongoConnectionPool::get(const string& host) {
	string errMsg;
	ConnectionString cs = ConnectionString::parse(host, errMsg);
	if(cs.isValid()) {
		return get(cs);
	}
	return 0;
}

DBClientBase* MongoConnectionPool::get(const ConnectionString& host) {
	DBClientBase* client = 0;
	{
		lock_guard<mutex> lock(m_Mutex);
		MongoHostConnectionPool& pool = m_Pools[BuildHostString(host)];
		pool.setMaxPoolSize(m_MaxPoolSize);
		pool.setConnString(host);
		client = pool.get();
	}
	if(client) {
		return client;
	}

	string errMessage;
	client = host.connect(errMessage);
	if(!client) {
		cerr<< "Could not connect to " << host.getServers()[0].toString() << ": " << errMessage << endl;
		return 0;
	}
	{
		lock_guard<mutex> lock(m_Mutex);
		MongoHostConnectionPool& pool = m_Pools[BuildHostString(host)];
		pool.setMaxPoolSize(m_MaxPoolSize);
		pool.setConnString(host);
		pool.m_Created++;
	}
	return client;
}

void MongoConnectionPool::clear() {
	lock_guard<mutex> lock(m_Mutex);
	cout<< "Removing connections on all pools" << endl;
	for(auto i: m_Pools) {
		i.second.clear();
	}
}

void MongoConnectionPool::PeriodicTask(const system::error_code& e) {
	list<DBClientBase*> deleteList;
	{
		lock_guard<mutex> lock(m_Mutex);
		for(auto i: m_Pools) {
			i.second.getStaleConnections(deleteList);
		}
	}

	for(auto i: deleteList) {
		delete i;
	}
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

void MongoConnectionPool::removeHost(const ConnectionString& host) {
	lock_guard<mutex> lock(m_Mutex);
	cout<< "Removing connections from all pools for host " << host.getServers()[0].toString() << endl;
	for(auto i: m_Pools) {
		if(BuildHostString(host).compare(i.first) == 0)
			i.second.clear();
	}
}

void MongoConnectionPool::removeHost(const string& host) {
	string errMsg;
	ConnectionString cs = ConnectionString::parse(host, errMsg);
	if(cs.isValid())
		removeHost(cs);
}

void MongoConnectionPool::flush() {
	lock_guard<mutex> lock(m_Mutex);
	for(auto i: m_Pools) {
		i.second.flush();
	}
}
void MongoConnectionPool::release(const ConnectionString& host,
		DBClientBase* conn) {
	lock_guard<mutex> lock(m_Mutex);
	m_Pools[BuildHostString(host)].done(conn);
}

string MongoConnectionPool::BuildHostString(const ConnectionString& host) {
	return host.toString() + "/" + host.getDatabase() + "/" + host.getUser();
}
MongoConnectionPool g_Pool;
} /* namespace mongopool */

