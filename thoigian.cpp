#include<iostream>
#include<string>
#include<math.h> 
#include<iomanip>
using namespace std;

struct Vehicle {
	int type;					// loai xe: L1 = xe 2 banh, L2 = xe oto, L3 = xe bus/xe tai  
	int priority;				// do uu tien
	int quantity;				// so luong xe
	struct vehicle *left;		// luu dia chi truy cap xe loai 2
	struct vehicle *right;		// luu dia chi truy cap xe loai 3
}; 

struct Lane {
	Vehicle *lead;
	void init();
	int id;  					//lane: 0 = dong, 1 = tay, 2 = nam, 3 = bac 
	int vehicleCount;  			// so luong xe trong lane
	Vehicle *input(int soLuong, int loaiXe);

    int capacity;                // suc chua quy uoc
	double density; 			// mat do xe trong lane
	
	int basePriority; 			// muc do uu tien cua lane
	int currenPriority; 		// muc do uu tien thoi gian thuc
	
	int waitingTime(intersection &ngaTu,TrafficLight output, int id);				// thoi gian lane do da phai cho
	bool priorityVehicle;		//lane nay co xe uu tien hay khong?
    int highestVehiclePriority;   // Muc uu tien cao nhat cua xe trong huong
    
};

void Lane::init() {
        lead = NULL;
        vehicleCount = 0;
        capacity = 100;
        density = 0.0;
        priorityVehicle = false;
    }

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

Vehicle *Lane::input(int soLuong, int loaiXe) {
	Vehicle *p = new Vehicle();
	p->type = loaiXe;
	p->quantity = soLuong;
	p->left = NULL;
	p->right = NULL;
	switch(loaiXe){
		case 1:
			p->priority = 1;
			lead = p;
			break;
		case 2:
			p->priority = 4;
			if(lead != NULL)
			lead->left = p;
			break;
		case 3:
			p->priority = 10;
			if(lead != NULL)
			lead->right = p;
			break;
		default:
			delete p;
			break;	
	}
}
// 1. Ham nhap so luong xe va trang thai xe uu tien
void nhapSoLuongXe(intersection &ngaTu, Vehicle *lead) {
    string tenHuong[4] = {"Dong", "Tay", "Nam", "Bac"};
    int temp;
    for (int i = 0; i < 4; i++) {
        cout << "- Nhap so xe huong " << tenHuong[i] << ": " << endl;
        cout << "	+ Xe 2 banh: ";
        cin >> temp;
		ngaTu.lanes[i].input(temp, 1);
        cout << "	+ Xe oto: ";
        cin >> temp;
        ngaTu.lanes[i].input(temp, 2);
        cout << "	+ Xe bus / Xe tai: ";
        cin >> temp;
		ngaTu.lanes[i].input(temp, 3);
        cout << "- Co xe uu tien khong (1: Co, 0: Khong): ";
        cin >> ngaTu.lanes[i].priorityVehicle;
        cout << endl;
    }
}

// 2. Ham tinh mat do xe (gia su suc chua moi lan la 100 xe)
void tinhMatDoXe(intersection &ngaTu) {
    for (int i = 0; i < 4; i++) {
    	int a = ngaTu.lanes[i].lead->quantity;
    	int b = ngaTu.lanes[i].lead->left->quantity;
    	int c = ngaTu.lanes[i].lead->right->quantity;
    	
        ngaTu.lanes[i].density = (double)(a + 4*b + 10*c) / 15 ;
    }
}

// 3. Ham tim lan co mat do cao nhat
int timLanMatDoLonNhat(intersection &ngaTu) {
    int viTriMax = 0;
    for (int i = 1; i < 4; i++) {
        if (ngaTu.lanes[i].density > ngaTu.lanes[viTriMax].density) {
            viTriMax = i;
        }
    }
    return viTriMax;
}
// 4. Ham tinh thoi gian den tin hieu
int Lane::waitingTime(intersection &ngaTu, TrafficLight output, int id){
	double temp = ngaTu.lanes[id].density;
	int time;
	if(temp < 10 && temp >= 0)
		time = 30 + pow(temp, 2.12);
	else time = 160;
	switch(id){
		case 0 || 1:
			ngaTu.lights[0].greenTime = ngaTu.lights[1].greenTime = time;
			ngaTu.lights[2].redTime = ngaTu.lights[3].redTime = time;
			break;
		case 2 || 3:
			ngaTu.lights[0].redTime = ngaTu.lights[1].redTime = time;
			ngaTu.lights[2].greenTime = ngaTu.lights[3].greenTime = time;
			break;
		default:
			break;
	}
	if(temp < 5){
		for(i = 0 ; i < 4 ; i++)
			ngaTu.lights[i].yellowTime = 5;
	}

	else{
		for(i = 0 ; i < 4 ; i++)
			ngaTu.lights[i].yellowTime = 15;
	}
}
int main() {
    intersection ngaTu;
    Lane xe;
    xe.init();
    string tenHuong[4] = {"Dong", "Tay", "Nam", "Bac"};

    nhapSoLuongXe(ngaTu, xe.lead);
    tinhMatDoXe(ngaTu);
	
    cout << "\n--- MAT DO VA XE UU TIEN CAC LAN ---\n";
    for (int i = 0; i < 4; i++) {
        cout << "Huong " << tenHuong[i] << ": " 
             << ngaTu.lanes[i].density << "%"
             << " - Xe uu tien: " 
             << (ngaTu.lanes[i].priorityVehicle ? "CO" : "khong") << "\n";
    }

    // In lan co mat do lon nhat
    int maxIdx = timLanMatDoLonNhat(ngaTu);
    cout << "Lan dong nhat la huong: " << tenHuong[maxIdx] << " (" << ngaTu.lanes[maxIdx].density << ")\n";

    return 0;
}


