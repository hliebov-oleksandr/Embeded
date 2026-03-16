Import("env")
import os

# Добавляем все HAL исходники в сборку
hal_src_dir = os.path.join(env["PROJECT_DIR"], "Drivers", "STM32F4xx_HAL_Driver", "Src")
cmsis_src_dir = os.path.join(env["PROJECT_DIR"], "src")

sources = []
for root, _, files in os.walk(hal_src_dir):
    for f in files:
        if f.endswith(".c"):
            sources.append(os.path.join(root, f))

for root, _, files in os.walk(cmsis_src_dir):
    for f in files:
        if f.endswith(".c"):
            sources.append(os.path.join(root, f))

env.Append(SRC_FILTER=["+" + s for s in sources])