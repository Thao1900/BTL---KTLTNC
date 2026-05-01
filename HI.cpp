#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cctype>
#include <ctime>
using namespace std;

// ========================== CAU HOI ==========================
struct CauHoi {
    int id;
    string noiDung;
    string dapAn[4];
    int dapAnDung;
};
//Nhap so nguyen
int nhapSoNguyen(string thongBao) {
    int n;
    cout << thongBao;
    while (!(cin >> n) || (n < 0) || (n > 9999)) {
        cout << "Loi! Nhap lai: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cin.ignore();
    return n;
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
        ch.id = nhapSoNguyen("ID: ");
        cout << "Noi dung: ";
        getline(cin, ch.noiDung);
        for (int i = 0; i < 4; i++) {
            cout << "Lua chon " << i + 1 << ": ";
            getline(cin, ch.dapAn[i]);
        }
        do {
            ch.dapAnDung = nhapSoNguyen("dapAnDung ");
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
// ========================== TEACHER INFO ==========================
struct TeacherInfo {
    int id;
    string username;
    string password;
    string fullName;
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
        : id(id), username(username), password(password), fullName(fullName) {}

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
        : Person(id, u, p, ten) {}
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
    string mssv;
    string gioitinh;
    string birthDay;
    string sdt;
    vector<double> diem;
    double diemTB;
    int teacherId;

public:
    HocSinh(int id, string userN, string passW, string ten,
        string lop, string mssv, string gioitinh, string birthDay, string sdt, double diemTB, int teacherId)
        : Person(id, userN, passW, ten), lop(lop), mssv(mssv), gioitinh(gioitinh), birthDay(birthDay), sdt(sdt), diem(diem), diemTB(diemTB), teacherId(teacherId) {}
    string getRole() const override {
        return "HocSinh";
    }

    int layMaGV() const { return teacherId; }
    double layDiem() const { return diemTB; }
    void datDiem(double d) { diemTB = d; }

    void hienThiThongTin() const {
        cout << "[HS] " << fullName
            << " | Lop: " << lop
            << " | Diem: " << diemTB;
    }
    int traLoiCauHoi(vector<CauHoi> danhSachCauHoi) const {
        int n;
        double diemThi = 0;
        for (auto& ch : danhSachCauHoi) {
            cout << ch.id << ". " << ch.noiDung << endl;
            n = nhapSoNguyen("Dap an ban chon: ");
            if (n == ch.dapAnDung) {
                diemThi += 10.0 / danhSachCauHoi.size();
            }

        }
        return diemThi;
    }
    //Lam bai thi
    void lamBaiThi(const vector<deThi>& maDe) {
        cout << "Vui long nhap mon muon lam bai thi\n";
        string mon;
        cin.ignore();
        getline(cin, mon);

        for (auto& de : maDe) {
            if (de.tenMon == mon) {
                cout << "Bat dau lam bai mon: " << mon << endl;
                diem.push_back(traLoiCauHoi(de.danhSachCauHoi));
            }
        }
    }
    string chuyenThanhChuoiFile() const {
        return to_string(id) + "|" + username + "|" + password + "|" +
            fullName + "|" + lop + "|" + to_string(diemTB) + "|" + to_string(teacherId);
    }

    //Menu Hoc Sinh
    void menu(const vector<deThi>& danhSachDe) {
        int choice;
        do {
            cout << "1. Lam bai thi";
            cout << "2. Xem ket qua thi";
            cout << "3. Xem thong tin ca nhan";
            cout << "4. Dat lai mat khau";
            cout << "0. Dang Xuat";
            cin >> choice;
            cin.ignore();
        } while (choice != 0);
        switch (choice) {
        case 1: lamBaiThi(danhSachDe);
            break;
        case 2:
            break;
        case 3: hienThiThongTin();
            break;
        case 4:
            break;
        case 0:
            break;
        }
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
        de.id = nhapSoNguyen("Ma de:");
        cout << "Ten mon: ";
        getline(cin, de.tenMon);
        de.thoiGianLamBai = nhapSoNguyen("Thoi gian lam bai: ");
        maDe.push_back(de);
    }
    void batDauThi() {
        cout << "Vui long chon de thi de bat dau\n";
        cin.ignore();
        getline(cin, tenMon);
        int n;
        cout << "Nhap 1 de bat dau thoi gian thi\n";
        cout << "Nhap 0 de khoa bai thi";
        while (!(cin >> n) && (n != 0) && (n != 1)) {
            cout << "Vui long nhap lai";
            cin >> n;
        }
        for (auto& de : maDe) {
            if (de.tenMon == tenMon)
                de.khoa = n;
        }
    }
    void nhapKetQua() {
        KetQua kq;
        kq.idSV = nhapSoNguyen("ID SV: ");
        cout << "Ten: ";
        getline(cin, kq.tenSV);
        cout << "Diem: ";
        cin >> kq.diem;
        kq.thoiGian = nhapSoNguyen("Thoi gian: ");
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
    GiaoVien(int id, string u, string p, string ten, string mon)
        : Person(id, u, p, ten), mon(mon) {}
    string getRole() const override {
        return "GiaoVien";
    }

    void hienThiThongTin() const {
        cout << "[GV] " << fullName << " | Mon: " << mon << endl;
    }

    string chuyenThanhChuoiFile() const {
        return to_string(id) + "|" + username + "|" + password + "|" + fullName + "|" + mon;
    }

    void menu(vector<Person*>& users, QuanLyThi& thi) {
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

    void datLaiMatKhau() {
        string mk;
        cout << "Nhap MK moi: ";
        getline(cin, mk);
        password = mk;
    }

    void xemHocSinh(const vector<Person*>& users) {
        for (auto p : users) {
            HocSinh* hs = dynamic_cast<HocSinh*>(p);
            if (hs && hs->layMaGV() == id) {
                hs->hienThiThongTin();
            }
        }
    }

    void thongKe(const vector<Person*>& users) {
        double tong = 0;
        int dem = 0;

        for (auto p : users) {
            HocSinh* hs = dynamic_cast<HocSinh*>(p);
            if (hs && hs->layMaGV() == id) {
                tong += hs->layDiem();
                dem++;
            }
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
    vector<TeacherInfo> teachers;
    vector<Person*> users;
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
        ranking.push_back({ 101, "Nguyen Van A", 9.5 });
        ranking.push_back({ 102, "Tran Thi B", 8.75 });
        ranking.push_back({ 103, "Le Van C", 9.0 });
        sortRanking();
    }
    Person* loginUser() {
        string user, pass;

        cout << "Username: ";
        getline(cin, user);

        cout << "Password: ";
        getline(cin, pass);

        if (adminAccount.login(user, pass)) {
            cout << "Dang nhap ADMIN thanh cong\n";
            return &adminAccount;
        }
        for (auto u : users) {
            if (u->getUsername() == user &&
                u->getPassword() == pass) {
                cout << "Dang nhap thanh cong\n";
                return u;
            }
        }

        cout << "Sai tai khoan hoac mat khau\n";
        return nullptr;
    }
    // ==================== INPUT HELPERS ====================
    int inputInt(const string& prompt) {
        int x;
        while (true) {
            cout << prompt;
            cin >> x;
            if (!cin.fail()) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return x;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Loi: vui long nhap so nguyen.\n";
        }
    }

    double inputDouble(const string& prompt) {
        double x;
        while (true) {
            cout << prompt;
            cin >> x;
            if (!cin.fail()) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return x;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Loi: vui long nhap so.\n";
        }
    }

    string inputLine(const string& prompt) {
        string s;
        while (true) {
            cout << prompt;
            getline(cin, s);
            if (!s.empty()) return s;
            cout << "Loi: khong duoc de trong.\n";
        };
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
            if (t.id == id) return true;
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
            if (t.username == username) return true;
        }
        return false;
    }

    int findTeacherIndexById(int id) const {
        for (int i = 0; i < (int)teachers.size(); i++) {
            if (teachers[i].id == id) return i;
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

    // ==================== TEACHER CRUD ====================
    void addTeacher() {
        cout << "\n===== THEM GIAO VIEN =====\n";

        TeacherInfo t;
        t.id = inputInt("Nhap ID giao vien: ");

        if (teacherIdExists(t.id)) {
            cout << "ID giao vien da ton tai.\n";
            return;
        }

        t.username = inputLine("Nhap username: ");
        if (!isValidUsername(t.username)) {
            cout << "Username khong hop le. Chi gom chu, so, dau _.\n";
            return;
        }
        if (usernameExists(t.username)) {
            cout << "Username da ton tai.\n";
            return;
        }

        t.password = inputLine("Nhap password: ");
        if (!isValidPassword(t.password)) {
            cout << "Password khong hop le. Toi thieu 6 ky tu.\n";
            return;
        }

        t.fullName = inputLine("Nhap ho ten: ");
        if (!isValidName(t.fullName)) {
            cout << "Ho ten khong hop le.\n";
            return;
        }

        teachers.push_back(t);

        users.push_back(new GiaoVien(
            t.id,
            t.username,
            t.password,
            t.fullName,
            "Chua ro mon"
        ));
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
                << " | ID: " << teachers[i].id
                << " | Username: " << teachers[i].username
                << " | Full name: " << teachers[i].fullName << '\n';
        }
    }

    void editTeacher() {
        cout << "\n===== SUA GIAO VIEN =====\n";
        int id = inputInt("Nhap ID giao vien can sua: ");
        int idx = findTeacherIndexById(id);

        if (idx == -1) {
            cout << "Khong tim thay giao vien.\n";
            return;
        }

        cout << "1. Sua username\n";
        cout << "2. Sua password\n";
        cout << "3. Sua ho ten\n";
        cout << "4. Sua tat ca\n";

        int choice = inputInt("Chon: ");

        if (choice == 1 || choice == 4) {
            string newUsername = inputLine("Nhap username moi: ");
            if (!isValidUsername(newUsername)) {
                cout << "Username khong hop le.\n";
                return;
            }
            if (newUsername != teachers[idx].username && usernameExists(newUsername)) {
                cout << "Username da ton tai.\n";
                return;
            }
            teachers[idx].username = newUsername;
        }

        if (choice == 2 || choice == 4) {
            string newPassword = inputLine("Nhap password moi: ");
            if (!isValidPassword(newPassword)) {
                cout << "Password khong hop le.\n";
                return;
            }
            teachers[idx].password = newPassword;
        }

        if (choice == 3 || choice == 4) {
            string newFullName = inputLine("Nhap ho ten moi: ");
            if (!isValidName(newFullName)) {
                cout << "Ho ten khong hop le.\n";
                return;
            }
            teachers[idx].fullName = newFullName;
        }

        cout << "Sua giao vien thanh cong.\n";
    }

    void deleteTeacher() {
        cout << "\n===== XOA GIAO VIEN =====\n";
        int id = inputInt("Nhap ID giao vien can xoa: ");
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

    vector<Person*>& layDanhSach() {
        return users;
    }

    // ==================== CLASS CRUD ====================
    void addClass() {
        cout << "\n===== THEM LOP =====\n";

        ClassInfo c;
        c.id = inputInt("Nhap ID lop: ");

        if (classIdExists(c.id)) {
            cout << "ID lop da ton tai.\n";
            return;
        }

        c.className = inputLine("Nhap ten lop: ");
        if (c.className.empty()) {
            cout << "Ten lop khong duoc rong.\n";
            return;
        }

        c.teacherId = inputInt("Nhap ID giao vien phu trach (0 neu chua gan): ");
        if (c.teacherId != 0 && !teacherIdExists(c.teacherId)) {
            cout << "Giao vien khong ton tai.\n";
            return;
        }

        c.studentCount = inputInt("Nhap so luong hoc sinh: ");
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
        int id = inputInt("Nhap ID lop can sua: ");
        int idx = findClassIndexById(id);

        if (idx == -1) {
            cout << "Khong tim thay lop.\n";
            return;
        }

        cout << "1. Sua ten lop\n";
        cout << "2. Sua giao vien phu trach\n";
        cout << "3. Sua so luong hoc sinh\n";
        cout << "4. Sua tat ca\n";

        int choice = inputInt("Chon: ");

        if (choice == 1 || choice == 4) {
            string newClassName = inputLine("Nhap ten lop moi: ");
            classes[idx].className = newClassName;
        }

        if (choice == 2 || choice == 4) {
            int newTeacherId = inputInt("Nhap ID giao vien moi (0 neu bo gan): ");
            if (newTeacherId != 0 && !teacherIdExists(newTeacherId)) {
                cout << "Giao vien khong ton tai.\n";
                return;
            }
            classes[idx].teacherId = newTeacherId;
        }

        if (choice == 3 || choice == 4) {
            int newStudentCount = inputInt("Nhap so luong hoc sinh moi: ");
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
        int id = inputInt("Nhap ID lop can xoa: ");
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
        int classId = inputInt("Nhap ID lop: ");
        int teacherId = inputInt("Nhap ID giao vien: ");

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

        int choice = inputInt("Chon: ");

        if (choice == 1) {
            int teacherId = inputInt("Nhap ID giao vien: ");
            if (!teacherIdExists(teacherId)) {
                cout << "Giao vien khong ton tai.\n";
                return;
            }
            addNotification("Giao vien ID " + to_string(teacherId) + " da doi mat khau.");
            cout << "Da tao thong bao.\n";
        }
        else if (choice == 2) {
            int studentId = inputInt("Nhap ID hoc sinh: ");
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
            fout << t.id << '|'
                << t.username << '|'
                << t.password << '|'
                << t.fullName << '\n';
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

    void loadTeachers() {
        teachers.clear();
        users.clear();

        ifstream fin("teachers.txt");
        if (!fin) return;

        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string idStr, username, password, fullName;

            getline(ss, idStr, '|');
            getline(ss, username, '|');
            getline(ss, password, '|');
            getline(ss, fullName);

            TeacherInfo t;
            t.id = stoi(idStr);
            t.username = username;
            t.password = password;
            t.fullName = fullName;

            teachers.push_back(t);
            users.push_back(new GiaoVien(
                t.id,
                t.username,
                t.password,
                t.fullName,
                "Chua ro mon"
            ));
        }
    }

    void loadClasses() {
        classes.clear();
        ifstream fin("classes.txt");
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
        loadTeachers();
        loadClasses();
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
            cout << "1. Them giao vien\n";
            cout << "2. Xoa giao vien\n";
            cout << "3. Sua giao vien\n";
            cout << "4. Xem danh sach giao vien\n";
            cout << "5. Them lop\n";
            cout << "6. Xoa lop\n";
            cout << "7. Sua lop\n";
            cout << "8. Xem danh sach lop\n";
            cout << "9. Gan lop cho giao vien\n";
            cout << "10. Xem thong bao\n";
            cout << "11. Xem thong ke toan he thong\n";
            cout << "12. Xem app co bao nhieu hoc sinh, bao nhieu giao vien\n";
            cout << "13. Xem bang xep hang\n";
            cout << "14. Luu du lieu\n";
            cout << "15. Tai du lieu\n";
            cout << "16. Mo phong thong bao doi mat khau\n";
            cout << "0. Thoat\n";
            choice = inputInt("Chon chuc nang: ");

            switch (choice) {
            case 1: addTeacher(); break;
            case 2: deleteTeacher(); break;
            case 3: editTeacher(); break;
            case 4: viewTeachers(); break;
            case 5: addClass(); break;
            case 6: deleteClass(); break;
            case 7: editClass(); break;
            case 8: viewClasses(); break;
            case 9: assignClassToTeacher(); break;
            case 10: viewNotifications(); break;
            case 11: viewSystemStatistics(); break;
            case 12: countUsers(); break;
            case 13: viewRanking(); break;
            case 14: saveData(); break;
            case 15: loadData(); break;
            case 16: simulatePasswordChangeNotification(); break;
            case 0: cout << "Thoat chuong trinh.\n"; break;
            default: cout << "Lua chon khong hop le.\n";
            }
        } while (choice != 0);
    }
};

// ========================== MAIN ==========================
int main() {
    AdminSystem system;
    QuanLyThi thi;
    HocSinh a(123, "Hong", "123456", "Nguyen Thi Hong", "KTOT", "6651020034", "Nu", "20/02/2007", "00", 0.0, 123456);

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
        cin >> vaiTro;
        cin.ignore();

        if (vaiTro == 1) {
            if (system.loginAdmin()) {
                system.adminMenu();
            }
        }
        else if (vaiTro == 2) {
            Person* user = system.loginUser();
            if (user != nullptr) {
                if (user->getRole() == "GiaoVien") {
                    GiaoVien* gv = dynamic_cast<GiaoVien*>(user);
                    if (gv) {
                        gv->menu(system.layDanhSach(), thi);
                    }
                }
            }
        }
        else if (vaiTro == 3) {
            Person* user = system.loginUser();
            if (user->getRole() == "HocSinh") {
                a.menu(thi.getMaDe());
            }
        }

    } while (vaiTro != 0);

    return 0;
}