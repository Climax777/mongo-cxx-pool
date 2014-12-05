/*
 * ex01.cpp
 *
 *  Created on: 04 Dec 2014
 *      Author: pieter
 */
#include "mongopool/MongoConnection.h"
#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

using namespace std;
using namespace boost;

int showed = 0;
void newConnection() {
	// I believe authMechanism and authSource should default to MONGODB-CR and the db field in the string already respectively
	mongopool::MongoConnection conn("mongodb://test2:123456@localhost/testpool?authMechanism=MONGODB-CR&authSource=testpool&replicaSet=myset");
	if(conn.get() && !showed) {
		showed = 1;
		auto cs = conn.getConnectionString();
		cout<< cs.getServers()[0].toString() << " ";
		cout<< cs.getSetName() << " ";
		cout<< cs.getDatabase() << " ";
		cout<< cs.getUser() << " ";
		cout<< cs.getPassword() << " ";
		cout<< cs.toString();
		cout<< endl;
	}
	boost::this_thread::sleep_for(chrono::seconds(1));
	conn.done();
}
int main() {
	mongo::client::initialize();
	boost::thread_group worker_threads;
	cout<< "starting 10 connections" << endl;
	for(int i = 0; i < 10; ++i) {
		worker_threads.create_thread(newConnection);
	}

	worker_threads.join_all();
	cout<< "10 should be in pool again, starting another 10 (should not create new connections)" << endl;
	boost::thread_group worker_threads2;
	for(int j = 0; j < 10; ++j) {
		worker_threads2.create_thread(newConnection);
	}
	worker_threads2.join_all();
	cout<< "done";

	return 0;
}
