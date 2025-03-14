package com.fishadminserver.Controller;

import com.fishadminserver.Entity.DataCheck;
import com.fishadminserver.Service.DataCheckService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/datacheck")
public class DataCheckController {
    @Autowired
    DataCheckService dataCheckService;
    // 查询所有鱼记录
    @GetMapping("/datachecks")
    public ResponseEntity<List<DataCheck>> getAllDataChecks() {
        List<DataCheck> dataChecks = dataCheckService.getDataChecks();
        return ResponseEntity.ok(dataChecks);
    }
    @PostMapping("/update")
    public ResponseEntity<String> updateDataCheck(@RequestBody List<DataCheck> dataChecks) {
        for (DataCheck dataCheck : dataChecks) {
            dataCheckService.setDataCheck(dataCheck);
        }

        return ResponseEntity.ok("Data Check updated successfully.");
    }
}
