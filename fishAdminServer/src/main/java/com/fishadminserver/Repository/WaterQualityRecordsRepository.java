package com.fishadminserver.Repository;

import com.fishadminserver.Entity.WaterQualityRecords;
import org.springframework.data.jpa.repository.JpaRepository;

public interface WaterQualityRecordsRepository extends JpaRepository<WaterQualityRecords, Long> {
    public WaterQualityRecords findById(int id);
    public void deleteById(int id);
}
