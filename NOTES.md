# Notes

Running log for ios-daisy. Newest entries at the top.

## Device facts (Mi A2 Lite, daisy)
- SoC: Snapdragon 625 (MSM8953), 8x Cortex-A53
- RAM: 3 or 4 GB (TODO: confirm mine)
- Storage: eMMC (TODO: confirm with Linux dump)
- Bootloader: unlocked
- Currently runs: LineageOS 22.2

## Hardware references
- msm8953-mainline Linux sources (device tree for memory map, interrupt
  controller, display, touch, eMMC)
- postmarketOS xiaomi-daisy and lk2nd

## Open questions
- Can a 4K-page XNU run on Cortex-A53, and does iOS 16 userland tolerate it?
- Which interrupt controller driver does XNU need (GICv2)?
- Does the stock bootloader accept `fastboot boot` of a custom image? Which format?
- Is lk2nd usable as a way to start our own code?

## Log
- 06-11-2026: repo created, README written
