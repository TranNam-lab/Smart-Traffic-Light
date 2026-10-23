#include<iostream>
#include<string>
#include<math.h> 
#include<iomanip>
#include<ctime>       // time()
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

// [MOI] Hang so cho phan den giao thong
const int XANH_MAC_DINH   = 30;   // xanh khi 2 huong khong chenh lech
const int XANH_NHIEU      = 50;   // xanh cho huong dong hon (chenh > 60%)
const int XANH_IT         = 15;   // xanh cho huong vang hon
const int VANG            = 3;    // den vang
const int BAO_TRUOC       = 5;    // do con 5s truoc khi chuyen xanh cho xe uu tien / xanh con 5s sau khi xe uu tien qua

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

void doi1giay() {
    time_t batDau = time(0);       
    while (time(0) == batDau) {     
    }
}

// ================== [MOI] PHAN DEN GIAO THONG LIEN KET ==================
// Quy uoc "huong den": 0 = Dong-Tay (lan 0,1), 1 = Nam-Bac (lan 2,3)
const string TEN_HUONG_DEN[2] = {"Dong-Tay", "Nam-Bac"};

// Tong mat do cua 1 huong den
double matDoHuongDen(intersection &ngaTu, int h) {
    if (h == 0) return ngaTu.lanes[0].density + ngaTu.lanes[1].density;
    return ngaTu.lanes[2].density + ngaTu.lanes[3].density;
}

// Huong den co xe uu tien khong?
bool coXeUuTien(intersection &ngaTu, int h) {
    if (h == 0) return ngaTu.lanes[0].priorityVehicle || ngaTu.lanes[1].priorityVehicle;
    return ngaTu.lanes[2].priorityVehicle || ngaTu.lanes[3].priorityVehicle;
}

// Thoi gian xanh cua huong h: chenh lech mat do > 60% thi huong dong hon xanh nhieu hon
int tinhThoiGianXanh(intersection &ngaTu, int h) {
    double a = matDoHuongDen(ngaTu, h);
    double b = matDoHuongDen(ngaTu, 1 - h);
    if (a > b * 1.6) return XANH_NHIEU;     // huong nay dong hon
    if (b > a * 1.6) return XANH_IT;        // huong nay vang hon
    return XANH_MAC_DINH;
}

// Cap nhat 4 cot den theo trang thai hien tai (0 = xanh, 1 = do, 2 = vang)
void capNhatDen(intersection &ngaTu, int hXanh, int mau, int t) {
    for (int i = 0; i < 4; i++) {
        int huongCuaCot = (i < 2) ? 0 : 1;
        if (huongCuaCot == hXanh) {
            ngaTu.lights[i].state = mau;
            ngaTu.lights[i].remainingTime = t;
        } else {
            ngaTu.lights[i].state = 1;
            ngaTu.lights[i].remainingTime = t + (mau == 0 ? VANG : 0);
        }
    }
}

string tenMau(int mau) {
    if (mau == 0) return "XANH";
    if (mau == 2) return "VANG";
    return "DO";
}

void chayDen(intersection &ngaTu, int huongDauTien) {
    cout << "Xanh: " << XANH_MAC_DINH << "s (hoac " << XANH_NHIEU << "s/" << XANH_IT
         << "s neu chenh mat do > 60%), vang: " << VANG << "s\n" << endl;

    bool coXe[2] = { coXeUuTien(ngaTu, 0), coXeUuTien(ngaTu, 1) };   // xe uu tien dang cho / dang di qua

    int g = huongDauTien;                    // huong dang xanh
    int mau = 0;                             // 0 = xanh, 2 = vang
    int t = tinhThoiGianXanh(ngaTu, g);      // dem nguoc

    while (true) {
        int d = 1 - g;                       // huong dang do

        // (a) Xe uu tien o huong dang xanh: giu den xanh den khi xe qua
        if (coXe[g] && mau == 0) {
            capNhatDen(ngaTu, g, 0, t);
            cout << TEN_HUONG_DEN[g]
                 << ": XANH (giu den cho xe uu tien)  |  "
                 << TEN_HUONG_DEN[d] << ": DO" << endl;

            int daQua;
            cout << "Xe uu tien da qua het nga tu chua? (1: Roi, 0: Chua): ";
            cin >> daQua;

            if (daQua == 1) {
                coXe[g] = false;  // khong con xe uu tien cho
            }
            // Neu nhap 0, lap lai va tiep tuc giu den xanh
            continue;
        }

        // (b) Xe uu tien o huong DANG DO: giam dem nguoc do xuong con 5s
        if (coXe[d]) {
            int gioiHan = (mau == 0) ? (BAO_TRUOC - VANG) : BAO_TRUOC;   // 2s xanh + 3s vang = 5s
            if (t > gioiHan) t = gioiHan;
        }

        // Hien thi
        int tDo = t + (mau == 0 ? VANG : 0);
        capNhatDen(ngaTu, g, mau, t);
        cout << TEN_HUONG_DEN[g] << ": " << tenMau(mau) << " " << t << "s  |  "
             << TEN_HUONG_DEN[d] << ": DO " << tDo << "s" << endl;
        doi1giay();

        // Dem nguoc va chuyen trang thai
        t--;
        if (t <= 0) {
            if (mau == 0) {              // xanh -> vang
                mau = 2;
                t = VANG;
            } else {                     // vang -> doi huong, xanh theo mat do
                g = d;
                mau = 0;
                t = tinhThoiGianXanh(ngaTu, g);
            }
        }
    }
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
         << " (Diem uu tien = " << ngaTu.lanes[lanUuTien].currenPriority << ")\n\n";

    // [SUA] chay den lien ket: huong cua lan uu tien nhat duoc xanh dau tien
    int huongDauTien = (lanUuTien < 2) ? 0 : 1;
    chayDen(ngaTu, huongDauTien);
    return 0;
}
