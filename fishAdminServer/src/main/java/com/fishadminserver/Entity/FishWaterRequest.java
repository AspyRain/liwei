package com.fishadminserver.Entity;

import lombok.Data;

@Data
public class FishWaterRequest {
    private float speed;
    private float size;
    private int fishStatus;
    private float phValue;
    private float turbidity;
    private float temperature;
    private int waterStatus;
}
