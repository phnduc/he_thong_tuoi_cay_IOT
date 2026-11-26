/*
 * ĐỀ TÀI: MÔ HÌNH STEM TƯỚI CÂY TỰ ĐỘNG - BÀI TẬP LỚN MÔN IOT
 * VI ĐIỀU KHIỂN: ESP32 DEVKIT V1
 * NGUỒN: NĂNG LƯỢNG MẶT TRỜI + 2 PIN 18650
 * NỀN TẢNG CLOUD: E-RA IOT (HTTPS://IOT.E-RA.IO)
 * 
 * Các Virtual Pin trên E-Ra:
 * - V0: Nhiệt độ không khí (°C) [Double/Float]
 * - V1: Độ ẩm không khí (%) [Double/Float]
 * - V2: Độ ẩm đất (%) [Integer]
 * - V3: Dung lượng pin (%) [Double/Float]
 * - V4: Chế độ Auto / Manual [Switch - 0: Manual, 1: Auto]
 * - V5: Nút bật/tắt bơm thủ công [Switch - 0: OFF, 1: ON]
 * - V6: Cài đặt ngưỡng độ ẩm đất [Slider - 10 to 90%]
 * - V7: Trạng thái máy bơm gửi lên App [Switch/LED - 0: Dừng, 1: Đang bơm]
 */

#include "config.h"
#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <ERa.hpp>

// --- Khởi tạo các ngoại vi ---
DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(LCD_I2C_ADDR, LCD_COLS, LCD_ROWS);

// --- Biến trạng thái toàn cục ---
bool isAutoMode = true;             // true: Tự động, false: Thủ công
bool manualPumpCmd = false;         // Lệnh bơm từ nút bấm App ở chế độ thủ công
int currentSoilThreshold = DEFAULT_SOIL_THRESHOLD;

float currentTemp = 0.0;
float currentHumidity = 0.0;
int currentSoilMoisture = 0;
float currentBatVoltage = 0.0;
float currentBatPercent = 0.0;

bool isPumpActive = false;
unsigned long pumpStartTime = 0;
unsigned long pumpStopTime = 0;

// ==========================================
// CALLBACK TỪ E-RA IOT KHI NGƯỜI DÙNG THAO TÁC TRÊN APP
// ==========================================

// Nhận lệnh đổi chế độ (V4: 0 = Manual, 1 = Auto)
ERA_WRITE(V4) {
    int val = param.getInt();
    isAutoMode = (val == 1);
    digitalWrite(LED_MODE_PIN, isAutoMode ? HIGH : LOW);
    Serial.printf("[E-Ra] Mode changed to: %s\n", isAutoMode ? "AUTO" : "MANUAL");
}

// Nhận lệnh bật/tắt bơm thủ công (V5: 0 = Tắt, 1 = Bật)
ERA_WRITE(V5) {
    int val = param.getInt();
    manualPumpCmd = (val == 1);
    Serial.printf("[E-Ra] Manual pump command: %s\n", manualPumpCmd ? "ON" : "OFF");
}

// Nhận thay đổi ngưỡng độ ẩm đất (V6)
ERA_WRITE(V6) {
    int val = param.getInt();
    if (val >= 10 && val <= 90) {
        currentSoilThreshold = val;
        Serial.printf("[E-Ra] New Soil Threshold: %d%%\n", currentSoilThreshold);
    }
}

// ==========================================
// CÁC HÀM XỬ LÝ PHẦN CỨNG & CẢM BIẾN
// ==========================================

// Điều khiển bật/tắt Relay máy bơm kèm bảo vệ an toàn
void setPump(bool state) {
    // Bảo vệ pin yếu: Nếu pin dưới ngưỡng cạn (3.2V) thì không cho phép bơm
    if (state && currentBatVoltage < BAT_VOLT_MIN && currentBatVoltage > 1.0f) {
        Serial.println("[CẢNH BÁO] Pin quá yếu! Khóa bơm để bảo vệ pin 18650.");
        state = false;
    }

    // Thời gian nghỉ giữa 2 lần bơm (cooldown)
    unsigned long now = millis();
    if (state && !isPumpActive) {
        if ((now - pumpStopTime) < (PUMP_COOLDOWN_SEC * 1000UL)) {
            Serial.println("[INFO] Bơm đang trong thời gian nghỉ (cooldown), chờ thêm...");
            return;
        }
        pumpStartTime = now;
    }

    if (!state && isPumpActive) {
        pumpStopTime = now;
    }

    isPumpActive = state;

    // Xuất tín hiệu ra Relay theo active level
    if (RELAY_ACTIVE_LEVEL == LOW) {
        digitalWrite(RELAY_PIN, state ? LOW : HIGH);
    } else {
        digitalWrite(RELAY_PIN, state ? HIGH : LOW);
    }

    // LED báo bơm
    digitalWrite(LED_PUMP_PIN, state ? HIGH : LOW);
}

// Đọc và tính toán điện áp pin 18650 qua cầu chia áp
void readBattery() {
    // Đọc ADC (ESP32 ADC 12-bit: 0 -> 4095)
    // Lấy trung bình 10 mẫu để giảm nhiễu ADC
    long sum = 0;
    for (int i = 0; i < 10; i++) {
        sum += analogRead(BATTERY_PIN);
        delayMicroseconds(200);
    }
    float rawADC = (float)sum / 10.0;

    // Vout = rawADC / 4095.0 * 3.3V
    // Cầu chia áp: R1 = 100k, R2 = 100k -> Vbat = Vout * 2.0
    float vOut = (rawADC / 4095.0) * 3.3;
    currentBatVoltage = vOut * 2.0;

    // Quy đổi sang phần trăm pin 18650
    if (currentBatVoltage <= BAT_VOLT_MIN) {
        currentBatPercent = 0.0;
    } else if (currentBatVoltage >= BAT_VOLT_MAX) {
        currentBatPercent = 100.0;
    } else {
        currentBatPercent = ((currentBatVoltage - BAT_VOLT_MIN) / (BAT_VOLT_MAX - BAT_VOLT_MIN)) * 100.0;
    }

    // Đèn LED cảnh báo mức pin
    if (currentBatPercent <= BAT_LOW_PERCENT) {
        digitalWrite(LED_BAT_PIN, HIGH); // Pin yếu
    } else {
        digitalWrite(LED_BAT_PIN, LOW);
    }
}

// Đọc các cảm biến môi trường
void readSensors() {
    // 1. Đọc DHT11 / DHT22
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t)) currentTemp = t;
    if (!isnan(h)) currentHumidity = h;

    // 2. Đọc cảm biến độ ẩm đất (Điện dung / Điện trở)
    long soilSum = 0;
    for (int i = 0; i < 10; i++) {
        soilSum += analogRead(SOIL_PIN);
        delayMicroseconds(200);
    }
    int rawSoil = soilSum / 10;

    // Map giá trị ADC sang % (Càng ướt giá trị ADC càng nhỏ đối với cảm biến điện dung)
    int mappedPercent = map(rawSoil, SOIL_DRY_RAW, SOIL_WET_RAW, 0, 100);
    currentSoilMoisture = constrain(mappedPercent, 0, 100);

    // 3. Đọc pin
    readBattery();
}

// Thuật toán ra quyết định tưới cây
void processWateringLogic() {
    unsigned long now = millis();

    // Bảo vệ: Tự động ngắt nếu bơm chạy liên tục vượt thời gian cho phép
    if (isPumpActive && ((now - pumpStartTime) >= (MAX_PUMP_DURATION_SEC * 1000UL))) {
        Serial.println("[BẢO VỆ] Đã đạt thời gian bơm tối đa -> Ngắt bơm chống cháy!");
        setPump(false);
        manualPumpCmd = false; // Reset cờ thủ công nếu có
        return;
    }

    if (isAutoMode) {
        // CHẾ ĐỘ TỰ ĐỘNG
        if (currentSoilMoisture < currentSoilThreshold) {
            // Đất khô dưới ngưỡng -> Bắt đầu bơm
            if (!isPumpActive) {
                setPump(true);
            }
        } else if (currentSoilMoisture >= (currentSoilThreshold + SOIL_HYSTERESIS)) {
            // Đất đã đủ ẩm (có bù hysteresis tránh bật tắt rung) -> Tắt bơm
            if (isPumpActive) {
                setPump(false);
            }
        }
    } else {
        // CHẾ ĐỘ THỦ CÔNG: Bật/tắt theo lệnh nút nhấn trên App E-Ra
        setPump(manualPumpCmd);
    }
}

// Cập nhật thông tin lên màn hình LCD 1602
void updateLCD() {
    // Dòng 1: Nhiệt độ, Độ ẩm không khí, Mức Pin
    // Ví dụ: "T:28C H:75% P:85%"
    lcd.setCursor(0, 0);
    lcd.printf("T:%dC H:%d%% P:%d%% ", (int)currentTemp, (int)currentHumidity, (int)currentBatPercent);

    // Dòng 2: Độ ẩm đất, Chế độ (A/M), Trạng thái bơm
    // Ví dụ: "Dat:45% [A] ON "
    lcd.setCursor(0, 1);
    lcd.printf("Dat:%2d%% [%c] %s ", 
        currentSoilMoisture, 
        isAutoMode ? 'A' : 'M', 
        isPumpActive ? "PUMP" : "STOP");
}

// Đồng bộ trạng thái và gửi dữ liệu lên E-Ra Cloud
void syncToERaCloud() {
    ERa.virtualWrite(V0, currentTemp);
    ERa.virtualWrite(V1, currentHumidity);
    ERa.virtualWrite(V2, currentSoilMoisture);
    ERa.virtualWrite(V3, currentBatPercent);
    ERa.virtualWrite(V7, isPumpActive ? 1 : 0);
}

// ==========================================
// CHƯƠNG TRÌNH CHÍNH (SETUP & LOOP)
// ==========================================
void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("\n=== HỆ THỐNG STEM TƯỚI CÂY THÔNG MINH - ESP32 ===");

    // Cấu hình các chân điều khiển Output
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(LED_MODE_PIN, OUTPUT);
    pinMode(LED_PUMP_PIN, OUTPUT);
    pinMode(LED_BAT_PIN, OUTPUT);

    // Khởi tạo mức ban đầu
    setPump(false);
    digitalWrite(LED_MODE_PIN, isAutoMode ? HIGH : LOW);
    digitalWrite(LED_PUMP_PIN, LOW);
    digitalWrite(LED_BAT_PIN, LOW);

    // Khởi tạo I2C và màn hình LCD
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("STEM SMART WATER");
    lcd.setCursor(0, 1);
    lcd.print("WiFi Connecting.");

    // Khởi tạo cảm biến DHT
    dht.begin();

    // Kết nối WiFi và E-Ra Cloud Platform
    Serial.println("Đang kết nối WiFi & E-Ra Cloud...");
    ERa.begin(WIFI_SSID, WIFI_PASS);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ket noi E-Ra OK!");
    delay(1500);
    lcd.clear();

    // Lập lịch định kỳ đọc cảm biến & điều khiển
    ERa.addInterval(SENSOR_INTERVAL_MS, []() {
        readSensors();
        processWateringLogic();
        updateLCD();
    });

    // Lập lịch định kỳ gửi dữ liệu lên Cloud
    ERa.addInterval(CLOUD_INTERVAL_MS, []() {
        syncToERaCloud();
    });
}

void loop() {
    // E-Ra tự động duy trì kết nối WiFi, MQTT và đồng bộ Virtual Pins
    ERa.run();
}
