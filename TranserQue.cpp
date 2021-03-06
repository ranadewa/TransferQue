// TranserQue.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

template <class T> 
class TranserQue {

public:
	void push(T object);
	T pop();
	bool isEmpty() { return objectQue.empty(); }
	

private:
	queue<T> objectQue;


};

template<class T>
void TranserQue<T>::push(T object)
{
	objectQue.push(object);
}

template<class T>
T TranserQue<T>::pop()
{
	T object = objectQue.front();
	objectQue.pop();

	return object;
}


TranserQue<int> transerque;
mutex mu;
condition_variable condVar;

const int END = 100;

class Publisher {
public:

	void publishData() {
		
		for (size_t i = 0; i < END + 1; i++)
		{
			unique_lock<mutex> locker(mu);
			transerque.push(i);
			locker.unlock();
			condVar.notify_one();

			cout << " Publish Data " << i << endl;
		}
	}
private:

};

class Subscriber {
public: 
	void receiveData() {

		cout << " Recieveing " << endl;

		int data = 0;
		while (data != END)
		{
			unique_lock<mutex> locker(mu);
			condVar.wait(locker, []() { return !transerque.isEmpty(); });


				data = transerque.pop();
				cout << " Recived Data" << data << endl;
			
			locker.unlock();			
		}
		
	}
};

int main()
{
	Publisher publisher;
	Subscriber subscriber;

	thread publisherThread(&Publisher::publishData, &publisher);
	thread subscriberThread(&Subscriber::receiveData, &subscriber);

	publisherThread.join();
	subscriberThread.join();

	int i;
	cin >> i;

    return 0;
}

