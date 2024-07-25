import subprocess
import os, sys

crop_area = '250'

# python3 tools/mp4_to_gif.py build/linux/x86_64/debug/

def convert_mp4_to_gif(input_file, output_file):
    # 生成调色板的临时文件
    palette_file = 'palette.png'
    
    # 检查输入文件尺寸
    probe_cmd = [
        'ffmpeg', '-i', input_file, '-vframes', '1', '-vf', 'scale=' + crop_area + ':' + crop_area + '',
        '-f', 'null', '-'
    ]
    result = subprocess.run(probe_cmd, stderr=subprocess.PIPE, text=True)
    if 'No such file or directory' in result.stderr:
        print(f"Error: Input file '{input_file}' not found.")
        return
    if 'Invalid argument' in result.stderr:
        print(f"Error: Input video size is too small for cropping to " + crop_area + 'x' + crop_area + ".")
        return
    
    # 生成调色板
    palettegen_cmd = [
        'ffmpeg', '-i', input_file, '-vf',
        'crop=' + crop_area + ':' + crop_area + ':(in_w-' + crop_area + ')/2:(in_h-' + crop_area + ')/2,fps=10,scale=' + crop_area + ':' + crop_area + ':flags=lanczos,palettegen',
        '-y', palette_file
    ]
    subprocess.run(palettegen_cmd, check=True)
    
    # 使用调色板生成高质量的 GIF
    gifgen_cmd = [
        'ffmpeg', '-i', input_file, '-i', palette_file, '-lavfi',
        'crop=' + crop_area + ':' + crop_area + ':(in_w-' + crop_area + ')/2:(in_h-' + crop_area + ')/2,fps=10,scale=' + crop_area + ':' + crop_area + ':flags=lanczos [x]; [x][1:v] paletteuse',
        '-y', output_file
    ]
    subprocess.run(gifgen_cmd, check=True)
    
    # 删除临时调色板文件
    os.remove(palette_file)

    print(f"Successfully converted '{input_file}' to '{output_file}'.")

def process_directory(input_directory, output_directory):
    # 确保输出目录存在
    if not os.path.exists(output_directory):
        os.makedirs(output_directory)
    
    # 遍历输入目录下的所有文件
    for filename in os.listdir(input_directory):
        if filename.lower().endswith('.mp4'):
            input_file = os.path.join(input_directory, filename)
            output_file = os.path.join(output_directory, os.path.splitext(filename)[0] + '.gif')
            convert_mp4_to_gif(input_file, output_file)  

if __name__ == "__main__":
    assert len(sys.argv) >= 2, "need file directory args"
    target_dir = sys.argv[1]
    if len(sys.argv) == 3:
        crop_area = str(sys.argv[2])
    process_directory(target_dir, target_dir)