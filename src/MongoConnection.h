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
#include "MongoConnectionPool.h"

using namespace boost;
using namespace mongo;
using std::string;
namespace mongopool {
class MongoConnection;
typedef shared_ptr<MongoConnection> MongoConnectionPtr;
class MongoConnection {
public:
	MongoConnection();
	explicit MongoConnection(const string& host);
	explicit MongoConnection(const ConnectionString& cs);
	virtual ~MongoConnection();

	DBClientBase* operator->();
	DBClientBase& conn();
	DBClientBase* get();

	ConnectionString getConnectionString();

	void done();
	bool ok();
private:
	ConnectionString m_ConnString;
	DBClientBase* m_Connection;
};
extern MongoConnectionPool g_Pool;
} /* namespace mongopool */

#endif /* MONGOCONNECTION_H_ */
