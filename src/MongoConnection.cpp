/*
 * MongoConnection.cpp
 *
 *  Created on: 01 Dec 2014
 *      Author: pieter
 */

#include "MongoConnection.h"

namespace mongopool {

MongoConnection::MongoConnection() : m_Connection(0) {
}

MongoConnection::~MongoConnection() {
}

} /* namespace mongopool */

mongopool::MongoConnection::MongoConnection(const string& host) {
	string errMessage;
	m_ConnString = ConnectionString::parse(host, errMessage);
}

mongopool::MongoConnection::MongoConnection(const ConnectionString& cs) : m_ConnString(cs) {
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
}

bool mongopool::MongoConnection::ok() {
	return m_Connection != 0;
}
