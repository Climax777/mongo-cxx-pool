/*
 * MongoConnection.h
 *
 *  Created on: 01 Dec 2014
 *      Author: pieter
 */

#ifndef MONGOCONNECTION_H_
#define MONGOCONNECTION_H_
#include <boost/shared_ptr.hpp>
#include <mongo/client/dbclient.h>
#include "mongopool/MongoConfig.h"
#include "mongopool/MongoConnectionPool.h"

namespace mongopool {
class MongoConnection {
public:
	MongoConnection();
	explicit MongoConnection(const std::string& host);
	explicit MongoConnection(const mongo::ConnectionString& cs);
	virtual ~MongoConnection();

	mongo::DBClientBase* operator->();
	mongo::DBClientBase& conn();
	mongo::DBClientBase* get();

	mongo::ConnectionString getConnectionString();

	void done();
	bool ok();
private:
	mongo::ConnectionString m_ConnString;
	mongo::DBClientBase* m_Connection;
};
extern MongoConnectionPool g_Pool;
} /* namespace mongopool */

#endif /* MONGOCONNECTION_H_ */
