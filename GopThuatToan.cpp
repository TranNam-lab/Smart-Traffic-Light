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

// Ham phu: doi loai xe uu tien (1..5) thanh diem uu tien
// (dat truoc ham nhapSoLuongXe vi ham do can goi no)
int layDiemUuTien(int loaiXe) {
    if (loaiXe == 1) return 2;      // xe can ve            : +2
    if (loaiXe == 2) return 3;      // doan xe tang         : +3
    if (loaiXe == 3) return 4;      // xe cuu thuong        : +4
    if (loaiXe == 4) return 5;      // xe quan su/cong an (co dan duong + coi hu) : +5
    if (loaiXe == 5) return 6;      // xe cuu hoa           : +6
    return 0;                       // loai khac: khong cong diem
}

// 1. Ham nhap so luong xe, trang thai xe uu tien va loai xe uu tien
void nhapSoLuongXe(intersection &ngaTu) {
    string tenHuong[4] = {"Dong", "Tay", "Nam", "Bac"};
    
    for (int i = 0; i < 4; i++) {
        cout << "\nNhap so xe huong " << tenHuong[i] << ": ";
        cin >> ngaTu.lanes[i].vehicleCount;

        cout << "Co xe uu tien khong (1: Co, 0: Khong): ";
        cin >> ngaTu.lanes[i].priorityVehicle;

        ngaTu.lanes[i].highestVehiclePriority = 0;      // mac dinh: khong co xe uu tien

        //  Neu co xe uu tien thi nhap loai xe.
        // Neu khong co thi bo qua va vong for tu chuyen sang lan tiep theo.
        if (ngaTu.lanes[i].priorityVehicle) {
            int loaiXe;
            cout << "Chon loai xe uu tien:\n";
            cout << "  1. Xe can ve (+2)\n";
            cout << "  2. Doan xe tang (+3)\n";
            cout << "  3. Xe cuu thuong (+4)\n";
            cout << "  4. Xe quan su/cong an co dan duong va coi hu (+5)\n";
            cout << "  5. Xe cuu hoa (+6)\n";
            do {
                cout << "Nhap lua chon (1-5): ";
                cin >> loaiXe;
            } while (loaiXe < 1 || loaiXe > 5);         // nhap sai thi nhap lai

            ngaTu.lanes[i].highestVehiclePriority = layDiemUuTien(loaiXe);
        }
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

//  4. Tinh muc uu tien thoi gian thuc cua tung lan
void tinhDoUuTien(intersection &ngaTu) {
    // Buoc 1: uu tien ban dau = diem cua xe uu tien (neu khong co xe thi = 0)
    for (int i = 0; i < 4; i++) {
        ngaTu.lanes[i].currenPriority = ngaTu.lanes[i].highestVehiclePriority;
    }

    // Buoc 2: gop mat do 2 huong
    double matDoDongTay = ngaTu.lanes[0].density + ngaTu.lanes[1].density;   // dong + tay
    double matDoBacNam  = ngaTu.lanes[2].density + ngaTu.lanes[3].density;   // nam + bac

    // Buoc 3: huong nao dong hon huong con lai 60% tro len thi +1 cho 2 lan cua huong do
    if (matDoDongTay > matDoBacNam * 1.6) {
        ngaTu.lanes[0].currenPriority += 1;
        ngaTu.lanes[1].currenPriority += 1;
    }
    else if (matDoBacNam > matDoDongTay * 1.6) {
        ngaTu.lanes[2].currenPriority += 1;
        ngaTu.lanes[3].currenPriority += 1;
    }
}
// 5. Ham tim lan co muc do uu tien nhat
int timLanUuTienNhat(intersection ngaTu) {
    int viTriMax = 0;
    for (int i = 1; i < 4; i++) {
        if (ngaTu.lanes[i].currenPriority > ngaTu.lanes[viTriMax].currenPriority) {
            viTriMax = i;
        }
    }
    return viTriMax;
}
int main() {
    intersection ngaTu;
    string tenHuong[4] = {"Dong", "Tay", "Nam", "Bac"};

    nhapSoLuongXe(ngaTu);       // [SUA] ham nay gio da bao gom nhap loai xe uu tien
    tinhMatDoXe(ngaTu);
    tinhDoUuTien(ngaTu);        // [MOI] tinh muc uu tien (can chay SAU tinhMatDoXe)

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

    //  In muc do uu tien thoi gian thuc cua tung lan
    cout << "\n--- MUC DO UU TIEN CAC LAN ---\n";
    for (int i = 0; i < 4; i++) {
        cout << "Huong " << tenHuong[i] << ": uu tien = " 
             << ngaTu.lanes[i].currenPriority << "\n";
    }
    // In ket luan lan co muc do uu tien nhat
    int lanUuTien = timLanUuTienNhat(ngaTu);
    cout << "\n=> LAN CO MUC DO UU TIEN LON NHAT LA HUONG: " 
         << tenHuong[lanUuTien] 
         << " (Diem uu tien = " << ngaTu.lanes[lanUuTien].currenPriority << ")\n";
    return 0;
}
