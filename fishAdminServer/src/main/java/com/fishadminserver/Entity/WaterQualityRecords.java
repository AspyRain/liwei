package com.fishadminserver.Entity;

import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.Table;
import lombok.Data;

import java.sql.Date;
@Data
@Entity
@Table(name = "wqrecords")  // 指定表名
public class WaterQualityRecords {
    @Id
    private int id;
    private float phValue;
    private float turbidity;
    private float temperature;
    private int status;

    public WaterQualityRecords(int id, float phValue, float turbidity, float temperature, int status) {
        this.id = id;
        this.phValue = phValue;
        this.turbidity = turbidity;
        this.temperature = temperature;
        this.status = status;
    }

    public WaterQualityRecords() {

    }
}
