/*
 * MongoHostConnectionPool.h
 *
 *  Created on: 01 Dec 2014
 *      Author: pieter
 */

#ifndef MONGOHOSTCONNECTIONPOOL_H_
#define MONGOHOSTCONNECTIONPOOL_H_
#include <boost/container/deque.hpp>
#include <boost/container/list.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <mongo/client/dbclient.h>
#include "mongopool/MongoConfig.h"

#define MCP_CONNECTION_IDLE_TIME 60*5
namespace mongopool {
class MongoHostConnectionPool {
public:
	MongoHostConnectionPool();
	MongoHostConnectionPool(const MongoHostConnectionPool& other);
	virtual ~MongoHostConnectionPool();
	int getMaxPoolSize() const;
	void setMaxPoolSize(int maxPoolSize);

	const mongo::ConnectionString& getConnString() const;
	void setConnString(const mongo::ConnectionString& connString);
	uint64_t getCreated() const;
	int numAvailable();

	void getStaleConnections(boost::container::list<mongo::DBClientBase*>& stale);
	mongo::DBClientBase* get();
	void done(mongo::DBClientBase* conn);
	void clear();
	void flush();
private:
	struct StoredConnection {
		StoredConnection();
		StoredConnection(mongo::DBClientBase *c);
		StoredConnection(const StoredConnection& other);
		bool ok();

		mongo::DBClientBase *conn;
		boost::posix_time::ptime lastUsed;
	};
	int m_MaxPoolSize;
	boost::container::deque<StoredConnection> m_Pool;
	mongo::ConnectionString m_ConnString;
	uint64_t m_Created;
	uint64_t m_minValidCreationTimeMicroSec;

	friend class MongoConnectionPool;
};
class MongoConnectionPool;
extern MongoConnectionPool g_Pool;
} /* namespace mongopool */

#endif /* MONGOHOSTCONNECTIONPOOL_H_ */
