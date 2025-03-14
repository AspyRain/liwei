package com.fishadminserver.Entity;

import com.fishadminserver.Repository.DataCheckRepository;
import lombok.Data;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

@Data
public class FishWaterResponse {
    private List<DataCheck> dataChecks;
    private LocalDateTime date;
    private int imageId=0;

    public FishWaterResponse(List<DataCheck> dataChecks ,FishWater data) {
        this.date = data.getDate();
        DataCheck.setValueForLabel(dataChecks,"speed",data.getFish().getSpeed());
        DataCheck.setValueForLabel(dataChecks,"size",data.getFish().getSize());
        DataCheck.setValueForLabel(dataChecks,"phValue",data.getWaterQuality().getPhValue());
        DataCheck.setValueForLabel(dataChecks,"temperature",data.getWaterQuality().getTemperature());
        DataCheck.setValueForLabel(dataChecks,"turbidity",data.getWaterQuality().getTurbidity());
        this.dataChecks = dataChecks;
        this.imageId = data.getFish().getImageId();
    }

    public static List<FishWaterResponse> getFishWaterResponses(DataCheckRepository dataCheckRepository, List<FishWater> fishWaters) {
        List<DataCheck> baseDataChecks = dataCheckRepository.findAll();
        List<FishWaterResponse> fishWaterResponses = new ArrayList<>();
        for (FishWater fish : fishWaters) {
            FishWaterResponse fishWaterResponse = new FishWaterResponse(DataCheck.copyDataChecks(baseDataChecks),fish);
            fishWaterResponses.add(fishWaterResponse);
        }
        return fishWaterResponses;
    }

}
