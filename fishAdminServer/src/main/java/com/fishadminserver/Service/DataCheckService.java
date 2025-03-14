package com.fishadminserver.Service;

import com.fishadminserver.Entity.DataCheck;
import com.fishadminserver.Repository.DataCheckRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class DataCheckService {
    @Autowired
    DataCheckRepository dataCheckRepository;
    public void setDataCheck(DataCheck dataCheck) {
        dataCheckRepository.save(dataCheck);
    }
    public List<DataCheck> getDataChecks() {
        return dataCheckRepository.findAll();
    }
}
