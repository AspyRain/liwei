package com.fishadminserver.Entity;

import lombok.AllArgsConstructor;
import lombok.Data;
import java.time.LocalDate;

@Data
@AllArgsConstructor
public class DailyAverageDTO {
    private LocalDate date;
    private Double avgSpeed;
    private Double avgSize;
    private Integer avgFishStatus;
    private Double avgPhValue;
    private Double avgTurbidity;
    private Double avgTemperature;
    private Integer avgWaterStatus;
}
