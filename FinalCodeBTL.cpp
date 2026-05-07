#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <commdlg.h>
#include <algorithm>
#include <limits>
#include <cctype>
#include <ctime>
#include <limits>
using namespace std;


//======Tai file======
string taiFile() {

    OPENFILENAME ofn;

    char szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);

    ofn.lpstrFilter =
        "Text Files\0*.txt\0"
        "All Files\0*.*\0";

    ofn.nFilterIndex = 1;

    ofn.Flags =
        OFN_PATHMUSTEXIST |
        OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        return string(szFile);
    }

    return "";
}

// ========================== CAU HOI ==========================
struct CauHoi {
    int id;
    string noiDung;
    string dapAn[4];
    int dapAnDung;
};
//===================== Ham Nhap ==========================
string inputLine(const string& prompt) {
    string s;
    while (true) {
        cout << prompt;
        getline(cin, s);
        if (!s.empty()) return s;
        cout << "Loi: khong duoc de trong.\n";
    };
}
template <typename T>
T nhapSo(const string& thongBao) {
    T value;
    while (true) {
        cout << thongBao;
        if (cin >> value) {
            cin.ignore(1000, '\n');
            return value;
        }
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Nhap sai, thu lai!\n";
    }
}
// ========================== DE THI ==========================
struct deThi {
    int id;
    int khoa;
    string tenMon;
    vector<CauHoi> danhSachCauHoi;
    CauHoi ch;
    int thoiGianLamBai;
    deThi() : id(0), khoa(0), tenMon(""), thoiGianLamBai(0) {}
    void themCauHoi() {
        ch.id = nhapSo<int>("ID: ");
        cout << "Noi dung: ";
        getline(cin, ch.noiDung);
        for (int i = 0; i < 4; i++) {
            cout << "Lua chon " << i + 1 << ": ";
            getline(cin, ch.dapAn[i]);
        }
        do {
            ch.dapAnDung = nhapSo<int>("dapAnDung ");
        } while ((ch.dapAnDung < 0) || (ch.dapAnDung >= 4));
        danhSachCauHoi.push_back(ch);
    }

    void hienThiCauHoi() {
        for (auto& ch : danhSachCauHoi) {
            cout << ch.id << ". " << ch.noiDung << endl;
        }
    }
};
// ========================== KET QUA ==========================
struct KetQua {
    int idSV;
    string tenSV;
    double diem;
    string mon;
    int thoiGian;
};

// ========================== CLASS INFO ==========================
struct ClassInfo {
    int id;
    string className;
    int teacherId;      // 0 = chua gan
    int studentCount;
};
// ========================== PERSON ==========================
class Person {
protected:
    int id;
    string username;
    string password;
    string fullName;

public:
    Person() : id(0), username(""), password(""), fullName("") {}

    Person(int id, const string& username, const string& password, const string& fullName)
        : id(id), username(username), password(password), fullName(fullName) {
    }

    virtual ~Person() {}
    virtual string getRole() const = 0;
    // ================= LOGIN =================
    virtual bool login(const string& u, const string& p) const {
        return u == username && p == password;
    }

    int getId() const { return id; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getFullName() const { return fullName; }
    void setId(int newId) { id = newId; }
    void setUsername(const string& newUsername) { username = newUsername; }
    void setPassword(const string& newPassword) { password = newPassword; }
    void setFullName(const string& newFullName) { fullName = newFullName; }
};

// ========================== ADMIN ==========================
class Admin : public Person {
public:
    Admin() : Person(1, "admin", "CoDungXinhDep", "System Admin") {}

    Admin(int id, string u, string p, string ten)
        : Person(id, u, p, ten) {
    }
    string getRole() const override {
        return "Admin";
    }
    // ================= HIEN THI =================
    void hienThiThongTin() const {
        cout << "[ADMIN] " << fullName << endl;
    }

    // ================= FILE =================
    string chuyenThanhChuoiFile() const {
        return to_string(id) + "|" + username + "|" + password + "|" + fullName;
    }
};

// ========================== STUDENT ==========================
class HocSinh : public Person {
private:
    string lop;
    string gioitinh;
    string birthDay;
    string sdt;
    vector<KetQua> danhSachKetQua;
    double diemTB;
    int teacherId;

public:
    HocSinh() : Person() {
        lop = "";
        gioitinh = "";
        birthDay = "";
        sdt = "";
        diemTB = 0.0;
        teacherId = 0;
    }
    HocSinh(int id, string userN, string passW, string ten,
        string lop, string gioitinh, string birthDay, string sdt, double diemTB, int teacherId)
        : Person(id, userN, passW, ten), lop(lop), gioitinh(gioitinh), birthDay(birthDay), sdt(sdt), diemTB(diemTB), teacherId(teacherId) {
    }
    string getRole() const override {
        return "HocSinh";
    }

    int layMaGV() const { return teacherId; }
    double layDiem() const { return diemTB; }
    void datDiem(double d) { diemTB = d; }

    void setLop(const string& lop) { this->lop = lop; }
    void setGioiTinh(const string& gioitinh) { this->gioitinh = gioitinh; }
    void setBirthDay(const string& birthDay) { this->birthDay = birthDay; }
    void setSdt(const string& sdt) { this->sdt = sdt; }
    void setDiemTB(double diemTB) { this->diemTB = diemTB; }
    void setTeacherId(int teacherId) { this->teacherId = teacherId; }
    void setDanhSachKetQua(const vector<KetQua>& ds) { danhSachKetQua = ds; }

    void hienThiThongTin() const {
        cout <<"==========Thong Tin Hoc Sinh===========" 
            << "[HS] " << fullName
            << " | Lop: " << lop
            << " | Gioi tinh: " << gioitinh
            << " | Birth: " << birthDay
            << " | SDT: " << sdt
            << " | Diem TB: " << diemTB
            << " | TeacherID: " << teacherId;
    }
    void datLaiMatKhau() {
        string mk;
        cout << "Nhap MK moi: ";
        getline(cin, mk);
        password = mk;
    }
    double traLoiCauHoi(vector<CauHoi> danhSachCauHoi,
        time_t start,
        int thoiGian) const {

        int n;
        double diemThi = 0;
        for (auto& ch : danhSachCauHoi) {
            time_t now = time(0);
            if (difftime(now, start) >= thoiGian) {
                cout << "Het gio lam bai!\n";
                break;
            }
            cout << ch.id << ". " << ch.noiDung << endl;
            n = nhapSo<int>("Dap an ban chon: ");
            if (n == ch.dapAnDung) {
                diemThi += 10.0 / danhSachCauHoi.size();
            }
        }
        return diemThi;
    }
    //Lam bai thi
    void lamBaiThi(const vector<deThi>& maDe) {
        string mon;
        cin.ignore();
        mon = inputLine("Nhap mon muon lam bai thi: ");
        double n;
        for (auto& de : maDe) {
            if (de.tenMon == mon) {
                if (de.khoa == 0) {
                    time_t start = time(0);
                    cout << "Bat dau lam bai mon: " << mon << endl;
                    cout << "Thoi gian lam bai: "
                        << de.thoiGianLamBai
                        << " giay\n";
                    n = traLoiCauHoi(de.danhSachCauHoi,
                        start,
                        de.thoiGianLamBai);
                    time_t stop = time(0);
                    KetQua kq;
                    kq.idSV = this->id;
                    kq.tenSV = this->fullName;
                    kq.diem = n;
                    kq.mon = mon;
                    kq.thoiGian = (int)difftime(stop, start);
                    this->danhSachKetQua.push_back(kq);
                }
                else
                    cout << "De chua mo khoa!";
            }
        }
    }
    void hienThiDanhSachKetQua(const vector<KetQua>& ds) const {
        if (ds.empty()) {
            cout << "Chua co ket qua nao!\n";
            return;
        }

        for (const auto& kq : ds) {
            cout << "Ten SV: " << kq.tenSV
                << " | Mon: " << kq.mon
                << " | Diem: " << kq.diem
                << " | Thoi gian: " << kq.thoiGian << " giay"
                << endl;
        }
    }
    string chuyenThanhChuoiFile() const {
        return to_string(id) + "|" + username + "|" + password + "|" +
            fullName + "|" + lop + "|" + to_string(diemTB) + "|" + to_string(teacherId);
    }

    //========Menu Hoc Sinh=======
    void menu(const vector<deThi>& danhSachDe) {
        int choice;
        do {
            cout << "=================Menu Hoc Sinh==============>>";
            cout << "1. Lam bai thi\n";
            cout << "2. Xem ket qua thi\n";
            cout << "3. Xem thong tin ca nhan\n";
            cout << "4. Dat lai mat khau\n";
            cout << "0. Dang Xuat";
            cin >> choice;
            cin.ignore();
            switch (choice) {
            case 1: lamBaiThi(danhSachDe);
                break;
            case 2: hienThiDanhSachKetQua(danhSachKetQua);
                break;
            case 3: hienThiThongTin();
                break;
            case 4: datLaiMatKhau();
                break;
            case 0:
                cout << "Dang xuat thanh cong!";
                break;
            }
        } while (choice != 0);

    }

};
// ========================== QUAN LY THI ==========================
class QuanLyThi {
private:
    string tenMon;
    deThi de;
    vector<KetQua> danhSachKetQua;
    vector<deThi> maDe;
public:
    vector<deThi>& getMaDe() {
        return maDe;
    }
    QuanLyThi() {
        maDe = {};
        tenMon = "Chua dat ten";
    }
    void thietLapDeThi() {
        de.id = nhapSo<int>("Ma de:");
        cout << "Ten mon: ";
        getline(cin, de.tenMon);
        de.thoiGianLamBai = nhapSo<int>("Thoi gian lam bai: ");
        int m;
        cout << "Nhap so cau hoi cua de: ";
        cin >> m;
        for (int i = 0; i < m; i++) {
            de.themCauHoi();
        }
        maDe.push_back(de);
    }
    void batDauThi() {
        cout << "Vui long chon de thi de bat dau\n";
        cin.ignore();
        getline(cin, tenMon);
        int n;
        cout << "Nhap 1 de bat dau thoi gian thi\n";
        cout << "Nhap 0 de khoa bai thi\n";
        while (!(cin >> n) && (n != 0) && (n != 1)) {
            cout << "Vui long nhap lai!";
            cin >> n;
        }
        for (auto& de : maDe) {
            if (de.tenMon == tenMon)
                de.khoa = n;
        }
    }
    void nhapKetQua() {
        KetQua kq;
        kq.idSV = nhapSo<int>("ID SV: ");
        cout << "Ten: ";
        getline(cin, kq.tenSV);
        cout << "Diem: ";
        cin >> kq.diem;
        kq.thoiGian = nhapSo<int>("Thoi gian: ");
        danhSachKetQua.push_back(kq);
    }

    void hienThiXepHang() {
        sort(danhSachKetQua.begin(), danhSachKetQua.end(),
            [](KetQua a, KetQua b) {
                return a.diem > b.diem;
            });

        for (int i = 0; i < danhSachKetQua.size(); i++) {
            cout << i + 1 << ". " << danhSachKetQua[i].tenSV
                << " | Diem: " << danhSachKetQua[i].diem << endl;
        }
    }

    void menu() {
        int ch;
        do {
            cout << "\n=== MENU THI ===\n";
            cout << "1. Cai dat de\n";
            cout << "2. Bat dau thi\n";
            cout << "3. Nhap ket qua\n";
            cout << "4. Xep hang\n";
            cout << "0. Thoat\n";
            cin >> ch;
            cin.ignore();

            switch (ch) {
            case 1: thietLapDeThi(); break;
            case 2: batDauThi(); break;
            case 3: nhapKetQua(); break;
            case 4: hienThiXepHang(); break;
            }
        } while (ch != 0);
    }
};



// ========================== TEACHER ==========================
class GiaoVien : public Person {
private:
    string mon;

public:
    GiaoVien() : Person() {};

    GiaoVien(int id, string u, string p, string ten, string mon)
        : Person(id, u, p, ten), mon(mon) {
    }
    string getRole() const override {
        return "GiaoVien";
    }

    void hienThiThongTin() const {
        cout << "[GV] " << fullName << " | Mon: " << mon << endl;
    }

    string chuyenThanhChuoiFile() const {
        return to_string(id) + "|" + username + "|" + password + "|" + fullName + "|" + mon;
    }

    void datLaiMatKhau() {
        string mk;
        cout << "Nhap MK moi: ";
        getline(cin, mk);
        password = mk;
    }

    void xemHocSinh(const Person* users) {
        const HocSinh* hs = dynamic_cast<const HocSinh*>(users);
        if (hs && hs->layMaGV() == id) {
            hs->hienThiThongTin();
        }
    }

    void thongKe(const Person* users) {
        double tong = 0;
        int dem = 0;

        const HocSinh* hs = dynamic_cast<const HocSinh*>(users);
        if (hs && hs->layMaGV() == id) {
            tong += hs->layDiem();
            dem++;
        }

        if (dem)
            cout << "Diem TB: " << tong / dem << endl;
        else
            cout << "Khong co du lieu!\n";
    }

    void sapXepDiem(vector<Person*>& users) {
        vector<HocSinh*> ds;

        for (auto p : users) {
            HocSinh* hs = dynamic_cast<HocSinh*>(p);
            if (hs && hs->layMaGV() == id) {
                ds.push_back(hs);
            }
        }

        sort(ds.begin(), ds.end(), [](HocSinh* a, HocSinh* b) {
            return a->layDiem() > b->layDiem();
            });

        cout << "Sau sap xep:\n";
        for (auto hs : ds) {
            hs->hienThiThongTin();
        }
    }

    void menu(Person* users, QuanLyThi& thi) {
        int choice;
        do {
            cout << "\n========== MENU GIAO VIEN ==========\n";
            cout << "1. Xem danh sach hoc sinh\n";
            cout << "2. Thong ke diem trung binh\n";
            cout << "3. Quan ly de thi & Diem thi\n"; // Mục mới thêm
            cout << "4. Doi mat khau\n";
            cout << "0. Dang xuat\n";
            cout << "Chon chuc nang: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
            case 1:
                xemHocSinh(users);
                break;
            case 2:
                thongKe(users);
                break;
            case 3:
                thi.menu();
                break;
            case 4:
                datLaiMatKhau();
                break;
            case 0:
                cout << "Dang xuat thanh cong!\n";
                break;
            default:
                cout << "Lua chon khong hop le!\n";
            }
        } while (choice != 0);
    }


};
// ========================== QUAN LY USER ==========================
class QuanLyNguoiDung {
private:
    vector<Person*> users;

public:
    void them(Person* p) {
        users.push_back(p);
    }

    Person* dangNhap() {
        string u, p;
        cout << "User: "; cin >> u;
        cout << "Pass: "; cin >> p;
        cin.ignore();

        for (auto user : users) {
            if (user->getUsername() == u &&
                user->getPassword() == p) {
                return user;
            }
        }
        return NULL;
    }

    vector<Person*>& layDanhSach() {
        return users;
    }
};

// ========================== NOTIFICATION ==========================
struct Notification {
    string message;
};

// ========================== RANK ITEM ==========================
struct RankItem {
    int id;
    string fullName;
    double score;
};

// ========================== ADMIN SYSTEM ==========================
class AdminSystem {
private:
    Admin adminAccount;
    vector<GiaoVien> teachers;
    vector<HocSinh> students;
    vector<ClassInfo> classes;
    vector<Notification> notifications;
    vector<RankItem> ranking;
    int totalStudents;

public:
    void sortRanking() {
        sort(ranking.begin(), ranking.end(), [](const RankItem& a, const RankItem& b) {
            if (a.score != b.score) return a.score > b.score;
            return a.fullName < b.fullName;
            });
    }
    AdminSystem() : totalStudents(0) {
        // Du lieu mau de test bang xep hang
        //ranking.push_back({ 101, "Nguyen Van A", 9.5 });
        //ranking.push_back({ 102, "Tran Thi B", 8.75 });
        //ranking.push_back({ 103, "Le Van C", 9.0 });
        sortRanking();
    }
    // ==================== VALIDATION ====================
    bool isValidName(const string& s) const {
        if (s.empty()) return false;
        for (char c : s) {
            unsigned char uc = static_cast<unsigned char>(c);
            if (!(isalnum(uc) || isspace(uc))) {
                return false;
            }
        }
        return true;
    }

    bool isValidUsername(const string& s) const {
        if (s.empty()) return false;
        for (char c : s) {
            unsigned char uc = static_cast<unsigned char>(c);
            if (!(isalnum(uc) || c == '_')) {
                return false;
            }
        }
        return true;
    }

    bool isValidPassword(const string& s) const {
        return s.length() >= 6;
    }

    bool teacherIdExists(int id) const {
        for (const auto& t : teachers) {
            if (t.getId() == id) return true;
        }
        return false;
    }

    bool studentIdExists(int id) const {
        for (const auto& hs : students) {
            if (hs.getId() == id) return true;
        }
        return false;
    }

    bool classIdExists(int id) const {
        for (const auto& c : classes) {
            if (c.id == id) return true;
        }
        return false;
    }

    bool usernameExists(const string& username) const {
        if (username == "admin") return true;
        for (const auto& t : teachers) {
            if (t.getUsername() == username) return true;
        }
        return false;
    }

    int findTeacherIndexById(int id) const {
        for (int i = 0; i < (int)teachers.size(); i++) {
            if (teachers[i].getId() == id) return i;
        }
        return -1;
    }

    int findStudentIndexById(int id) const {
        for (int i = 0; i < (int)students.size(); i++) {
            if (students[i].getId() == id) return i;
        }
        return -1;
    }

    int findClassIndexById(int id) const {
        for (int i = 0; i < (int)classes.size(); i++) {
            if (classes[i].id == id) return i;
        }
        return -1;
    }

    bool teacherHasAssignedClass(int teacherId) const {
        for (const auto& c : classes) {
            if (c.teacherId == teacherId) return true;
        }
        return false;
    }

    void recalcTotalStudents() {
        totalStudents = 0;
        for (const auto& c : classes) {
            totalStudents += c.studentCount;
        }
    }
    // ==================== LOGIN ====================
    bool loginAdmin() {
        cout << "========== DANG NHAP ADMIN ==========\n";
        string user, pass;
        cout << "Username: ";
        getline(cin, user);
        cout << "Password: ";
        getline(cin, pass);

        if (adminAccount.login(user, pass)) {
            cout << "Dang nhap thanh cong.\n";
            return true;
        }

        cout << "Sai username hoac password.\n";
        return false;
    }
    //==================== STUDENT CRUD =====================
    void addStudent() {
        cout << "\n===== THEM HOC SINH =====\n";

        HocSinh hs;
        hs.setId(nhapSo<int>("Nhap ID hoc sinh: "));

        if (studentIdExists(hs.getId())) {
            cout << "ID hoc sinh da ton tai.\n";
            return;
        }

        hs.setUsername(inputLine("Nhap username: "));
        if (!isValidUsername(hs.getUsername())) {
            cout << "Username khong hop le. Chi gom chu, so, dau _.\n";
            return;
        }

        if (usernameExists(hs.getUsername())) {
            cout << "Username da ton tai.\n";
            return;
        }

        hs.setPassword(inputLine("Nhap password: "));
        if (!isValidPassword(hs.getPassword())) {
            cout << "Password khong hop le. Toi thieu 6 ky tu.\n";
            return;
        }

        hs.setFullName(inputLine("Nhap ho ten: "));
        if (!isValidName(hs.getFullName())) {
            cout << "Ho ten khong hop le.\n";
            return;
        }

        hs.setLop(inputLine("Nhap lop: "));


        hs.setGioiTinh(inputLine("Nhap gioi tinh: "));

        hs.setBirthDay(inputLine("Nhap ngay sinh: "));

        hs.setSdt(inputLine("Nhap so dien thoai: "));

        hs.setDiemTB(nhapSo<double>("Nhap diem trung binh: "));

        int teacherId =
            nhapSo<int>("Nhap ID giao vien phu trach: ");

        if (!teacherIdExists(teacherId)) {
            cout << "ID giao vien khong ton tai.\n";
            return;
        }

        hs.setTeacherId(teacherId);

        students.push_back(hs);
        cout << "Them hoc sinh thanh cong.\n";
    }

    void viewStudents() const {
        cout << "\n===== DANH SACH HOC SINH =====\n";

        if (students.empty()) {
            cout << "Chua co hoc sinh nao.\n";
            return;
        }

        for (int i = 0; i < (int)students.size(); i++) {
            cout << "STT: " << i + 1
                << " | ID: " << students[i].getId()
                << " | Username: " << students[i].getUsername()
                << " | Full name: " << students[i].getFullName() << '\n';
        }
    }

    void editStudent() {
        cout << "\n===== SUA HOC SINH =====\n";

        int id = nhapSo<int>("Nhap ID hoc sinh can sua: ");
        int idx = findStudentIndexById(id);

        if (idx == -1) {
            cout << "Khong tim thay hoc sinh.\n";
            return;
        }

        cout << "1. Sua username\n";
        cout << "2. Sua password\n";
        cout << "3. Sua ho ten\n";
        cout << "4. Sua tat ca\n";

        int choice = nhapSo<int>("Chon: ");

        if (choice == 1 || choice == 4) {
            string newUsername = inputLine("Nhap username moi: ");

            if (!isValidUsername(newUsername)) {
                cout << "Username khong hop le.\n";
                return;
            }

            if (newUsername != students[idx].getUsername() &&
                usernameExists(newUsername)) {
                cout << "Username da ton tai.\n";
                return;
            }

            students[idx].setUsername(newUsername);
        }

        if (choice == 2 || choice == 4) {
            string newPassword = inputLine("Nhap password moi: ");

            if (!isValidPassword(newPassword)) {
                cout << "Password khong hop le.\n";
                return;
            }

            students[idx].setPassword(newPassword);
        }

        if (choice == 3 || choice == 4) {
            string newFullName = inputLine("Nhap ho ten moi: ");

            if (!isValidName(newFullName)) {
                cout << "Ho ten khong hop le.\n";
                return;
            }

            students[idx].setFullName(newFullName);
        }

        cout << "1. Sua username\n";
        cout << "2. Sua password\n";
        cout << "3. Sua ho ten\n";
        cout << "4. Sua lop\n";
        cout << "5. Sua gioi tinh\n";
        cout << "6. Sua ngay sinh\n";
        cout << "7. Sua so dien thoai\n";
        cout << "8. Sua diem trung binh\n";
        cout << "9. Sua giao vien phu trach\n";
        cout << "10. Sua tat ca\n";

        int chHS = nhapSo<int>("Chon: ");

        if (choice == 1 || chHS == 10) {

            string newUsername =
                inputLine("Nhap username moi: ");

            if (!isValidUsername(newUsername)) {
                cout << "Username khong hop le.\n";
                return;
            }

            if (newUsername != students[idx].getUsername()
                && usernameExists(newUsername)) {

                cout << "Username da ton tai.\n";
                return;
            }

            students[idx].setUsername(newUsername);
        }

        if (chHS == 2 || chHS == 10) {

            string newPassword =
                inputLine("Nhap password moi: ");

            if (!isValidPassword(newPassword)) {
                cout << "Password khong hop le.\n";
                return;
            }

            students[idx].setPassword(newPassword);
        }

        if (chHS == 3 || chHS == 10) {

            string newFullName =
                inputLine("Nhap ho ten moi: ");

            if (!isValidName(newFullName)) {
                cout << "Ho ten khong hop le.\n";
                return;
            }

            students[idx].setFullName(newFullName);
        }

        if (chHS == 4 || chHS == 10) {

            string newClass =
                inputLine("Nhap lop moi: ");

            students[idx].setLop(newClass);
        }

        if (chHS == 5 || chHS == 10) {

            string newGender =
                inputLine("Nhap gioi tinh moi: ");

            students[idx].setGioiTinh(newGender);
        }

        if (chHS == 6 || chHS == 10) {

            string newBirthDay =
                inputLine("Nhap ngay sinh moi: ");

            students[idx].setBirthDay(newBirthDay);
        }

        if (chHS == 7 || chHS == 10) {

            string newPhone =
                inputLine("Nhap so dien thoai moi: ");

            students[idx].setSdt(newPhone);
        }

        if (chHS == 8 || chHS == 10) {

            double newDiemTB =
                nhapSo<double>("Nhap diem trung binh moi: ");

            students[idx].setDiemTB(newDiemTB);
        }

        if (chHS == 9 || chHS == 10) {

            int newTeacherId =
                nhapSo<int>("Nhap ID giao vien moi: ");

            if (!teacherIdExists(newTeacherId)) {
                cout << "ID giao vien khong ton tai.\n";
                return;
            }

            students[idx].setTeacherId(newTeacherId);
        }

        cout << "Sua hoc sinh thanh cong.\n";
    }

    void deleteStudent() {
        cout << "\n===== XOA HOC SINH =====\n";

        int id = nhapSo<int>("Nhap ID hoc sinh can xoa: ");
        int idx = findStudentIndexById(id);

        if (idx == -1) {
            cout << "Khong tim thay hoc sinh.\n";
            return;
        }

        students.erase(students.begin() + idx);

        cout << "Xoa hoc sinh thanh cong.\n";
    }

    vector<HocSinh>& getStudents() {
        return students;
    }
    // ==================== TEACHER CRUD ====================
    void addTeacher() {
        cout << "\n===== THEM GIAO VIEN =====\n";

        GiaoVien t;
        t.setId(nhapSo<int>("Nhap ID giao vien: "));

        if (teacherIdExists(t.getId())) {
            cout << "ID giao vien da ton tai.\n";
            return;
        }

        t.setUsername(inputLine("Nhap username: ")) ;
        if (!isValidUsername(t.getUsername())) {
            cout << "Username khong hop le. Chi gom chu, so, dau _.\n";
            return;
        }
        if (usernameExists(t.getUsername())) {
            cout << "Username da ton tai.\n";
            return;
        }

        t.setPassword(inputLine("Nhap password: ")) ;
        if (!isValidPassword(t.getPassword())) {
            cout << "Password khong hop le. Toi thieu 6 ky tu.\n";
            return;
        }

        t.setFullName(inputLine("Nhap ho ten: ")) ;
        if (!isValidName(t.getFullName())) {
            cout << "Ho ten khong hop le.\n";
            return;
        }

        teachers.push_back(t);
        cout << "Them giao vien thanh cong.\n";
    }

    void viewTeachers() const {
        cout << "\n===== DANH SACH GIAO VIEN =====\n";
        if (teachers.empty()) {
            cout << "Chua co giao vien nao.\n";
            return;
        }

        for (int i = 0; i < (int)teachers.size(); i++) {
            cout << "STT: " << i + 1
                << " | ID: " << teachers[i].getId()
                << " | Username: " << teachers[i].getUsername()
                << " | Full name: " << teachers[i].getFullName() << '\n';
        }
    }

    void editTeacher() {
        cout << "\n===== SUA GIAO VIEN =====\n";
        int id = nhapSo<int>("Nhap ID giao vien can sua: ");
        int idx = findTeacherIndexById(id);

        if (idx == -1) {
            cout << "Khong tim thay giao vien.\n";
            return;
        }

        cout << "1. Sua username\n";
        cout << "2. Sua password\n";
        cout << "3. Sua ho ten\n";
        cout << "4. Sua tat ca\n";

        int choice = nhapSo<int>("Chon: ");

        if (choice == 1 || choice == 4) {
            string newUsername = inputLine("Nhap username moi: ");
            if (!isValidUsername(newUsername)) {
                cout << "Username khong hop le.\n";
                return;
            }
            if (newUsername != teachers[idx].getUsername() && usernameExists(newUsername)) {
                cout << "Username da ton tai.\n";
                return;
            }
            teachers[idx].setUsername(newUsername);
        }

        if (choice == 2 || choice == 4) {
            string newPassword = inputLine("Nhap password moi: ");
            if (!isValidPassword(newPassword)) {
                cout << "Password khong hop le.\n";
                return;
            }
            teachers[idx].setPassword(newPassword);
        }

        if (choice == 3 || choice == 4) {
            string newFullName = inputLine("Nhap ho ten moi: ");
            if (!isValidName(newFullName)) {
                cout << "Ho ten khong hop le.\n";
                return;
            }
            teachers[idx].setFullName(newFullName);
        }

        cout << "Sua giao vien thanh cong.\n";
    }

    void deleteTeacher() {
        cout << "\n===== XOA GIAO VIEN =====\n";
        int id = nhapSo<int>("Nhap ID giao vien can xoa: ");
        int idx = findTeacherIndexById(id);

        if (idx == -1) {
            cout << "Khong tim thay giao vien.\n";
            return;
        }

        if (teacherHasAssignedClass(id)) {
            cout << "Khong the xoa. Giao vien dang duoc gan cho lop.\n";
            return;
        }
        teachers.erase(teachers.begin() + idx);
        cout << "Xoa giao vien thanh cong.\n";
    }

    vector<GiaoVien>& getTeachers()
    {
        return teachers;
    }

    // ==================== CLASS CRUD ====================
    void addClass() {
        cout << "\n===== THEM LOP =====\n";

        ClassInfo c;
        c.id = nhapSo<int>("Nhap ID lop: ");

        if (classIdExists(c.id)) {
            cout << "ID lop da ton tai.\n";
            return;
        }

        c.className = inputLine("Nhap ten lop: ");
        if (c.className.empty()) {
            cout << "Ten lop khong duoc rong.\n";
            return;
        }

        c.teacherId = nhapSo<int>("Nhap ID giao vien phu trach (0 neu chua gan): ");
        if (c.teacherId != 0 && !teacherIdExists(c.teacherId)) {
            cout << "Giao vien khong ton tai.\n";
            return;
        }

        c.studentCount = nhapSo<int>("Nhap so luong hoc sinh: ");
        if (c.studentCount < 0) {
            cout << "So luong hoc sinh khong hop le.\n";
            return;
        }

        classes.push_back(c);
        recalcTotalStudents();
        cout << "Them lop thanh cong.\n";
    }

    void viewClasses() const {
        cout << "\n===== DANH SACH LOP =====\n";
        if (classes.empty()) {
            cout << "Chua co lop nao.\n";
            return;
        }

        for (int i = 0; i < (int)classes.size(); i++) {
            cout << "STT: " << i + 1
                << " | ID lop: " << classes[i].id
                << " | Ten lop: " << classes[i].className
                << " | Teacher ID: " << classes[i].teacherId
                << " | So hoc sinh: " << classes[i].studentCount << '\n';
        }
    }

    void editClass() {
        cout << "\n===== SUA LOP =====\n";
        int id = nhapSo<int>("Nhap ID lop can sua: ");
        int idx = findClassIndexById(id);

        if (idx == -1) {
            cout << "Khong tim thay lop.\n";
            return;
        }

        cout << "1. Sua ten lop\n";
        cout << "2. Sua giao vien phu trach\n";
        cout << "3. Sua so luong hoc sinh\n";
        cout << "4. Sua tat ca\n";

        int choice = nhapSo<int>("Chon: ");

        if (choice == 1 || choice == 4) {
            string newClassName = inputLine("Nhap ten lop moi: ");
            classes[idx].className = newClassName;
        }

        if (choice == 2 || choice == 4) {
            int newTeacherId = nhapSo<int>("Nhap ID giao vien moi (0 neu bo gan): ");
            if (newTeacherId != 0 && !teacherIdExists(newTeacherId)) {
                cout << "Giao vien khong ton tai.\n";
                return;
            }
            classes[idx].teacherId = newTeacherId;
        }

        if (choice == 3 || choice == 4) {
            int newStudentCount = nhapSo<int>("Nhap so luong hoc sinh moi: ");
            if (newStudentCount < 0) {
                cout << "So luong hoc sinh khong hop le.\n";
                return;
            }
            classes[idx].studentCount = newStudentCount;
            recalcTotalStudents();
        }

        cout << "Sua lop thanh cong.\n";
    }

    void deleteClass() {
        cout << "\n===== XOA LOP =====\n";
        int id = nhapSo<int>("Nhap ID lop can xoa: ");
        int idx = findClassIndexById(id);

        if (idx == -1) {
            cout << "Khong tim thay lop.\n";
            return;
        }

        classes.erase(classes.begin() + idx);
        recalcTotalStudents();
        cout << "Xoa lop thanh cong.\n";
    }

    void assignClassToTeacher() {
        cout << "\n===== GAN LOP CHO GIAO VIEN =====\n";
        int classId = nhapSo<int>("Nhap ID lop: ");
        int teacherId = nhapSo<int>("Nhap ID giao vien: ");

        int cidx = findClassIndexById(classId);
        int tidx = findTeacherIndexById(teacherId);

        if (cidx == -1) {
            cout << "Lop khong ton tai.\n";
            return;
        }

        if (tidx == -1) {
            cout << "Giao vien khong ton tai.\n";
            return;
        }

        classes[cidx].teacherId = teacherId;
        cout << "Gan lop cho giao vien thanh cong.\n";
    }

    // ==================== NOTIFICATION ====================
    void addNotification(const string& msg) {
        Notification n;
        n.message = msg;
        notifications.push_back(n);
    }

    void viewNotifications() const {
        cout << "\n===== DANH SACH THONG BAO =====\n";
        if (notifications.empty()) {
            cout << "Khong co thong bao nao.\n";
            return;
        }

        for (int i = 0; i < (int)notifications.size(); i++) {
            cout << i + 1 << ". " << notifications[i].message << '\n';
        }
    }

    // Mo phong de test thong bao
    void simulatePasswordChangeNotification() {
        cout << "\n===== MO PHONG DOI MAT KHAU =====\n";
        cout << "1. Giao vien doi mat khau\n";
        cout << "2. Hoc sinh doi mat khau\n";

        int choice = nhapSo<int>("Chon: ");

        if (choice == 1) {
            int teacherId = nhapSo<int>("Nhap ID giao vien: ");
            if (!teacherIdExists(teacherId)) {
                cout << "Giao vien khong ton tai.\n";
                return;
            }
            addNotification("Giao vien ID " + to_string(teacherId) + " da doi mat khau.");
            cout << "Da tao thong bao.\n";
        }
        else if (choice == 2) {
            int studentId = nhapSo<int>("Nhap ID hoc sinh: ");
            addNotification("Hoc sinh ID " + to_string(studentId) + " da doi mat khau.");
            cout << "Da tao thong bao.\n";
        }
        else {
            cout << "Lua chon khong hop le.\n";
        }
    }

    // ==================== STATISTICS ====================
    void countUsers() const {
        cout << "\n===== SO LUONG NGUOI DUNG =====\n";
        cout << "So giao vien: " << teachers.size() << '\n';
        cout << "So hoc sinh: " << totalStudents << '\n';
    }

    void viewSystemStatistics() const {
        cout << "\n===== THONG KE TOAN HE THONG =====\n";
        cout << "So giao vien: " << teachers.size() << '\n';
        cout << "So lop: " << classes.size() << '\n';
        cout << "Tong so hoc sinh: " << totalStudents << '\n';
        cout << "So thong bao: " << notifications.size() << '\n';
        cout << "So muc trong bang xep hang: " << ranking.size() << '\n';

        int assignedClassCount = 0;
        for (const auto& c : classes) {
            if (c.teacherId != 0) assignedClassCount++;
        }
        cout << "So lop da gan giao vien: " << assignedClassCount << '\n';
    }

    void viewRanking() {
        cout << "\n===== BANG XEP HANG =====\n";
        if (ranking.empty()) {
            cout << "Bang xep hang rong.\n";
            return;
        }

        sortRanking();
        for (int i = 0; i < (int)ranking.size(); i++) {
            cout << i + 1 << ". "
                << "ID: " << ranking[i].id
                << " | Ho ten: " << ranking[i].fullName
                << " | Diem: " << ranking[i].score << '\n';
        }
    }

    // ==================== SAVE / LOAD ====================
    void saveTeachers() const {
        ofstream fout("teachers.txt");
        for (const auto& t : teachers) {
            fout << t.getId() << '|'
                << t.getUsername() << '|'
                << t.getPassword() << '|'
                << t.getFullName() << '\n';
        }
    }

    void saveClasses() const {
        ofstream fout("classes.txt");
        for (const auto& c : classes) {
            fout << c.id << '|'
                << c.className << '|'
                << c.teacherId << '|'
                << c.studentCount << '\n';
        }
    }

    void saveNotifications() const {
        ofstream fout("notifications.txt");
        for (const auto& n : notifications) {
            fout << n.message << '\n';
        }
    }

    void saveRanking() const {
        ofstream fout("ranking.txt");
        for (const auto& r : ranking) {
            fout << r.id << '|'
                << r.fullName << '|'
                << r.score << '\n';
        }
    }

    void saveData() const {
        saveTeachers();
        saveClasses();
        saveNotifications();
        saveRanking();
        cout << "Luu du lieu thanh cong.\n";
    }

    void loadStudents(const string& fileName) {
        students.clear();

        ifstream fin(fileName);

        if (!fin) {
            cout << "Khong mo duoc file hoc sinh!\n";
            return;
        }

        string line;

        while (getline(fin, line)) {
            if (line.empty()) continue;

            stringstream ss(line);

            string idStr, username, password, fullName;
            string lop, gioitinh, birthDay, sdt;
            string diemTBStr, teacherIdStr;

            getline(ss, idStr, '|');
            getline(ss, username, '|');
            getline(ss, password, '|');
            getline(ss, fullName, '|');
            getline(ss, lop, '|');
            getline(ss, gioitinh, '|');
            getline(ss, birthDay, '|');
            getline(ss, sdt, '|');
            getline(ss, diemTBStr, '|');
            getline(ss, teacherIdStr);

            HocSinh s;

            s.setId(stoi(idStr));
            s.setUsername(username);
            s.setPassword(password);
            s.setFullName(fullName);
            s.setLop(lop);
            s.setGioiTinh(gioitinh);
            s.setBirthDay(birthDay);
            s.setSdt(sdt);
            s.setDiemTB(stod(diemTBStr));
            s.setTeacherId(stoi(teacherIdStr));

            students.push_back(s);
        }

        fin.close();

        cout << "Tai danh sach hoc sinh thanh cong.\n";
    }
    void loadTeachers(const string& fileName) {
        teachers.clear();

        ifstream fin(fileName);
        if (!fin) return;

        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            int idS;
            string idStr, username, password, fullName;

            cin >> idS;
            getline(ss, idStr, '|');
            getline(ss, username, '|');
            getline(ss, password, '|');
            getline(ss, fullName);

            GiaoVien t;
            t.setId(idS);
            t.setUsername(username);
            t.setPassword(password);
            t.setFullName(fullName);

            teachers.push_back(t);
        }
    }

    void loadClasses(const string& fileName) {
        classes.clear();
        ifstream fin(fileName);
        if (!fin) return;

        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string idStr, className, teacherIdStr, studentCountStr;

            getline(ss, idStr, '|');
            getline(ss, className, '|');
            getline(ss, teacherIdStr, '|');
            getline(ss, studentCountStr);

            ClassInfo c;
            c.id = stoi(idStr);
            c.className = className;
            c.teacherId = stoi(teacherIdStr);
            c.studentCount = stoi(studentCountStr);

            classes.push_back(c);
        }

        recalcTotalStudents();
    }

    void loadNotifications() {
        notifications.clear();
        ifstream fin("notifications.txt");
        if (!fin) return;

        string line;
        while (getline(fin, line)) {
            if (!line.empty()) {
                notifications.push_back({ line });
            }
        }
    }

    void loadRanking() {
        ranking.clear();
        ifstream fin("ranking.txt");
        if (!fin) return;

        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string idStr, fullName, scoreStr;

            getline(ss, idStr, '|');
            getline(ss, fullName, '|');
            getline(ss, scoreStr);

            RankItem r;
            r.id = stoi(idStr);
            r.fullName = fullName;
            r.score = stod(scoreStr);
            ranking.push_back(r);
        }

        sortRanking();
    }

    void loadData() {
        //Tai danh sach hoc sinh
        cout << "Chon file hoc sinh...\n";
        string studentFile = taiFile();
        if (!studentFile.empty()) {
            loadStudents(studentFile);
        }
        else {
            cout << "Khong chon file hoc sinh!\n";
        }
        //Tai danh sach giao vien
        cout << "Chon file giao vien...\n";
        string teacherFile = taiFile();
        if (!teacherFile.empty()) {
            loadTeachers(teacherFile);
        }
        else {
            cout << "Khong chon file giao vien!\n";
        }
        //Tai danh sach lop
        cout << "Chon file lop hoc...\n";
        string classFile = taiFile();
        if (!classFile.empty()) {
            loadClasses(classFile);
        }
        else {
            cout << "Khong chon file lop hoc!\n";
            return;
        }

        loadNotifications();
        loadRanking();
        recalcTotalStudents();
        cout << "Tai du lieu thanh cong.\n";
    }

    // ==================== MENU ====================
    void adminMenu() {
        int choice;
        do {
            cout << "\n================ MENU ADMIN ================\n";
            cout << "1. Them hoc sinh\n";
            cout << "2. Xoa hoc sinh\n";
            cout << "3. Sua hoc sinh\n";
            cout << "4. Xem danh sach hoc sinh\n";
            cout << "5. Them giao vien\n";
            cout << "6. Xoa giao vien\n";
            cout << "7. Sua giao vien\n";
            cout << "8. Xem danh sach giao vien\n";
            cout << "9. Them lop\n";
            cout << "10. Xoa lop\n";
            cout << "11. Sua lop\n";
            cout << "12. Xem danh sach lop\n";
            cout << "13. Gan lop cho giao vien\n";
            cout << "14. Xem thong bao\n";
            cout << "15. Xem thong ke toan he thong\n";
            cout << "16. Xem app co bao nhieu hoc sinh, bao nhieu giao vien\n";
            cout << "17. Xem bang xep hang\n";
            cout << "18. Luu du lieu\n";
            cout << "19. Tai du lieu\n";
            cout << "20. Mo phong thong bao doi mat khau\n";
            cout << "0. Thoat\n";

            choice = nhapSo<int>("Chon chuc nang: ");

            switch (choice) {
            case 1: addStudent(); break;
            case 2: deleteStudent(); break;
            case 3: editStudent(); break;
            case 4: viewStudents(); break;
            case 5: addTeacher(); break;
            case 6: deleteTeacher(); break;
            case 7: editTeacher(); break;
            case 8: viewTeachers(); break;
            case 9: addClass(); break;
            case 10: deleteClass(); break;
            case 11: editClass(); break;
            case 12: viewClasses(); break;
            case 13: assignClassToTeacher(); break;
            case 14: viewNotifications(); break;
            case 15: viewSystemStatistics(); break;
            case 16: countUsers(); break;
            case 17: viewRanking(); break;
            case 18:saveData();break;
            case 19:loadData();break;
            case 20:simulatePasswordChangeNotification();break;
            case 0:cout << "Thoat chuong trinh.\n";break;
            default:
                cout << "Lua chon khong hop le.\n";
            }

        } while (choice != 0);
    }
};

// ========================== MAIN ==========================
int main() {
    AdminSystem system;
    QuanLyNguoiDung ql;
    QuanLyThi thi;

    int preloadChoice;
    cout << "Ban co muon tai du lieu tu file khong?\n";
    cout << "1. Co\n2. Khong\n";

    cin >> preloadChoice;
    cin.ignore();

    if (preloadChoice == 1) {
        system.loadData();
    }

    int vaiTro;
    do {
        cout << "\n===== CHON VAI TRO =====\n";
        cout << "1. Admin\n";
        cout << "2. Giao vien\n";
        cout << "3. Hoc sinh\n";
        cout << "0. Thoat\n";
        cout << "Chon: ";
        vaiTro = nhapSo<int>("");

        if (vaiTro == 1) {
            if (system.loginAdmin()) {
                system.adminMenu();
            }
        }
        else if (vaiTro == 2) {
            for (auto& t : system.getTeachers()) {
                ql.them(&t);
            }
            Person* user = ql.dangNhap();
            if (user != nullptr) {
                cout << "Dang nhap thanh cong!" << user->getFullName() << endl;
                GiaoVien* gv =
                    dynamic_cast<GiaoVien*>(user);
                if (gv != nullptr)
                    gv->menu(user, thi);
            }
        }
        else if (vaiTro == 3) {
            for (auto& hs : system.getStudents()) {
                ql.them(&hs);
            }
            Person* user = ql.dangNhap();
            if (user != nullptr) {
                cout << "Dang nhap thanh cong!" << user->getFullName() << endl;
                HocSinh* hs =
                    dynamic_cast<HocSinh*>(user);
                if(hs!=nullptr)
                    hs->menu(thi.getMaDe());
            }
        }

    } while (vaiTro != 0);

    return 0;
}