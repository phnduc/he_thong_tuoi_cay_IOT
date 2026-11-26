#ifndef CONFIG_H
#define CONFIG_H

#define ERA_LOCATION_VN

// --- 1. CẤU HÌNH KẾT NỐI WI-FI & ERA ---
#define ERA_AUTH_TOKEN "NHAP_TOKEN_ERA_CUA_BAN"
#define WIFI_SSID      "Wokwi-GUEST"
#define WIFI_PASS      ""

// --- 2. CẤU HÌNH CHÂN GPIO (ESP32) ---
#define DHT_PIN        15
#define DHT_TYPE       DHT22       // Đổi thành DHT11 nếu mạch dùng cảm biến DHT11

#define I2C_SDA_PIN    21
#define I2C_SCL_PIN    22

#define SOIL_PIN       34          // Chân Analog đọc cảm biến độ ẩm đất
#define BATTERY_PIN    35          // Chân Analog đọc điện áp pin

#define RELAY_PIN      26          // Chân điều khiển Relay máy bơm
#define RELAY_ACTIVE_LEVEL HIGH    // Mức kích hoạt Relay (HIGH hoặc LOW)

// 3 đèn LED báo trạng thái (Chỉnh lại đúng số chân GPIO bạn đang cắm trên mạch)
#define LED_MODE_PIN   2
#define LED_PUMP_PIN   4
#define LED_BAT_PIN    5

// --- 3. THÔNG SỐ MÀN HÌNH LCD I2C ---
#define LCD_I2C_ADDR   0x27
#define LCD_COLS       16
#define LCD_ROWS       2

// --- 4. THÔNG SỐ HIỆU CHUẨN CẢM BIẾN & TƯỚI TỰ ĐỘNG ---
#define SOIL_DRY_RAW           4095    // Giá trị ADC khi đất khô hoàn toàn
#define SOIL_WET_RAW           1500    // Giá trị ADC khi đất ướt hoàn toàn
#define DEFAULT_SOIL_THRESHOLD 40      // Ngưỡng độ ẩm mặc định kích hoạt bơm (%)
#define SOIL_HYSTERESIS        5       // Độ trễ chống bật/tắt bơm liên tục (%)

#define PUMP_COOLDOWN_SEC      10      // Thời gian nghỉ tối thiểu giữa 2 lần bơm (giây)
#define MAX_PUMP_DURATION_SEC  30      // Thời gian bơm tối đa mỗi lần chạy (giây)

// --- 5. THÔNG SỐ PIN & CHU KỲ CẬP NHẬT ---
#define BAT_VOLT_MIN           3.2f    // Điện áp pin cạn (V)
#define BAT_VOLT_MAX           4.2f    // Điện áp pin đầy (V)
#define BAT_LOW_PERCENT        20      // Ngưỡng báo đèn pin yếu (%)

#define SENSOR_INTERVAL_MS     2000UL  // Chu kỳ đọc cảm biến (2 giây)
#define CLOUD_INTERVAL_MS      5000UL  // Chu kỳ gửi dữ liệu lên ERa (5 giây)

#endif
