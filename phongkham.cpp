#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Lớp Người
class Nguoi {
protected:
    string ten;
    int tuoi;
    
public:
    Nguoi(string ten, int tuoi) : ten(ten), tuoi(tuoi) {}
    
    void hienThi() {
        cout << "Ten: " << ten << ", Tuoi: " << tuoi;
    }
    
    string getTen() { return ten; }
};

// Lớp Bệnh nhân
class BenhNhan : public Nguoi {
private:
    string maBN;
    
public:
    BenhNhan(string ten, int tuoi, string maBN) 
        : Nguoi(ten, tuoi), maBN(maBN) {}
    
    void hienThi() {
        cout << "Benh nhan: ";
        Nguoi::hienThi();
        cout << ", Ma: " << maBN << endl;
    }
    
    string getMaBN() { return maBN; }
};

// Lớp Bệnh nhân mãn tính
class BenhNhanManTinh : public BenhNhan {
private:
    string benhManTinh;
    
public:
    BenhNhanManTinh(string ten, int tuoi, string maBN, string benh)
        : BenhNhan(ten, tuoi, maBN), benhManTinh(benh) {}
    
    void hienThi() {
        cout << "Benh nhan man tinh: ";
        Nguoi::hienThi();
        cout << ", Ma: " << getMaBN() << ", Benh: " << benhManTinh << endl;
    }
};

// Lớp Bác sĩ
class BacSi : public Nguoi {
private:
    string chuyenKhoa;
    
public:
    BacSi(string ten, int tuoi, string chuyenKhoa)
        : Nguoi(ten, tuoi), chuyenKhoa(chuyenKhoa) {}
    
    void hienThi() {
        cout << "Bac si: ";
        Nguoi::hienThi();
        cout << ", Chuyen khoa: " << chuyenKhoa << endl;
    }
};

// Lớp Lịch hẹn
class LichHen {
private:
    string maHen;
    string ngay;
    BenhNhan* benhNhan;
    BacSi* bacSi;
    
public:
    LichHen(string ma, string ngay, BenhNhan* bn, BacSi* bs)
        : maHen(ma), ngay(ngay), benhNhan(bn), bacSi(bs) {}
    
    void hienThi() {
        cout << "Lich hen: " << maHen << " - Ngay: " << ngay << endl;
        cout << "  "; benhNhan->hienThi();
        cout << "  "; bacSi->hienThi();
        cout << "-----------------------" << endl;
    }
};

// Hàm main
int main() {
    cout << "=== HE THONG QUAN LY PHONG KHAM ===" << endl;
    
    // Tạo bác sĩ
    BacSi bs1("Nguyen Van A", 40, "Noi khoa");
    BacSi bs2("Tran Thi B", 35, "Nhi khoa");
    
    // Tạo bệnh nhân
    BenhNhan bn1("Le Van C", 25, "BN001");
    BenhNhan bn2("Pham Thi D", 30, "BN002");
    BenhNhanManTinh bn3("Tran Van E", 60, "BN003", "Tieu duong");
    
    // Tạo lịch hẹn
    LichHen lh1("LH001", "15/04/2024", &bn1, &bs1);
    LichHen lh2("LH002", "16/04/2024", &bn3, &bs2);
    
    // Hiển thị thông tin
    cout << "\n=== DANH SACH BAC SI ===" << endl;
    bs1.hienThi();
    bs2.hienThi();
    
    cout << "\n=== DANH SACH BENH NHAN ===" << endl;
    bn1.hienThi();
    bn2.hienThi();
    bn3.hienThi();
    
    cout << "\n=== DANH SACH LICH HEN ===" << endl;
    lh1.hienThi();
    lh2.hienThi();
    
    // Thêm lịch hẹn mới
    cout << "\n=== THEM LICH HEN MOI ===" << endl;
    LichHen lh3("LH003", "17/04/2024", &bn2, &bs1);
    lh3.hienThi();
    
    cout << "\n=== KET THUC CHUONG TRINH ===" << endl;
    return 0;
}