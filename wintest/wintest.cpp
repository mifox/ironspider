// wintest.cpp : 定义控制台应用程序的入口点。
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
	//锁子
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
    
     time(&timep); /*获取time_t类型的当前时间*/
     /*用gmtime将time_t类型的时间转换为struct tm类型的时间按，／／没有经过时区转换的UTC时间
       然后再用asctime转换为我们常见的格式 Fri Jan 11 17:25:24 2008
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

