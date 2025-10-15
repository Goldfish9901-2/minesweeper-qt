import shutil
import pathlib
import sys
import subprocess

if __name__ == "__main__":
    # 当前脚本所在目录
    dir_path = pathlib.Path(__file__).resolve().parent

    # 查找 uic 可执行文件
    uic_executable = shutil.which("uic")
    if uic_executable is None:
        print("uic executable not found. Please install it and try again.", file=sys.stderr)
        sys.exit(1)

    # 查找 lupdate 可执行文件
    lupdate_executable = shutil.which("lupdate")
    if lupdate_executable is None:
        print("lupdate executable not found. Please install it and try again.", file=sys.stderr)
        sys.exit(1)

    # 遍历所有 .ui 文件
    for ui_file in dir_path.rglob("*.ui"):
        # 输出文件名，例如 mainwindow.ui -> ui_mainwindow.h
        output_file = ui_file.with_name(f"ui_{ui_file.stem}.h")

        # 调用 uic 生成头文件
        print(f"Generating {output_file} from {ui_file}")
        result = subprocess.run([uic_executable, str(ui_file), "-o", str(output_file)])
        if result.returncode != 0:
            print(f"Failed to generate {output_file}", file=sys.stderr)

    # 调用 lupdate 更新翻译文件
    ts_files = list(dir_path.rglob("*.ts"))
    if ts_files:
        print(f"Updating {len(ts_files)} translation files...")
        lupdate_cmd = [lupdate_executable, str(dir_path), "-ts"] + [str(ts_file) for ts_file in ts_files]
        result = subprocess.run(lupdate_cmd)
        if result.returncode != 0:
            print("Failed to update translation files", file=sys.stderr)
        else:
            print("Translation files updated successfully")
    else:
        print("No .ts files found")