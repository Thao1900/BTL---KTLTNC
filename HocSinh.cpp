// ========================== STUDENT ==========================
class Student : public Person {
private:
    string mssv;
    string lop;
    string ngaySinh;
    string gioiTinh;
    int tuoi;
    string soDienThoai;

    double diemThi;
    double diemTB;
    int thoiGianLamBai; // phút

public:
    Student() {
        mssv = "";
        lop = "";
        ngaySinh = "";
        gioiTinh = "";
        tuoi = 0;
        soDienThoai = "";
        diemThi = 0;
        diemTB = 0;
        thoiGianLamBai = 0;
    }

    // ================= LOGIN =================
    bool login(const string& user, const string& pass) {
        return (user == username && pass == password);
    }

    // ================= RESET PASSWORD =================
    void resetPassword() {
        string oldPass, newPass;
        cout << "Nhap mat khau hien tai: ";
        cin >> oldPass;

        if (oldPass != password) {
            cout << "Sai mat khau!\n";
            return;
        }

        cout << "Nhap mat khau moi: ";
        cin >> newPass;

        if (newPass.length() < 8) {
            cout << "Mat khau phai >= 8 ky tu\n";
            return;
        }

        password = newPass;
        cout << "Doi mat khau thanh cong!\n";
    }

    // ================= NHAP THONG TIN =================
    void nhapThongTin() {
        cout << "Nhap MSSV: "; 
        getline(cin, mssv);
        cout << "Nhap lop: "; 
        getline(cin, lop);
        cout << "Nhap ngay sinh: "; 
        getline(cin, ngaySinh);
        cout << "Nhap gioi tinh: "; 
        getline(cin, gioiTinh);
        cout << "Nhap tuoi: "; cin >> tuoi;
        cin.ignore();
        cout << "Nhap so dien thoai: "; 
        getline(cin, soDienThoai);
    }

    // ================= XEM THONG TIN =================
    void xemThongTin() {
        cout << "\n===== THONG TIN CA NHAN =====\n";
        cout << "MSSV: " << mssv << endl;
        cout << "Ho ten: " << fullName << endl;
        cout << "Lop: " << lop << endl;
        cout << "Ngay sinh: " << ngaySinh << endl;
        cout << "Gioi tinh: " << gioiTinh << endl;
        cout << "Tuoi: " << tuoi << endl;
        cout << "SDT: " << soDienThoai << endl;
    }

    // ================= LAM BAI THI =================

    // ================= XEM KET QUA =================
    void xemKetQua() {
        cout << "\n===== KET QUA =====\n";
        cout << "Diem thi: " << diemThi << endl;
    }

    // ================= DIEM TRUNG BINH =================
    void xemDiemTB() {
        cout << "Diem trung binh: " << diemTB << endl;
    }

    // ================= THOI GIAN LAM BAI =================
    void xemThoiGianLamBai() {
        cout << "Thoi gian lam bai: " << thoiGianLamBai << " phut\n";
    }

    // ================= XEP LOAI =================
    void xepLoai() {
        cout << "\n===== XEP LOAI =====\n";
        if (diemTB > 10 || diemTB < 0)
            cout << "Loi!";
        else if (diemTB >= 9)
            cout << "Hoc sinh Xuat sac\n";
        else if (diemTB >= 8)
            cout << "Hoc sinh Gioi\n";
        else if (diemTB >= 7)
            cout << "Hoc sinh Kha\n";
        else if (diemTB >= 5)
            cout << "Hoc sinh Trung binh\n";
        else if (diemTB >= 4)
            cout << "Hoc sinh Yeu\n";
        else
            cout << "Hoc sinh kem";
    }

    // ================= TINH DIEM CAN DAT =================
    void diemCanDat() {
        cout << "\n===== DIEM CAN DAT =====\n";

        if (diemTB >= 9)
            cout << "Ban da la hoc sinh Xuat sac\n";
        else
            cout << "Can >= " << 9 - diemTB << " diem nua de len Xuat sac\n";
        if (diemTB >= 8)
            cout << "Can >= " << 8 - diemTB << " diem nua de len Gioi\n";
    }
};