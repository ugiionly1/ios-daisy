# ios-daisy

Experimental port of Apple's open-source XNU kernel (iOS 16 generation)
to the **Xiaomi Mi A2 Lite** (codename `daisy`, Snapdragon 625 / MSM8953).

> **Status: just started. Nothing boots yet.**
> This is not a usable phone OS and may never become one.

## Goal

Boot XNU on the phone via `fastboot boot` (RAM only, nothing written to
the boot partitions), then run the unmodified iPhone X iOS 16 userland on top.

## Approach

Follows the same method as existing ports of this family:

- XNU 8796 (macOS 13.5, same generation as iOS 16) is built on GitHub's
  macOS runners using [blacktop/darwin-xnu-build](https://github.com/blacktop/darwin-xnu-build).
- Device support lives in a stack of patch files (`xnu/patches/`),
  applied on top of Apple's source. No Apple code is stored in this repo.
- A small shim hands control from the phone's bootloader to the kernel.

## Why daisy is harder than existing ports

| Area | Existing ports | Mi A2 Lite |
|------|----------------|------------|
| CPU | Cortex-A73 cores can run the 16K-page kernel | 8x Cortex-A53, no 16K translation granule, so a 4K-page XNU is needed |
| Interrupts | GICv3 | Likely GICv2 (TODO: confirm) |
| Storage | UFS | eMMC (TODO: confirm) |
| Bootloader | Newer ABL, boot image v4 | Older bootloader (TODO: find accepted boot image format) |
| Debugging | | No serial console |

Whether iOS 16 userland tolerates 4K pages is an open question.

## Roadmap

- [ ] Dump partitions and write a recovery guide ([RECOVERY.md](RECOVERY.md))
- [ ] Collect boot details: load address, memory map, framebuffer
- [ ] Read msm8953-mainline Linux sources as a hardware reference
- [ ] Get the bootloader to accept a RAM-booted image (try stock, then lk2nd)
- [ ] Get the XNU build running on GitHub Actions
- [ ] Print text on the display
- [ ] XNU starts on one core
- [ ] 4K-page XNU experiment
- [ ] Interrupt controller support
- [ ] Storage driver (eMMC)
- [ ] iOS userland boots

Progress and findings are logged in [NOTES.md](NOTES.md).

## Warning

Can brick your phone and erase all data. Tested on one device only.
Back up your partitions first and keep the stock fastboot ROM on your PC.

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

TODO: choose after checking the licenses of the projects above.

ALSO: i used to make my own os it was SarventeOS for g350e and daisy (daisy got stuck on bootloop and softbricked it) under development
