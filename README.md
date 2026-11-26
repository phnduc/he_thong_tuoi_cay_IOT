# DỰ ÁN MÔ HÌNH STEM TƯỚI CÂY THÔNG MINH (IOT)

Mô hình hệ thống tưới cây tự động thông minh sử dụng **Năng lượng mặt trời**, **2 Pin Li-ion 18650**, vi điều khiển **ESP32** và nền tảng đám mây **E-Ra IoT**.

---

## 📁 Cấu trúc thư mục dự án

```
IOT/
├── SmartWatering_ESP32.ino     # Mã nguồn chính Arduino / ESP32
├── config.h                    # Cấu hình chân GPIO, WiFi, E-Ra Token & thông số
├── SCHEMATIC_AND_WIRING.md     # Sơ đồ nguyên lý, sơ đồ khối & bảng đấu nối chân
├── ERA_IOT_SETUP_GUIDE.md      # Hướng dẫn tạo tài khoản, Virtual Pins & Dashboard E-Ra
├── CALIBRATION_GUIDE.md        # Hướng dẫn hiệu chuẩn cảm biến độ ẩm đất và đo pin
├── BAO_CAO_BAI_TAP_LON_MAU.md  # Khung đề cương báo cáo bài tập lớn môn IoT
└── README.md                   # Tài liệu hướng dẫn này
```

---

## 🚀 Các tính năng chính

1. **Năng lượng tái tạo:** Tấm pin mặt trời 5V-6V sạc vào 2 cell pin 18650 qua mạch sạc TP4056 + Tăng áp Boost 5V.
2. **Đo đạc môi trường:**
   - Nhiệt độ & Độ ẩm không khí bằng cảm biến DHT11/DHT22.
   - Độ ẩm đất bằng cảm biến điện dung Capacitive Soil Moisture v1.2 (chống ăn mòn).
   - Đo dung lượng điện áp pin 18650 qua cầu chia áp an toàn.
3. **Hiển thị trực quan:** Màn hình LCD 1602 giao tiếp I2C hiển thị thông số thời gian thực.
4. **Hệ thống LED chỉ thị:**
   - **LED Mode (GPIO 18):** Báo chế độ (Sáng = Tự động, Tắt = Thủ công).
   - **LED Pump (GPIO 19):** Báo trạng thái máy bơm (Sáng = Đang bơm).
   - **LED Battery (GPIO 23):** Cảnh báo pin yếu (< 20%).
5. **Điều khiển & Giám sát từ xa (E-Ra IoT):**
   - Theo dõi biểu đồ nhiệt độ, độ ẩm không khí, độ ẩm đất và % pin.
   - Chuyển đổi linh hoạt chế độ Tưới tự động / Tưới thủ công.
   - Nút bật/tắt bơm thủ công trên điện thoại.
   - Thanh trượt điều chỉnh ngưỡng độ ẩm đất tùy ý từ 10% đến 90%.
6. **Bảo vệ an toàn thông minh:**
   - Hysteresis chống rung đóng ngắt relay liên tục.
   - Tự động ngắt bơm sau 25s đề phòng cháy bơm hoặc hết nước.
   - Khóa bơm tự động khi điện áp pin giảm dưới 3.2V để tránh chai hỏng pin 18650.

---

## 🛠 Hướng dẫn cài đặt & Nạp code

### 1. Cài đặt Arduino IDE & Board ESP32
- Tải [Arduino IDE](https://www.arduino.cc/en/software) (bản 2.x trở lên).
- Vào **File -> Preferences -> Additional Boards Manager URLs**, thêm link:
  ```text
  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
  ```
- Vào **Tools -> Board -> Boards Manager**, tìm `esp32` và cài đặt bản mới nhất.

### 2. Cài đặt các thư viện cần thiết
Vào **Sketch -> Include Library -> Manage Libraries...**, tìm và cài đặt:
- `ERa` (by E-Ra)
- `DHT sensor library` (by Adafruit)
- `LiquidCrystal_I2C` (by Frank de Brabander)

### 3. Cấu hình thông số
Mở file [config.h](file:///Users/phanduc/Documents/IOT/config.h) và điền:
- `ERA_AUTH_TOKEN`: Lấy từ mục Device trên trang [E-Ra IoT](https://iot.e-ra.io).
- `WIFI_SSID` và `WIFI_PASS`: Tên và mật khẩu WiFi 2.4GHz tại nơi chạy mô hình.

### 4. Nạp code
- Chọn Board: **ESP32 Dev Module** (hoặc NodeMCU-32S).
- Chọn đúng Port COM của ESP32.
- Nhấn **Upload** để nạp chương trình vào bo mạch.

---

## 📚 Tài liệu chi tiết đính kèm
- Chi tiết sơ đồ nối dây: Xem [SCHEMATIC_AND_WIRING.md](file:///Users/phanduc/Documents/IOT/SCHEMATIC_AND_WIRING.md).
- Cấu hình E-Ra IoT: Xem [ERA_IOT_SETUP_GUIDE.md](file:///Users/phanduc/Documents/IOT/ERA_IOT_SETUP_GUIDE.md).
- Hiệu chuẩn cảm biến: Xem [CALIBRATION_GUIDE.md](file:///Users/phanduc/Documents/IOT/CALIBRATION_GUIDE.md).
- Mẫu báo cáo bài tập lớn: Xem [BAO_CAO_BAI_TAP_LON_MAU.md](file:///Users/phanduc/Documents/IOT/BAO_CAO_BAI_TAP_LON_MAU.md).
