#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/* ==================== LOP CO BAN: Human ==================== */

class Human {
protected:
    string name; 
    string id;   
public:
    Human(): name(""), id("") {}
    Human(const string& n, const string& i): name(n), id(i) {}
    virtual ~Human() {}
    string getName() const { return name; }
    string getId() const { return id; }
    virtual void displayInfo() const = 0; // phuong thuc truu tuong
};

/* ==================== LOP Appointment ==================== */
  
enum class AppointmentStatus { SCHEDULED, COMPLETED, CANCELED };

class Appointment {
private:
    string apptId;      // ma lich hen
    string datetime;    // "YYYY-MM-DD HH:MM"
    string reason;      // ly do kham
    AppointmentStatus status;
    string patientId;   // id benh nhan
    string doctorId;    // id bac si
public:
    Appointment() : apptId(""), datetime(""), reason(""),
                    status(AppointmentStatus::SCHEDULED),
                    patientId(""), doctorId("") {}
    Appointment(const string& id, const string& dt, const string& r,
                const string& pid, const string& did)
        : apptId(id), datetime(dt), reason(r), status(AppointmentStatus::SCHEDULED),
          patientId(pid), doctorId(did) {}
    string getId() const { return apptId; }
    string getDateTime() const { return datetime; }
    string getPatientId() const { return patientId; }
    string getDoctorId() const { return doctorId; }
    AppointmentStatus getStatus() const { return status; }
    void setStatus(AppointmentStatus s) { status = s; }

    // In thong tin ngan gon cua lich hen (dung trong list)
    void displayBrief() const {
        cout << "Appointment " << apptId << " | " << datetime
             << " | Patient: " << patientId << " | Doctor: " << doctorId
             << " | Status: "
             << (status==AppointmentStatus::SCHEDULED ? "Scheduled"
                 : status==AppointmentStatus::COMPLETED ? "Completed" : "Canceled")
             << " | Reason: " << reason << "\n";
    }
};

/* ==================== LOP Patient (base) ====================*/
class Patient : public Human {
private:
    int age;
    vector<string> history; // lich su benh an (dang chu thich)
    vector<string> apptIds; // danh sach id lich hen dang cho
public:
    Patient(): Human(), age(0) {}
    Patient(const string& n, const string& i, int a) : Human(n,i), age(a) {}
    virtual ~Patient() {}

    int getAge() const { return age; }
    void addHistory(const string& note) { history.push_back(note); }
    void addAppointmentId(const string& a) { apptIds.push_back(a); }
    void removeAppointmentId(const string& a) {
        apptIds.erase(remove(apptIds.begin(), apptIds.end(), a), apptIds.end());
    }
    const vector<string>& getAppointments() const { return apptIds; }

    // Phuong thuc ao: lop con co the override
    virtual bool needsFrequentFollowUp() const { return false; }

    // Kiem tra truoc khi len lich: regular patient mac dinh chap nhan
    virtual bool preScheduleCheck(const string& datetime) {
        // Thong bao nhe de quan sat hanh vi
        cout << "[Patient] " << name << " requests appointment at " << datetime << "\n";
        return true;
    }

    // In thong tin chi tiet benh nhan
    virtual void displayInfo() const override {
        cout << "Patient: " << name << " | ID: " << id << " | Age: " << age << "\n";
        if (history.empty()) cout << "  No medical history\n";
        else {
            cout << "  Medical history:\n";
            for (const auto& h : history) cout << "   - " << h << "\n";
        }
    }
};

/* ==================== LOP ChronicPatient ====================*/
class ChronicPatient : public Patient {
private:
    string condition;     // tinh trang benh man tinh
    string lastCheckup;   // ngay kiem tra cuoi cung (YYYY-MM-DD)
public:
    ChronicPatient() : Patient(), condition(""), lastCheckup("") {}
    ChronicPatient(const string& n, const string& i, int a,
                   const string& cond, const string& last)
        : Patient(n,i,a), condition(cond), lastCheckup(last) {}
    virtual ~ChronicPatient() {}

    // Chronic patient yeu cau theo doi thong thuong
    virtual bool needsFrequentFollowUp() const override { return true; }

    // Ghi de de thong bao, kiem tra them truoc khi len lich
    virtual bool preScheduleCheck(const string& datetime) override {
        cout << "[ChronicPatient] " << getName() << " (condition: " << condition
             << ", last check: " << lastCheckup << ") requests " << datetime << "\n";
        cout << "  Note: Khuyen nghi kiem tra dinh ky ~3 thang.\n";
        return true;
    }

    // In thong tin ke thua va them thong tin chronic
    virtual void displayInfo() const override {
        cout << "Chronic Patient: " << getName() << " | ID: " << getId()
             << " | Condition: " << condition << " | Last Checkup: " << lastCheckup << "\n";
        Patient::displayInfo();
    }
};

/* ==================== LOP Doctor ====================*/
class Doctor : public Human {
private:
    string specialty;
    vector<string> apptIds;
public:
    Doctor() : Human(), specialty("") {}
    Doctor(const string& n, const string& i, const string& s) : Human(n,i), specialty(s) {}
    virtual ~Doctor() {}

    void addAppointmentId(const string& a) { apptIds.push_back(a); }
    void removeAppointmentId(const string& a) {
        apptIds.erase(remove(apptIds.begin(), apptIds.end(), a), apptIds.end());
    }
    const vector<string>& getAppointments() const { return apptIds; }

    // Bac si cap nhat trang thai tren doi tuong Appointment (tham chieu)
    void updateAppointmentStatus(Appointment& a, AppointmentStatus s) {
        a.setStatus(s);
        cout << "[Doctor] " << name << " set " << a.getId() << " -> "
             << (s==AppointmentStatus::SCHEDULED ? "Scheduled"
                 : s==AppointmentStatus::COMPLETED ? "Completed" : "Canceled") << "\n";
    }

    virtual void displayInfo() const override {
        cout << "Doctor: " << name << " | ID: " << id << " | Specialty: " << specialty << "\n";
    }
};

/* ==================== LOP Clinic (quan ly) ====================
   - Clinic la lop quan ly chinh: luu danh sach benh nhan, bac si, lich hen
*/
class Clinic {
private:
    vector<Patient*> patients;     // con tro den Patient (de su dung da hinh)
    vector<Doctor*> doctors;       // con tro den Doctor
    vector<Appointment> appointments;
    int nextApptNo;                // de sinh id lich dang tang
public:
    Clinic(): nextApptNo(1) {}
    ~Clinic() {
        // Giai phong bo nho cho cac doi tuong duoc tao dong (new)
        for (auto p : patients) delete p;
        for (auto d : doctors) delete d;
    }

    // Them benh nhan vao danh sach clinic
    void addPatient(Patient* p) {
        patients.push_back(p);
        cout << "[Clinic] Added patient " << p->getName() << " (ID: " << p->getId() << ")\n";
    }

    // Them bac si vao danh sach clinic
    void addDoctor(Doctor* d) {
        doctors.push_back(d);
        cout << "[Clinic] Added doctor " << d->getName() << " (ID: " << d->getId() << ")\n";
    }

    // Tim benh nhan theo id, tra ve con tro hoac nullptr neu khong tim thay
    Patient* findPatient(const string& pid) {
        for (auto p : patients) if (p->getId() == pid) return p;
        return nullptr;
    }

    // Tim bac si theo id
    Doctor* findDoctor(const string& did) {
        for (auto d : doctors) if (d->getId() == did) return d;
        return nullptr;
    }

    // Sinh ma lich hen don gian: "A1", "A2", ...
    string genApptId() { return "A" + to_string(nextApptNo++); }

    /* scheduleAppointment:
       - Kiem tra benh nhan va bac si ton tai
       - Tao Appointment va luu, cap nhat danh sach benh nhan va bac si
    */
    bool scheduleAppointment(const string& pid, const string& did,
                             const string& datetime, const string& reason) {
        Patient* p = findPatient(pid);
        Doctor* d = findDoctor(did);
        if (!p) { cout << "[Clinic] Khong tim thay benh nhan ID " << pid << "\n"; return false; }
        if (!d) { cout << "[Clinic] Khong tim thay bac si ID " << did << "\n"; return false; }

        // Goi ham kiem tra cua benh nhan (polymorphism)
        if (!p->preScheduleCheck(datetime)) { cout << "[Clinic] Kiem tra lich that bai\n"; return false; }

        string apid = genApptId();
        Appointment a(apid, datetime, reason, pid, did);
        appointments.push_back(a);
        p->addAppointmentId(apid);
        d->addAppointmentId(apid);
        cout << "[Clinic] Da len lich " << apid << " cho benh nhan " << p->getName()
             << " voi bac si " << d->getName() << " vao " << datetime << "\n";
        return true;
    }

    /* cancelAppointment:    */
    bool cancelAppointment(const string& apid) {
        for (auto &a : appointments) {
            if (a.getId() == apid) {
                if (a.getStatus() == AppointmentStatus::CANCELED) {
                    cout << "[Clinic] " << apid << " da bi huy truoc do\n";
                    return false;
                }
                a.setStatus(AppointmentStatus::CANCELED);
                Patient* p = findPatient(a.getPatientId());
                Doctor* d = findDoctor(a.getDoctorId());
                if (p) p->removeAppointmentId(apid);
                if (d) d->removeAppointmentId(apid);
                cout << "[Clinic] Huy lich " << apid << "\n";
                return true;
            }
        }
        cout << "[Clinic] Khong tim thay lich " << apid << "\n";
        return false;
    }

    /* completeAppointment:    */
    bool completeAppointment(const string& apid, const string& notes) {
        for (auto &a : appointments) {
            if (a.getId() == apid) {
                a.setStatus(AppointmentStatus::COMPLETED);
                Patient* p = findPatient(a.getPatientId());
                if (p) {
                    p->addHistory("Hoan tat " + apid + ": " + notes);
                    p->removeAppointmentId(apid);
                }
                Doctor* d = findDoctor(a.getDoctorId());
                if (d) d->removeAppointmentId(apid);
                cout << "[Clinic] Hoan tat " << apid << "\n";
                return true;
            }
        }
        cout << "[Clinic] Khong tim thay lich " << apid << "\n";
        return false;
    }

    // In tat ca lich hen (dung de test)
    void listAppointments() const {
        cout << "=== Danh sach lich hen ===\n";
        if (appointments.empty()) { cout << "  Chua co lich hen\n"; return; }
        for (const auto &a : appointments) a.displayBrief();
    }

    // In thong tin benh nhan (dung trong test)
    void displayPatientInfo(const string& pid) const {
        for (auto p : patients) {
            if (p->getId() == pid) { p->displayInfo(); return; }
        }
        cout << "Khong tim thay benh nhan " << pid << "\n";
    }

    // Lay danh sach id lich hien co (de test)
    vector<string> getAppointmentIds() const {
        vector<string> res;
        for (const auto &a : appointments) res.push_back(a.getId());
        return res;
    }
};

/* ==================== MAIN: demo / test cases ====================*/
int main() {
    cout << "=== Demo Small Clinic Management System ===\n\n";
    Clinic clinic;

    // Tao benh nhan regular va them lich su
    Patient* p1 = new Patient("Doan Trong Trung", "P01", 30);
    p1->addHistory("2025-01-10: Kham cam, ke don nghi ngoi");
    clinic.addPatient(p1);

    // Tao benh nhan chronic voi thong tin condition va lastCheckup
    ChronicPatient* p2 = new ChronicPatient("Ly Tran Gia Khang", "P02", 55, "Tieu duong typ2", "2025-06-01");
    p2->addHistory("2025-06-01: Tang lieu thuoc dieu tri");
    clinic.addPatient(p2);

    // Tao mot bac si
    Doctor* d1 = new Doctor("Dr. Phi", "D01", "Noi tong quat");
    clinic.addDoctor(d1);

    // Len lich cho 2 benh nhan
    clinic.scheduleAppointment("P01", "D01", "2025-09-20 10:00", "Trieu chung cam");
    clinic.scheduleAppointment("P02", "D01", "2025-09-22 09:00", "Theo doi tieu duong");

    cout << "\n-- Sau khi len lich --\n";
    clinic.listAppointments();

    // Hoan tat lich dau tien 
    auto ids = clinic.getAppointmentIds();
    if (!ids.empty()) clinic.completeAppointment(ids[0], "Khoi, khong can thuoc them");

    // Huy lich thu hai neu co
    if (ids.size() > 1) clinic.cancelAppointment(ids[1]);

    cout << "\n-- Trang thai cuoi --\n";
    clinic.listAppointments();

    cout << "\n-- Thong tin benh nhan P02 --\n";
    clinic.displayPatientInfo("P02");

    cout << "\n=== Ket thuc demo ===\n";
    return 0;
}