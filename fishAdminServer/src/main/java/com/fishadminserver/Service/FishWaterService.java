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
        LocalDateTime startDateTime = startDate.atStartOfDay(); // 设定起始时间
        List<Object[]> results = fishWaterRepository.findDailyAverages(startDateTime, days);

        return results.stream().map(obj -> new DailyAverageDTO(
                ((java.sql.Date) obj[0]).toLocalDate().atStartOfDay(), // 日期（转换为 LocalDateTime）
                ((Number) obj[1]).doubleValue(), // 平均速度
                ((Number) obj[2]).doubleValue(), // 平均体型
                ((Number) obj[3]).intValue(),   // 鱼状态
                ((Number) obj[4]).doubleValue(), // 平均 pH 值
                ((Number) obj[5]).doubleValue(), // 平均浑浊度
                ((Number) obj[6]).doubleValue(), // 平均温度
                ((Number) obj[7]).intValue()    // 水质状态
        )).collect(Collectors.toList());
    }


}
