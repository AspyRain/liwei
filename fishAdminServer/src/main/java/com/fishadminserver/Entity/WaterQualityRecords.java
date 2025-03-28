package com.fishadminserver.Entity;

import jakarta.persistence.*;
import lombok.Data;
@Data
@Entity
@Table(name = "wqrecords")  // 指定表名
public class WaterQualityRecords {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
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
