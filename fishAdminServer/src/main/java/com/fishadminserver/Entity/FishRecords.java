package com.fishadminserver.Entity;

import jakarta.persistence.*;
import lombok.Data;



@Data
@Entity
@Table(name = "fishrecords")  // 指定表名
public class FishRecords {

    @Id
    private int id;

    private float speed;
    private float size;

    @Column(name = "image_id")  // 仅存储 ImageRecords 的主键 ID
    private Integer imageId=0;
}
