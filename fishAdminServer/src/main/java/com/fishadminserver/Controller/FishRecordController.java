package com.fishadminserver.Controller;

import com.fishadminserver.Entity.FishRecords;
import com.fishadminserver.Service.FishRecordsService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/fishrecords")
public class FishRecordController {
    @Autowired
    private FishRecordsService fishRecordService;


    @GetMapping("/findAll")
    public List<FishRecords> findAllRecords() {
        return fishRecordService.findAll();
    }

    @PostMapping("/save")
    public String createRecord(@RequestBody FishRecords record) {
        fishRecordService.save(record);
        return "success";
    }
}
