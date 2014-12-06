/*
 * MongoConnection.cpp
 *
 *  Created on: 01 Dec 2014
 *      Author: pieter
 */

#include "mongopool/MongoConnection.h"
#include <iostream>
using namespace mongo;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
namespace mongopool {
MongoConnection::MongoConnection() : m_Connection(0) {
}

MongoConnection::~MongoConnection() {
	if(m_Connection) {
		if(m_Connection->isFailed()) {
			if(m_Connection->getSockCreationMicroSec() == DBClientBase::INVALID_SOCK_CREATION_TIME)
				delete m_Connection;
			else {
				done();
			}
		} else {
			cerr<< "scoped connection to " << m_Connection->getServerAddress()
						<< " not being returned to the pool";
			delete m_Connection;
		}
	}
}

} /* namespace mongopool */

mongopool::MongoConnection::MongoConnection(const string& host) : m_Connection(g_Pool.get(host)){
	string errMessage;
	m_ConnString = ConnectionString::parse(host, errMessage);
}

mongopool::MongoConnection::MongoConnection(const ConnectionString& cs) : m_ConnString(cs), m_Connection(g_Pool.get(cs)) {
}

DBClientBase* mongopool::MongoConnection::operator ->() {
	return m_Connection;
}

DBClientBase& mongopool::MongoConnection::conn() {
	return *m_Connection;
}

DBClientBase* mongopool::MongoConnection::get() {
	return m_Connection;
}

ConnectionString mongopool::MongoConnection::getConnectionString() {
	return m_ConnString;
}

void mongopool::MongoConnection::done() {
	if(!m_Connection)
		return;
	g_Pool.release(m_ConnString, m_Connection);
	m_Connection = 0;
}

bool mongopool::MongoConnection::ok() {
	return m_Connection != 0;
}
