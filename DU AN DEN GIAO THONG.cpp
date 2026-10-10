#include<iostream>
#include<string>
#include<math.h> 
using namespace std;

struct Vehicle {
	int id;						// ma dinh danh xe 
	int type;					// loai xe  
	int priority;				// do uu tien 
}; 

struct Lane {
	int id;  					//lane: 0 = dong, 1 = tay, 2 = nam, 3 = bac 
	int vehicleCount;  			// so luong xe trong lane
	double density; 			// mat do xe trong lane
	
	int basePriority; 			// muc do uu tien cua lane
	int currenPriority; 		// muc do uu tien thoi gian thuc
	
	int waitingTime;				// thoi gian lane do da phai cho
	bool priorityVehicle;		//lane nay co xe uu tien hay khong?
};

struct TrafficLight {
	int id;						//cot den: 0 = dong, 1 = tay, 2 = nam, 3 = bac
	
	int state;				 	//trang thi den hien tai  0 = xanh, 1 = do, 2 = vang 
	int remainingTime;			//thoi gian con lai cua trang thai den
	
	int greenTime;				//thoi gian xanh 
	int yellowTime;				//tuong tu
	int redTime;					//tuong tu
}; 

struct intersection {			//tong quan nga 4
	Lane lanes[4];				//co 4 lane
	TrafficLight lights[4];		//co 4 cot den
};









