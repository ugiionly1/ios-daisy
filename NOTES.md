# iOS 16 on daisy (Xiaomi Mi A2 Lite, MSM8953)

## 06-11-2026, 20:50 UTC+4, Baku

i havent started but tomorrow it is the weekends plus monday and tuesday are my dayoff cuz its victory day and national flag day ill update yall soon

## 07-11-2026, 21:55 UTC+4, Baku

I downloaded the .ipsw file from ipws.me and the internet locked in it was 6.17gb file and it downloaded under 5 minutes so 0.5 sec 500mb download speed type shii and i extracted to my pc

## 08-11-2026, 14:25 UTC+4, Baku

its time to work on BuildManifest.plist i just extracted the .ipsw file and theres 10 file and 1 folder called Firmwares out of 10 files 5 of em is .dmg file and the others are .plist

## 08-11-2026, 19:14 UTC+4, Baku

1st milestone `fastboot boot boot_b.img` (stock LineageOS boot image: header v0,
  page size 2048, appended DTB) boots from RAM on this LK bootloader. First
  milestone reached. Partitions backed up and verified beforehand.

  2nd milestone: repacked boot_b.img with one extra cmdline word via a Python
  script; `fastboot boot` accepted it and the word appeared in /proc/cmdline.
  LK does not reject an edited (unsigned) image: verifiedbootstate=orange.

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

## Hardware seen in the stock cmdline
- Display: Himax HX8399C FHD+ video-mode DSI panel, single DSI, MDP (mdss_mdp)
- Storage: eMMC, sdhci 7824900, system on /dev/mmcblk0p57
- Debug UART: msm_hsl_uart at 0x78af000

### Hardware confirmed (2026-11-08)

- RAM: ~2.75 GB usable
  - Main: 0x40000000 – 0x849FFFFF
  - Extra: 0x86800000 – 0x86BFFFFF
  - High:  0x8EF00000 – 0xFFFFFFFF
- Interrupt controller: GICv2 (mpm-gic)
- Bootloader: Stock LK accepts `fastboot boot` of page-size 2048 images
- eMMC controller: 7824900.sdhci

## Open questions
- Can a 4K-page XNU run on Cortex-A53, and does iOS 16 userland tolerate it?
- Which interrupt controller driver does XNU need (GICv2)?
- Does the stock bootloader accept `fastboot boot` of a custom image? Which format?
- Is lk2nd usable as a way to start our own code?


