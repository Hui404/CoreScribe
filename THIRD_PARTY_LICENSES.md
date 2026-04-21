# Third-Party Licenses

This repository contains code under multiple open-source licenses.
The root `LICENSE` file applies to original CoreScribe code, while third-party code keeps its own license.

## Component Summary

| Component | Path | License |
|---|---|---|
| CoreScribe original code | `Core/`, `sys/`, project-level files unless noted | MIT |
| Grbl | `grbl/` | GPL-3.0-or-later |
| FreeRTOS | `Middlewares/Third_Party/FreeRTOS/Source/` | MIT |
| LVGL | `LVGL/lvgl/` | MIT (see file headers; some bundled extra libs may use their own compatible terms) |
| STM32 HAL Driver | `Drivers/STM32F4xx_HAL_Driver/` | BSD-3-Clause (per `LICENSE.txt`) |
| CMSIS Core | `Drivers/CMSIS/` | Apache-2.0 (per `LICENSE.txt`) |
| CMSIS Device (STM32F4xx) | `Drivers/CMSIS/Device/ST/STM32F4xx/` | Apache-2.0 (per `LICENSE.txt`) |

## Notes for Redistribution

- Do not remove or overwrite upstream copyright/license headers.
- Distributions that include `grbl/` code should follow GPL-3.0-or-later obligations for that combined distribution.
- Keep this file and all upstream license texts when redistributing source or binaries.

## Included License Texts

- MIT text for CoreScribe: root `LICENSE`
- GPL-3.0 text: root `COPYING.GPL-3.0` and `grbl/COPYING.GPL-3.0`
- Other third-party license texts are in their component directories (for example under `Drivers/` and `Middlewares/`).
