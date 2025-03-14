package com.fishadminserver.Controller;

import com.fishadminserver.Service.ImageService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/image")
public class ImageController {
    @Autowired
    private ImageService imageService;
    // 处理 Base64 图片上传
    @PostMapping("/add")
    public ResponseEntity<String> uploadFishImage(@RequestBody String request) {
        imageService.addImage(request);
        return ResponseEntity.ok("Image add successfully.");
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
