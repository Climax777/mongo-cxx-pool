/*
 * MongoHostConnectionPool.h
 *
 *  Created on: 01 Dec 2014
 *      Author: pieter
 */

#ifndef MONGOHOSTCONNECTIONPOOL_H_
#define MONGOHOSTCONNECTIONPOOL_H_
#include <boost/container/deque.hpp>
#include <mongo/client/dbclient.h>
#include <boost/date_time/posix_time/posix_time.hpp>

#define MCP_CONNECTION_IDLE_TIME 60*5
namespace mongopool {
using namespace boost;
using namespace mongo;
using namespace boost::container;
class MongoHostConnectionPool {
public:
	MongoHostConnectionPool();
	virtual ~MongoHostConnectionPool();
	int getMaxPoolSize() const;
	void setMaxPoolSize(int maxPoolSize);

	const ConnectionString& getConnString() const;
	void setConnString(const ConnectionString& connString);

	void getStaleConnections(list<DBClientBase*>& stale);
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
};
class MongoConnectionPool;
extern MongoConnectionPool g_Pool;
} /* namespace mongopool */

#endif /* MONGOHOSTCONNECTIONPOOL_H_ */
