package com.fishadminserver.Repository;

import com.fishadminserver.Entity.FishRecords;
import org.springframework.data.jpa.repository.JpaRepository;

public interface FishRecordsRepository extends JpaRepository<FishRecords, Long> {
    public FishRecords findById(int id);
    public void deleteById(int id);
}
