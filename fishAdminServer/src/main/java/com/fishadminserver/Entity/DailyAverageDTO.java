package com.fishadminserver.Entity;

import lombok.AllArgsConstructor;
import lombok.Data;

import java.time.LocalDateTime;

@Data
@AllArgsConstructor
public class DailyAverageDTO {
    private LocalDateTime date;
    private Double avgSpeed;
    private Double avgSize;
    private Double avgPhValue;
    private Double avgTurbidity;
    private Double avgTemperature;
}
