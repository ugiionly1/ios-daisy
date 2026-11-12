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
- DTB platform: Qualcomm Technologies, Inc. MSM8953 + PMI8950 QRD SKU3
- DTB compatible: qcom,msm8953
- CPU nodes: 8x arm,cortex-a53 (cpu@0-3 and cpu@100-103)
- GPU: Qualcomm KGSL node `qcom,kgsl-3d0`; exact GPU model not specified in this DTB
- Display subsystem: Qualcomm MDSS MDP (`qcom,mdss_mdp`) with MIPI-DSI controllers
- Interrupt controller: `qcom,msm-qgic2`, with `qcom,mpm-gic-msm8953` support
- Storage controllers: `sdhci@7824900` and `sdhci@7864900`; boot device is `7824900.sdhci`
- Touchscreen DTB entries:
  - Synaptics DSX I2C at 0x4b
  - Goodix GT9xx at 0x5d
  - FocalTech 5446 at 0x38
  - These are alternative DTB entries; installed controller not confirmed
- Kernel boot arguments from DTB `/chosen`: `core_ctl_disable_cpumask=0-7 kpti=0`

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
### November 12, 2026 — First Daisy-specific XNU syntax-check progress

- Added a Daisy-specific Cortex-A53 cache-line configuration to `osfmk/arm64/proc_reg.h`.
- Updated the standalone Clang syntax-check flags with `-DARM64_BOARD_CONFIG_DAISY`.
- Successfully passed the previous `processor not supported` error, confirming the Daisy configuration is now selected by the compiler.
- Remaining syntax-check blockers:
  - `arm_neon_saved_state_t` is undefined in the selected ARM headers.
  - The compiler cannot locate `string.h`.
- The check still does **not** constitute a successful XNU build. No kernel image has been produced or booted yet.

- **2026-11-10 — Daisy GICv2 implementation started:** Created `pexpert/pexpert/arm64/DAISY_GICv2.h` with initial ARM GICv2 Distributor and CPU-interface register offsets for the MSM8953.
- Added the GICv2 header to `DAISY.h` and listed it in `pexpert/pexpert/arm64/Makefile`.
- Confirmed the Daisy device-tree GIC regions: Distributor at `0x0B000000` and CPU interface at `0x0B002000`, each with a size of `0x1000`.
- Traced `pe_arm_init_interrupts()` to `PE_init_platform()` in `pe_init.c`. The interrupt initialization call occurs in the VM-initialized branch.
- Inspected the existing Apple interrupt-controller mapping code and confirmed it assumes an Apple-style `arm-io` layout, which Daisy does not provide.
- **Current status:** GICv2 register definitions and board/build-file integration are in place. The attempted Daisy-specific mapping patch did not apply because its expected source formatting did not match; the file was confirmed unchanged after the failed attempt.
- **Not yet completed:** GICv2 initialization, IRQ acknowledgement/EOI, interrupt dispatch, Cortex-A53 CPU-family support, and architectural timer integration.
- **Build status:** XNU has not yet been compiled. No XNU code has been booted or executed on the device.
- 2026-11-10 — Android Ramdisk and Init Analysis Confirmed `ramdisk.img` is gzip-compressed. Decompressed ramdisk size: 35,113,472 bytes. Identified the archive format as CPIO newc (`070701`). Listed the first 40 archive entries and identified important startup components, including `init`, `first_stage_ramdisk/`, `fstab.qcom`, `default.prop`, `bin/`, and `odm/`. Confirmed the top-level `init` entry is a symbolic link to `/system/bin/init`. Located `system/bin/init` inside the CPIO archive. Verified its file size is 2,693,872 bytes and permissions are `100755`. Confirmed the executable has ELF magic `7F 45 4C 46`. Confirmed Android's init executable targets the same AArch64 architecture used by the Mi A2 Lite's Snapdragon 625. Parsed its ELF header:
  - ELF class: 64-bit
  - Endianness: little-endian
  - Machine ID: 183 (`EM_AARCH64`)
  - Entry point: `0xBD000`
- 2026-11-10: mapped all 13 live device-tree memory-region
  references to their corresponding reserved-memory nodes
  using big-endian phandle values.
  -2026-11-10: identified seven reusable device-linked
  shared-DMA pools with a combined requested size of
  228 MiB.
- 2026-11-10: added the 20 MiB default linux,cma pool
  to the inventory, giving a combined declared pool size
  of 248 MiB.
- 2026-11-10: observed that the combined 248 MiB
  declaration matches the runtime CmaTotal reported
  by /proc/meminfo.
- 2026-11-10: noted that CONFIG_CMA_AREAS is set to 7;
  runtime initialization of the individual pools remains
  to be independently verified.
- 2026-11-10: identified `linux,cma` as the default
  `shared-dma-pool` in the live device tree.
- 2026-11-10: decoded its requested size as 20 MiB
  with 4 MiB alignment. The `reusable` property is present.
- 2026-11-10: inspected the live device tree's default
  Linux CMA pool. `linux,cma` declares a 20 MiB allocation.
- 2026-11-10: confirmed that the Linux `ramoops` platform
  driver is bound to the device `91400000.ramoops`.
- 2026-11-10: completed the ramoops investigation,
  verifying its device-tree reservation, platform-driver
  binding, mounted pstore filesystem, and exposed records.
- 2026-11-10: inspected `/sys/fs/pstore` and found
  `console-ramoops-0` (223,613 bytes) and
  `pmsg-ramoops-0` (2,097,140 bytes).
- 2026-11-10: confirmed that persistent ramoops records
  are exposed by the running Linux system.
- 2026-11-10: observed that the pmsg record size is
  approximately 2 MiB, consistent with the device-tree
  `pmsg-size` allocation.
- 2026-11-10: confirmed that `pstore` is mounted at
  `/sys/fs/pstore` on the running Android system.
- 2026-11-10: correlated the mounted pstore filesystem
  with the live device tree's `ramoops` reserved-memory
  node at `0x91400000`.
- 2026-11-10: confirmed that the live device tree identifies
  the `0x91400000` persistent logging region as `ramoops`.
- 2026-11-10: documented its 4 MiB reserved region,
  1 MiB console buffer, 2 MiB persistent message buffer,
  and 512 KiB record size.
- 2026-11-10: inspected the live device tree's ramoops
  configuration at `0x91400000`.
- 2026-11-10: confirmed a 4 MiB reserved region,
  a 1 MiB console buffer, a 2 MiB persistent message
  buffer, and a 512 KiB record size.
- 2026-11-10: inspected the live device tree's splash
  reserved-memory region. It starts at `0x90001000`,
  has a size of `0x013FF000` bytes (approximately 20 MiB),
  and ends at `0x913FFFFF`.
- 2026-11-10: observed that the splash reservation ends
  immediately before the ramoops region at `0x91400000`.
- 2026-11-10: decoded the GPU reserved-memory allocation range.
  `gpu_region@0` requests 8 MiB with 4 MiB alignment,
  within the allowed range `0x80000000-0x8FFFFFFF`
  (256 MiB). Its actual allocated address remains unknown.
- 2026-11-10: identified `gpu_region@0` as a dynamically
  allocated reserved-memory node. Its requested allocation
  size is 8 MiB; no fixed physical address is specified.
- 2026-11-10: inspected the live device tree's WCNSS firmware
  reserved-memory region. `wcnss_fw_region@0` starts at
  `0x8E800000` and reserves `0x00700000` bytes (7 MiB).
- 2026-11-10: confirmed that the live WCNSS reservation matches
  the previously extracted device tree.
- 2026-11-10: inspected the live device tree's ADSP firmware
  reserved-memory region. `adsp_fw_region@0` starts at
  `0x8D600000` and reserves `0x01200000` bytes (18 MiB).
- 2026-11-10: confirmed that the live ADSP firmware reservation
  matches the previously extracted device tree.
- 2026-11-10: inspected the live device tree's modem reserved-memory
  region. `modem_region@0` starts at `0x86C00000` and reserves
  `0x06A00000` bytes (106 MiB).
- 2026-11-10: confirmed that the live modem reservation matches
  the corresponding region in the previously extracted device tree.
- 2026-11-10: inspected the live device tree's
  `qcom,mdss_dsi_hx8399c_fhdplus_video` node.
  Its panel name is `hx8399c fhdplus video mode dsi panel`.
- 2026-11-10: decoded the panel's big-endian device-tree
  dimensions: 1080 pixels wide and 2280 pixels high.
- 2026-11-10: cross-checked the live device-tree definition
  against `/sys/class/graphics/fb0/msm_fb_panel_info`,
  which reports the same HX8399C FHD+ panel configuration.
- 2026-11-10: inspected the running framebuffer's Qualcomm-specific
  `msm_fb_panel_info` attribute. The kernel reports
  `panel_name=hx8399c fhdplus video mode dsi panel` and
  `primary_panel=1`, identifying the active primary display
  configuration as HX8399C FHD+.
- 2026-11-10: confirmed the framebuffer panel type is
  `mipi dsi video panel`. The runtime panel configuration matches
  the device's reported 1080x2280 display resolution.
- 2026-11-10: resolved the discrepancy between the extracted
  device tree's preferred OTM1911 FHD reference and the running
  kernel's panel information. The runtime framebuffer reports
  HX8399C FHD+; the physical panel part number remains unverified.
- 2026-11-10: inspected `/sys/class/tty` and `/proc/tty/drivers`.
  The kernel has registered the `msm_serial_hs` TTY driver, but
  no `ttyHS` devices are exposed.
- 2026-11-10: checked `/dev/ttyHS*` and the
  `/sys/bus/platform/drivers/msm_serial_hs` directory. No
  high-speed UART device nodes or bound platform devices were found.
- 2026-11-10: the UART platform device at `0x078AF000` remains
  registered without a bound normal platform driver. Its actual
  hardware operation and early-console output remain unverified.
- 2026-11-10: verified the running kernel command line contains
  `earlycon=msm_hsl_uart,0x78af000`, matching the enabled UARTDM
  controller at `0x078AF000` in the extracted device tree.
- 2026-11-10: checked `/proc/consoles`; `pstore0` is listed as a
  registered console. No regular serial console is listed at runtime.
  This does not rule out early UART output, which may be disabled after
  normal console initialization.
- 2026-11-10: confirmed the experimental boot argument
  `ios_daisy_test=1` is present in the running kernel command line.
- 2026-11-10: inspected the UART controller nodes in `daisy.dts`.
  The Qualcomm UARTDM v1.4 controller at `0x078AF000` is marked
  `status = "ok"` and has a default pinctrl configuration.
  The UARTDM controller at `0x078B0000` and the listed BLSP high-speed
  UART controllers are marked disabled.
  The enabled controller is a potential debug-console candidate;
  its actual use as the running Linux console has not yet been verified.
- 2026-11-10: analyzed the extracted device tree (`daisy.dts`) and identified the
  Qualcomm MSM8953 platform, including eight Cortex-A53 CPU nodes using PSCI.
- 2026-11-10: identified the Qualcomm QGIC2 interrupt controller at
  `0x0B000000`, with register regions at `0x0B000000` and `0x0B002000`.
- 2026-11-10: inspected `/proc/iomem` on the running device and recorded
  three System RAM ranges:
  `0x40000000-0x849FFFFF`,
  `0x86800000-0x86BFFFFF`, and
  `0x8EF00000-0xFFFFFFFF`.
- 2026-11-10: compared the physical memory map with the DTS reserved-memory
  entries. The reserved-region boundaries match the gaps in the reported
  System RAM ranges.
- 2026-11-10: identified an ARMv8 generic timer in the DTS with a declared
  clock frequency of 19.2 MHz and four timer interrupt entries.
- 2026-11-10: identified a Qualcomm UARTDM v1.4 serial controller at
  `0x078AF000`, with a register window of `0x200` bytes.
- 2026-11-10: confirmed a PSCI 1.0 firmware interface using the `smc` method.
  UART availability and physical debug-console access remain unverified.
- 2026-11-10: successfully decompressed the gzip-compressed kernel extracted from `boot_b.img`. The decompressed ARM64 Linux Image is 33,916,952 bytes, with `ARM\x64` magic at offset `0x38`. SHA256: `11e292ece1a54ee2559dc155771fca70d44b66a685c49fb6cb5e15d3e3255484`.
- 2026-11-10: located and extracted the appended DTB from the kernel payload. DTB offset: `0xBE92A4`. DTB size: 283,536 bytes. Verified FDT magic: `D0 0D FE ED`. Saved as `daisy.dtb`. SHA256: `bb4eb3b871a963fbaf06138a9adb3fba499e42e176ffa205e7337fe84ffa36e4`.
- 2026-11-10: decompressed the gzip-compressed kernel from boot_b.img.
  The decompressed ARM64 Linux Image is 33,916,952 bytes, with ARM\x64
  magic at offset 0x38. SHA256:
  11e292ece1a54ee2559dc155771fca70d44b66a685c49fb6cb5e15d3e3255484.
  Found an appended FDT beginning with D0 0D FE ED. Its header reports
  a total size of 283,536 bytes.
- 2026-11-10: inspected the ARM64 kernel Image header in HxD. Found `ARM\x64` magic at offset `0x38`. Decoded header fields: text offset `0x80000`, image size `0x0236B000` (37,109,760 bytes), and flags `0xA`. Results are consistent with the standard ARM64 Linux Image format.
- 2026-11-10: inspected `kernel_raw` in HxD. Found ARM64 Linux Image magic `41 52 4D 64` (`ARM\x64`) at offset `0x38`, confirming the extracted kernel uses the standard ARM64 Linux Image header format.
- 2026-11-10: calculated SHA-256 hash of `kernel_raw`:
  `d140e95c75c3b30bf76dc57ffa6892d0f7890cd3072c4b504b0e70f8a5d7dfc5`
- 2026-11-10: investigated the device tree and identified additional hardware details, including the MSM8953 + PMI8950 platform, Cortex-A53 CPU configuration, KGSL GPU subsystem, MDSS display controllers, GICv2 interrupt controller, SDHCI storage controllers, and multiple touchscreen driver entries. Also examined the extracted `kernel_raw` and `ramdisk_raw`; no DTB magic (`0xd00dfeed`) was found in the inspected kernel data.
- 2026-11-09: extracted boot_b.img now i have kernel and ramdisk.img i made them kernel_raw and ramdisk_raw there is no DTB Magic
- 2026-11-08: backed up boot_a/b, aboot_a/b, persist, modemst1/2, fsg (sizes verified);
  both milestones above reached.
- 2026-11-08: extracted the IPSW; BuildManifest lists two device classes (d22ap, d221ap).
- 2026-11-07: downloaded the iPhone X IPSW from ipsw.me.
