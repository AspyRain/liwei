package com.fishadminserver.Entity;

import jakarta.persistence.*;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDateTime;


@Entity
@Table(name = "fish_water")
@Data
@NoArgsConstructor
@AllArgsConstructor
public class FishWater {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer fwId;

    @ManyToOne
    @JoinColumn(name = "fish_id", nullable = false)
    private FishRecords fish;

    @ManyToOne
    @JoinColumn(name = "wq_id", nullable = false)
    private WaterQualityRecords waterQuality;

    @Column(name = "date", columnDefinition = "TIMESTAMP")
    private LocalDateTime date;



}
