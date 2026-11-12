# Blink LED – LAUNCHXL-F28P55X

Project blink LED cho LaunchPad **LAUNCHXL-F28P55X** (MCU **TMS320F28P550SJ9**, C2000).

LED4 (GPIO20) và LED5 (GPIO21) nhấp nháy luân phiên, chu kỳ 500 ms.

| LED trên board | GPIO   | Macro trong `device.h`  | Mức sáng   |
|----------------|--------|-------------------------|------------|
| LED4           | GPIO20 | `DEVICE_GPIO_PIN_LED1`  | active-low |
| LED5           | GPIO21 | `DEVICE_GPIO_PIN_LED2`  | active-low |

## Cấu trúc thư mục

```
.
├── main.c                     # Chương trình chính (blink LED)
├── device/                    # Device support từ C2000Ware (device.c/h, code_start)
├── driverlib/                 # Mã nguồn driverlib F28P55x từ C2000Ware
├── cmd/                       # Linker command file (FLASH / RAM)
├── CCS/blinkled.projectspec   # File project để import vào Code Composer Studio
└── Makefile                   # Build bằng dòng lệnh với cl2000
```

Project **tự chứa** (self-contained): toàn bộ device support và driverlib đã có sẵn trong repo,
nên không cần cài C2000Ware hay cấu hình đường dẫn `C2000WARE_ROOT`.
Các file trong `device/`, `driverlib/`, `cmd/` lấy từ C2000Ware v26.02.00.00 của Texas Instruments
(giấy phép BSD-3-Clause, xem header từng file).

## Yêu cầu

- Code Composer Studio (CCS) v12 trở lên, có cài hỗ trợ C2000.
- TI C2000 Code Generation Tools (cl2000) **22.6.1.LTS** trở lên.
- Board LAUNCHXL-F28P55X nối với máy tính qua cổng USB (debugger XDS110 onboard).

## Build & nạp bằng CCS

1. `File > Import Project(s)...` → chọn thư mục repo này → tick project **f28p55x_blinkled** → *Finish*.
2. Chọn build configuration (chuột phải project → *Build Configurations > Set Active*):
   - `CPU1_LAUNCHXL_FLASH` – nạp vào Flash, chạy lại được sau khi rút nguồn (mặc định).
   - `CPU1_LAUNCHXL_RAM` – chạy từ RAM, chỉ dùng để debug.
3. *Project > Build Project*.
4. Cắm board, bấm **Debug** (F11) → **Resume** (F8). LED4 và LED5 sẽ nhấp nháy luân phiên.

> Với cấu hình FLASH: sau khi nạp, để board tự chạy khi cấp nguồn, đảm bảo switch boot mode
> trên LaunchPad đang ở chế độ **Boot from Flash** (xem User's Guide của LAUNCHXL-F28P55X).

## Build bằng dòng lệnh

```bash
make CGT_ROOT=/path/to/ti-cgt-c2000_22.6.1.LTS
```

```bash
make CGT_ROOT=/path/to/ti-cgt-c2000_22.6.1.LTS CONFIG=RAM
```

File output: `build/FLASH/f28p55x_blinkled.out` (hoặc `build/RAM/...`), có thể nạp bằng CCS
hoặc UniFlash.

## Tuỳ chỉnh

- Đổi tốc độ nháy: sửa `BLINK_DELAY_US` trong `main.c`.
- Clock hệ thống: 150 MHz từ thạch anh 20 MHz (cấu hình trong `device/device.h`).
