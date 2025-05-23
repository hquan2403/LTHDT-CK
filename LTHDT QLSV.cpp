#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QMessageBox>
#include <QInputDialog>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <set>

using namespace std;

// --- Giữ nguyên các lớp và hàm của bạn ---
const double HocPhi1TC = 450000;
const double HocPhiEvening = 4000000;
const double PhuPhiTaiLieu = 50000;


// Định nghĩa lớp Sinh viên
class SV {
private:
    string Masinhvien;
    string Hoten;
    string Date;
    string Nganh;
    string Gmail;
public:
	// Construct , coppy construct , destruct
    SV(string _Masinhvien = "1234", string _Hoten = "quandepzai", string _Date = "1/1/2000",
       string _Nganh = "CNTT", string _Gmail = "ElonMusk@gmail.com") {
        Masinhvien = _Masinhvien;
        Hoten = chuanHoaTen(_Hoten);
        Date = chuanHoaNS(_Date);
        Nganh = _Nganh;
        Gmail = taoGmail(Hoten, Masinhvien);
    }
    SV(const SV &k) {
        Masinhvien = k.Masinhvien;
        Hoten = k.Hoten;
        Date = k.Date;
        Nganh = k.Nganh;
        Gmail = k.Gmail;
    }
    ~SV() {}
    
    //Hàm tạo gmail tự động
    string taoGmail(string hoten, string masinhvien) {
        string gmail = "";
        for (char c : hoten) {
            if (c != ' ') gmail += tolower(c);
        }
        gmail += "_" + masinhvien;
        gmail += "@gmail.hcmue.edu.vn";
        return gmail;
    }
    
    // Hàm chuẩn hóa tên tự động in hoa chữ cái đầu
    string chuanHoaTen(string s) {
        stringstream ss(s);
        string res = "", x;
        while (ss >> x) {
            x[0] = toupper(x[0]);
            for (int i = 1; i < x.size(); i++) x[i] = tolower(x[i]);
            res += x + " ";
        }
        res.pop_back();
        return res;
    }
    
    //Hàm xác định năm nhuận
    bool NamNhuan(int nam) {
        return (nam % 4 == 0 && nam % 100 != 0) || (nam % 400 == 0);
    }
    
    
    //Hàm xem ngày nhập có hợp lệ không
    bool Ngay(int ngay, int thang, int nam) {
        if (nam < 1900 || nam > 2100 || thang < 1 || thang > 12 || ngay < 1) return false;
        int ngayTrongThang[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (thang == 2 && NamNhuan(nam)) return ngay <= 29;
        return ngay <= ngayTrongThang[thang];
    }
    
    
    //Hàm chuẩn hóa ngày tháng theo dạng dd/mm/yyyy
    string chuanHoaNS(const string& s) {
        int ngay, thang, nam;
        char sep1, sep2;
        stringstream ss1(s);
        if (ss1 >> ngay >> sep1 >> thang >> sep2 >> nam) {
            string thua;
            if (sep1 != '/' || sep2 != '/' || (ss1 >> thua)) return "";
            if (!Ngay(ngay, thang, nam)) return "";
            stringstream kq;
            kq << setw(2) << setfill('0') << ngay << "/" << setw(2) << setfill('0') << thang << "/" << nam;
            return kq.str();
        }
        // Nhập không gạch (3052006 hay 300052006)
        string s_clean = s;
        for (char& c : s_clean) {
            if (!isdigit(c)) return "";
        }
        if (s_clean.length() == 8) {
        	// Giả sử là dd mm yyyy
            ngay = stoi(s_clean.substr(0, 2));
            thang = stoi(s_clean.substr(2, 2));
            nam = stoi(s_clean.substr(4, 4));
        } else if (s_clean.length() == 7) {
        	// Giả sử là d mm yyyy hoặc dd m yyyy
            int d1 = stoi(s_clean.substr(0, 1));
            int m1 = stoi(s_clean.substr(1, 2));
            int y1 = stoi(s_clean.substr(3, 4));
            if (Ngay(d1, m1, y1)) {
                ngay = d1;
                thang = m1;
                nam = y1;
            } else {
                int d2 = stoi(s_clean.substr(0, 2));
                int m2 = stoi(s_clean.substr(2, 1));
                int y2 = stoi(s_clean.substr(3, 4));
                if (Ngay(d2, m2, y2)) {
                    ngay = d2;
                    thang = m2;
                    nam = y2;
                } else return "";
            }
        } else return "";
        if (!Ngay(ngay, thang, nam)) return "";
        stringstream kq;
        kq << setw(2) << setfill('0') << ngay << "/" << setw(2) << setfill('0') << thang << "/" << nam;
        return kq.str();
    }
    
    
    // Get
    string getMasinhvien() { return Masinhvien; }
    string getHoten() { return Hoten; }
    string getDate() { return Date; }
    string getNganh() { return Nganh; }
    string getGmail() { return Gmail; }
    
    
    //Set
    void setMasinhvien(string a = " ") {
        Masinhvien = a;
        Gmail = taoGmail(Hoten, Masinhvien);
    }
    void setHoten(string a = " ") {
        Hoten = chuanHoaTen(a);
        Gmail = taoGmail(Hoten, Masinhvien);
    }
    void setDate(string a = " ") { Date = a; }
    void setNganh(string a = " ") { Nganh = a; }
    void setGmail(string a = " ") { Gmail = a; }
    
    
    // Quá tải nhập, xuất
    friend istream& operator>>(istream& is, SV &a) {
        cout << "Ma sinh vien: " << a.Masinhvien << endl;
        cout << "Nhap ho ten: ";
        getline(is, a.Hoten);
        a.Hoten = a.chuanHoaTen(a.Hoten);
        cout << "Nhap ngay sinh: ";
        getline(is, a.Date);
        a.Date = a.chuanHoaNS(a.Date);
        cout << "Nhap Nganh: ";
        getline(is, a.Nganh);
        a.Gmail = a.taoGmail(a.Hoten, a.Masinhvien);
        return is;
    }
    
    
    friend ostream& operator<<(ostream& out, SV &a) {
        out << ". Loai sinh vien: " << a.getLoai() << endl;
        out << "Ma sinh vien: " << a.getMasinhvien() << endl;
        out << "Ho ten: " << a.getHoten() << endl;
        out << "Ngay sinh: " << a.getDate() << endl;
        out << "Nganh: " << a.getNganh() << endl;
        out << "Gmail: " << a.getGmail() << endl;
        return out;
    }
    
    
    //Hàm chuẩn hóa định dạng tiền (1tr100000 dong)
    string formatTien(long long soTien) const {
        if (soTien < 0) return "-" + formatTien(-soTien);
        if (soTien < 1000000) return to_string(soTien) + " dong";
        long long trieu = soTien / 1000000;
        long long le = (soTien % 1000000) / 1000;    // Lấy phần nghìn
        if (le == 0) return to_string(trieu) + "tr dong";
        return to_string(trieu) + "tr" + to_string(le) + " dong";
    }
    
    
    // Hàm nhập, xuất
    virtual void Nhap() {
        cin >> *this;
        Hoten = chuanHoaTen(Hoten);
        Date = chuanHoaNS(Date);
    }
    virtual void Xuat() { cout << *this; }
    
    
    // Đa hình tính học phí, loại sinh viên
    virtual long long TinhHocPhi() = 0;
    virtual string getLoai() = 0;
    virtual string getLoai2() = 0;
};

//Định nghĩa lớp Sinh Viên Chính Quy
class SV_ChinhQuy : public SV {
private:
    int Hocbong;
    int Tinchi;
public:
	
	// Construct , coppy construct , destruct
    SV_ChinhQuy(string _Masinhvien = "1234", string _Hoten = "quandepzai", string _Date = "1/1/2000",
                string _Nganh = "CNTT", string _Gmail = "ElonMusk@gmail.com", int _Tinchi = 1, int _Hocbong = 1)
        : SV(_Masinhvien, _Hoten, _Date, _Nganh, _Gmail) {
        Hocbong = _Hocbong;
        Tinchi = _Tinchi;
    }
    SV_ChinhQuy(const SV_ChinhQuy &k) : SV(k) {
        Hocbong = k.Hocbong;
        Tinchi = k.Tinchi;
    }
    ~SV_ChinhQuy() {}
    
    
    
    // Get
    int getHocbong() { return Hocbong; }
    int getTinchi() { return Tinchi; }
    
    
    
    
    // Set
    void setHocbong(int a = 1) { Hocbong = a; }
    void setTinchi(int a = 1) { Tinchi = a; }
    
    
    //Quá tải nhập, xuất
    friend istream& operator>>(istream& is, SV_ChinhQuy &a) {
        a.SV::Nhap();
        cout << "So tin chi: ";
        is >> a.Tinchi;
        cout << "Hoc bong: ";
        is >> a.Hocbong;
        return is;
    }
    
    
    friend ostream& operator<<(ostream& os, SV_ChinhQuy &a) {
        a.SV::Xuat();
        os << "So hoc phi can dong la: " << a.formatTien(a.TinhHocPhi()) << " dong" << endl;
        return os;
    }
    
    
    //Tính học phí, lấy loại sinh viên
    long long TinhHocPhi() { return Tinchi * HocPhi1TC - Hocbong; }  //Tính theo công thức: (Tín chỉ * Học phí 1 tín chỉ) - Học bổng
    string getLoai2() { return "CQ"; }
    
    
    
    //lấy loại sinh viên để xuất
    string getLoai() { return "SV Chinh Quy"; }
    
    
    //Hàm nhập, xuất
    void Nhap() {
        cin >> *this;
        cout << endl;
    }
    void Xuat() { cout << *this; }
};



//Định nghĩa lớp Sinh Viên Vừa Học Vừa Làm
class SV_VuaHocVuaLam : public SV {
private:
    string Noilamviec;
    int Ky;
public:
	
	
	// Construct , coppy construct , destruct
    SV_VuaHocVuaLam(string _Masinhvien = "1234", string _Hoten = "quandepzai", string _Date = "1/1/2000",
                    string _Nganh = "CNTT", string _Gmail = "ElonMusk@gmail.com", string _Noilamviec = "HCMUE", int _Ky = 1)
        : SV(_Masinhvien, _Hoten, _Date, _Nganh, _Gmail) {
        Noilamviec = _Noilamviec;
        Ky = _Ky;
    }
    SV_VuaHocVuaLam(const SV_VuaHocVuaLam &k) : SV(k) {
        Noilamviec = k.Noilamviec;
        Ky = k.Ky;
    }
    ~SV_VuaHocVuaLam() {}
    
    
    //Get
    string getNoilamviec() { return Noilamviec; }
    int getKy() { return Ky; }
    
    
    
    //Set
    void setNoilamviec(string a = " ") { Noilamviec = a; }
    void setKy(int a = 1) { Ky = a; }
    
    
    //Quá tải nhập, xuất
    friend istream& operator>>(istream& is, SV_VuaHocVuaLam &a) {
        a.SV::Nhap();
        cout << "Noi lam viec: ";
        is.ignore();
        getline(is, a.Noilamviec);
        cout << "So luong hoc ki: ";
        is >> a.Ky;
        return is;
    }
    
    friend ostream& operator<<(ostream& os, SV_VuaHocVuaLam &a) {
        a.SV::Xuat();
        os << "Noi lam viec: " << a.Noilamviec << endl;
        os << "So luong hoc ki: " << a.Ky << endl;
        os << "So hoc phi can dong la: " << a.formatTien(a.TinhHocPhi()) << " dong" << endl;
        return os;
    }
    
    
    //Tính học phí, lấy loại sinh viên
    long long TinhHocPhi() { return HocPhiEvening * Ky; }  //Tính theo công thức: Học phí buổi tối * số kỳ
    string getLoai2() { return "VHVL"; }
    
    //Láy loại sinh viên để xuất
    string getLoai() { return "SV Vua hoc vua lam"; }
    
    
    //Hàm nhập, xuất
    void Nhap() {
        cin >> *this;
        cout << endl;
    }
    void Xuat() { cout << *this; }
};


//Định nghĩa lớp Sinh Viên Đào Tạo Từ Xa
class SV_DaoTaoTuXa : public SV {
private:
    string Diachi;
    int Tinchi;
    int CacLoaiPhiKhac;
public:
	
	
	// Construct , coppy construct , destruct
    SV_DaoTaoTuXa(string _Masinhvien = "1234", string _Hoten = "quandepzai", string _Date = "1/1/2000",
                  string _Nganh = "CNTT", string _Gmail = "ElonMusk@gmail.com", string _Diachi = "quan 5",
                  int _Tinchi = 1, int _CacLoaiPhiKhac = 1)
        : SV(_Masinhvien, _Hoten, _Date, _Nganh, _Gmail) {
        Diachi = _Diachi;
        Tinchi = _Tinchi;
        CacLoaiPhiKhac = _CacLoaiPhiKhac;
    }
    SV_DaoTaoTuXa(const SV_DaoTaoTuXa &k) : SV(k) {
        Diachi = k.Diachi;
        Tinchi = k.Tinchi;
        CacLoaiPhiKhac = k.CacLoaiPhiKhac;
    }
    ~SV_DaoTaoTuXa() {}
    
    
    //Get
    string getDiachi() { return Diachi; }
    int getTinchi() { return Tinchi; }
    int getCacLoaiPhiKhac() { return CacLoaiPhiKhac; }
    
    
    
    //Set
    void setDiachi(string a = " ") { Diachi = a; }
    void setTinchi(int a = 1) { Tinchi = a; }
    void setCacLoaiPhiKhac(int a = 1) { CacLoaiPhiKhac = a; }
    
    
    //Quá tải nhập, xuất
    friend istream& operator>>(istream& is, SV_DaoTaoTuXa &a) {
        a.SV::Nhap();
        cout << "Dia chi: ";
        is.ignore();
        getline(is, a.Diachi);
        cout << "Tin chi: ";
        is >> a.Tinchi;
        cout << "Cac loai phi khac: ";
        is >> a.CacLoaiPhiKhac;
        return is;
    }
    
    friend ostream& operator<<(ostream& os, SV_DaoTaoTuXa &a) {
        a.SV::Xuat();
        os << "So hoc phi can dong la: " << a.formatTien(a.TinhHocPhi()) << " dong" << endl;
        os << "Dia chi: " << a.Diachi << endl;
        return os;
    }
    
    
    //Tính tiền, lấy loại sinh viên
    long long TinhHocPhi() { return Tinchi * HocPhi1TC + PhuPhiTaiLieu + CacLoaiPhiKhac; }  //Tính tiền theo công thức: (Tín chỉ * Phí tài liệu) - Các chi phí khác
    string getLoai2() { return "DTTX"; }
    
    
    //Lấy loại sinh viên để xuất
    string getLoai() { return "SV Dao tao tu xa"; }
    
    
    //Hàm Nhập, xuất
    void Nhap() {
        cin >> *this;
        cout << endl;
    }
    void Xuat() { cout << *this; }
};


//Định nghĩa lớp Quản Lý Sinh viên
class QuanLySV {
private:
    int n = 0;
    vector<SV*> sv;
    
    //Hàm tự động tạo mã sinh viên theo thứ tự ( SV000001 ) 
    string TaoMaSinhVien() {
        int maxId = 0;
        for (SV* student : sv) {
            string msv = student->getMasinhvien();
            int id = stoi(msv.substr(2));
            maxId = max(maxId, id);
        }
        stringstream ss;
        ss << "SV" << setw(6) << setfill('0') << (maxId + 1);
        return ss.str();
    }
public:
	
	
	//Hàm Cập nhật sinh viên mỗi lần chạy chương trình
    void CapNhatSV() {
        ifstream fin("DanhSachSinhVien.txt");  //Mở file DanhSachSinhVien.txt để đọc dữ liệu sinh viên
        if (!fin.is_open()) {
            cout << "Khong the mo file DanhSachSinhVien.txt!" << endl; 
            return;
        }
        string s;
        while (fin >> s) {	//Đọc loại sinh viên
            fin.ignore();
            string masv, hoten, date, nganh, gmail;
            getline(fin, hoten);
            getline(fin, date);
            getline(fin, nganh);
            
            
            // Các biến phục vụ cho từng loại sinh viên cụ thể.
            int hocbong, tinchi;
            string Noilamviec;
            int Ky;
            string Diachi;
            int CacLoaiPhiKhac;
            
            
            SV* t_sv = nullptr;	// Con trỏ cơ sở để trỏ đến đối tượng sinh viên tạo ra.
            
            masv = TaoMaSinhVien();	 // Tạo mã sinh viên mới tự động theo dạng SV000001.
            
            if (s == "CQ") {
                fin >> tinchi >> hocbong;
                t_sv = new SV_ChinhQuy(masv, hoten, date, nganh, gmail, tinchi, hocbong); 	// Tạo đối tượng SV_ChinhQuy và gán vào con trỏ t_sv.
            } else if (s == "VHVL") {
                getline(fin, Noilamviec);
                fin >> Ky;
                t_sv = new SV_VuaHocVuaLam(masv, hoten, date, nganh, gmail, Noilamviec, Ky);	// Tạo đối tượng SV_VuaHocVuaLam và gán vào con trỏ t_sv.
            } else if (s == "DTTX") {
                getline(fin, Diachi);
                fin >> tinchi >> CacLoaiPhiKhac;
                t_sv = new SV_DaoTaoTuXa(masv, hoten, date, nganh, gmail, Diachi, tinchi, CacLoaiPhiKhac);	// Tạo đối tượng SV_DaoTaoTuXa và gán vào con trỏ t_sv.
            } else return;
            sv.push_back(t_sv);	//Thêm sinh viên mới vào file
            n++;	//Tăng biến đếm
        }
        fin.close();
    }
    
    
    //Hàm thêm sinh viên bằng cách nhập
    void ThemSVQt(string s, string hoten, string date, string nganh, int tinchi, int hocbong,
                  string noilamviec, int ky, string diachi, int cacloaiphikhac) {
        ofstream fout("DanhSachSinhVien.txt", ios::app);
        
        fout << s << endl;	// Ghi loại sinh viên ("CQ", "VHVL", "DTTX") vào file
        
        SV* t_sv = nullptr;		// Con trỏ sinh viên, sẽ trỏ đến đối tượng tương ứng
        
        string masv = TaoMaSinhVien();	// Tạo mã sinh viên tự động
        
        fout << hoten << endl;	//Thêm tên sinh viên vào file
        fout << date << endl;	//Thêm ngày tháng năm sinh sinh viên vào vile
        fout << nganh << endl;	//Thêm ngành sinh viên vào vile
        if (s == "CQ") {
            fout << tinchi << endl;
            fout << hocbong << endl;
            t_sv = new SV_ChinhQuy(masv, hoten, date, nganh, "", tinchi, hocbong);	// Tạo đối tượng SV_ChinhQuy
        } else if (s == "VHVL") {
            fout << noilamviec << endl;
            fout << ky << endl;
            t_sv = new SV_VuaHocVuaLam(masv, hoten, date, nganh, "", noilamviec, ky);	// Tạo đối tượng SV_VuaHocVuaLam
        } else {
            fout << diachi << endl;
            fout << tinchi << endl;
            fout << cacloaiphikhac << endl;
            t_sv = new SV_DaoTaoTuXa(masv, hoten, date, nganh, "", diachi, tinchi, cacloaiphikhac);	// Tạo đối tượng SV_DaoTaoTuXa
        }
        sv.push_back(t_sv); // Thêm con trỏ sinh viên vào danh sách sv
    	fout << endl; 
    	fout.close(); 
    	n++; // Tăng biến đếm số lượng sinh viên
    }
    
    
    //Hàm xóa sinh viên bằng Binary Sreach
    void XoaSVTheoMSVQt(string maSV) {
        SV* svCanXoa = BS(sv, maSV);	// Tìm sinh viên có mã maSV bằng tìm kiếm nhị phân
        if (svCanXoa != nullptr) {	// Nếu tìm thấy sinh viên cần xóa
            auto it = find(sv.begin(), sv.end(), svCanXoa);
            if (it != sv.end()) {
                delete *it;
                sv.erase(it);
                n--;
                ofstream fout("DanhSachSinhVien.txt", ios::trunc);	 // Mở file và ghi đè
                if (!fout.is_open()) {
                    cout << "Khong the mo file DanhSachSinhVien.txt!" << endl;
                    return;
                }
                for (int i = 0; i < n; i++) {
                    fout << sv[i]->getLoai2() << endl; // Ghi loại sinh viên (CQ, VHVL, DTTX)
                	fout << sv[i]->getHoten() << endl; // Ghi họ tên
                	fout << sv[i]->getDate() << endl; // Ghi ngày sinh
                	fout << sv[i]->getNganh() << endl; // Ghi ngành học
                    if (sv[i]->getLoai2() == "CQ") {
                        SV_ChinhQuy* cq = dynamic_cast<SV_ChinhQuy*>(sv[i]);	// Ép kiểu con trỏ
                        fout << cq->getTinchi() << endl;
                        fout << cq->getHocbong() << endl;
                    } else if (sv[i]->getLoai2() == "VHVL") {
                        SV_VuaHocVuaLam* vhvl = dynamic_cast<SV_VuaHocVuaLam*>(sv[i]);
                        fout << vhvl->getNoilamviec() << endl;
                        fout << vhvl->getKy() << endl;
                    } else if (sv[i]->getLoai2() == "DTTX") {
                        SV_DaoTaoTuXa* dttx = dynamic_cast<SV_DaoTaoTuXa*>(sv[i]);
                        fout << dttx->getDiachi() << endl;
                        fout << dttx->getTinchi() << endl;
                        fout << dttx->getCacLoaiPhiKhac() << endl;
                    }
                    fout << endl;
                }
                fout.close();
            }
        }
    }
    
    
    //Binary Sreach
    SV* BS(vector<SV*> &ds, string maSV) {
        sort(ds.begin(), ds.end(), [](SV* a, SV* b) {
            return a->getMasinhvien() < b->getMasinhvien();
        });
        int l = 0, r = ds.size() - 1;
        while (l <= r) {
            int m = (l + r) / 2;
            if (ds[m]->getMasinhvien() == maSV) return ds[m];
            else if (ds[m]->getMasinhvien() < maSV) l = m + 1;
            else r = m - 1;
        }
        return nullptr;
    }
    
    
    // Hàm tìm kiếm sinh viên theo mã sinh viên
    string TimSVTheoMSVQt(string maSV) {
        SV* ketQua = BS(sv, maSV);	// Gọi hàm BS để tìm sinh viên theo mã maSV
        if (ketQua != nullptr) {	// Nếu tìm thấy sinh viên
            stringstream ss;
            ss << "THONG TIN SINH VIEN TIM DUOC:\n";
            ss << "Ma SV: " << ketQua->getMasinhvien() << endl;
            ss << "Ho ten: " << ketQua->getHoten() << endl;
            ss << "Ngay sinh: " << ketQua->getDate() << endl;
            ss << "Nganh: " << ketQua->getNganh() << endl;
            ss << "Gmail: " << ketQua->getGmail() << endl;
            ss << "Loai SV: " << ketQua->getLoai() << endl;
            ss << "Hoc phi: " << ketQua->formatTien(ketQua->TinhHocPhi()) << endl;
            return ss.str();	// Trả về toàn bộ nội dung chuỗi
        }
        return "Khong tim thay sinh vien!\n";	// Nếu không tìm thấy, trả về thông báo
    }
    
    
    //Hàm đọc danh sách sinh viên
    string DocDanhSachSVQt() {
        ofstream fout("view.txt");	// Mở file view.txt để ghi danh sách sinh viên
        if (!fout.is_open()) {
            return "Khong the mo file find.txt de ghi!\n";	// Báo lỗi nếu không mở được file
        }
        stringstream ss;	// Dùng để xây dựng chuỗi kết quả trả về
        ss << "DANH SACH SINH VIEN\n";
        ss << "===================\n";
        for (int i = 0; i < n; i++) {
            ss << (i + 1) << ". Loai sinh vien: " << sv[i]->getLoai() << endl;
            ss << "Ma sinh vien: " << sv[i]->getMasinhvien() << endl;
            ss << "Ho ten: " << sv[i]->getHoten() << endl;
            ss << "Ngay sinh: " << sv[i]->getDate() << endl;
            ss << "Nganh: " << sv[i]->getNganh() << endl;
            ss << "Gmail: " << sv[i]->getGmail() << endl;
            ss << "So hoc phi can dong la: " << sv[i]->formatTien(sv[i]->TinhHocPhi()) <<  endl;
            
            // Ghi thêm thông tin riêng nếu là SV VHVL hoặc DTTX
            if (sv[i]->getLoai2() == "VHVL") {
                SV_VuaHocVuaLam* vhvl = dynamic_cast<SV_VuaHocVuaLam*>(sv[i]);
                ss << "Noi lam viec: " << vhvl->getNoilamviec() << endl;
                fout << "Noi lam viec: " << vhvl->getNoilamviec() << endl;
            } else if (sv[i]->getLoai2() == "DTTX") {
                SV_DaoTaoTuXa* dttx = dynamic_cast<SV_DaoTaoTuXa*>(sv[i]);
                ss << "Dia chi: " << dttx->getDiachi() << endl;
                fout << "Dia chi: " << dttx->getDiachi() << endl;
            }
            
			ss << endl;
            
            // Ghi nội dung tương tự vào file view.txt
            fout << (i + 1) << ". Loai sinh vien: " << sv[i]->getLoai() << endl;
            fout << "Ma sinh vien: " << sv[i]->getMasinhvien() << endl;
            fout << "Ho ten: " << sv[i]->getHoten() << endl;
            fout << "Ngay sinh: " << sv[i]->getDate() << endl;
            fout << "Nganh: " << sv[i]->getNganh() << endl;
            fout << "Gmail: " << sv[i]->getGmail() << endl;
            fout << "So hoc phi can dong la: " << sv[i]->formatTien(sv[i]->TinhHocPhi()) << endl;
            fout << endl;
        }
        ss << "===================\n";
        fout << "===================" << endl;
        fout.close();
        return ss.str();
    }
    
    
    //Hàm xóa toàn bộ sinh viên
    string XoaToanBoSVQt() {
        for (SV* student : sv) delete student;	// Giải phóng bộ nhớ cấp phát cho từng sinh viên
        sv.clear();	// Xóa toàn bộ con trỏ khỏi vector sv
        n = 0;	// Đặt lại số lượng sinh viên về 0
        
        ofstream foutDSSV("DanhSachSinhVien.txt", ios::trunc);
        if (!foutDSSV.is_open()) return "Khong the mo file DanhSachSinhVien.txt!\n";
        foutDSSV.close();
        ofstream foutView("view.txt", ios::trunc);	// Mở file view.txt để xóa toàn bộ nội dung

        if (!foutView.is_open()) return "Khong the mo file view.txt!\n";	// Nếu không mở được file thì báo lỗi
        foutView << "DANH SACH SINH VIEN" << endl;
        foutView << "===================" << endl;
        foutView << "===================" << endl;
        foutView.close();
        
        return "Da xoa toan bo danh sach sinh vien va cap nhat file!\n";	// Trả về thông báo thành công
    }
    ~QuanLySV() {
        for (int i = 0; i < n; i++) delete sv[i];
    }
};

// --- Giao diện Qt ---
class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    QuanLySV quanLy; // Đối tượng quản lý sinh viên
    QTextEdit *outputText; // Ô văn bản hiển thị kết quả
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        // Thiết lập tiêu đề và kích thước cửa sổ
        setWindowTitle("Quản Lý Sinh Viên");
        resize(600, 400);

        // Tạo widget trung tâm và layout dọc
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);

        // Tạo ô văn bản hiển thị kết quả
        outputText = new QTextEdit(this);
        outputText->setReadOnly(true);
        outputText->setText("Chào mừng bạn đến với chương trình Quản Lý Sinh Viên!");
        layout->addWidget(outputText);

        // Tạo các nút bấm
        QPushButton *addButton = new QPushButton("Thêm Sinh Viên", this);
        QPushButton *deleteButton = new QPushButton("Xóa Sinh Viên Theo MSSV (cùng tệp view.txt)", this);
        QPushButton *searchButton = new QPushButton("Tìm Sinh Viên Theo MSSV", this);
        QPushButton *listButton = new QPushButton("Xuất ra Danh Sách Sinh Viên (cùng tệp view.txt)", this);
        QPushButton *deleteAllButton = new QPushButton("Xóa Toàn Bộ Danh Sách (cùng tệp view.txt)", this);

        // Thêm các nút vào layout
        layout->addWidget(addButton);
        layout->addWidget(deleteButton);
        layout->addWidget(searchButton);
        layout->addWidget(listButton);
        layout->addWidget(deleteAllButton);

        // Kết nối các nút với hàm xử lý
        connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
        connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteButtonClicked);
        connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchButtonClicked);
        connect(listButton, &QPushButton::clicked, this, &MainWindow::onListButtonClicked);
        connect(deleteAllButton, &QPushButton::clicked, this, &MainWindow::onDeleteAllButtonClicked);

        // Đặt widget trung tâm cho cửa sổ
        setCentralWidget(centralWidget);

        // Cập nhật danh sách từ file
        quanLy.CapNhatSV();
    }
private slots:
    void onAddButtonClicked() {
        // Chọn loại sinh viên
        QStringList loaiSV = {"CQ", "VHVL", "DTTX"};
        bool ok;
        QString loai = QInputDialog::getItem(this, "Thêm Sinh Viên", "Chọn loại sinh viên:", loaiSV, 0, false, &ok);
        if (!ok) return;

        // Nhập thông tin chung
        QString hoten = QInputDialog::getText(this, "Thêm Sinh Viên", "Nhập họ tên:", QLineEdit::Normal, "", &ok);
        if (!ok) return;
        QString date;
        bool validFormat = false;
        do {
            date = QInputDialog::getText(this, "Thêm Sinh Viên", "Nhập ngày sinh (dd/mm/yyyy hoặc ddmmyyyy):", QLineEdit::Normal, "", &ok);
            if (!ok) return;

            // Kiểm tra độ dài chuỗi
            if (date.length() != 8 && date.length() != 10) {
                QMessageBox::warning(this, "Lỗi Định Dạng", "Ngày sinh phải có 8 (ddmmyyyy) hoặc 10 (dd/mm/yyyy) ký tự!");
                continue;
            }

            // Kiểm tra chỉ chứa số hoặc dấu /
            bool validChars = true;
            for (QChar c : date) {
                if (!c.isDigit() && c != '/') {
                    validChars = false;
                    break;
                }
            }
            if (!validChars) {
                QMessageBox::warning(this, "Lỗi Ký Tự", "Ngày sinh chỉ được chứa số (0-9) hoặc dấu /!");
                continue;
            }

            validFormat = true;
        } while (!validFormat);
        if (!ok) return;
        QString nganh = QInputDialog::getText(this, "Thêm Sinh Viên", "Nhập ngành:", QLineEdit::Normal, "", &ok);
        if (!ok) return;

        int tinchi = 0, hocbong = 0, ky = 0, cacloaiphikhac = 0;
        QString noilamviec, diachi;

        // Nhập thông tin theo loại sinh viên
        if (loai == "CQ") {
            tinchi = QInputDialog::getInt(this, "Thêm Sinh Viên", "Số tín chỉ:", 0, 0, 100, 1, &ok);
            if (!ok) return;
            hocbong = QInputDialog::getInt(this, "Thêm Sinh Viên", "Số học bổng:", 0, 0, 100000000, 1, &ok);
            if (!ok) return;
        } else if (loai == "VHVL") {
            noilamviec = QInputDialog::getText(this, "Thêm Sinh Viên", "Nơi làm việc:", QLineEdit::Normal, "", &ok);
            if (!ok) return;
            ky = QInputDialog::getInt(this, "Thêm Sinh Viên", "Số kỳ:", 0, 0, 100, 1, &ok);
            if (!ok) return;
        } else {
            diachi = QInputDialog::getText(this, "Thêm Sinh Viên", "Địa chỉ:", QLineEdit::Normal, "", &ok);
            if (!ok) return;
            tinchi = QInputDialog::getInt(this, "Thêm Sinh Viên", "Số tín chỉ:", 0, 0, 100, 1, &ok);
            if (!ok) return;
            cacloaiphikhac = QInputDialog::getInt(this, "Thêm Sinh Viên", "Các loại phí khác:", 0, 0, 100000000, 1, &ok);
            if (!ok) return;
        }

        // Thêm sinh viên
        quanLy.ThemSVQt(loai.toStdString(), hoten.toStdString(), date.toStdString(), nganh.toStdString(),
                        tinchi, hocbong, noilamviec.toStdString(), ky, diachi.toStdString(), cacloaiphikhac);
        outputText->setText("Đã thêm sinh viên thành công!");
    }
    void onDeleteButtonClicked() {
        bool ok;
        QString maSV = QInputDialog::getText(this, "Xóa Sinh Viên", "Nhập mã sinh viên:", QLineEdit::Normal, "", &ok);
        if (!ok) return;
        quanLy.XoaSVTheoMSVQt(maSV.toStdString());
        outputText->setText("Đã xóa sinh viên (nếu tồn tại)!");
    }
    void onSearchButtonClicked() {
        bool ok;
        QString maSV = QInputDialog::getText(this, "Tìm Sinh Viên", "Nhập mã sinh viên:", QLineEdit::Normal, "", &ok);
        if (!ok) return;
        string result = quanLy.TimSVTheoMSVQt(maSV.toStdString());
        outputText->setText(QString::fromStdString(result));
    }
    void onListButtonClicked() {
        string result = quanLy.DocDanhSachSVQt();
        outputText->setText(QString::fromStdString(result));
    }
    void onDeleteAllButtonClicked() {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Xóa Toàn Bộ",
                                                                  "Bạn có chắc muốn xóa toàn bộ danh sách sinh viên?",
                                                                  QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            string result = quanLy.XoaToanBoSVQt();
            outputText->setText(QString::fromStdString(result));
        } else {
            outputText->setText("Hủy thao tác xóa!");
        }
    }
};

#include "main.moc" // Cần cho moc compiler của Qt

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}



