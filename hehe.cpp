#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

// ==============================================================================
//                              PHẦN 0: CẤU TRÚC DỮ LIỆU & TIỆN ÍCH
// ==============================================================================
const int MAX_USERS = 100;
const int MAX_CLASSES = 50;
const int MAX_EXAMS = 20;
const int MAX_QUESTIONS = 50;
const int MAX_RESULTS = 200;
const int MAX_NOTI = 50;

struct CauHoi {
    int id;
    string noiDung;
    string dapAn[4];
    int dapAnDung;
};

struct DeThi {
    int maDe;
    string tenMon;
    int thoiGian;
    int loaiThi; // 1: Chinh thuc, 2: On tap
    CauHoi danhSachCauHoi[MAX_QUESTIONS];
    int soLuongCauHoi;
};

struct KetQuaThi {
    int idSV;
    string tenSV;
    int maDe;
    string tenMon;
    double diem;
    int thoiGianLamBai;
    int loaiThi;
};

struct ClassInfo {
    string tenLop;
    int teacherId;
};

// Khai bao truoc de tranh loi phu thuoc cheo
class TruongHoc;

// --- Cac ham tien ich nhap xuat an toan ---
int inputInt(string prompt) {
    int x;
    while (true) {
        cout << prompt;
        cin >> x;
        if (!cin.fail()) {
            string thuRac; getline(cin, thuRac); 
            return x;
        }
        cin.clear();
        string thuRac; getline(cin, thuRac);
        cout << "Loi: Vui long nhap so nguyen hop le.\n";
    }
}

int inputPositiveInt(string prompt) {
    int x;
    while (true) {
        x = inputInt(prompt);
        if (x >= 0) return x;
        cout << "Loi: Vui long nhap so lon hon hoac bang 0.\n";
    }
}

string inputLine(string prompt) {
    string s;
    while (true) {
        cout << prompt;
        getline(cin, s);
        if (s != "") return s;
        cout << "Loi: Khong duoc de trong.\n";
    }
}

string intToString(int n) {
    if (n == 0) return "0";
    string res = "";
    bool isNeg = (n < 0);
    if (isNeg) n = -n;
    while (n > 0) {
        res = char('0' + (n % 10)) + res;
        n /= 10;
    }
    return isNeg ? "-" + res : res;
}

string doubleToString(double d) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%.2f", d);
    return string(buf);
}

int splitString(string s, char delimiter, string tokens[], int maxTokens) {
    int count = 0;
    string token = "";
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == delimiter) {
            if (count < maxTokens) tokens[count++] = token;
            token = "";
        } else {
            token += s[i];
        }
    }
    if (count < maxTokens) tokens[count++] = token;
    return count;
}

template <typename T>
void hoanVi(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}


// ==============================================================================
//                              PHẦN 1: LỚP TRỪU TƯỢNG PERSON
// ==============================================================================
class Person {
protected:
    int id;
    string username;
    string password;
    string fullName;

public:
    Person(int i, string u, string p, string ten) {
        id = i; username = u; password = p; fullName = ten;
    }
    virtual ~Person() {}

    virtual string getRole() = 0;
    virtual void hienThiThongTin() = 0;
    virtual void menu(TruongHoc& sys) = 0;

    int getId() { return id; }
    string getUsername() { return username; }
    string getPassword() { return password; }
    string getFullName() { return fullName; }

    void setPassword(string p) { password = p; }
    void setFullName(string n) { fullName = n; }
    virtual string toFileString() = 0;
};


// ==============================================================================
//                              PHẦN 2: LỚP ADMIN
// ==============================================================================
class Admin : public Person {
public:
    Admin() : Person(1, "admin", "admin123", "System Admin") {}
    string getRole() { return "Admin"; }
    void hienThiThongTin() { cout << "[ADMIN] " << fullName << endl; }
    
    // Khai bao menu, chi tiet duoc viet o cuoi file
    void menu(TruongHoc& sys);
    
    string toFileString() { return "Admin|1|admin|admin123|Admin"; }
};


// ==============================================================================
//                              PHẦN 3: LỚP GIÁO VIÊN
// ==============================================================================
class GiaoVien : public Person {
private:
    string monDay;
public:
    GiaoVien(int i, string u, string p, string ten, string mon) : Person(i, u, p, ten) {
        monDay = mon;
    }
    string getRole() { return "GiaoVien"; }
    string getMonDay() { return monDay; }
    void setMonDay(string m) { monDay = m; }

    void hienThiThongTin() {
        cout << "[GV] ID: " << id << " | Ten: " << fullName << " | Mon: " << monDay << endl;
    }
    
    // Khai bao menu, chi tiet duoc viet o cuoi file
    void menu(TruongHoc& sys);
    
    string toFileString() {
        return "GiaoVien|" + intToString(id) + "|" + username + "|" + password + "|" + fullName + "|" + monDay;
    }
};


// ==============================================================================
//                              PHẦN 4: LỚP HỌC SINH
// ==============================================================================
class HocSinh : public Person {
private:
    string lop;
    string gioiTinh;
    int tuoi;
    string sdt;
public:
    HocSinh(int i, string u, string p, string ten, string l, string gt = "Chua ro", int t = 0, string sdtInfo = "Chua ro")
        : Person(i, u, p, ten) {
        lop = l; gioiTinh = gt; tuoi = t; sdt = sdtInfo;
    }

    string getRole() { return "HocSinh"; }
    string getLop() { return lop; }
    void setLop(string l) { lop = l; }

    void hienThiThongTin() {
        cout << "--- THONG TIN CA NHAN ---\n"
            << "- MSSV: " << id << "\n- Ho Ten: " << fullName << "\n"
            << "- Lop: " << lop << "\n- Gioi tinh: " << gioiTinh << "\n"
            << "- Tuoi: " << tuoi << "\n- SDT: " << sdt << "\n--------------------------\n";
    }
    
    // Khai bao menu, chi tiet duoc viet o cuoi file
    void menu(TruongHoc& sys);
    
    string toFileString() {
        return "HocSinh|" + intToString(id) + "|" + username + "|" + password + "|" + fullName + "|" + lop + "|" + gioiTinh + "|" + intToString(tuoi) + "|" + sdt;
    }
};


// ==============================================================================
//                              PHẦN 5: LỚP QUẢN LÝ HỆ THỐNG
// ==============================================================================
class TruongHoc {
private:
    Person* users[MAX_USERS];
    int userCount;

    ClassInfo classes[MAX_CLASSES];
    int classCount;

    DeThi danhSachDeThi[MAX_EXAMS];
    int examCount;

    KetQuaThi danhSachKetQua[MAX_RESULTS];
    int resultCount;

    string notifications[MAX_NOTI];
    int notiCount;

public:
    TruongHoc() {
        userCount = 0; classCount = 0; examCount = 0; resultCount = 0; notiCount = 0;
        users[userCount++] = new Admin();
        srand(time(0));
    }

    ~TruongHoc() {
        for (int i = 0; i < userCount; i++) delete users[i];
    }

    Person** getUsers() { return users; }
    int getUserCount() { return userCount; }

    ClassInfo* getClasses() { return classes; }
    int getClassCount() { return classCount; }

    DeThi* getExams() { return danhSachDeThi; }
    int getExamCount() { return examCount; }
    void setExamCount(int c) { examCount = c; }

    KetQuaThi* getResults() { return danhSachKetQua; }
    int getResultCount() { return resultCount; }
    void addResult(KetQuaThi kq) {
        if (resultCount < MAX_RESULTS) danhSachKetQua[resultCount++] = kq;
    }

    void addNotification(string msg) {
        if (notiCount < MAX_NOTI) notifications[notiCount++] = msg;
    }
    
    void viewNotifications() {
        cout << "\n=== THONG BAO HE THONG ===\n";
        if (notiCount == 0) cout << "Khong co thong bao nao.\n";
        for (int i = 0; i < notiCount; i++) cout << "- " << notifications[i] << "\n";
    }

    bool isUsernameExists(string u) {
        for (int i = 0; i < userCount; i++) {
            if (users[i]->getUsername() == u) return true;
        }
        return false;
    }

    bool isIdExists(int id) {
        for (int i = 0; i < userCount; i++) {
            if (users[i]->getId() == id) return true;
        }
        return false;
    }

    // --- LUU VA TAI FILE ---
    void saveData() {
        ofstream fUsers("users.txt");
        for (int i = 0; i < userCount; i++) {
            if (users[i]->getRole() != "Admin") fUsers << users[i]->toFileString() << "\n";
        }
        fUsers.close();

        ofstream fClasses("classes.txt");
        for (int i = 0; i < classCount; i++) fClasses << classes[i].tenLop << "|" << classes[i].teacherId << "\n";
        fClasses.close();

        ofstream fExams("exams.txt");
        ofstream fQuestions("questions.txt");
        for (int i = 0; i < examCount; i++) {
            fExams << danhSachDeThi[i].maDe << "|" << danhSachDeThi[i].tenMon << "|" 
                   << danhSachDeThi[i].thoiGian << "|" << danhSachDeThi[i].loaiThi << "\n";
            for (int j = 0; j < danhSachDeThi[i].soLuongCauHoi; j++) {
                CauHoi ch = danhSachDeThi[i].danhSachCauHoi[j];
                fQuestions << danhSachDeThi[i].maDe << "|" << ch.id << "|" << ch.noiDung << "|"
                    << ch.dapAn[0] << "|" << ch.dapAn[1] << "|" << ch.dapAn[2] << "|" << ch.dapAn[3] << "|" << ch.dapAnDung << "\n";
            }
        }
        fExams.close(); fQuestions.close();

        ofstream fResults("results.txt");
        for (int i = 0; i < resultCount; i++) {
            fResults << danhSachKetQua[i].idSV << "|" << danhSachKetQua[i].tenSV << "|" 
                     << danhSachKetQua[i].maDe << "|" << danhSachKetQua[i].tenMon << "|" 
                     << doubleToString(danhSachKetQua[i].diem) << "|" << danhSachKetQua[i].thoiGianLamBai << "|" 
                     << danhSachKetQua[i].loaiThi << "\n";
        }
        fResults.close();

        ofstream fNoti("noti.txt");
        for (int i = 0; i < notiCount; i++) fNoti << notifications[i] << "\n";
        fNoti.close();

        cout << "=> Da luu toan bo du lieu vao he thong!\n";
    }

    void loadData() {
        for (int i = 1; i < userCount; i++) delete users[i];
        userCount = 1; 
        classCount = 0; examCount = 0; resultCount = 0; notiCount = 0;

        string line;
        string data[10];

        ifstream fUsers("users.txt");
        if (fUsers.is_open()) {
            while (getline(fUsers, line)) {
                if (userCount >= MAX_USERS) break; 
                int count = splitString(line, '|', data, 10);
                if (count >= 6 && data[0] == "GiaoVien") {
                    int id = atoi(data[1].c_str());
                    if (!isIdExists(id)) users[userCount++] = new GiaoVien(id, data[2], data[3], data[4], data[5]);
                }
                else if (count >= 9 && data[0] == "HocSinh") {
                    int id = atoi(data[1].c_str());
                    if (!isIdExists(id)) users[userCount++] = new HocSinh(id, data[2], data[3], data[4], data[5], data[6], atoi(data[7].c_str()), data[8]);
                }
            }
            fUsers.close();
        }

        ifstream fClasses("classes.txt");
        if (fClasses.is_open()) {
            while (getline(fClasses, line)) {
                if (classCount >= MAX_CLASSES) break; 
                int count = splitString(line, '|', data, 10);
                if (count >= 2) {
                    classes[classCount].tenLop = data[0];
                    classes[classCount].teacherId = atoi(data[1].c_str());
                    classCount++;
                }
            }
            fClasses.close();
        }

        ifstream fExams("exams.txt");
        if (fExams.is_open()) {
            while (getline(fExams, line)) {
                if (examCount >= MAX_EXAMS) break;
                int count = splitString(line, '|', data, 10);
                if (count >= 3) {
                    danhSachDeThi[examCount].maDe = atoi(data[0].c_str());
                    danhSachDeThi[examCount].tenMon = data[1];
                    danhSachDeThi[examCount].thoiGian = atoi(data[2].c_str());
                    danhSachDeThi[examCount].loaiThi = (count >= 4) ? atoi(data[3].c_str()) : 1;
                    danhSachDeThi[examCount].soLuongCauHoi = 0;
                    examCount++;
                }
            }
            fExams.close();
        }

        ifstream fQuestions("questions.txt");
        if (fQuestions.is_open()) {
            while (getline(fQuestions, line)) {
                int count = splitString(line, '|', data, 10);
                if (count >= 8) {
                    int maDe = atoi(data[0].c_str());
                    for (int i = 0; i < examCount; i++) {
                        if (danhSachDeThi[i].maDe == maDe) {
                            int idx = danhSachDeThi[i].soLuongCauHoi;
                            if (idx < MAX_QUESTIONS) { 
                                danhSachDeThi[i].danhSachCauHoi[idx].id = atoi(data[1].c_str());
                                danhSachDeThi[i].danhSachCauHoi[idx].noiDung = data[2];
                                danhSachDeThi[i].danhSachCauHoi[idx].dapAn[0] = data[3];
                                danhSachDeThi[i].danhSachCauHoi[idx].dapAn[1] = data[4];
                                danhSachDeThi[i].danhSachCauHoi[idx].dapAn[2] = data[5];
                                danhSachDeThi[i].danhSachCauHoi[idx].dapAn[3] = data[6];
                                danhSachDeThi[i].danhSachCauHoi[idx].dapAnDung = atoi(data[7].c_str());
                                danhSachDeThi[i].soLuongCauHoi++;
                            }
                            break;
                        }
                    }
                }
            }
            fQuestions.close();
        }

        ifstream fResults("results.txt");
        if (fResults.is_open()) {
            while (getline(fResults, line)) {
                if (resultCount >= MAX_RESULTS) break;
                int count = splitString(line, '|', data, 10);
                if (count >= 6) {
                    danhSachKetQua[resultCount].idSV = atoi(data[0].c_str());
                    danhSachKetQua[resultCount].tenSV = data[1];
                    danhSachKetQua[resultCount].maDe = atoi(data[2].c_str());
                    danhSachKetQua[resultCount].tenMon = data[3];
                    danhSachKetQua[resultCount].diem = atof(data[4].c_str());
                    danhSachKetQua[resultCount].thoiGianLamBai = atoi(data[5].c_str());
                    danhSachKetQua[resultCount].loaiThi = (count >= 7) ? atoi(data[6].c_str()) : 1;
                    resultCount++;
                }
            }
            fResults.close();
        }

        ifstream fNoti("noti.txt");
        if (fNoti.is_open()) {
            while (getline(fNoti, line)) {
                if (notiCount < MAX_NOTI) notifications[notiCount++] = line;
            }
            fNoti.close();
        }

        cout << "=> Da tai du lieu tu file thanh cong!\n";
    }

    // --- QUAN LY DOI TUONG ---
    void qlGiaoVien() {
        int ch;
        do {
            ch = inputInt("\n--- QL GIAO VIEN ---\n1. Them GV\n2. Xem danh sach\n3. Sua GV\n4. Xoa GV\n0. Back\nChon: ");
            if (ch == 1) {
                if (userCount >= MAX_USERS) { cout << "Danh sach da day!\n"; continue; }
                int id = inputPositiveInt("ID: ");
                if (isIdExists(id)) { cout << "ID trung!\n"; continue; }
                string u = inputLine("User: ");
                if (isUsernameExists(u)) { cout << "User trung!\n"; continue; }
                users[userCount++] = new GiaoVien(id, u, inputLine("Pass: "), inputLine("Ho Ten: "), inputLine("Mon Day: "));
                cout << "Da them GV.\n";
            }
            else if (ch == 2) {
                for (int i = 0; i < userCount; i++) {
                    if (users[i]->getRole() == "GiaoVien") users[i]->hienThiThongTin();
                }
            }
            else if (ch == 3) {
                int id = inputInt("Nhap ID GV can sua: ");
                for (int i = 0; i < userCount; i++) {
                    if (users[i]->getRole() == "GiaoVien" && users[i]->getId() == id) {
                        GiaoVien* gv = (GiaoVien*)users[i]; 
                        cout << "Thong tin cu: \n"; gv->hienThiThongTin();
                        gv->setFullName(inputLine("Ten moi: "));
                        gv->setMonDay(inputLine("Mon moi: "));
                        gv->setPassword(inputLine("Pass moi: "));
                        cout << "Sua thanh cong.\n";
                        break;
                    }
                }
            }
            else if (ch == 4) {
                int id = inputInt("Nhap ID can xoa: ");
                for (int i = 0; i < userCount; i++) {
                    if (users[i]->getRole() == "GiaoVien" && users[i]->getId() == id) {
                        delete users[i]; 
                        for (int j = i; j < userCount - 1; j++) users[j] = users[j + 1];
                        userCount--;
                        for (int k = 0; k < classCount; k++) {
                            if (classes[k].teacherId == id) classes[k].teacherId = 0;
                        }
                        cout << "Da xoa GV.\n";
                        break;
                    }
                }
            }
        } while (ch != 0);
    }

    void qlHocSinh() {
        int ch;
        do {
            ch = inputInt("\n--- QL HOC SINH ---\n1. Them HS vao Lop\n2. Xem danh sach\n3. Sua HS\n4. Xoa HS\n0. Back\nChon: ");
            if (ch == 1) {
                if (userCount >= MAX_USERS) { cout << "Danh sach da day!\n"; continue; }
                string tenLop = inputLine("Ten lop de them HS: ");
                bool lopExist = false;
                for (int i = 0; i < classCount; i++) {
                    if (classes[i].tenLop == tenLop) { lopExist = true; break; }
                }
                if (!lopExist) { cout << "Lop khong ton tai!\n"; continue; }

                int id = inputPositiveInt("ID: ");
                if (isIdExists(id)) { cout << "ID trung!\n"; continue; }
                string u = inputLine("User: ");
                if (isUsernameExists(u)) { cout << "User trung!\n"; continue; }
                
                users[userCount++] = new HocSinh(id, u, inputLine("Pass: "), inputLine("Ho Ten: "), tenLop);
                cout << "Da them HS.\n";
            }
            else if (ch == 2) {
                for (int i = 0; i < userCount; i++) {
                    if (users[i]->getRole() == "HocSinh") users[i]->hienThiThongTin();
                }
            }
            else if (ch == 3) {
                int id = inputInt("Nhap ID HS can sua: ");
                for (int i = 0; i < userCount; i++) {
                    if (users[i]->getRole() == "HocSinh" && users[i]->getId() == id) {
                        HocSinh* hs = (HocSinh*)users[i];
                        cout << "Thong tin cu: \n"; hs->hienThiThongTin();
                        hs->setFullName(inputLine("Ten moi: "));
                        hs->setLop(inputLine("Lop moi: "));
                        hs->setPassword(inputLine("Pass moi: "));
                        cout << "Sua thanh cong.\n";
                        break;
                    }
                }
            }
            else if (ch == 4) {
                int id = inputInt("Nhap ID can xoa: ");
                for (int i = 0; i < userCount; i++) {
                    if (users[i]->getRole() == "HocSinh" && users[i]->getId() == id) {
                        delete users[i];
                        for (int j = i; j < userCount - 1; j++) users[j] = users[j + 1];
                        userCount--;
                        cout << "Da xoa HS.\n";
                        break;
                    }
                }
            }
        } while (ch != 0);
    }

    void viewSystemRanking() {
        cout << "\n=== BANG XEP HANG TOAN TRUONG (CHI TINH BAI THI CHINH THUC) ===\n";
        struct HSScore { int id; string ten; double avgDiem; };
        HSScore bxh[MAX_USERS];
        int bxhCount = 0;

        for (int i = 0; i < userCount; i++) {
            if (users[i]->getRole() == "HocSinh") {
                HocSinh* hs = (HocSinh*)users[i];
                double total = 0; int count = 0;
                for (int j = 0; j < resultCount; j++) {
                    if (danhSachKetQua[j].idSV == hs->getId() && danhSachKetQua[j].loaiThi == 1) {
                        total += danhSachKetQua[j].diem;
                        count++;
                    }
                }
                double avg = (count == 0) ? 0 : (total / count);
                bxh[bxhCount].id = hs->getId();
                bxh[bxhCount].ten = hs->getFullName();
                bxh[bxhCount].avgDiem = avg;
                bxhCount++;
            }
        }

        for (int i = 0; i < bxhCount - 1; i++) {
            for (int j = 0; j < bxhCount - i - 1; j++) {
                if (bxh[j].avgDiem < bxh[j+1].avgDiem) {
                    hoanVi(bxh[j], bxh[j+1]); 
                }
            }
        }

        for (int i = 0; i < bxhCount; i++) {
            cout << i + 1 << ". ID: " << bxh[i].id << " | Ten: " << bxh[i].ten << " | Diem TB: " << bxh[i].avgDiem << "\n";
        }
    }
    
    void themLopHoc() {
        if(classCount >= MAX_CLASSES) { cout << "Danh sach lop day!\n"; return; }
        ClassInfo cl;
        cl.tenLop = inputLine("Ten lop: ");
        cl.teacherId = 0;
        classes[classCount++] = cl;
        cout << "Da them lop.\n";
    }
    
    void xemVaGanGV() {
        for (int i=0; i<classCount; i++) cout << "Lop: " << classes[i].tenLop << " | GVCN ID: " << classes[i].teacherId << "\n";
        int idGV = inputInt("Nhap ID GV de gan (0 de bo qua): ");
        if (idGV != 0) {
            string lop = inputLine("Gan cho lop nao: ");
            for (int i=0; i<classCount; i++) {
                if (classes[i].tenLop == lop) classes[i].teacherId = idGV;
            }
        }
    }
};


// ==============================================================================
//                              PHẦN 6: TRIỂN KHAI MENU CHỨC NĂNG
// ==============================================================================

// --- TRIỂN KHAI TỪ LỚP ADMIN ---
void Admin::menu(TruongHoc& sys) {
    int choice;
    do {
        cout << "\n========== MENU ADMIN ==========\n";
        cout << "1. Quan ly Lop hoc (Them/Xem/Gan GV)\n2. Quan ly Giao Vien\n3. Quan ly Hoc Sinh\n"
             << "4. Xem Thong bao\n5. Xem Bang xep hang\n6. Luu file\n7. Tai file\n0. Thoat\n";
        choice = inputInt("Chon: ");

        if (choice == 1) {
            int c = inputInt("\n1. Them Lop  2. Xem & Gan GV \nChon: ");
            if (c == 1) sys.themLopHoc();
            else if (c == 2) sys.xemVaGanGV();
        }
        else if (choice == 2) sys.qlGiaoVien();
        else if (choice == 3) sys.qlHocSinh();
        else if (choice == 4) sys.viewNotifications();
        else if (choice == 5) sys.viewSystemRanking();
        else if (choice == 6) sys.saveData();
        else if (choice == 7) sys.loadData();
    } while (choice != 0);
}

// --- TRIỂN KHAI TỪ LỚP GIÁO VIÊN ---
void GiaoVien::menu(TruongHoc& sys) {
    int choice;
    do {
        cout << "\n========== MENU GIAO VIEN ==========\n";
        cout << "1. Xem hoc sinh dang chu nhiem\n2. Thong ke Diem TB (Chinh thuc)\n3. Quan ly De Thi\n4. Xem Ket Qua Thi\n5. Doi mat khau\n0. Thoat\n";
        choice = inputInt("Chon: ");

        if (choice == 1) {
            cout << "--- HOC SINH LOP BAN CHU NHIEM ---\n";
            ClassInfo* cls = sys.getClasses();
            for (int i = 0; i < sys.getClassCount(); i++) {
                if (cls[i].teacherId == this->id) {
                    cout << ">> Lop: " << cls[i].tenLop << "\n";
                    Person** users = sys.getUsers();
                    for (int j = 0; j < sys.getUserCount(); j++) {
                        if (users[j]->getRole() == "HocSinh") {
                            HocSinh* hs = (HocSinh*)users[j];
                            if (hs->getLop() == cls[i].tenLop) hs->hienThiThongTin();
                        }
                    }
                }
            }
        }
        else if (choice == 2) {
            string monThongKe = inputLine("Nhap Ten Mon can thong ke: ");
            cout << "--- THONG KE DIEM MON " << monThongKe << " ---\n";
            
            struct HSData { string ten; double diemMon; };
            HSData dsThongKe[MAX_USERS];
            int dsCount = 0;

            ClassInfo* cls = sys.getClasses();
            for (int i = 0; i < sys.getClassCount(); i++) {
                if (cls[i].teacherId == this->id) {
                    Person** users = sys.getUsers();
                    for (int j = 0; j < sys.getUserCount(); j++) {
                        if (users[j]->getRole() == "HocSinh") {
                            HocSinh* hs = (HocSinh*)users[j];
                            if (hs->getLop() == cls[i].tenLop) {
                                double maxDiem = -1;
                                KetQuaThi* kq = sys.getResults();
                                for (int k = 0; k < sys.getResultCount(); k++) {
                                    if (kq[k].idSV == hs->getId() && kq[k].tenMon == monThongKe && kq[k].loaiThi == 1) {
                                        if (kq[k].diem > maxDiem) maxDiem = kq[k].diem;
                                    }
                                }
                                if (maxDiem >= 0) {
                                    dsThongKe[dsCount].ten = hs->getFullName();
                                    dsThongKe[dsCount].diemMon = maxDiem;
                                    dsCount++;
                                }
                            }
                        }
                    }
                }
            }
            
            if(dsCount == 0) cout << "Khong co du lieu bai thi Chinh thuc.\n";
            else {
                for(int i=0; i<dsCount-1; i++) {
                    for(int j=0; j<dsCount-i-1; j++) {
                        if(dsThongKe[j].diemMon < dsThongKe[j+1].diemMon) {
                            hoanVi(dsThongKe[j], dsThongKe[j+1]); 
                        }
                    }
                }
                for(int i=0; i<dsCount; i++) cout << "HS: " << dsThongKe[i].ten << " | Diem mon: " << dsThongKe[i].diemMon << "\n";
            }
        }
        else if (choice == 3) {
            int ch2;
            do {
                ch2 = inputInt("\n--- QUAN LY DE THI ---\n1. Tao De Moi\n2. Them Cau Hoi\n3. Sua Cau Hoi\n4. Xoa Cau Hoi\n0. Back\nChon: ");
                DeThi* deThis = sys.getExams();
                if (ch2 == 1) {
                    if(sys.getExamCount() >= MAX_EXAMS) { cout << "Toi da so luong de.\n"; continue; }
                    int idx = sys.getExamCount();
                    deThis[idx].maDe = inputPositiveInt("Ma de: ");
                    deThis[idx].tenMon = inputLine("Ten mon: ");
                    deThis[idx].thoiGian = inputPositiveInt("Thoi gian (phut): ");
                    deThis[idx].soLuongCauHoi = 0;
                    do {
                        deThis[idx].loaiThi = inputInt("Loai bai thi (1. Chinh thuc | 2. On tap): ");
                    } while (deThis[idx].loaiThi != 1 && deThis[idx].loaiThi != 2);
                    sys.setExamCount(idx + 1);
                    cout << "Da tao de.\n";
                }
                else if (ch2 == 2) {
                    int maDe = inputInt("Them vao Ma De nao: ");
                    for (int i = 0; i < sys.getExamCount(); i++) {
                        if (deThis[i].maDe == maDe) {
                            if(deThis[i].soLuongCauHoi >= MAX_QUESTIONS) { cout << "De nay da day cau hoi.\n"; break; }
                            int qIdx = deThis[i].soLuongCauHoi;
                            deThis[i].danhSachCauHoi[qIdx].id = inputPositiveInt("ID cau hoi: ");
                            deThis[i].danhSachCauHoi[qIdx].noiDung = inputLine("Noi dung: ");
                            for (int j = 0; j < 4; j++) deThis[i].danhSachCauHoi[qIdx].dapAn[j] = inputLine("Dap an " + intToString(j+1) + ": ");
                            do {
                                deThis[i].danhSachCauHoi[qIdx].dapAnDung = inputInt("Dap an dung (1-4): ");
                            } while(deThis[i].danhSachCauHoi[qIdx].dapAnDung < 1 || deThis[i].danhSachCauHoi[qIdx].dapAnDung > 4);
                            
                            deThis[i].soLuongCauHoi++;
                            cout << "Da them cau hoi!\n"; break;
                        }
                    }
                }
                else if (ch2 == 3 || ch2 == 4) {
                    int maDe = inputInt("Nhap Ma De: ");
                    for (int i = 0; i < sys.getExamCount(); i++) {
                        if (deThis[i].maDe == maDe) {
                            for(int j=0; j<deThis[i].soLuongCauHoi; j++) cout << "Cau " << deThis[i].danhSachCauHoi[j].id << ": " << deThis[i].danhSachCauHoi[j].noiDung << "\n";
                            
                            int idCH = inputInt("Nhap ID cau hoi can thao tac: ");
                            if (ch2 == 3) { 
                                for(int j=0; j<deThis[i].soLuongCauHoi; j++) {
                                    if(deThis[i].danhSachCauHoi[j].id == idCH) {
                                        deThis[i].danhSachCauHoi[j].noiDung = inputLine("Noi dung moi: ");
                                        for(int k=0; k<4; k++) deThis[i].danhSachCauHoi[j].dapAn[k] = inputLine("Dap an " + intToString(k+1) + ": ");
                                        do {
                                            deThis[i].danhSachCauHoi[j].dapAnDung = inputInt("Dap an dung (1-4): ");
                                        } while(deThis[i].danhSachCauHoi[j].dapAnDung < 1 || deThis[i].danhSachCauHoi[j].dapAnDung > 4);
                                        cout << "Sua xong!\n"; break;
                                    }
                                }
                            } else { 
                                for(int j=0; j<deThis[i].soLuongCauHoi; j++) {
                                    if(deThis[i].danhSachCauHoi[j].id == idCH) {
                                        for(int k=j; k<deThis[i].soLuongCauHoi-1; k++) deThis[i].danhSachCauHoi[k] = deThis[i].danhSachCauHoi[k+1];
                                        deThis[i].soLuongCauHoi--;
                                        cout << "Xoa xong!\n"; break;
                                    }
                                }
                            }
                        }
                    }
                }
            } while (ch2 != 0);
        }
        else if (choice == 4) {
            int md = inputInt("Nhap Ma De muon xem diem: ");
            cout << "--- KET QUA MA DE " << md << " ---\n";
            KetQuaThi* kq = sys.getResults();
            for (int i = 0; i < sys.getResultCount(); i++) {
                if (kq[i].maDe == md) cout << "ID HS: " << kq[i].idSV << " | Ten: " << kq[i].tenSV << " | Diem: " << kq[i].diem << " | Thoi gian: " << kq[i].thoiGianLamBai << "p\n";
            }
        }
        else if (choice == 5) {
            setPassword(inputLine("Nhap mat khau moi: "));
            sys.addNotification("[Bao Mat] Giao vien " + fullName + " da doi mat khau.");
            cout << "Doi mat khau thanh cong!\n";
        }
    } while (choice != 0);
}

// --- TRIỂN KHAI TỪ LỚP HỌC SINH ---
void HocSinh::menu(TruongHoc& sys) {
    int choice;
    do {
        cout << "\n========== MENU HOC SINH ==========\n";
        cout << "1. Xem Thong tin\n2. DS bai thi chua lam\n3. Lam bai\n4. Lich su thi\n5. Thong ke muc tieu\n6. Doi mat khau\n0. Thoat\nChon: ";
        choice = inputInt("");

        if (choice == 1) { hienThiThongTin(); }
        else if (choice == 2) {
            cout << "--- DANH SACH BAI THI CO THE LAM ---\n";
            DeThi* dtList = sys.getExams();
            KetQuaThi* kqList = sys.getResults();
            bool coBai = false;

            for (int i = 0; i < sys.getExamCount(); i++) {
                bool thiChinhThucRoi = false;
                if (dtList[i].loaiThi == 1) {
                    for (int j = 0; j < sys.getResultCount(); j++) {
                        if (kqList[j].idSV == this->id && kqList[j].maDe == dtList[i].maDe && kqList[j].loaiThi == 1) {
                            thiChinhThucRoi = true; break;
                        }
                    }
                }
                if (!thiChinhThucRoi) {
                    string loaiStr = (dtList[i].loaiThi == 1) ? "[CHINH THUC (1 lan)]" : "[ON TAP (n lan)]";
                    cout << "Ma de: " << dtList[i].maDe << " | Mon: " << dtList[i].tenMon << " " << loaiStr << "\n";
                    coBai = true;
                }
            }
            if (!coBai) cout << "Ban da lam het de Chinh thuc hien co!\n";
        }
        else if (choice == 3) {
            int chonDe = inputInt("Nhap Ma De thi: ");
            DeThi* deHienTai = NULL;
            DeThi* dtList = sys.getExams();
            for (int i = 0; i < sys.getExamCount(); i++) {
                if (dtList[i].maDe == chonDe) { deHienTai = &dtList[i]; break; }
            }

            if (deHienTai == NULL || deHienTai->soLuongCauHoi == 0) { cout << "De trong!\n"; continue; }
            
            bool block = false;
            KetQuaThi* kqList = sys.getResults();
            if (deHienTai->loaiThi == 1) {
                for (int i = 0; i < sys.getResultCount(); i++) {
                    if (kqList[i].idSV == this->id && kqList[i].maDe == deHienTai->maDe && kqList[i].loaiThi == 1) { block = true; break; }
                }
            }
            if (block) { cout << "Day la de Chinh thuc ban DA THI!\n"; continue; }
            
            int soCauDung = 0;
            for (int i = 0; i < deHienTai->soLuongCauHoi; i++) {
                cout << "Cau " << deHienTai->danhSachCauHoi[i].id << ": " << deHienTai->danhSachCauHoi[i].noiDung << "\n";
                for (int j = 0; j < 4; j++) cout << j + 1 << ". " << deHienTai->danhSachCauHoi[i].dapAn[j] << "\n";
                int chon;
                do {
                    chon = inputInt("Dap an (1-4): ");
                } while(chon < 1 || chon > 4);
                if (chon == deHienTai->danhSachCauHoi[i].dapAnDung) soCauDung++;
            }
            
            double diemCuoi = (double)soCauDung / deHienTai->soLuongCauHoi * 10.0;
            int tgThucTe = (rand() % deHienTai->thoiGian) + 1; 
            cout << "=> Nop bai thanh cong. Thoi gian: " << tgThucTe << "p. Diem: " << diemCuoi << "\n";
            
            KetQuaThi newKq = { this->id, this->fullName, deHienTai->maDe, deHienTai->tenMon, diemCuoi, tgThucTe, deHienTai->loaiThi };
            sys.addResult(newKq);
        }
        else if (choice == 4) {
            KetQuaThi* kqList = sys.getResults();
            for (int i = 0; i < sys.getResultCount(); i++) {
                if (kqList[i].idSV == this->id) {
                    string loaiStr = (kqList[i].loaiThi == 1) ? "[Chinh thuc]" : "[On tap]";
                    cout << "Ma: " << kqList[i].maDe << " " << loaiStr << " | Mon: " << kqList[i].tenMon 
                         << " | T.Gian: " << kqList[i].thoiGianLamBai << "p | Diem: " << kqList[i].diem << "\n";
                }
            }
        }
        else if (choice == 5) {
            double tong = 0; int dem = 0;
            KetQuaThi* kqList = sys.getResults();
            for (int i = 0; i < sys.getResultCount(); i++) {
                if (kqList[i].idSV == this->id && kqList[i].loaiThi == 1) { tong += kqList[i].diem; dem++; }
            }
            double dtb = (dem == 0) ? 0 : tong / dem;
            cout << "=> DTB Tich Luy (Chinh thuc): " << dtb << "\n";
            if (dtb >= 8.0) cout << "Ban dang dat muc Gioi.\n";
            else {
                cout << "De len Gioi (8.0), can tich luy them: " << 8.0 - dtb << " diem.\n";
                if (dtb >= 6.5) cout << "Ban dang o muc Kha.\n";
                else cout << "De len Kha (6.5), can them: " << 6.5 - dtb << " diem.\n";
            }
        }
        else if (choice == 6) {
            setPassword(inputLine("Mat khau moi: "));
            sys.addNotification("[Bao Mat] HS " + fullName + " da doi pass.");
            cout << "Thanh cong.\n";
        }
    } while (choice != 0);
}


// ==============================================================================
//                              PHẦN 7: HÀM MAIN
// ==============================================================================
int main() {
    TruongHoc heThong;
    int load = inputInt("Ban co muon load data tu file khong? (1.Co / 2.Khong): ");
    if (load == 1) heThong.loadData();

    while (true) {
        cout << "\n===== HE THONG TRUONG HOC =====\n";
        string u = inputLine("Username (Nhap 'exit' de thoat): ");
        if (u == "exit") break;
        string p = inputLine("Password: ");

        Person* loggedInUser = NULL;
        Person** users = heThong.getUsers();
        for (int i = 0; i < heThong.getUserCount(); i++) {
            if (users[i]->getUsername() == u && users[i]->getPassword() == p) {
                loggedInUser = users[i];
                break;
            }
        }

        if (loggedInUser != NULL) {
            cout << "\n=> DANG NHAP THANH CONG! Vai tro: " << loggedInUser->getRole() << "\n";
            loggedInUser->menu(heThong);
        } else {
            cout << "Sai thong tin dang nhap!\n";
        }
    }
    return 0;
}