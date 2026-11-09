# Notes

Running log for ios-daisy (iOS 16 on Xiaomi Mi A2 Lite, daisy, MSM8953).

## Device facts (Mi A2 Lite, daisy)
- SoC: Snapdragon 625 (MSM8953), 8x Cortex-A53, no 16K granule
- RAM: 3 GB model (MemTotal ~2.75 GiB seen by Linux)
  - 0x40000000-0x849fffff, 0x86800000-0x86bfffff, 0x8ef00000-0xffffffff
- Storage: eMMC (variant QRD eMMC), controller 7824900.sdhci, system on /dev/mmcblk0p57
- Interrupt controller: GICv2 (GIC-0 in /proc/interrupts), node interrupt-controller@b000000
- Bootloader: LK, MSM8953_DAISY2.0_20210630163634, unlocked
- Slots: A/B (current b); boot_a/boot_b 64 MiB each
- fastboot max-download-size: 0x1fe00000 (~510 MiB)
- Display: Himax HX8399C FHD+ video-mode DSI panel, single DSI, MDP (mdss_mdp),
  framebuffer 1080x2280, continuous splash; physical address TBD
- Debug UART: msm_hsl_uart at 0x78af000 (earlycon)
- Currently runs: LineageOS 22.2

## Stock boot image (boot_b)
- Header v0, page size 2048, appended DTB
- Header addresses: kernel 0x80008000, ramdisk 0x81000000, tags 0x80000100
- Linux actually runs at 0x40080000 (= RAM base + 0x80000);
  guess: this LK ignores the header's kernel address for arm64 (unverified)

## ramoops (survives warm reboot)
- 4 MiB at 0x91400000: record_size 512 KiB, console 1 MiB, pmsg 2 MiB, ecc off
- Predicted console zone: 0x91500000 (from memory of the kernel code, unverified)
- PSHOLD reset register: 0x004ab000

## iOS firmware (16.7.15, 20H380)
- IPSW: iPhone10,3 / iPhone10,6 (A11, ChipID 0x8015); topaz README names 16.7.16
- Root filesystem: 087-86698-041.dmg; cryptexes 087-86593-044.dmg, 087-86715-044.dmg
- SHA1 check of the .ipsw: TODO

## Milestones
1. `fastboot boot boot_b.img` (stock boot image) boots from RAM on this LK.
2. Repacked boot_b.img with one extra cmdline word; LK accepted the edited,
   unsigned image (verifiedbootstate=orange) and the word appeared in /proc/cmdline.

## Hardware references
- msm8953-mainline Linux sources; postmarketOS xiaomi-daisy and lk2nd

## Open questions
- Can a 4K-page XNU run on Cortex-A53, and does iOS 16 userland tolerate it?
- Does the stock LK accept a custom (non-Linux) arm64 payload? Which header does it need?
- Is lk2nd usable as a way to start our own code?
- Framebuffer physical address; display driver (MDP + DSI panel)

## Answered
- Interrupt controller: GICv2
- Storage: eMMC
- Stock LK accepts `fastboot boot` of a header-v0 image, including edited ones

## Log
- 2026-11-09: extracted boot_b.img now i have kernel and ramdisk.img i made them kernel_raw and ramdisk_raw there is no DTB Magic
- 2026-11-08: backed up boot_a/b, aboot_a/b, persist, modemst1/2, fsg (sizes verified);
  both milestones above reached.
- 2026-11-08: extracted the IPSW; BuildManifest lists two device classes (d22ap, d221ap).
- 2026-11-07: downloaded the iPhone X IPSW from ipsw.me.

