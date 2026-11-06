# Recovery

How to get the Mi A2 Lite back to a working phone.

## Before testing anything
1. Dump partitions (boot_a, boot_b, and anything else the device has)
   and copy them off the phone. TODO: add exact commands once tested.
2. Download the stock fastboot ROM for daisy and keep it on the PC.
3. Install Android platform-tools (adb, fastboot) and check that
   `fastboot devices` lists the phone.

## Normal recovery (phone reaches fastboot)
1. Power off, then hold Power + Volume Down to enter fastboot.
2. Connect to the PC and run `fastboot devices` to confirm.
3. Flash the stock fastboot ROM (Mi Flash on Windows, or the flash
   script from the ROM package).
4. If the phone boots, flash LineageOS again if wanted.

## Slots
The device is likely A/B. Check with `fastboot getvar current-slot`.
Switch with `fastboot set_active a` or `b`. TODO: verify on my phone.

## Last resort (phone does not reach fastboot)
EDL (emergency download) mode. TODO: research whether it needs
authorization and what the test-point method is for daisy.

## Rules
- RAM-boot first (`fastboot boot`), flash partitions only when needed.
- Never flash userdata before a backup exists.
