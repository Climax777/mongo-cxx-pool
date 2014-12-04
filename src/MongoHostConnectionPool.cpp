/*
 * MongoHostConnectionPool.cpp
 *
 *  Created on: 01 Dec 2014
 *      Author: pieter
 */

#include "MongoHostConnectionPool.h"
#include <boost/container/list.hpp>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

namespace mongopool {
using namespace boost;
using namespace mongo;
using namespace boost::container;
MongoHostConnectionPool::MongoHostConnectionPool() : m_minValidCreationTimeMicroSec(0), m_Created(0), m_MaxPoolSize(50) {
}

MongoHostConnectionPool::~MongoHostConnectionPool() {
	clear();
}


int MongoHostConnectionPool::getMaxPoolSize() const {
	return m_MaxPoolSize;
}

void MongoHostConnectionPool::setMaxPoolSize(int maxPoolSize) {
	m_MaxPoolSize = maxPoolSize;
}


void MongoHostConnectionPool::setConnString(
		const ConnectionString& connString) {
	if(m_Pool.empty())
		m_ConnString = connString;
}

const ConnectionString& MongoHostConnectionPool::getConnString() const {
	return m_ConnString;
}

MongoHostConnectionPool::StoredConnection::StoredConnection() : conn(0){
}

MongoHostConnectionPool::StoredConnection::StoredConnection(
		DBClientBase* c) : conn(c), lastUsed(posix_time::second_clock::local_time()){
}

MongoHostConnectionPool::StoredConnection::StoredConnection(const StoredConnection& other) :
		conn(other.conn), lastUsed(other.lastUsed){
}

bool MongoHostConnectionPool::StoredConnection::ok() {
	return conn->isStillConnected();
}

void MongoHostConnectionPool::getStaleConnections(list<DBClientBase*>& stale) {
	posix_time::ptime now = posix_time::second_clock::local_time();

	list<MongoHostConnectionPool::StoredConnection> all;
	while ( !m_Pool.empty() ) {
		StoredConnection conn = m_Pool.front();
		m_Pool.pop_front();

		if ( conn.ok() && (now - conn.lastUsed).total_seconds() < MCP_CONNECTION_IDLE_TIME )
			all.push_back( conn );
		else
			stale.push_back( conn.conn );
	}

	for(auto i : all) {
		m_Pool.push_back(i);
	}
}

DBClientBase* MongoHostConnectionPool::get() {
	posix_time::ptime now = posix_time::second_clock::local_time();

	while(!m_Pool.empty()) {
		StoredConnection conn = m_Pool.front();
		m_Pool.pop_front();

		if(!conn.ok() || (now - conn.lastUsed).total_seconds() >= MCP_CONNECTION_IDLE_TIME) {
			delete conn.conn;
			continue;
		}

		return conn.conn;
	}
	return 0;
}

void MongoHostConnectionPool::done(DBClientBase* conn) {
	bool isFailed = conn->isFailed();

	if(isFailed) {
		if(conn->getSockCreationMicroSec() != DBClientBase::INVALID_SOCK_CREATION_TIME
				&& conn->getSockCreationMicroSec() > m_minValidCreationTimeMicroSec) {
			m_minValidCreationTimeMicroSec = conn->getSockCreationMicroSec();
			cerr << "Detected bad connection created at " << m_minValidCreationTimeMicroSec
					<< " us, clearing pool for " << m_ConnString.getServers()[0].toString() << " of "
					<< m_Pool.size() << " connections" << endl;
			clear();
		}
	}

	if(isFailed ||
		conn->getSockCreationMicroSec() < m_minValidCreationTimeMicroSec ||
		(m_MaxPoolSize >= 0 && (int)m_Pool.size() >= m_MaxPoolSize)) {
		delete conn;
	} else {
		StoredConnection sc(conn);
		sc.lastUsed = posix_time::second_clock::local_time();
		m_Pool.push_back(conn);
	}
}

void MongoHostConnectionPool::clear() {
	while(!m_Pool.empty()) {
		StoredConnection conn = m_Pool.front();
		m_Pool.pop_front();
		delete conn.conn;
	}
}

MongoHostConnectionPool::MongoHostConnectionPool(
		const MongoHostConnectionPool& other) : m_Created(other.m_Created), m_minValidCreationTimeMicroSec(other.m_minValidCreationTimeMicroSec),
				m_ConnString(other.m_ConnString), m_MaxPoolSize(other.m_MaxPoolSize){
}

int MongoHostConnectionPool::numAvailable() {
	return m_Pool.size();
}

void MongoHostConnectionPool::flush() {
	list<StoredConnection> all;
	while(!m_Pool.empty()) {
		StoredConnection conn = m_Pool.front();
		m_Pool.pop_front();
		bool master;
		bool alive = false;
		try {
			conn.conn->isMaster(master);
			alive = true;
		} catch (const DBException& e) {
			cerr << "Exception thrown when checking pooled connection to " << conn.conn->getServerAddress()
					<< ": " << e.toString() << endl;
			delete conn.conn;
			conn.conn = 0;
		}
		if(alive)
			all.push_back(conn);
	}

	for(auto i : all) {
		m_Pool.push_back(i);
	}
}

uint64_t MongoHostConnectionPool::getCreated() const {
	return m_Created;
}

} /* namespace mongopool */

