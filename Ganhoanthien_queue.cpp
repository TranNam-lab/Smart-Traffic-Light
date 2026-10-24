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
	int capacity;               // suc chua quy uoc
	double density; 			// mat do xe trong lane

	int basePriority; 			// muc do uu tien cua lane
	int currenPriority; 		// muc do uu tien thoi gian thuc

	int waitingTime;			// thoi gian lane do da phai cho
	bool priorityVehicle;		// lane nay co xe uu tien hay khong?
	int highestVehiclePriority; // Muc uu tien cao nhat cua xe trong huong
};

struct TrafficLight {
	int id;						//cot den: 0 = dong, 1 = tay, 2 = nam, 3 = bac

	int state;				 	//trang thai den hien tai  0 = xanh, 1 = do, 2 = vang
	int remainingTime;			//thoi gian con lai cua trang thai den

	int greenTime;				//thoi gian xanh
	int yellowTime;				//tuong tu
	int redTime;				//tuong tu
};

struct intersection {			//tong quan nga 4
	Lane lanes[4];				//co 4 lane
	TrafficLight lights[4];		//co 4 cot den
};

// ===================== QUEUE (HANG DOI) =====================
// Hang doi cai dat bang danh sach lien ket don (FIFO), theo bai giang.
// Dung de quan li THU TU cac huong den duoc cap den xanh.
struct QNode {
	int data;					// ma huong den (0 = Dong-Tay, 1 = Nam-Bac)
	QNode *next;
};

struct Queue {
	QNode *head;				// dau hang (se duoc lay ra truoc)
	QNode *tail;				// cuoi hang (noi them vao)
	int size;
};

void initQueue(Queue &q) {
	q.head = NULL;
	q.tail = NULL;
	q.size = 0;
}

bool isEmpty(Queue &q) {
	return (q.size == 0);
}

// Them phan tu vao cuoi hang doi
void enqueue(Queue &q, int v) {
	QNode *p = new QNode;
	p->data = v;
	p->next = NULL;
	if (q.size == 0) {
		q.head = p;
		q.tail = p;
	} else {
		q.tail->next = p;
		q.tail = p;
	}
	q.size++;
}

// Lay phan tu o dau hang doi ra (dieu kien: hang doi khong rong)
int dequeue(Queue &q) {
	QNode *p = q.head;
	int x = p->data;
	q.head = p->next;
	if (q.head == NULL)
		q.tail = NULL;
	delete p;
	q.size--;
	return x;
}

// Xem phan tu dau hang doi nhung KHONG lay ra (dieu kien: hang doi khong rong)
int front(Queue &q) {
	return q.head->data;
}

// Xoa toan bo hang doi
void clearQueue(Queue &q) {
	while (!isEmpty(q))
		dequeue(q);
}

// ===================== HANG SO DEN GIAO THONG =====================
const int XANH_MAC_DINH   = 30;   // xanh khi 2 huong khong chenh lech
const int XANH_NHIEU      = 50;   // xanh cho huong dong hon (chenh > 60%)
const int XANH_IT         = 15;   // xanh cho huong vang hon
const int VANG            = 3;    // den vang
const int BAO_TRUOC       = 5;    // do con 5s truoc khi chuyen xanh cho xe uu tien / xanh con 5s sau khi xe uu tien qua

// Ham phu: doi loai xe uu tien (1..5) thanh diem uu tien
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

// 4. Tinh muc uu tien thoi gian thuc cua tung lan
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

// ================== PHAN DEN GIAO THONG LIEN KET ==================
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

// Muc uu tien cua 1 huong den = muc uu tien lon nhat trong 2 lan cua huong do
int diemUuTienHuong(intersection &ngaTu, int h) {
	int a = (h == 0) ? 0 : 2;
	int b = a + 1;
	return (ngaTu.lanes[a].currenPriority > ngaTu.lanes[b].currenPriority)
	       ? ngaTu.lanes[a].currenPriority : ngaTu.lanes[b].currenPriority;
}

// [QUEUE] Xep thu tu cac huong den vao hang doi:
// huong co muc uu tien cao hon dung truoc (bang nhau thi huong dong hon dung truoc)
void xepHangDoi(intersection &ngaTu, Queue &q) {
	int p0 = diemUuTienHuong(ngaTu, 0);
	int p1 = diemUuTienHuong(ngaTu, 1);

	int truoc;
	if (p0 > p1) truoc = 0;
	else if (p1 > p0) truoc = 1;
	else truoc = (matDoHuongDen(ngaTu, 0) >= matDoHuongDen(ngaTu, 1)) ? 0 : 1;

	clearQueue(q);
	enqueue(q, truoc);
	enqueue(q, 1 - truoc);
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

// [QUEUE] Dieu khien den: moi vong lap lay 1 huong o DAU hang doi ra cho xanh,
// chay xong (xanh -> vang) thi dua huong do tro lai CUOI hang doi.
void chayDen(intersection &ngaTu, Queue &q) {
	cout << "Xanh: " << XANH_MAC_DINH << "s (hoac " << XANH_NHIEU << "s/" << XANH_IT
	     << "s neu chenh mat do > 60%), vang: " << VANG << "s\n" << endl;

	bool coXe[2] = { coXeUuTien(ngaTu, 0), coXeUuTien(ngaTu, 1) };   // xe uu tien dang cho / dang di qua

	while (true) {
		int g = dequeue(q);                  // huong duoc xanh = dau hang doi
		int d = front(q);                    // huong dang do   = huong ke tiep trong hang doi

		int mau = 0;                         // 0 = xanh, 2 = vang
		int t = tinhThoiGianXanh(ngaTu, g);  // dem nguoc

		while (true) {
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
				continue;
			}

			// (b) Xe uu tien o huong DANG DO: giam dem nguoc xuong con 5s
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
				} else {                     // vang xong -> ket thuc luot cua huong g
					break;
				}
			}
		}

		enqueue(q, g);                       // huong g xuong CUOI hang doi, cho den luot sau
	}
}

int main() {
	intersection ngaTu;
	Queue hangDoi;
	string tenHuong[4] = {"Dong", "Tay", "Nam", "Bac"};

	initQueue(hangDoi);

	nhapSoLuongXe(ngaTu);
	tinhMatDoXe(ngaTu);
	tinhDoUuTien(ngaTu);        // can chay SAU tinhMatDoXe

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

	// In muc do uu tien thoi gian thuc cua tung lan
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

	// [QUEUE] Xep thu tu cac huong den vao hang doi roi cho chay den
	xepHangDoi(ngaTu, hangDoi);
	cout << "\n=> Thu tu cap den xanh (hang doi): "
	     << TEN_HUONG_DEN[front(hangDoi)] << " truoc, "
	     << TEN_HUONG_DEN[1 - front(hangDoi)] << " sau\n\n";

	chayDen(ngaTu, hangDoi);
	return 0;
}
