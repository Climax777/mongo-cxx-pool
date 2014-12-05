/*
 * MongoConnectionPool.h
 *
 *  Created on: 01 Dec 2014
 *      Author: pieter
 */

#ifndef MONGOCONNECTIONPOOL_H_
#define MONGOCONNECTIONPOOL_H_
#include <mongo/client/dbclient.h>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/container/map.hpp>
#include "mongopool/MongoHostConnectionPool.h"

#define MCP_PERIODIC_TASK_PERIOD 30
namespace mongopool {
using namespace boost;
using boost::container::map;
using namespace mongo;
using std::string;
class MongoConnectionPool {
public:
	MongoConnectionPool();
	virtual ~MongoConnectionPool();

	DBClientBase* get(const string& host);
	DBClientBase* get(const ConnectionString& host);

	void removeHost(const ConnectionString& host);
	void removeHost(const string& host);

	void release(const ConnectionString& host, DBClientBase* conn);
	void flush();
	void clear();
	int getMaxPoolSize() const;
	void setMaxPoolSize(int maxPoolSize);
private:
	string BuildHostString(const ConnectionString& host);
	void PeriodicTask(const system::error_code& e);
	asio::io_service m_IO;
	asio::deadline_timer m_TaskTimer;
	int m_MaxPoolSize;
	thread m_TaskRunner;

	mutex m_Mutex;
	map<string, MongoHostConnectionPool> m_Pools;
};

extern MongoConnectionPool g_Pool;
} /* namespace mongopool */


#endif /* MONGOCONNECTIONPOOL_H_ */
