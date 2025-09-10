Small Clinic Management System (single-file C++)

Mô tả:
- Hệ thống quản lý nhỏ cho phòng khám gia đình.
- Phiên bản đơn giản, gộp 1 file ClinicSystem.cpp để dễ nộp.
- Thể hiện: các lớp, kế thừa, override, tạo/hủy/hoàn tất lịch hẹn, cập nhật lịch sử bệnh án.

Yêu cầu build:
- C++11 hoặc mới hơn
Biên dịch:
g++ -std=c++11 ClinicSystem.cpp -o clinic
Chạy:
./clinic

Gồm các lớp:
- Human (abstract)
- Patient (kế thừa Human)
- ChronicPatient (kế thừa Patient; override preScheduleCheck và needsFrequentFollowUp)
- Doctor (kế thừa Human)
- Appointment
- Clinic (quản lý bệnh nhân, bác sĩ, lịch hẹn)

Lưu ý:
- Code viết cho mục đích học tập; xử lý lỗi cơ bản qua cout.
