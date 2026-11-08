# ios-daisy

Experimental port of Apple's open-source XNU kernel (iOS 16 generation)
to the **Xiaomi Mi A2 Lite** (codename `daisy`, Snapdragon 625 / MSM8953).

> **Status: early bring-up.** The stock bootloader accepts RAM-booted images,
> but no XNU code runs on the phone yet. This is not a usable phone OS and
> may never become one.

## Goal

Boot XNU on the phone via `fastboot boot` (RAM only, nothing written to the
boot partitions), then run the unmodified iPhone X iOS 16.7.15 (build 20H380)
userland on top of it.

Note: the ios-topaz port names iOS 16.7.16. This project uses 16.7.15, so
differences between the two builds may matter later.

## Approach

Follows the method of existing ports in this family:

- XNU 8796 (macOS 13.5, same generation as iOS 16) is built on GitHub's macOS
  runners with [blacktop/darwin-xnu-build](https://github.com/blacktop/darwin-xnu-build).
- Device support lives in a stack of patch files (`xnu/patches/`) applied on
  top of Apple's source. No Apple code is stored in this repo.
- A small shim hands control from the phone's bootloader to the kernel.

## What works so far

- [x] Partitions backed up and verified; recovery notes in [RECOVERY.md](RECOVERY.md)
- [x] `fastboot boot` of the stock boot image works (RAM boot, header v0)
- [x] A repacked boot image with an edited command line boots, so the
      bootloader accepts edited (unsigned) images
- [x] Hardware map collected from the running phone (RAM ranges, interrupt
      controller, eMMC controller, display panel, ramoops region)

No XNU or custom arm64 code has run on the device yet.

## Why daisy is harder than existing ports

| Area | Existing ports | Mi A2 Lite |
|------|----------------|------------|
| CPU | Cortex-A73 cores can run the 16K-page kernel | 8x Cortex-A53, no 16K translation granule, so a 4K-page XNU is needed |
| Interrupts | GICv3 | GICv2 (confirmed) |
| Storage | UFS | eMMC (confirmed) |
| Bootloader | Newer ABL, boot image v4 | Older LK, boot image v0, page size 2048 |
| Debugging | | No serial console; pstore/ramoops (4 MiB at `0x91400000`) as a log channel |
| RAM | | 3 GB model, three separate ranges |

Whether iOS 16 userland tolerates 4K pages is an open question.

## Roadmap

- [x] Dump partitions and write a recovery guide
- [x] Collect boot details: header addresses, memory map, interrupt controller
- [x] Get the bootloader to accept a RAM-booted image
- [x] Repack an edited boot image and boot it
- [ ] Run a custom arm64 stub and read its message back through ramoops
- [ ] Get the XNU build running on GitHub Actions
- [ ] Print text on the display (framebuffer address needed)
- [ ] XNU starts on one core
- [ ] 4K-page XNU experiment
- [ ] GICv2 interrupt controller support
- [ ] eMMC storage driver
- [ ] iOS userland boots

Findings are logged in [NOTES.md](NOTES.md).

## Reproducing the safe parts

These steps only read from the phone or boot from RAM. You need an unlocked
bootloader, `adb`/`fastboot`, and root over `adb`.

1. Back up the partitions first (see RECOVERY.md), and keep the stock fastboot
   ROM on your PC.
2. Dump `boot_b` and boot it from RAM: `fastboot boot boot_b.img`.
3. Read the header with `tools/readboot.py`.

Keep your partition dumps private: `persist`, `modemst1`, `modemst2` and `fsg`
hold calibration and radio data tied to your device.

## Warning

Can brick your phone and erase all data. Tested on one device only.

## Legal

No Apple files are included. Any iOS files must be downloaded from Apple's
servers on your own machine. Not affiliated with or endorsed by Apple.

## Credits

- [dipa-studio/ios-sweet](https://github.com/dipa-studio/ios-sweet): the original port
- [turgayasuk-sketch/ios-topaz](https://github.com/turgayasuk-sketch/ios-topaz): Redmi Note 12 4G port
- [oosexgang/hackios-xnu-kernel](https://github.com/oosexgang/hackios-xnu-kernel): porting docs and QEMU option
- [blacktop/darwin-xnu-build](https://github.com/blacktop/darwin-xnu-build): XNU build scripts (MIT)
- postmarketOS and msm8953-mainline: hardware reference for daisy
- Apple's open-source XNU

## License

See [LICENSE](LICENSE).
