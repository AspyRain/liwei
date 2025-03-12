package com.fishadminserver.Service;

import com.fishadminserver.Entity.DailyAverageDTO;
import com.fishadminserver.Entity.FishRecords;
import com.fishadminserver.Entity.FishWater;
import com.fishadminserver.Entity.WaterQualityRecords;
import com.fishadminserver.Repository.FishRecordsRepository;
import com.fishadminserver.Repository.FishWaterRepository;
import com.fishadminserver.Repository.WaterQualityRecordsRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.List;
import java.util.stream.Collectors;

@Service
public class FishWaterService {

    @Autowired
    private  FishRecordsRepository fishRecordsRepository;

    @Autowired
    private  WaterQualityRecordsRepository waterQualityRecordsRepository;

    @Autowired
    private  FishWaterRepository fishWaterRepository;


    @Transactional
    public void saveFishWaterRecord(FishRecords fish, WaterQualityRecords water) {
        // 保存 fish 记录
        FishRecords savedFish = fishRecordsRepository.save(fish);

        // 保存 water 记录
        WaterQualityRecords savedWater = waterQualityRecordsRepository.save(water);

        // 创建 FishWater 记录
        FishWater fishWater = new FishWater();
        fishWater.setFish(savedFish);
        fishWater.setWaterQuality(savedWater);

        // 获取系统时间（年月日小时）
        LocalDateTime now = LocalDateTime.now();
        LocalDate today = now.toLocalDate();
        fishWater.setDate(now);

        // 保存 FishWater 记录
        fishWaterRepository.save(fishWater);
    }

    @Transactional
    public void updateImageForFishRecord(int fishId, String imageStr) {
        fishRecordsRepository.findById(fishId).ifPresent(fish -> {
            fish.setImageStr(imageStr);
            fishRecordsRepository.save(fish);
        });
    }
    public List<FishRecords> getAllFishRecords() {
        return fishRecordsRepository.findAll();
    }

    /**
     * 查询最近 24 小时的数据
     */
    @Transactional(readOnly = true)
    public List<FishWater> getRecentRecords() {
        LocalDateTime startTime = LocalDateTime.now().minusHours(24);
        return fishWaterRepository.findRecentRecords(startTime);
    }

    /**
     * 查询前 n 天的每日平均数据
     */
    @Transactional(readOnly = true)
    public List<DailyAverageDTO> getDailyAverages(int days) {
        LocalDate startDate = LocalDate.now().minusDays(days);
        List<Object[]> results = fishWaterRepository.findDailyAverages(startDate);

        return results.stream().map(obj -> new DailyAverageDTO(
                (LocalDate) obj[0],  // 日期
                (Double) obj[1],      // 平均速度
                (Double) obj[2],      // 平均体型
                ((Double) obj[3]).intValue(),  // 鱼状态（转换为整数）
                (Double) obj[4],      // 平均 pH 值
                (Double) obj[5],      // 平均浑浊度
                (Double) obj[6],      // 平均温度
                ((Double) obj[7]).intValue()   // 水质状态（转换为整数）
        )).collect(Collectors.toList());
    }
}
