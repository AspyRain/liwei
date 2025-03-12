package com.fishadminserver.Entity;

import com.fishadminserver.Util.Base64Utils;
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
    private int status;

    @Lob  // 存储二进制数据
    @Column(name = "image", columnDefinition = "LONGBLOB")
    private byte[] image;

    @Transient
    private String imageStr;

    public FishRecords(int id, float speed, float size, int status, byte[] image) {
        this.id = id;
        this.speed = speed;
        this.size = size;
        this.status = status;
        this.image = image;
        this.imageStr = Base64Utils.encodeToString(image,"image/png");


    }
    public FishRecords(int id, float speed, float size, int status,String imageStr) {
        this.id = id;
        this.speed = speed;
        this.size = size;
        this.status = status;
        this.imageStr = imageStr;
        this.image = Base64Utils.decodeToByteArray(imageStr);

    }
    public void setImageStr(String imageStr) {
        if (imageStr != null) {
            this.imageStr = imageStr;
            this.image = Base64Utils.decodeToByteArray(imageStr);
        }
    }
    public void setImage(byte[] image) {
        if (image != null) {
            this.image = image;
            this.imageStr = Base64Utils.encodeToString(image,"image/png");
        }
    }
    public FishRecords() {
    }
}
