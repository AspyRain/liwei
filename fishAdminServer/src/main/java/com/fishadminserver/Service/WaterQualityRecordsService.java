package com.fishadminserver.Service;

import com.fishadminserver.Entity.WaterQualityRecords;
import com.fishadminserver.Repository.WaterQualityRecordsRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class WaterQualityRecordsService {
    @Autowired
    private WaterQualityRecordsRepository waterQualityRecordsRepository;

    public List<WaterQualityRecords> findAll(){
        return waterQualityRecordsRepository.findAll();
    }

    public void save(WaterQualityRecords waterQualityRecords){
        waterQualityRecordsRepository.save(waterQualityRecords);
    }

    public WaterQualityRecords findById(int id){
        return waterQualityRecordsRepository.findById(id);
    }
}
