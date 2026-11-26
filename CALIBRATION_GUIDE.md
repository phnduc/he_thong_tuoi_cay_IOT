# HƯỚNG DẪN HIỆU CHUẨN (CALIBRATION) CẢM BIẾN

Trong các bài tập lớn IoT, bước **Hiệu chuẩn (Calibration)** là điểm cộng học thuật rất lớn, giúp giảng viên đánh giá cao tính khoa học và thực tế của mô hình.

---

## 1. Hiệu chuẩn Cảm biến Độ ẩm đất (Capacitive Soil Moisture Sensor v1.2)

Cảm biến độ ẩm đất điện dung hoạt động theo nguyên lý đo sự thay đổi hằng số điện môi của môi trường. Giá trị ADC đọc về sẽ **nghịch đảo**:
- **Càng khô**: Điện dung nhỏ $\rightarrow$ Tần số dao động cao $\rightarrow$ Điện áp ra cao $\rightarrow$ Giá trị ADC lớn (khoảng 2800 - 3500).
- **Càng ẩm/ướt**: Điện dung lớn $\rightarrow$ Điện áp ra thấp $\rightarrow$ Giá trị ADC nhỏ (khoảng 1300 - 1800).

### Các bước thực nghiệm:
1. Mở code kiểm tra mẫu hoặc in trực tiếp giá trị `analogRead(SOIL_PIN)` ra Serial Monitor.
2. **Bước 1 (Đo mốc 0% - Không khí khô):**
   - Lau sạch đầu dò cảm biến, để ngoài không khí khô ráo.
   - Quan sát giá trị ADC hiển thị trên Serial Monitor (ví dụ: `3200`).
   - Đây chính là giá trị $ADC_{dry}$.
3. **Bước 2 (Đo mốc 100% - Nước bão hòa):**
   - Chuẩn bị 1 ly nước sạch.
   - Nhúng ngập phần đầu dò cảm biến vào nước (chú ý: **không nhúng ngập quá vạch cảnh báo trắng** để tránh làm chập mạch linh kiện phía trên).
   - Quan sát giá trị ADC trên Serial Monitor (ví dụ: `1500`).
   - Đây chính là giá trị $ADC_{wet}$.
4. **Cập nhật vào code:**
   - Mở file `config.h` và cập nhật lại 2 hằng số:
     ```cpp
     #define SOIL_DRY_RAW   3200   // Giá trị thực tế đo được ở bước 1
     #define SOIL_WET_RAW   1500   // Giá trị thực tế đo được ở bước 2
     ```
   - Công thức quy đổi trong code:
     $$\text{Soil}\% = \text{map}(\text{rawADC}, SOIL\_DRY\_RAW, SOIL\_WET\_RAW, 0, 100)$$

---

## 2. Hiệu chuẩn Cầu phân áp đo Pin 18650

### Nguyên lý:
ESP32 sử dụng bộ chuyển đổi tương tự - số (ADC) 12-bit (giá trị từ 0 đến 4095).
Điện áp tối đa chân ADC nhận được an toàn là $3.3\text{V}$.

Sử dụng 2 điện trở $R_1 = 100\text{k}\Omega$ và $R_2 = 100\text{k}\Omega$:
$$V_{ADC} = V_{bat} \times \frac{R_2}{R_1 + R_2} = \frac{V_{bat}}{2}$$
Do đó:
$$V_{bat} = V_{ADC} \times 2 = \left(\frac{ADC_{raw}}{4095} \times 3.3\right) \times 2$$

### Hiệu chuẩn thực tế (Sai số điện trở và ADC ESP32):
Bộ ADC của ESP32 có đặc tính phi tuyến tính nhẹ ở 2 đầu dải đo. Để kết quả chính xác nhất:
1. Dùng **đồng hồ vạn năng (VOM)** đo trực tiếp điện áp hai đầu cực của pin 18650 (ví dụ đo được $3.95\text{V}$).
2. Đọc giá trị $V_{bat}$ do ESP32 tính toán hiển thị trên Serial Monitor (ví dụ ESP32 tính ra $3.85\text{V}$).
3. Tính hệ số bù (Correction Factor):
   $$K = \frac{V_{thuc\_te}}{V_{esp32}} = \frac{3.95}{3.85} \approx 1.026$$
4. Nhân hệ số $K$ vào công thức tính $V_{bat}$ trong hàm `readBattery()`.
