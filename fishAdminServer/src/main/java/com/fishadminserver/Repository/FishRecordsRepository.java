package com.fishadminserver.Repository;

import com.fishadminserver.Entity.FishRecords;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;



public interface FishRecordsRepository extends JpaRepository<FishRecords, Integer> {}

