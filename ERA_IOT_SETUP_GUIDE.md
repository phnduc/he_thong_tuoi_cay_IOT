# HƯỚNG DẪN CẤU HÌNH NỀN TẢNG E-RA IOT

E-Ra IoT (`https://iot.e-ra.io`) là nền tảng IoT Make in Vietnam, hỗ trợ rất tốt cho sinh viên và kỹ sư phát triển sản phẩm với giao thức MQTT/WebSocket tốc độ cao, giao diện kéo thả trực quan và ứng dụng di động hoàn chỉnh.

---

## 1. Đăng ký tài khoản và tạo thiết bị (Device)

1. Truy cập vào trang web: [https://iot.e-ra.io](https://iot.e-ra.io) hoặc tải ứng dụng **E-Ra** trên App Store / Google Play.
2. Đăng ký tài khoản mới và đăng nhập.
3. Vào mục **Device Management (Quản lý thiết bị)** -> Nhấn **Thêm thiết bị mới (Add Device)**.
4. Chọn loại thiết bị: **ESP32** (kết nối qua WiFi).
5. Đặt tên thiết bị: `Mô hình STEM Tưới cây`.
6. Sau khi tạo xong, hệ thống sẽ cấp cho bạn một chuỗi **Auth Token** (Ví dụ: `1a2b3c4d-5e6f-7a8b-9c0d-1e2f3a4b5c6d`).
7. Mở file `config.h` trong mã nguồn và dán mã này vào dòng:
   ```cpp
   #define ERA_AUTH_TOKEN "DIEN_AUTH_TOKEN_CUA_BAN_VAO_DAY"
   ```

---

## 2. Bảng cấu hình Datastream / Virtual Pin

Trên trang quản lý thiết bị của E-Ra, vào phần **Datastreams (Luồng dữ liệu)** và tạo các Virtual Pin theo bảng sau:

| Virtual Pin | Tên hiển thị | Kiểu dữ liệu (Data Type) | Đơn vị | Giá trị Min - Max | Chức năng |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **V0** | Nhiệt độ không khí | `Double` / `Float` | °C | 0 - 60 | Hiển thị nhiệt độ từ DHT |
| **V1** | Độ ẩm không khí | `Double` / `Float` | % | 0 - 100 | Hiển thị độ ẩm từ DHT |
| **V2** | Độ ẩm đất | `Integer` | % | 0 - 100 | Hiển thị độ ẩm đất |
| **V3** | Mức Pin 18650 | `Double` / `Float` | % | 0 - 100 | Hiển thị phần trăm pin |
| **V4** | Chế độ Hoạt động | `Boolean` / `Integer` | - | 0 - 1 | Switch: 0 = Manual, 1 = Auto |
| **V5** | Bơm Thủ công | `Boolean` / `Integer` | - | 0 - 1 | Switch: 0 = OFF, 1 = ON |
| **V6** | Ngưỡng Độ ẩm đất | `Integer` | % | 10 - 90 | Slider: Điều chỉnh ngưỡng tưới tự động |
| **V7** | Trạng thái Bơm | `Boolean` / `Integer` | - | 0 - 1 | LED / Trạng thái phản hồi từ ESP32 |

---

## 3. Thiết kế giao diện Dashboard (Web & Mobile App)

Vào mục **Dashboard (Bảng điều khiển)** trên E-Ra, kéo thả các Widget tương ứng:

1. **Khối Giám sát Môi trường & Nguồn:**
   - **Widget Gauge (Đồng hồ đo) hoặc Card** gán vào `V0`: Hiển thị Nhiệt độ (°C).
   - **Widget Gauge hoặc Card** gán vào `V1`: Hiển thị Độ ẩm không khí (%).
   - **Widget Radial Gauge hoặc Progress Bar** gán vào `V2`: Hiển thị Độ ẩm đất (%).
   - **Widget Battery / Progress Bar** gán vào `V3`: Hiển thị Mức Pin Solar/18650 (%).

2. **Khối Điều khiển:**
   - **Widget Switch (Công tắc)** gán vào `V4`:
     - Nhãn: `Chế độ Tưới (Auto/Manual)`.
     - 0: Thủ công, 1: Tự động.
   - **Widget Switch hoặc Push Button** gán vào `V5`:
     - Nhãn: `Bật/Tắt Bơm (Thủ công)`.
     - 0: Tắt bơm, 1: Bật bơm.
   - **Widget Slider (Thanh trượt)** gán vào `V6`:
     - Nhãn: `Ngưỡng tưới tự động (%)`.
     - Min: 10, Max: 90, Bước nhảy: 5.

3. **Khối Trạng thái & Lịch sử:**
   - **Widget LED hoặc Value Display** gán vào `V7`: Báo xanh khi bơm đang chạy.
   - **Widget Line Chart (Biểu đồ đường):** Thêm các dòng dữ liệu `V0, V1, V2` để quan sát biến thiên nhiệt độ, độ ẩm đất theo thời gian thực.
