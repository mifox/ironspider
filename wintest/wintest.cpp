// wintest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


#include <iostream>
#include <string>
#include "zthread/Thread.h"
#include "zthread/Mutex.h"
#include "zthread/Guard.h"
#include "zthread/Condition.h"
#include "zthread/ThreadedExecutor.h"
using namespace ZThread;
using namespace std;

class Car 
{
	//����
	Mutex lock;
	// the base of trehad cooperation
	Condition condition;
	//flag for wax
	bool waxOn;
public:
	Car() : condition(lock), waxOn(false) {}    //lock nad not wax
	//waxing 
	void waxed() 
	{
		Guard<Mutex> g(lock);
		waxOn = true; // Ready to buff
		condition.signal();
	}
	//liggting
	void buffed() 
	{
		Guard<Mutex> g(lock);
		waxOn = false; // Ready for another coat of wax
		//weaken 
		condition.signal();
	}
	void waitForWaxing() 
	{
		Guard<Mutex> g(lock);
		while(waxOn == false)
			condition.wait();
	}
	void waitForBuffing() 
	{
		Guard<Mutex> g(lock);
		while(waxOn == true)
			condition.wait();
	}
};

class WaxOn : public Runnable 
{
	CountedPtr<Car> car;
public:
	WaxOn(CountedPtr<Car>& c) : car(c) {}
	void run() 
	{
		try 
		{
			while(!Thread::interrupted()) 
			{
				cout << "Wax On!" << endl;
				Thread::sleep(2);
				car->waxed();
				car->waitForBuffing();
			}
		} 
		catch(Interrupted_Exception&  )  
		{
			cout << "Ending Wax On process" << endl;  
		}
	}
};

class WaxOff : public Runnable {
	CountedPtr<Car> car;
public:
	WaxOff(CountedPtr<Car>& c) : car(c) {}
	void run() {
		try {
			while(!Thread::interrupted()) {
				car->waitForWaxing();
				cout << "Wax Off!" << endl;
				Thread::sleep(200);
				car->buffed();
			}
		} catch(Interrupted_Exception&) { /* Exit */ }
		cout << "Ending Wax Off process" << endl;
	}
};

#include <time.h>
 
int main()
 {
     time_t timep;
    
     time(&timep); /*��ȡtime_t���͵ĵ�ǰʱ��*/
     /*��gmtime��time_t���͵�ʱ��ת��Ϊstruct tm���͵�ʱ�䰴������û�о���ʱ��ת����UTCʱ��
       Ȼ������asctimeת��Ϊ���ǳ����ĸ�ʽ Fri Jan 11 17:25:24 2008
     */
     printf("%s", asctime(gmtime(&timep)));
     return 0;
 }

int main2() 
{
	cout << "Press <Enter> to quit" << endl;
	try 
	{
		CountedPtr<Car> car(new Car);
		ThreadedExecutor executor;
		executor.execute(new WaxOff(car));
		executor.execute(new WaxOn(car));
		cin.get();
		executor.interrupt();
	} 
	catch(Synchronization_Exception& e) 
	{
		cerr << e.what() << endl; 
	}
	return 1;
}

