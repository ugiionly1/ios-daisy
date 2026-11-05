# ios-daisy

Experimental port of Apple's open-source XNU kernel (iOS 16 generation)
to the Xiaomi Mi A2 Lite (daisy, Snapdragon 625).

**Status: just started. Nothing boots yet.**

## Goal
Boot XNU via `fastboot boot` (RAM only), then run the unmodified
iPhone X iOS 16 userland on top of it.

## Known blockers
- All-Cortex-A53 CPU: no 16K translation granule, so a 4K-page XNU is needed
- Interrupt controller differs from the GICv3 used by existing ports
- eMMC storage instead of UFS
- Older bootloader and boot image format
- No serial console

## Roadmap
- [ ] Dump partitions and collect boot details (load address, memory map, framebuffer)
- [ ] Get the bootloader to accept a RAM-booted shim
- [ ] Print text on the display
- [ ] XNU starts on a single core
- [ ] Storage driver
- [ ] iOS userland boots

## Warning
Can brick your phone and erase all data. Tested on one device only.
No Apple files are included. Any iOS files must be downloaded from
Apple's servers on your own machine.

## Credits
Based on dipa-studio/ios-sweet, turgayasuk-sketch/ios-topaz and
oosexgang/hackios-xnu-kernel, built on Apple's open-source XNU.
Not affiliated with or endorsed by Apple.
