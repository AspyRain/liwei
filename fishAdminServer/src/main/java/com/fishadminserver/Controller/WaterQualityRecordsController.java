package com.fishadminserver.Controller;


import com.fishadminserver.Entity.WaterQualityRecords;
import com.fishadminserver.Service.WaterQualityRecordsService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/WaterQualityrecords")
public class WaterQualityRecordsController {
    @Autowired
    private WaterQualityRecordsService WaterQualityRecordService;


    @GetMapping("/findAll")
    public List<WaterQualityRecords> findAllRecords() {
        return WaterQualityRecordService.findAll();
    }

    @PostMapping("/save")
    public String createRecord(@RequestBody WaterQualityRecords record) {
        WaterQualityRecordService.save(record);
        return "success";
    }
}
