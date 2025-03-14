package com.fishadminserver.Controller;

import com.fishadminserver.Entity.*;
import com.fishadminserver.Service.FishWaterService;
import com.fishadminserver.Service.ImageService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/fishwater")
public class FishWaterController {

    @Autowired
    private  FishWaterService fishWaterService;

    @Autowired
    private ImageService imageService;


    @PostMapping("/add")
    public ResponseEntity<String> addFishWaterRecord(@RequestBody FishWaterRequest request) {
        FishRecords fish = new FishRecords();
        fish.setSpeed(request.getSpeed());
        fish.setSize(request.getSize());

        WaterQualityRecords water = new WaterQualityRecords();
        water.setPhValue(request.getPhValue());
        water.setTurbidity(request.getTurbidity());
        water.setTemperature(request.getTemperature());

        fishWaterService.saveFishWaterRecord(fish, water);

        return ResponseEntity.ok("Fish and Water Quality records added successfully.");
    }

    // 处理 Base64 图片上传
    @PostMapping("/uploadImage")
    public ResponseEntity<String> uploadFishImage(@RequestBody ImageUploadRequest request) {
        imageService.updateImageForFishRecord(request.getFishId(), request.getImageData());
        return ResponseEntity.ok("Image uploaded successfully.");
    }
    // 查询所有鱼记录
    @GetMapping("/fishRecords")
    public ResponseEntity<List<FishRecords>> getAllFishRecords() {
        List<FishRecords> fishRecords = fishWaterService.getAllFishRecords();
        return ResponseEntity.ok(fishRecords);
    }

    /**
     * 获取最近 24 小时的数据
     */
    @GetMapping("/recent")
    public ResponseEntity<List<FishWaterResponse>> getRecentRecords() {
        return ResponseEntity.ok(fishWaterService.getRecentRecords());
    }


    /**
     * 获取前 n 天的每日平均数据
     */
    @GetMapping("/dailyAverage/{days}")
    public ResponseEntity<List<FishWaterResponse>> getDailyAverages(@PathVariable int days) {
        return ResponseEntity.ok(fishWaterService.getDailyAverages(days-1));
    }
}
