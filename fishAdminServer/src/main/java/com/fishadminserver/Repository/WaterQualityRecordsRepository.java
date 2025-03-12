package com.fishadminserver.Repository;

import com.fishadminserver.Entity.WaterQualityRecords;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;


public interface WaterQualityRecordsRepository extends JpaRepository<WaterQualityRecords, Integer> {}
