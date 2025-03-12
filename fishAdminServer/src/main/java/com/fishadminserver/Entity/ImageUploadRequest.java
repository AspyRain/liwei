package com.fishadminserver.Entity;

import lombok.Data;

@Data
public class ImageUploadRequest {
    private int fishId;
    private String imageData;  // Base64 编码的图片数据
}
