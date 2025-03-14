package com.fishadminserver.Service;

import com.fishadminserver.Entity.*;
import com.fishadminserver.Repository.DataCheckRepository;
import com.fishadminserver.Repository.FishRecordsRepository;
import com.fishadminserver.Repository.FishWaterRepository;
import com.fishadminserver.Repository.WaterQualityRecordsRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

@Service
public class FishWaterService {

    @Autowired
    private  FishRecordsRepository fishRecordsRepository;

    @Autowired
    private  WaterQualityRecordsRepository waterQualityRecordsRepository;

    @Autowired
    private  FishWaterRepository fishWaterRepository;

    @Autowired
    private DataCheckRepository dataCheckRepository;




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


    public List<FishRecords> getAllFishRecords() {
        return fishRecordsRepository.findAll();
    }

    /**
     * 查询最近 24 小时的数据
     */
    @Transactional(readOnly = true)
    public List<FishWaterResponse> getRecentRecords() {
        LocalDateTime startTime = LocalDateTime.now().minusHours(24);
        List<FishWater> fishWaters = fishWaterRepository.findRecentRecords(startTime);
        return FishWaterResponse.getFishWaterResponses(dataCheckRepository,fishWaters);
    }

    /**
     * 查询前 n 天的每日平均数据
     */
    @Transactional(readOnly = true)
    public List<FishWaterResponse> getDailyAverages(int days) {
        LocalDate startDate = LocalDate.now().minusDays(days);
        LocalDateTime startDateTime = startDate.atStartOfDay(); // 设定起始时间
        List<Object[]> results = fishWaterRepository.findDailyAverages(startDateTime, days);
        List<DailyAverageDTO> dailyAverageDTOS = results.stream().map(obj -> new DailyAverageDTO(
                ((java.sql.Date) obj[0]).toLocalDate().atStartOfDay(), // 日期（转换为 LocalDateTime）
                ((Number) obj[1]).doubleValue(), // 平均速度
                ((Number) obj[2]).doubleValue(), // 平均体型
                ((Number) obj[3]).intValue(),   // 鱼状态
                ((Number) obj[4]).doubleValue(), // 平均 pH 值
                ((Number) obj[5]).doubleValue(), // 平均浑浊度
                ((Number) obj[6]).doubleValue(), // 平均温度
                ((Number) obj[7]).intValue()    // 水质状态
        )).toList();
        List<FishWater> fishWaters = new ArrayList<>();
        for (DailyAverageDTO dailyAverageDTO : dailyAverageDTOS) {
            FishWater fishWater = getFishWater(dailyAverageDTO);
            fishWaters.add(fishWater);
        }
        return FishWaterResponse.getFishWaterResponses(dataCheckRepository,fishWaters);
    }

    private static FishWater getFishWater(DailyAverageDTO dailyAverageDTO) {
        FishWater fishWater = new FishWater();
        fishWater.setDate(dailyAverageDTO.getDate());
        FishRecords fishRecords = new FishRecords();
        fishRecords.setSize(dailyAverageDTO.getAvgSize().floatValue());
        fishRecords.setSpeed(dailyAverageDTO.getAvgSpeed().floatValue());
        fishWater.setFish(fishRecords);
        WaterQualityRecords waterQualityRecords = new WaterQualityRecords();
        waterQualityRecords.setTurbidity(dailyAverageDTO.getAvgTurbidity().floatValue());
        waterQualityRecords.setTemperature(dailyAverageDTO.getAvgTemperature().floatValue());
        waterQualityRecords.setPhValue(dailyAverageDTO.getAvgPhValue().floatValue());
        fishWater.setWaterQuality(waterQualityRecords);
        return fishWater;
    }


}
