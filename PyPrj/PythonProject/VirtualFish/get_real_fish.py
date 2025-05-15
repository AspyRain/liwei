import cv2
import os
import numpy as np
from PIL import Image, ImageEnhance, ImageFilter
import random

# 创建保存目录
os.makedirs("video_frame", exist_ok=True)
num = 3
# 打开视频文件
video_path = "video/"+str(num)+".mp4"  # 替换成你的视频文件路径
cap = cv2.VideoCapture(video_path)

if not cap.isOpened():
    print("无法打开视频文件")
    exit()

# 获取视频帧率（fps）和总帧数
fps = cap.get(cv2.CAP_PROP_FPS)
total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

# 每3秒取一帧
interval = int(fps * 1)
frame_index = 0
saved_count = 0


def random_image_processing(image: Image.Image) -> Image.Image:
    option = random.choice(['rotate', 'color', 'blur', 'noise'])

    if option == 'rotate':
        angle = random.randint(0, 360)
        image = image.rotate(angle)
    elif option == 'color':
        enhancer = ImageEnhance.Color(image)
        factor = random.uniform(0.5, 1.5)
        image = enhancer.enhance(factor)
    elif option == 'blur':
        image = image.filter(ImageFilter.GaussianBlur(radius=random.uniform(1, 3)))
    elif option == 'noise':
        np_img = np.array(image)
        noise = np.random.normal(0, 25, np_img.shape)
        np_img = np.clip(np_img + noise, 0, 255)
        image = Image.fromarray(np_img.astype(np.uint8))

    return image


while frame_index < total_frames:
    cap.set(cv2.CAP_PROP_POS_FRAMES, frame_index)
    ret, frame = cap.read()
    if not ret:
        break

    # 保存原始帧（转换为 PIL 并缩放至 640x640）
    frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    pil_image = Image.fromarray(frame_rgb).resize((640, 640), Image.LANCZOS)
    original_path = f"video_frame/{num}-frame_{saved_count:03d}.png"
    pil_image.save(original_path)

    # 随机处理并保存
    processed_image = random_image_processing(pil_image)
    processed_path = f"video_frame/{num}-processed_frame_{saved_count:03d}.png"
    processed_image.save(processed_path)

    print(f"Saved {original_path} and {processed_path}")

    saved_count += 1
    frame_index += interval

cap.release()
print("处理完成")
