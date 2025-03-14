package com.fishadminserver.Entity;

import com.fishadminserver.Util.Base64Utils;
import jakarta.persistence.*;
import lombok.Data;

@Data
@Entity
@Table(name = "image")  // 指定表名
public class Image {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int id;
    private String type;
    @Lob  // 存储二进制数据
    @Column(name = "image", columnDefinition = "LONGBLOB")
    private byte[] image;

    @Transient
    private String imageStr;

    public void setImageStr(String imageStr) {
        if (imageStr != null) {
            this.imageStr = imageStr;
            this.image = Base64Utils.decodeToByteArray(imageStr);
        }
    }
    public void setImage(byte[] image) {
        if (image != null) {
            this.image = image;
            this.imageStr = Base64Utils.encodeToString(image,type);
        }
    }
}
