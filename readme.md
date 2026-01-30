# STM32F429I_DISCO_REV_D01 TBS

TM32F429 Brick Breaker Game (TouchGFX)
Một dự án game phá gạch (Brick Breaker) hoàn chỉnh được phát triển trên Kit STM32F429I-Discovery. Dự án kết hợp giữa giao diện đồ họa TouchGFX mượt mà và hệ thống xử lý âm thanh đa tầng (Multi-layered Sound) thông qua Bit-banging.

Tính năng nổi bật
Giao diện 2 màn hình:

Screen2 (Start Screen): Màn hình chờ với ảnh nền tĩnh, nút Start và hiển thị Kỷ lục (High Score).

Screen1 (Gameplay): Màn hình chơi game với logic vật lý bóng, thanh chèo (Paddle) và gạch đa cấp (HP).

Âm thanh đa nhiệm (Parallel Audio):

Nhạc nền Happy Birthday phát liên tục.

Hiệu ứng "Bonk" đanh gọn phát đồng thời khi bóng va chạm (không ngắt nhạc nền).

Nhạc "Game Over" buồn bã phát duy nhất 1 lần khi thua, sau đó là khoảng lặng 1 giây trước khi chuyển màn.

Hệ thống kỷ lục (Persistence): High Score được lưu trữ trong Model, tồn tại xuyên suốt khi chuyển đổi giữa các màn hình.

Cấp độ động: Paddle tự động ngắn lại mỗi khi người chơi quét sạch gạch trên màn hình để tăng độ khó.

Kiến trúc hệ thống
1. Đồ họa (TouchGFX MVP Pattern)
Dự án tuân thủ mô hình Model-View-Presenter:

Model: Lưu trữ dữ liệu quan trọng như highScore.

Presenter: Trung gian điều phối dữ liệu giữa Model và các View.

View: Xử lý hiển thị và logic game (vật lý va chạm, bộ đếm timer).

2. Xử lý âm thanh (Timer Interrupt & XOR Mixing)
Sử dụng Timer 7 với tần số ngắt 8000Hz để điều khiển chân GPIO (PC3) nối với loa.

Nhạc nền: Sử dụng mảng tần số (melody[]) và nhịp điệu (durations[]).

Cơ chế Trộn âm (Mixing): Sử dụng phép toán logic XOR (^) giữa sóng nhạc nền và sóng hiệu ứng va chạm tần số cao (4000Hz).

Công thức: Output = toggle_music ^ toggle_bonk;

Ưu tiên nhạc buồn: Khi cờ play_sad_music bật, hệ thống dùng lệnh return để chặn hoàn toàn nhạc nền, ưu tiên âm thanh thua cuộc.

Logic Game chi tiết
Xử lý va chạm (Collision Physics)
Bóng di chuyển dựa trên vận tốc ballVX và ballVY.

Va chạm gạch: Kiểm tra AABB Collision. Mỗi viên gạch có chỉ số HP (1-4) tương ứng với độ bền.

Va chạm Paddle: Tính toán offset từ tâm Paddle để điều hướng góc nảy của bóng (tâm nảy thẳng, rìa nảy chéo).

Cơ chế Chuyển màn (Screen Transition)
Khi Win: Nếu activeBrickCount == 0, hàm nextLevel() được gọi để giảm độ dài Paddle và hồi sinh gạch.

Khi Lose: 1. Kích hoạt play_sad_music. 2. Dừng logic vật lý bóng (isWaitingReset = true). 3. Đợi 120 ticks (~2 giây) bao gồm nhạc buồn và khoảng lặng. 4. Lưu highScore và thực hiện lệnh gotoScreen2ScreenNoTransition().

Cấu trúc thư mục quan trọng
TouchGFX/gui/src/screen1_screen/: Chứa logic chính của game.

TouchGFX/gui/src/model/Model.cpp: Nơi lưu trữ điểm số kỷ lục.

Core/Src/main.c: Chứa Driver âm thanh trong hàm callback của Timer.

TouchGFX/assets/: Chứa hình ảnh (.png) và Font chữ (.ttf).

Hướng dẫn cài đặt
Mở dự án bằng TouchGFX Designer 4.x để generate code cho các assets.

Import dự án vào STM32CubeIDE.

Cấu hình chân PC3 là GPIO_Output (nối với loa/buzzer).

Cấu hình Timer 7 với chu kỳ ngắt 125us (8000Hz).

Build và nạp code xuống Kit STM32F429I-Discovery.

Điều khiển
Sử dụng các nút nhấn hoặc màn hình cảm ứng (tùy biến theo Model) để di chuyển Paddle sang trái/phải.

Dự án game phá gạch (Brick Breaker) hoàn chỉnh trên kit **STM32F429I-Discovery**, kết hợp giữa đồ họa TouchGFX mượt mà và hệ thống âm thanh đa tầng xử lý bằng ngắt Timer.

---

##  Tính năng nổi bật
* **Hệ thống 2 màn hình:**
    * **Start Screen (Screen2):** Giao diện chờ, hiển thị Kỷ lục (High Score) và nút bắt đầu.
    * **Gameplay (Screen1):** Logic vật lý bóng, thanh chèo (Paddle) và gạch đa cấp (HP).
* **Âm thanh đa nhiệm (XOR Mixing):** * Nhạc nền **Happy Birthday** phát liên tục.
    * Hiệu ứng **"Bonk"** đanh gọn khi va chạm mà không làm ngắt nhạc nền.
    * Nhạc **"Game Over"** buồn bã kèm khoảng lặng 1 giây trước khi reset.
* **Hệ thống MVP:** Quản lý dữ liệu High Score xuyên suốt giữa các màn hình thông qua Model.
* **Độ khó tăng tiến:** Paddle tự động ngắn lại mỗi khi qua màn.

---

##  Phân Công Nhiệm Vụ (Project Task Allocation)

| Thành viên | Vai trò | Nhiệm vụ chi tiết | Công cụ |
| :--- | :--- | :--- | :--- |
| **Tống Phú Lâm** | **Embedded & Audio** | • Cấu hình Hardware (Clock, Timer, RNG, GPIO).<br>• Lập trình Driver âm thanh (XOR Mixing logic).<br>• Soạn nhạc (Happy Birthday & Sad Melody).<br>• Tối ưu hóa ngắt (Interrupt). | CubeMX, IDE |
| **Hoàng Quốc Hùng** | **UI/UX Designer** | • Thiết kế giao diện (Screen1, Screen2) trên Designer.<br>• Quản lý Assets (Hình ảnh, Fonts, Buttons).<br>• Thiết lập Interaction/Transition giữa các màn hình.<br>• Xử lý hiển thị Wildcards cho điểm số. | TouchGFX Designer |
| **Đỗ Đức Tú** | **Game Logic** | • Lập trình vật lý bóng (Va chạm AABB, góc nảy).<br>• Triển khai mô hình MVP (Model-View-Presenter).<br>• Xử lý Logic Game (Tăng cấp, giảm độ dài Paddle).<br>• Quản lý trạng thái Game Over & Persistence. | C++ |

---

##  Cấu Hình Chân Cắm (Pinout)

| Thành phần | Chân (Pin) | Chế độ (Mode) | Chức năng |
| :--- | :--- | :--- | :--- |
| **Loa (Buzzer)** | **PC3** | GPIO_Output | Phát nhạc & Hiệu ứng (XOR Mixing) |
| **Màn hình LCD** | **Nhiều chân** | LTDC | Hiển thị giao diện 240x320 |
| **Touchscreen** | **PA8, PC9** | I2C3 | Nhận tín hiệu điều khiển Paddle |

---


