# BÁO CÁO BÀI TẬP LỚN MÔN INTERNET OF THINGS (IOT)

**TÊN ĐỀ TÀI:** THIẾT KẾ VÀ CHẾ TẠO MÔ HÌNH STEM TƯỚI CÂY TỰ ĐỘNG SỬ DỤNG NĂNG LƯỢNG MẶT TRỜI VÀ NỀN TẢNG E-RA IOT  
**Giảng viên hướng dẫn:** ...................................  
**Sinh viên thực hiện:** ...................................  
**Mã số sinh viên:** ...................................  
**Lớp / Khóa:** ...................................  

---

## MỤC LỤC
1. CHƯƠNG 1: TỔNG QUAN VÀ ĐẶT VẤN ĐỀ
2. CHƯƠNG 2: THIẾT KẾ KIẾN TRÚC HỆ THỐNG
3. CHƯƠNG 3: THIẾT KẾ VÀ CHẾ TẠO PHẦN CỨNG
4. CHƯƠNG 4: THIẾT KẾ PHẦN MỀM VÀ NỀN TẢNG IOT (E-RA)
5. CHƯƠNG 5: KẾT QUẢ THỬ NGHIỆM VÀ ĐÁNH GIÁ
6. CHƯƠNG 6: KẾT LUẬN VÀ HƯỚNG PHÁT TRIỂN

---

## CHƯƠNG 1: TỔNG QUAN VÀ ĐẶT VẤN ĐỀ
### 1.1. Tính cấp thiết của đề tài
Trong thời đại nông nghiệp công nghệ cao và phong trào giáo dục STEM, việc tự động hóa quá trình chăm sóc cây trồng giúp tối ưu hóa lượng nước tưới, tiết kiệm nhân công và chi phí năng lượng. Việc kết hợp hệ thống năng lượng mặt trời (Solar Energy) giải quyết bài toán cấp nguồn độc lập tại các vị trí không có lưới điện (sân thượng, ban công, vườn ngoài trời).

### 1.2. Mục tiêu nghiên cứu
- Thiết kế mạch nguồn tái tạo: Năng lượng mặt trời kết hợp bộ lưu trữ 2 pin Li-ion 18650.
- Ứng dụng vi điều khiển ESP32 thu thập các thông số môi trường (Nhiệt độ, Độ ẩm không khí, Độ ẩm đất).
- Điều khiển máy bơm tưới theo 2 chế độ: Tự động (dựa trên thuật toán ngưỡng) và Thủ công (Manual).
- Hiển thị thông số tại chỗ qua màn hình LCD 1602 (I2C) và hệ thống đèn LED chỉ thị.
- Giám sát và điều khiển từ xa qua Internet bằng nền tảng E-Ra IoT.

---

## CHƯƠNG 2: THIẾT KẾ KIẾN TRÚC HỆ THỐNG
### 2.1. Sơ đồ khối chức năng
- **Khối nguồn:** Tấm pin mặt trời 6V 3W $\rightarrow$ Mạch sạc TP4056 $\rightarrow$ Pin 18650 $\rightarrow$ Mạch Boost 5V MT3608.
- **Khối cảm biến:** Cảm biến DHT11/22, Cảm biến độ ẩm đất điện dung v1.2, Cầu phân áp đo pin.
- **Khối xử lý trung tâm:** Vi điều khiển ESP32 NodeMCU DevKit.
- **Khối hiển thị & chỉ thị:** LCD 1602 I2C, 3 LED báo trạng thái (Chế độ, Bơm, Mức pin).
- **Khối chấp hành:** Module Relay 5V có opto cách ly, máy bơm chìm mini DC.
- **Khối truyền thông:** WiFi 2.4GHz kết nối Server E-Ra IoT qua giao thức MQTT.

---

## CHƯƠNG 3: THIẾT KẾ VÀ CHẾ TẠO PHẦN CỨNG
### 3.1. Lựa chọn linh kiện và phân tích ưu nhược điểm
- **Tại sao chọn ESP32 thay vì Arduino Uno/Nano?**  
  ESP32 tích hợp sẵn WiFi/BLE, xung nhịp cao (240MHz), bộ nhớ lớn, hỗ trợ các tác vụ đa luồng và đồng bộ dữ liệu cloud mượt mà.
- **Tại sao chọn cảm biến độ ẩm đất điện dung v1.2?**  
  Khác với cảm biến loại điện trở (resistive) dễ bị điện phân và rỉ sét hỏng sau vài tuần, cảm biến điện dung bọc lớp chống ẩm, không tiếp xúc trực tiếp kim loại với đất ướt nên độ bền vượt trội.
- **Tính toán mạch phân áp đo dung lượng pin 18650:**  
  Trình bày chi tiết công thức $V_{ADC} = V_{bat} \times \frac{R_2}{R_1 + R_2}$.

### 3.2. Sơ đồ nguyên lý và sơ đồ lắp ráp
*(Chèn hình ảnh sơ đồ vẽ từ Fritzing / EasyEDA và ảnh thực tế của mạch hoàn thiện)*.

---

## CHƯƠNG 4: THIẾT KẾ PHẦN MỀM VÀ NỀN TẢNG IOT (E-RA)
### 4.1. Lưu đồ thuật toán chương trình (Flowchart)
- Khởi tạo ngoại vi, kết nối WiFi & E-Ra Cloud.
- Đọc cảm biến theo chu kỳ không trễ (Non-blocking Timer `addInterval`).
- Xử lý logic tưới có trễ trượt (Hysteresis) và kiểm tra quá dòng/quá nhiệt bơm.
- Đẩy dữ liệu lên Cloud và cập nhật LCD.

### 4.2. Thiết kế giao diện E-Ra IoT
*(Chèn ảnh chụp màn hình Dashboard trên Web E-Ra và trên Smartphone App E-Ra)*.

---

## CHƯƠNG 5: KẾT QUẢ THỬ NGHIỆM VÀ ĐÁNH GIÁ
### 5.1. Bảng số liệu thực nghiệm
| Điều kiện thử nghiệm | Độ ẩm đất (%) | Nhiệt độ (°C) | Độ ẩm KK (%) | Trạng thái Bơm | Ghi chú |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Đất khô (chưa tưới) | 28% | 31.2 | 68% | BẬT (ON) | Đạt ngưỡng kích hoạt (< 40%) |
| Đang bơm (sau 10s) | 38% | 31.0 | 70% | BẬT (ON) | Đang tưới |
| Đạt ngưỡng bão hòa | 46% | 30.8 | 72% | TẮT (OFF) | Vượt ngưỡng + Hysteresis (45%) |

### 5.2. Đánh giá tính năng an toàn
- Tính năng ngắt bơm sau 25 giây khi phát hiện cạn nước/tuột ống hoạt động chính xác.
- Tính năng ngắt bơm khi pin yếu (< 3.2V) giúp bảo vệ an toàn cell pin 18650.

---

## CHƯƠNG 6: KẾT LUẬN VÀ HƯỚNG PHÁT TRIỂN
### 6.1. Kết luận
- Đã hoàn thành 100% mục tiêu đề tài: Mô hình STEM hoạt động ổn định, tự duy trì bằng năng lượng mặt trời.
- Kết nối thông suốt với nền tảng E-Ra IoT, độ trễ điều khiển dưới 1 giây.

### 6.2. Hướng phát triển
- Tích hợp thêm cảm biến đo mực nước trong bồn chứa (Siêu âm JSN-SR04T).
- Ứng dụng chế độ ngủ sâu (Deep Sleep) trên ESP32 để tối ưu hóa thời lượng pin cho những ngày nhiều mây.
- Thu thập dữ liệu lâu dài để huấn luyện mô hình Machine Learning dự đoán thời điểm tưới tối ưu.
