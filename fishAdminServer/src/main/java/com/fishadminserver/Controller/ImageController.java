package com.fishadminserver.Controller;

import com.fishadminserver.Service.ImageService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import java.io.IOException;
import java.util.Base64;

@RestController
@RequestMapping("/image")
public class ImageController {
    @Autowired
    private ImageService imageService;
    // 处理 Base64 图片上传
    @PostMapping("/add")
    public ResponseEntity<String> uploadImage(@RequestParam("file") String base64Data) {

        imageService.addImage(base64Data);
            // 返回成功消息
        return ResponseEntity.ok("Image uploaded and saved successfully!");
    }

    // 将图片文件转换为 Base64 字符串
    private String convertImageToBase64(MultipartFile file) throws IOException {
        // 获取图片字节数据
        byte[] imageBytes = file.getBytes();
        // 将字节数组编码为 Base64 字符串
        return Base64.getEncoder().encodeToString(imageBytes);
    }

    @GetMapping("/get/{id}")
    public ResponseEntity<String> getImageById(@PathVariable int id) {

        return ResponseEntity.ok(imageService.getImage(id));
    }

    @GetMapping("/recent")
    public ResponseEntity<String> getRecentImages() {
        return ResponseEntity.ok(imageService.getRecentImage());
    }
}
