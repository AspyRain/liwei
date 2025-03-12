package com.fishadminserver.Repository;

import com.fishadminserver.Entity.FishWater;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import java.time.LocalDateTime;
import java.time.LocalDate;
import java.util.List;

public interface FishWaterRepository extends JpaRepository<FishWater, Integer> {

    // 查找最近 24 小时的数据
    @Query("SELECT fw FROM FishWater fw WHERE fw.date >= :startTime")
    List<FishWater> findRecentRecords(@Param("startTime") LocalDateTime startTime);

    // 统计前 n 天的平均值
    @Query("SELECT fw.date, AVG(f.speed), AVG(f.size), AVG(f.status), " +
            "AVG(w.phValue), AVG(w.turbidity), AVG(w.temperature), AVG(w.status) " +
            "FROM FishWater fw " +
            "JOIN fw.fish f " +
            "JOIN fw.waterQuality w " +
            "WHERE fw.date >= :startDate " +
            "GROUP BY fw.date " +
            "ORDER BY fw.date DESC")
    List<Object[]> findDailyAverages(@Param("startDate") LocalDate startDate);

}
