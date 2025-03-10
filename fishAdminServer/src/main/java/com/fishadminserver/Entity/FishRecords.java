package com.fishadminserver.Entity;

import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.Table;
import lombok.Data;

import java.sql.Date;

@Data
@Entity
@Table(name = "fishrecords")  // 指定表名
public class FishRecords {
    @Id
    private int id;
    private float speed;
    private float size;
    private int status;
    private Date date;

    public FishRecords(int id, float speed, float size, int status, Date date) {
        this.id = id;
        this.speed = speed;
        this.size = size;
        this.status = status;
        this.date = date;
    }

    public FishRecords() {

    }
}
