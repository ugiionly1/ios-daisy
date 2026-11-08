# iOS 16 on daisy (Xiaomi Mi A2 Lite, MSM8953)

## 06-11-2026, 20:50 UTC+4, Baku

i havent started but tomorrow it is the weekends plus monday and tuesday are my dayoff cuz its victory day and national flag day ill update yall soon

## 07-11-2026, 21:55 UTC+4, Baku

I downloaded the .ipsw file from ipws.me and the internet locked in it was 6.17gb file and it downloaded under 5 minutes so 0.5 sec 500mb download speed type shii and i extracted to my pc

## 08-11-2026, 14:25 UTC+4, Baku

its time to work on BuildManifest.plist i just extracted the .ipsw file and theres 10 file and 1 folder called Firmwares out of 10 files 5 of em is .dmg file and the others are .plist

## Device facts (Mi A2 Lite, daisy)
- SoC: Snapdragon 625 (MSM8953), 8x Cortex-A53
- Storage: eMMC (fastboot: variant QRD eMMC)
- Bootloader: LK, MSM8953_DAISY2.0_20210630163634, unlocked
- Slots: A/B (current: b); boot_a/boot_b 64 MiB each
- fastboot max-download-size: 0x1fe00000 (~510 MiB)
- Userdata: ~22 GiB
- Currently runs: LineageOS 22.2
- RAM: TODO (adb shell cat /proc/meminfo)

## Hardware references
- msm8953-mainline Linux sources (device tree for memory map, interrupt
  controller, display, touch, eMMC)
- postmarketOS xiaomi-daisy and lk2nd

## Open questions
- Can a 4K-page XNU run on Cortex-A53, and does iOS 16 userland tolerate it?
- Which interrupt controller driver does XNU need (GICv2)?
- Does the stock bootloader accept `fastboot boot` of a custom image? Which format?
- Is lk2nd usable as a way to start our own code?


