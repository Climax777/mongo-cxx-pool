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
#include "mongopool/MongoConfig.h"
#include "mongopool/MongoHostConnectionPool.h"

#define MCP_PERIODIC_TASK_PERIOD 30
#define MCP_MAX_POOL_SIZE 500
namespace mongopool {
class MONGO_CXX_POOL_EXPORT MongoConnectionPool {
public:
	MongoConnectionPool();
	virtual ~MongoConnectionPool();

	mongo::DBClientBase* get(const std::string& host);
	mongo::DBClientBase* get(const mongo::ConnectionString& host);

	void removeHost(const mongo::ConnectionString& host);
	void removeHost(const std::string& host);

	void release(const mongo::ConnectionString& host, mongo::DBClientBase* conn);
	void flush();
	void clear();
	int getMaxPoolSize() const;
	void setMaxPoolSize(int maxPoolSize);
private:
	std::string BuildHostString(const mongo::ConnectionString& host);
	void PeriodicTask(const boost::system::error_code& e);
	boost::asio::io_service m_IO;
	boost::asio::deadline_timer m_TaskTimer;
	int m_MaxPoolSize;
	boost::thread m_TaskRunner;

	boost::mutex m_Mutex;
	boost::container::map<std::string, MongoHostConnectionPool> m_Pools;
};

MONGO_CXX_POOL_EXPORT extern MongoConnectionPool g_Pool;
} /* namespace mongopool */


#endif /* MONGOCONNECTIONPOOL_H_ */
