import torch
print(torch.cuda.is_available())  # True表示可用GPU
print(torch.cuda.device_count())  # GPU数量
print(torch.cuda.get_device_name(0))  # 获取GPU名称