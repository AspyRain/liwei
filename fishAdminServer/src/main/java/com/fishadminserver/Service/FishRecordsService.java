package com.fishadminserver.Service;

import com.fishadminserver.Entity.FishRecords;
import com.fishadminserver.Repository.FishRecordsRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class FishRecordsService {

    @Autowired
    private FishRecordsRepository fishRecordsRepository;

    public List<FishRecords> findAll() {
        return fishRecordsRepository.findAll();
    }

    public void save(FishRecords fishRecords) {
        fishRecordsRepository.save(fishRecords);
    }

    public FishRecords findById(int id) {
        return fishRecordsRepository.findById(id);
    }


}
