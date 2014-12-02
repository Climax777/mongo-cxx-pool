/*
 * MongoHostConnectionPool.cpp
 *
 *  Created on: 01 Dec 2014
 *      Author: pieter
 */

#include "MongoHostConnectionPool.h"
#include <boost/container/list.hpp>
namespace mongopool {
using namespace boost;
using namespace mongo;
using namespace boost::container;
MongoHostConnectionPool::MongoHostConnectionPool() {
	// TODO Auto-generated constructor stub

}

MongoHostConnectionPool::~MongoHostConnectionPool() {
	// TODO Auto-generated destructor stub
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

MongoHostConnectionPool::StoredConnection::StoredConnection(const StoredConnection&
		other) : conn(other.conn), lastUsed(other.lastUsed){
}

bool MongoHostConnectionPool::StoredConnection::ok() {
	return conn->isStillConnected();
}

void MongoHostConnectionPool::getStaleConnections(list<DBClientBase*>& stale) {
	posix_time::ptime now = posix_time::second_clock::local_time();

	list<MongoHostConnectionPool::StoredConnection> all;
	while ( !m_Pool.empty() ) {
		StoredConnection c = m_Pool.front();
		m_Pool.pop_front();

		if ( c.ok() && (now - c.lastUsed).total_seconds() > MCP_CONNECTION_IDLE_TIME )
			all.push_back( c );
		else
			stale.push_back( c.conn );
	}

	for (auto i = all.begin(); i != all.end(); ++i ) {
		m_Pool.push_back( *i );
	}
}
} /* namespace mongopool */

