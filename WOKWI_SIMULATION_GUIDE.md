# HƯỚNG DẪN CHẠY MÔ PHỎNG TRÊN WOKWI (CHỈ 3 BƯỚC)

Dự án đã được cấu hình sẵn các file để bạn có thể chạy mô phỏng trực tiếp trên trình duyệt Web **Wokwi** mà không cần cài đặt phần mềm hay nối dây thủ công:

---

### Bước 1: Mở dự án ESP32 trên Wokwi
1. Mở trình duyệt và truy cập: **[https://wokwi.com/projects/new/esp32](https://wokwi.com/projects/new/esp32)**

---

### Bước 2: Dán mã nguồn và sơ đồ linh kiện
Dự án đã chuẩn bị sẵn 3 file cho bạn:

1. **Tab `sketch.ino` (hoặc tạo file mới `SmartWatering_ESP32.ino` & `config.h`):**
   - Copy toàn bộ nội dung file [SmartWatering_ESP32.ino](file:///Users/phanduc/Documents/IOT/SmartWatering_ESP32.ino) dán vào `sketch.ino`.
   - Tạo tab mới tên `config.h` (nhấn vào mũi tên nhỏ cạnh tên file -> New File) và dán nội dung file [config.h](file:///Users/phanduc/Documents/IOT/config.h) vào.
   *(Hoặc gộp file `config.h` lên đầu file `sketch.ino`).*

2. **Tab `diagram.json` (Sơ đồ linh kiện nối sẵn):**
   - Chuyển sang tab `diagram.json` trên Wokwi.
   - Xóa hết nội dung cũ và copy toàn bộ nội dung trong file [diagram.json](file:///Users/phanduc/Documents/IOT/diagram.json) dán vào.
   - Bạn sẽ thấy **ESP32, Màn hình LCD 1602 I2C, DHT22, 2 Chiết áp (độ ẩm đất & mức pin), Relay và 3 Đèn LED** tự động xuất hiện và được nối dây hoàn chỉnh!

3. **Tab `libraries.txt` (Khai báo thư viện):**
   - Chuyển sang tab `libraries.txt`.
   - Dán 4 dòng sau vào:
     ```text
     ERa
     DHT sensor library
     LiquidCrystal I2C
     Adafruit Unified Sensor
     ```

---

### Bước 3: Nhấn nút Play (Màu xanh) để chạy Demo!

#### Cách tương tác và kiểm tra trong mô phỏng:
1. **Màn hình LCD 1602:**
   - Dòng 1: Hiển thị Nhiệt độ (°C), Độ ẩm không khí (%), Mức Pin (%).
   - Dòng 2: Hiển thị Độ ẩm đất (Dat: ...%), Chế độ `[A]` (Auto) và Trạng thái `PUMP` hoặc `STOP`.

2. **Chiết áp "Do am dat (Soil)" (nối GPIO 34):**
   - Xoay núm sang trái/phải để thay đổi độ ẩm đất:
     - Khi đất khô (< 40%): **Relay nhảy đóng tiếp điểm**, **LED Bơm (Xanh lá - GPIO 4) bật sáng**.
     - Khi đất đủ ẩm (> 45%): Relay ngắt, LED Bơm tắt.

3. **Chiết áp "Muc Pin (Bat)" (nối GPIO 35):**
   - Xoay xuống mức thấp (< 20%): **LED Pin yếu (Đỏ - GPIO 5) bật sáng cảnh báo**.

4. **Cảm biến DHT22:**
   - Click vào cảm biến DHT22 trên màn hình để kéo thanh trượt thay đổi Nhiệt độ và Độ ẩm không khí $\rightarrow$ LCD cập nhật tức thì.

5. **Kết nối Cloud E-Ra từ Wokwi:**
   - Mạng WiFi `Wokwi-GUEST` trên Wokwi có kết nối Internet thật!
   - Bạn chỉ cần thay `ERA_AUTH_TOKEN` bằng Auth Token thật lấy từ [https://iot.e-ra.io](https://iot.e-ra.io) vào `config.h`, hệ thống trên Wokwi sẽ kết nối trực tiếp đến App E-Ra trên điện thoại của bạn!
