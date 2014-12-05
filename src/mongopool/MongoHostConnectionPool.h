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

#define MCP_CONNECTION_IDLE_TIME 60*5
namespace mongopool {
using namespace boost;
using namespace mongo;
using boost::container::list;
using boost::container::deque;
class MongoHostConnectionPool {
public:
	MongoHostConnectionPool();
	MongoHostConnectionPool(const MongoHostConnectionPool& other);
	virtual ~MongoHostConnectionPool();
	int getMaxPoolSize() const;
	void setMaxPoolSize(int maxPoolSize);

	const ConnectionString& getConnString() const;
	void setConnString(const ConnectionString& connString);
	uint64_t getCreated() const;
	int numAvailable();

	void getStaleConnections(list<DBClientBase*>& stale);
	DBClientBase* get();
	void done(DBClientBase* conn);
	void clear();
	void flush();
private:
	struct StoredConnection {
		StoredConnection();
		StoredConnection(DBClientBase *c);
		StoredConnection(const StoredConnection& other);
		bool ok();

		DBClientBase *conn;
		posix_time::ptime lastUsed;
	};
	int m_MaxPoolSize;
	deque<StoredConnection> m_Pool;
	ConnectionString m_ConnString;
	uint64_t m_Created;
	uint64_t m_minValidCreationTimeMicroSec;

	friend class MongoConnectionPool;
};
class MongoConnectionPool;
extern MongoConnectionPool g_Pool;
} /* namespace mongopool */

#endif /* MONGOHOSTCONNECTIONPOOL_H_ */
