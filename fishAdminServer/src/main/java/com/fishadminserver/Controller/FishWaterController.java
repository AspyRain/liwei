package com.fishadminserver.Controller;

import com.fishadminserver.Entity.*;
import com.fishadminserver.Service.FishWaterService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import java.util.List;

@RestController
@RequestMapping("/fishwater")
public class FishWaterController {

    @Autowired
    private  FishWaterService fishWaterService;


    @PostMapping("/add")
    public ResponseEntity<String> addFishWaterRecord(@RequestBody FishWaterRequest request) {
        FishRecords fish = new FishRecords();
        fish.setSpeed(request.getSpeed());
        fish.setSize(request.getSize());
        fish.setStatus(request.getFishStatus());

        WaterQualityRecords water = new WaterQualityRecords();
        water.setPhValue(request.getPhValue());
        water.setTurbidity(request.getTurbidity());
        water.setTemperature(request.getTemperature());
        water.setStatus(request.getWaterStatus());

        fishWaterService.saveFishWaterRecord(fish, water);

        return ResponseEntity.ok("Fish and Water Quality records added successfully.");
    }

    // 处理 Base64 图片上传
    @PostMapping("/uploadImage")
    public ResponseEntity<String> uploadFishImage(@RequestBody ImageUploadRequest request) {
        fishWaterService.updateImageForFishRecord(request.getFishId(), request.getImageData());
        return ResponseEntity.ok("Image uploaded successfully.");

    }
    // 查询所有鱼记录
    @GetMapping("/fishRecords")
    public ResponseEntity<List<FishRecords>> getAllFishRecords() {
        List<FishRecords> fishRecords = fishWaterService.getAllFishRecords();
        for (FishRecords fishRecord : fishRecords) {
            fishRecord.setImage(fishRecord.getImage());
        }
        return ResponseEntity.ok(fishRecords);
    }

    /**
     * 获取最近 24 小时的数据
     */
    @GetMapping("/recent")
    public ResponseEntity<List<FishWater>> getRecentRecords() {
        return ResponseEntity.ok(fishWaterService.getRecentRecords());
    }


    /**
     * 获取前 n 天的每日平均数据
     */
    @GetMapping("/dailyAverage/{days}")
    public ResponseEntity<List<DailyAverageDTO>> getDailyAverages(@PathVariable int days) {
        return ResponseEntity.ok(fishWaterService.getDailyAverages(days));
    }
}
