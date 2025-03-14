package com.fishadminserver.Entity;

import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.Table;
import jakarta.persistence.Transient;
import lombok.Data;

import java.util.ArrayList;
import java.util.List;

@Data
@Entity
@Table(name = "data_check")  // 指定表名
public class DataCheck {
    @Id
    public String label;
    float minValue;
    float maxValue;

    @Transient
    public boolean isError;
    @Transient
    float value;


    public void setvalue(float value) {
        this.value = value;
        isError = value < minValue || value > maxValue;
    }

    public static void setValueForLabel(List<DataCheck> dataChecks, String label, float value) {
        for (DataCheck dataCheck : dataChecks) {
            if (dataCheck.getLabel().equals(label)) {
                dataCheck.setvalue(value); // 赋值并更新 isError
                return; // 找到后直接返回，避免不必要的循环
            }
        }
    }
    public static List<DataCheck> copyDataChecks(List<DataCheck> dataChecks) {
        List<DataCheck> newDataChecks = new ArrayList<>();
        for (DataCheck dataCheck : dataChecks) {
            DataCheck newDataCheck = new DataCheck();
            newDataCheck.setLabel(dataCheck.getLabel());
            newDataCheck.setMinValue(dataCheck.getMinValue());
            newDataCheck.setMaxValue(dataCheck.getMaxValue());
            newDataChecks.add(newDataCheck);
        }

        return newDataChecks;
    }
}
