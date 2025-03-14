package com.fishadminserver.Entity;

import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.Table;
import lombok.Data;
@Data
@Entity
@Table(name = "wqrecords")  // 指定表名
public class WaterQualityRecords {
    @Id
    private int id;
    private float phValue;
    private float turbidity;
    private float temperature;

    public WaterQualityRecords(int id, float phValue, float turbidity, float temperature) {
        this.id = id;
        this.phValue = phValue;
        this.turbidity = turbidity;
        this.temperature = temperature;
    }

    public WaterQualityRecords() {

    }
}
