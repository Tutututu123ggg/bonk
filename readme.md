# STM32F429 Bonk! Game (TouchGFX Integration)

Dự án phát triển trò chơi phá gạch (Brick Breaker) hoàn chỉnh trên nền tảng kit STM32F429I-Discovery. Dự án tập trung vào việc tối ưu hóa hiệu năng đồ họa với TouchGFX và xây dựng hệ thống xử lý âm thanh thời gian thực thông qua kỹ thuật trộn sóng số (Digital Signal Mixing) trên một chân GPIO duy nhất.

## 1. Tổng Quan Tính Năng

* **Hệ thống đa màn hình:** Chuyển đổi trạng thái linh hoạt giữa màn hình khởi động (Screen2) và màn hình chơi game (Screen1).
* **Vật lý va chạm:** Xử lý va chạm bóng dựa trên thuật toán AABB (Axis-Aligned Bounding Box) và tính toán góc nảy phản xạ dựa trên vị trí tiếp xúc với thanh chèo (Paddle).
* **Độ khó động (Dynamic Difficulty):** Paddle tự động thu nhỏ kích thước sau mỗi lần người chơi hoàn thành một cấp độ (Level Clear).
* **Âm thanh đa nhiệm:** Hỗ trợ phát nhạc nền và hiệu ứng âm thanh (SFX) cùng lúc bằng kỹ thuật trộn XOR, không gây trễ hệ thống.
* **Lưu trữ dữ liệu:** Hệ thống High Score được lưu trữ tập trung tại Model, đảm bảo dữ liệu tồn tại xuyên suốt phiên làm việc.

## 2. Thành Phần Giao Diện (TouchGFX UI)

### Screen2 (Màn hình Khởi động - Mặc định)
* **Hình nền:** Sử dụng ảnh tĩnh được tối ưu hóa dung lượng.
* **Nút Start:** Sử dụng Interaction gọi hàm ảo startGameClicked để chuyển sang màn hình chơi.
* **Hiển thị Điểm cao:** TextArea sử dụng Wildcard để hiển thị giá trị High Score lấy từ Model thông qua Presenter.

### Screen1 (Màn hình Chơi game)
* **Đối tượng Game:** Bao gồm Paddle, Bóng, và mảng Gạch (Bricks) được quản lý bằng Container.
* **Bảng điểm:** Cập nhật điểm số hiện tại và điểm cao nhất theo thời gian thực mỗi khi bóng phá gạch.
* **Điều khiển:** Sử dụng cảm ứng điện dung để cập nhật tọa độ trục X của Paddle.

## 3. Kỹ Thuật Xử Lý Âm Thanh và Ngắt

### Logic Trộn Âm (Audio Mixing)
Hệ thống sử dụng Timer 7 với tần số ngắt 8000Hz để thực hiện kỹ thuật Bit-banging. Hai nguồn âm thanh được trộn bằng phép toán logic XOR:

* **Nhạc nền:** Tần số thấp (262Hz - 523Hz) - Giai điệu Happy Birthday.
* **Hiệu ứng (SFX):** Tần số cao (4000Hz) - Âm thanh va chạm (Bonk).
* **Công thức:** Output = Tin_hieu_Nhac XOR Tin_hieu_SFX.

### Cơ chế Ngắt (Interrupt)
* **Timer Interrupt:** Đảm bảo âm thanh được xuất ra đều đặn, độc lập với tốc độ khung hình (FPS) của giao diện đồ họa.
* **Hàng đợi thông điệp (Message Queue):** Các sự kiện game (va chạm, thua cuộc) đặt cờ hiệu (Flags) để trình xử lý ngắt thực thi âm thanh tương ứng.

## 4. Logic Vận Hành Game

1. **Khởi tạo (Initialization):** Hàm initLevel sử dụng bộ tạo số ngẫu nhiên phần cứng (RNG) để thiết lập chỉ số HP (1-4) cho từng viên gạch.
2. **Vòng lặp vật lý (Game Loop):**
   * Cập nhật vị trí bóng theo vector vận tốc.
   * Kiểm tra va chạm với tường, gạch và Paddle.
   * Điều chỉnh góc nảy của bóng dựa trên độ lệch tâm (Offset) khi chạm Paddle.
3. **Xử lý Thua cuộc (Game Over):**
   * Khi tọa độ Y của bóng vượt quá giới hạn màn hình (Y > 320).
   * Ngắt nhạc nền, phát giai điệu buồn (Sad melody).
   * Kích hoạt bộ đếm thời gian trễ (2 giây) trước khi lưu điểm và chuyển về Screen2.

## 5. Cấu Hình Phần Cứng (Hardware & Pinout)

### Danh sách thiết bị
* Kit phát triển: STM32F429I-Discovery.
* Thiết bị âm thanh: Loa buzzer (Passive) hoặc loa 8 Ohm.

### Sơ đồ chân (Pinout Table)

| Thành phần | Chân (Pin) | Chế độ (Mode) | Chức năng |
| :--- | :--- | :--- | :--- |
| **Audio Output** | **PC3** | GPIO_Output | Tín hiệu ra loa (dương) |
| **Touch SCL** | **PA8** | I2C3_SCL | Xung nhịp cảm ứng |
| **Touch SDA** | **PC9** | I2C3_SDA | Dữ liệu cảm ứng |
| **Buttons** | **PG2, PG3** | GPIO_Input | Nhận tín hiệu 2 nút bấm |

## 6. Phân Công Nhiệm Vụ (Task Allocation)

| Thành viên | Vai trò | Nhiệm vụ chi tiết | Công cụ |
| :--- | :--- | :--- | :--- |
| **Tống Phú Lâm - 20224870** | **Embedded & Audio** | Cấu hình Clock, Timer, RNG, GPIO. Lập trình Driver âm thanh (XOR Mixing). Soạn nhạc. Tối ưu ngắt. | CubeMX, IDE |
| **Hoàng Quốc Hùng - 20224857** | **UI/UX Designer** | Thiết kế giao diện Screen1, Screen2. Quản lý tài nguyên ảnh, font. Thiết lập Interaction chuyển màn. | TouchGFX Designer |
| **Đỗ Đức Tú - 20225106** | **Game Logic** | Lập trình vật lý bóng, va chạm AABB. Triển khai mô hình MVP. Xử lý logic tăng cấp độ và lưu điểm. | C++, STM32CubeIDE |

## 7. Hướng Dẫn Cài Đặt và Chạy

1. **Chuẩn bị tài nguyên:** Mở tệp .touchgfx bằng TouchGFX Designer, nhấn "Generate Code" để tạo mã nguồn giao diện.
2. **Biên dịch:** Mở dự án bằng STM32CubeIDE. Thực hiện "Clean Project" sau đó "Build Project" để đảm bảo không có lỗi liên kết.
3. **Nạp chương trình:** Kết nối kit STM32F429I-Discovery với máy tính qua cổng USB ST-Link. Nhấn "Run" để nạp code.
4. **Kết nối phần cứng:** Nối chân dương của loa vào PC3 và chân âm vào GND.

---
**Nhóm phát triển: Thú Mỏ Vịt**

**Năm thực hiện: 2026**




