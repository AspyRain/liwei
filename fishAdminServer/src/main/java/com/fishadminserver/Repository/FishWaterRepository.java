package com.fishadminserver.Repository;

import com.fishadminserver.Entity.FishWater;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;

import java.time.LocalDateTime;
import java.util.List;

public interface FishWaterRepository extends JpaRepository<FishWater, Integer> {

    // 查找最近 24 小时的数据
    @Query("SELECT fw FROM FishWater fw WHERE fw.date >= :startTime ORDER BY fw.date DESC")
    List<FishWater> findRecentRecords(@Param("startTime") LocalDateTime startTime);

    // 统计前 n 天的平均值
    @Query(value = "SELECT DATE(fw.date) AS day, AVG(f.speed), AVG(f.size), ROUND(AVG(f.status)), " +
            "AVG(wq.ph_value), AVG(wq.turbidity), AVG(wq.temperature), ROUND(AVG(wq.status)) " +
            "FROM fish_water fw " +
            "JOIN fishrecords f ON fw.fish_id = f.id " +
            "JOIN wqrecords wq ON fw.wq_id = wq.id " +
            "WHERE fw.date >= :startDate " +  // 过滤起始日期
            "AND fw.date < DATE_ADD(:startDate, INTERVAL :days DAY) " +  // 确保查询范围
            "GROUP BY DATE(fw.date) " +
            "ORDER BY DATE(fw.date) DESC", nativeQuery = true)
    List<Object[]> findDailyAverages(@Param("startDate") LocalDateTime startDate, @Param("days") int days);



}
