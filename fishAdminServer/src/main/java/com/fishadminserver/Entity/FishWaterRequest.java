package com.fishadminserver.Entity;

import lombok.Data;

@Data
public class FishWaterRequest {
    private float speed;
    private float size;
    private float phValue;
    private float turbidity;
    private float temperature;
}
