# HƯỚNG DẪN SƠ ĐỒ NGUYÊN LÝ & KẾT NỐI DÂY (WIRING & SCHEMATIC)

Tài liệu này hướng dẫn chi tiết cách kết nối phần cứng cho **Hệ thống tưới cây thông minh mô hình STEM** sử dụng ESP32, pin mặt trời và 2 pin 18650.

---

## 1. Sơ đồ khối tổng thể hệ thống

```
+---------------------------------------------------------------------------------+
|                                KHỐI NGUỒN                                       |
|  [Tấm Solar 6V 3W] ---> [Mạch Sạc TP4056] <---> [2 x Pin 18650 (Song song 3.7V)] |
|                                   │                                             |
|                                   ▼                                             |
|                        [Mạch Boost 5V (MT3608/UPS)]                             |
+-----------------------------------┬────────────────────────────────────────-----+
                                    │ Cấp nguồn 5V ổn định
                                    ▼
+---------------------------------------------------------------------------------+
|                           KHỐI XỬ LÝ TRUNG TÂM (ESP32)                          |
|                                                                                 |
|   [Cảm biến DHT11/22] ──────────► GPIO 4                                        |
|   [Cảm biến Độ ẩm đất] ─────────► GPIO 34 (ADC1)                                |
|   [Cầu phân áp Pin 18650] ──────► GPIO 35 (ADC1)                                |
|                                                                                 |
|   GPIO 21 (SDA) ────────────────┐                                               |
|   GPIO 22 (SCL) ────────────────┴─► [Màn hình LCD 1602 + Module I2C]            |
|                                                                                 |
|   GPIO 18 ──────────────────────► [LED Báo Chế độ (Auto/Manual)]                |
|   GPIO 19 ──────────────────────► [LED Báo Bơm chạy]                            |
|   GPIO 23 ──────────────────────► [LED Báo Pin yếu]                             |
|                                                                                 |
|   GPIO 25 ──────────────────────► [Module Relay 5V Opto] ──► [Máy Bơm Mini 5V]  |
|                                                                                 |
|   WiFi / Bluetooth ─────────────► [E-Ra IoT Cloud Platform & App Mobile]        |
+---------------------------------------------------------------------------------+
```

---

## 2. Bảng đấu nối chân chi tiết (Wiring Pinout Table)

### A. Màn hình LCD 1602 (Giao tiếp I2C)
| Chân LCD I2C | Chân ESP32 | Chú thích |
| :--- | :--- | :--- |
| **GND** | GND | Nối mass chung |
| **VCC** | 5V (VIN) | Cấp nguồn 5V từ mạch Boost |
| **SDA** | **GPIO 21** | Đường truyền dữ liệu I2C |
| **SCL** | **GPIO 22** | Đường truyền xung nhịp I2C |

### B. Cảm biến nhiệt độ & độ ẩm không khí (DHT11 hoặc DHT22)
| Chân DHT | Chân ESP32 | Chú thích |
| :--- | :--- | :--- |
| **VCC** | 3.3V | Cấp nguồn ổn định từ ESP32 |
| **DATA** | **GPIO 4** | Kèm trở kéo 4.7kΩ - 10kΩ lên 3.3V (nếu dùng loại 4 chân trần) |
| **GND** | GND | Nối mass chung |

### C. Cảm biến độ ẩm đất (Capacitive Soil Moisture Sensor v1.2)
| Chân Cảm biến | Chân ESP32 | Chú thích |
| :--- | :--- | :--- |
| **VCC** | 3.3V | Khuyên dùng 3.3V để ngõ ra không vượt quá điện áp ADC ESP32 |
| **GND** | GND | Nối mass chung |
| **AOUT** | **GPIO 34** | Chân Analog (thuộc nhóm ADC1, không bị xung đột WiFi) |

### D. Mạch cầu phân áp đo điện áp Pin 18650
| Điểm kết nối | Chân ESP32 / Linh kiện | Chú thích |
| :--- | :--- | :--- |
| Cực (+) Pin 18650 | Đầu điện trở **R1 (100kΩ)** | Điện áp pin dao động từ 3.2V - 4.2V |
| Điểm giữa R1 và R2 | **GPIO 35** | Điện áp đưa vào ADC ESP32 dao động từ 1.6V - 2.1V (rất an toàn) |
| Đầu còn lại **R2 (100kΩ)** | GND | Nối mass chung |

```
(+) Pin 18650 (3.2V - 4.2V)
        │
       [R1: 100kΩ]
        │
        ├───────────────► Nối vào GPIO 35 (ESP32 ADC1)
        │
       [R2: 100kΩ]
        │
       GND
```

### E. Module Relay 5V & Máy bơm nước
| Chân Module Relay | Nối đến | Chú thích |
| :--- | :--- | :--- |
| **VCC** | 5V | Nguồn 5V từ mạch Boost |
| **GND** | GND | Nối mass chung |
| **IN** | **GPIO 25** | Tín hiệu kích từ ESP32 (Mức LOW để đóng tiếp điểm) |
| **Tiếp điểm COM** | Cực (+) Nguồn 5V | Tiếp điểm chung |
| **Tiếp điểm NO** | Cực (+) Máy bơm mini | Tiếp điểm thường mở (Normally Open) |
| **Cực (-) Máy bơm**| GND | Nối trực tiếp mass |

### F. Hệ thống Đèn LED chỉ thị
| Đèn LED | Cực Anode (+) | Cực Cathode (-) | Ý nghĩa hiển thị |
| :--- | :--- | :--- | :--- |
| **LED Chế độ** (Xanh dương) | **GPIO 18** (qua trở 220Ω) | GND | **Sáng**: Chế độ Auto / **Tắt**: Chế độ Manual |
| **LED Báo bơm** (Xanh lá) | **GPIO 19** (qua trở 220Ω) | GND | **Sáng**: Máy bơm đang hoạt động |
| **LED Báo pin** (Đỏ) | **GPIO 23** (qua trở 220Ω) | GND | **Sáng**: Mức pin yếu (< 20%) |

---

## 3. Lưu ý sống còn khi làm phần cứng với ESP32

1. **Vấn đề chân ADC và WiFi:**
   - ESP32 có 2 bộ ADC: ADC1 (GPIO 32 - 39) và ADC2 (GPIO 0, 2, 4, 12, 13, 14, 15, 25, 26, 27).
   - Khi thư viện `WiFi.h` hoặc `ERa.hpp` khởi động, **bộ ADC2 bị bộ thu phát WiFi chiếm quyền hoàn toàn**, dẫn đến mọi hàm `analogRead()` trên chân ADC2 sẽ trả về kết quả sai hoặc bằng 0.
   - Do đó, bắt buộc phải nối Cảm biến đất vào **GPIO 34** và Cầu phân áp pin vào **GPIO 35** (đều thuộc ADC1).

2. **Cách ly nguồn máy bơm:**
   - Động cơ máy bơm mini khi khởi động tạo ra xung nhiễu điện từ (Back-EMF).
   - Nên mắc song song 1 diode 1N4007 ngược chiều với 2 cực của máy bơm (Flyback Diode) hoặc dùng module relay có sẵn opto quang cách ly để tránh ESP32 bị reset đột ngột khi bơm chạy.

3. **Nối chung GND (Common Ground):**
   - Toàn bộ mạch: Pin 18650, Mạch TP4056, Mạch MT3608, ESP32, Relay và Cảm biến **bắt buộc phải nối chung đường GND** với nhau.
