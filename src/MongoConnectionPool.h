/*
 * MongoConnectionPool.h
 *
 *  Created on: 01 Dec 2014
 *      Author: pieter
 */

#ifndef MONGOCONNECTIONPOOL_H_
#define MONGOCONNECTIONPOOL_H_
#include "MongoHostConnectionPool.h"
#include <mongo/client/dbclient.h>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#define MCP_PERIODIC_TASK_PERIOD 30
using namespace boost;
using namespace mongo;
namespace mongopool {

class MongoConnectionPool {
public:
	MongoConnectionPool();
	virtual ~MongoConnectionPool();

	DBClientBase* get(const ConnectionString& host);

	void clear();
	int getMaxPoolSize() const;
	void setMaxPoolSize(int maxPoolSize);

private:
	void PeriodicTask(const system::error_code& e);
	int m_MaxPoolSize;
	asio::io_service m_IO;
	asio::deadline_timer m_TaskTimer;
	thread m_TaskRunner;
};

extern MongoConnectionPool g_Pool;
} /* namespace mongopool */


#endif /* MONGOCONNECTIONPOOL_H_ */
