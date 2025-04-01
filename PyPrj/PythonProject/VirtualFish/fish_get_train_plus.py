import pygame
import random
import time
import os
import numpy as np
from PIL import Image, ImageEnhance, ImageFilter

# 初始化pygame
pygame.init()

# 设置窗口尺寸为640x640
WIDTH, HEIGHT = 640, 640
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Random Moving Blocks")

# 加载背景图片（可替换）
background = pygame.image.load("assets/background.jpg")
background = pygame.transform.scale(background, (WIDTH, HEIGHT))

# 可选的方块样式（图片路径）
ALL_BLOCK_STYLES = [
    ["assets/fish1_1.png", "assets/fish1_2.png", "assets/fish1_3.png"],
    ["assets/fish3_1.png", "assets/fish3_2.png", "assets/fish3_3.png"]
]
SIMPLE_BLOCK_STYLES = ["assets/fish2_1.png", "assets/fish2_2.png", "assets/fish2_3.png"]
BLOCK_SIZE = 45  # 初始大小
GROWTH_RATES = [0.0005, 0.0008, 0.001]  # 可选的成长速度
MAX_SIZE = 200  # 最大尺寸

SPEED_VARIATION = 0.01  # 速度变化的平滑度

# 添加固定图形
FIXED_ICON_SIZE = 40  # 固定图形的大小
FIXED_ICON_MARGIN = (650 / 2) - 20  # 离边缘的距离

# 如果有固定的图片，加载它：
fixed_icon = pygame.image.load("assets/锚定矩形.png")  # 你可以替换成自己的图标
fixed_icon = pygame.transform.scale(fixed_icon, (FIXED_ICON_SIZE, FIXED_ICON_SIZE))


# 方块类
class Block:
    def __init__(self, style_paths, growth_rate):
        self.x = random.randint(0, WIDTH - BLOCK_SIZE)
        self.y = random.randint(0, HEIGHT - BLOCK_SIZE)
        self.dx = random.uniform(-2, 2)  # 初始速度
        self.dy = random.uniform(-2, 2)
        self.styles = [pygame.image.load(path) for path in style_paths]
        self.current_style_index = 0  # 初始样式索引
        self.current_style = self.styles[self.current_style_index]
        self.size = BLOCK_SIZE  # 当前大小
        self.growth_rate = growth_rate  # 成长速度

    def switch_style(self):
        # 切换样式，按照尺寸和样式的规则
        if self.size >= ((MAX_SIZE - BLOCK_SIZE) / len(self.styles)) * (self.current_style_index + 2):
            if self.current_style_index < len(self.styles) - 1:
                self.current_style_index += 1
                self.current_style = self.styles[self.current_style_index]  # 更新样式

    def grow(self):
        if self.size < MAX_SIZE:
            self.size += self.growth_rate  # 按设定速度增长

    def move(self):
        # 平滑地调整速度，使其逐渐变化
        self.dx += random.uniform(-SPEED_VARIATION, SPEED_VARIATION)
        self.dy += random.uniform(-SPEED_VARIATION, SPEED_VARIATION)
        self.dx = max(-2, min(2, self.dx))  # 限制速度范围
        self.dy = max(-2, min(2, self.dy))

        self.x += self.dx
        self.y += self.dy

        # 边界检测并反弹
        if self.x <= 0:  # 左边界
            self.x = 0  # 保证不穿越边界
            self.dx = -self.dx  # 反转水平速度
        elif self.x >= WIDTH - self.size:  # 右边界
            self.x = WIDTH - self.size  # 保证不穿越边界
            self.dx = -self.dx  # 反转水平速度

        if self.y <= 0:  # 上边界
            self.y = 0  # 保证不穿越边界
            self.dy = -self.dy  # 反转垂直速度
        elif self.y >= HEIGHT - self.size:  # 下边界
            self.y = HEIGHT - self.size  # 保证不穿越边界
            self.dy = -self.dy  # 反转垂直速度

    def draw(self, screen):
        resized_image = pygame.transform.scale(self.current_style, (int(self.size), int(self.size)))
        screen.blit(resized_image, (self.x, self.y))


# 生成多个方块，每个方块存储多个预备样式，并有不同的成长速度
blocks = [Block(random.choice(ALL_BLOCK_STYLES), random.choice(GROWTH_RATES)) for _ in range(9)]
blocks.append(Block(SIMPLE_BLOCK_STYLES, 0.0025))

clock = pygame.time.Clock()
running = True

# 上次截图时间
last_screenshot_time = pygame.time.get_ticks()

# 创建image文件夹（如果不存在的话）
if not os.path.exists("images"):
    os.makedirs("images")

# 截图计数器
screenshot_count = 0


# 随机图片处理
def random_image_processing(image_path):
    # 打开图片
    img = Image.open(image_path)

    # 随机选择处理类型
    option = random.choice(['rotate', 'color', 'blur', 'noise'])

    if option == 'rotate':
        angle = random.randint(0, 360)
        img = img.rotate(angle)
    elif option == 'color':
        enhancer = ImageEnhance.Color(img)
        factor = random.uniform(0.5, 1.5)  # 调整色彩强度
        img = enhancer.enhance(factor)
    elif option == 'blur':
        img = img.filter(ImageFilter.GaussianBlur(radius=random.uniform(1, 3)))
    elif option == 'noise':
        np_img = np.array(img)
        noise = np.random.normal(0, 25, np_img.shape)  # 添加噪声
        np_img = np.clip(np_img + noise, 0, 255)
        img = Image.fromarray(np_img.astype(np.uint8))

    return img


# 游戏主循环
while running:
    screen.blit(background, (0, 0))  # 绘制背景

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                running = False  # 按 ESC 退出
            elif event.key == pygame.K_SPACE:
                for block in blocks:
                    block.switch_style()  # 按 SPACE 键切换样式

    for block in blocks:
        block.grow()
        block.switch_style()
        block.move()
        block.draw(screen)

    # 在左上角绘制固定图形
    screen.blit(fixed_icon, (FIXED_ICON_MARGIN, FIXED_ICON_MARGIN))

    # 检查是否到了截屏时间（每10秒）
    current_time = pygame.time.get_ticks()
    if current_time - last_screenshot_time >= 10000:  # 10秒
        last_screenshot_time = current_time  # 更新上次截图时间
        if screenshot_count < 100:  # 如果截图数量小于100
            screenshot_filename = f"images/screenshot_{screenshot_count + 1}.png"  # 截图文件名
            pygame.image.save(screen, screenshot_filename)  # 保存截图
            print(f"Saved screenshot: {screenshot_filename}")

            # 对截图进行随机处理
            processed_image = random_image_processing(screenshot_filename)
            processed_image_filename = f"images/processed_screenshot_{screenshot_count + 1}.png"
            processed_image.save(processed_image_filename)
            print(f"Saved processed screenshot: {processed_image_filename}")

            screenshot_count += 1  # 增加截图计数器
        if screenshot_count >= 100:  # 截图达到100时退出
            running = False
            print("Saved 100 screenshots, exiting...")

    pygame.display.flip()
    clock.tick(60)  # 控制帧率

pygame.quit()
