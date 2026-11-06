# Haven't tested on ios-daisy

- Read `NOTES.md` before starting; it contains the project goal, daisy hardware
  facts, current blocker, local artifact locations and the next concrete action.
- The user switches agents frequently. Update the active handoff in `NOTES.md`
  after each milestone and before stopping. Record commits, CI run IDs, exact
  build/boot results and unfinished work. Clearly distinguish observations from
  guesses.
- Commit completed changes. Use `gh` for GitHub Actions kernel builds and logs.
  Test local single-file cross-compiles before spending a CI run.
- Target device: Xiaomi Mi A2 Lite (`daisy`), Snapdragon 625, Cortex-A53 CPU,
  Adreno 506 and eMMC storage. This is an experimental XNU port; nothing boots
  yet. Account for the device's 4K-page requirements, interrupt-controller
  differences, older bootloader and boot-image format, and lack of a serial
  console.
- Device bring-up must prioritize temporary `fastboot boot` images and RAM-only
  testing. Preserve the working Android/LineageOS installation and all
  partition backups. Do not flash, erase, repartition or overwrite device
  storage without explaining the exact change and getting the user's explicit
  confirmation first. Keep the bootloader, recovery, modem, persist and other
  essential partitions intact.
- Do not assume UFS: daisy uses eMMC. Do not make GPT or partition-layout
  changes as part of the initial bring-up.
- Keep device dumps, serial numbers, private keys and downloaded Apple artifacts
  out of git. Never commit proprietary Apple files or device-specific personal
  data.
