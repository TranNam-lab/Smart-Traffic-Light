#include<iostream>
#include<string>
#include<math.h> 
#include<iomanip>
using namespace std;

struct Vehicle {
	int type;					// loai xe  
	int priority;				// do uu tien 
}; 

struct Lane {
	int id;  					//lane: 0 = dong, 1 = tay, 2 = nam, 3 = bac 
	int vehicleCount;  			// so luong xe trong lane
    int capacity;                // suc chua quy uoc
	double density; 			// mat do xe trong lane
	
	int basePriority; 			// muc do uu tien cua lane
	int currenPriority; 		// muc do uu tien thoi gian thuc
	
	int waitingTime;				// thoi gian lane do da phai cho
	bool priorityVehicle;		//lane nay co xe uu tien hay khong?
    int highestVehiclePriority;   // Muc uu tien cao nhat cua xe trong huong
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
// 1. Ham nhap so luong xe va trang thai xe uu tien
void nhapSoLuongXe(intersection &ngaTu) {
    string tenHuong[4] = {"Dong", "Tay", "Nam", "Bac"};
    
    for (int i = 0; i < 4; i++) {
        cout << "Nhap so xe huong " << tenHuong[i] << ": ";
        cin >> ngaTu.lanes[i].vehicleCount;

        cout << "Co xe uu tien khong (1: Co, 0: Khong): ";
        cin >> ngaTu.lanes[i].priorityVehicle;
    }
}

// 2. Ham tinh mat do xe (gia su suc chua moi lan la 100 xe)
void tinhMatDoXe(intersection &ngaTu) {
    for (int i = 0; i < 4; i++) {
        ngaTu.lanes[i].density = (ngaTu.lanes[i].vehicleCount / 100.0) * 100.0;
    }
}

// 3. Ham tim lan co mat do cao nhat
int timLanMatDoLonNhat(intersection ngaTu) {
    int viTriMax = 0;
    for (int i = 1; i < 4; i++) {
        if (ngaTu.lanes[i].density > ngaTu.lanes[viTriMax].density) {
            viTriMax = i;
        }
    }
    return viTriMax;
}

int main() {
    intersection ngaTu;
    string tenHuong[4] = {"Dong", "Tay", "Nam", "Bac"};

    nhapSoLuongXe(ngaTu);
    tinhMatDoXe(ngaTu);

    // In mat do kem theo trang thai xe uu tien tren cung mot dong
    cout << "\n--- MAT DO VA XE UU TIEN CAC LAN ---\n";
    for (int i = 0; i < 4; i++) {
        cout << "Huong " << tenHuong[i] << ": " 
             << ngaTu.lanes[i].density << "%"
             << " - Xe uu tien: " 
             << (ngaTu.lanes[i].priorityVehicle ? "CO" : "khong") << "\n";
    }

    // In lan co mat do lon nhat
    int maxIdx = timLanMatDoLonNhat(ngaTu);
    cout << "\n=> Lan dong nhat la huong: " << tenHuong[maxIdx] 
         << " (" << ngaTu.lanes[maxIdx].density << "%)\n";

    return 0;
}


