package com.fishadminserver.Repository;

import com.fishadminserver.Entity.FishRecords;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;



public interface FishRecordsRepository extends JpaRepository<FishRecords, Integer> {
    @Query("SELECT f FROM FishRecords f ORDER BY f.id DESC LIMIT 1")
    FishRecords findLatestFishRecord();

}

