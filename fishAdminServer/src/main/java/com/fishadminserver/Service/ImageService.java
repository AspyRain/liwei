package com.fishadminserver.Service;

import com.fishadminserver.Entity.FishRecords;
import com.fishadminserver.Entity.Image;
import com.fishadminserver.Repository.FishRecordsRepository;
import com.fishadminserver.Repository.ImageRepository;
import com.fishadminserver.Util.Base64Utils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

@Service
public class ImageService {
    @Autowired
    private ImageRepository imageRepository;
    @Autowired
    private FishRecordsRepository fishRecordsRepository;

    @Transactional
    public void updateImageForFishRecord(int fishId, String imageStr) {
        // 1. 先将 Base64 图片转换为 byte[]
        Image image = new Image();
        image.setImage(Base64Utils.decodeToByteArray(imageStr));
        image.setType(Base64Utils.extractMimeType(imageStr));

        int imageId = imageRepository.save(image).getId();  // 获取插入的 image ID

        // 3. 更新 FishRecords 记录，使其关联新的 imageId
        fishRecordsRepository.findById(fishId).ifPresent(fish -> {
            fish.setImageId(imageId);  // 只存 imageId
            fishRecordsRepository.save(fish);
        });
    }

    public void addImage(String imageStr) {
        // 1. 先将 Base64 图片转换为 byte[]
        Image image = new Image();
        image.setImage(Base64Utils.decodeToByteArray(imageStr));
        image.setType(Base64Utils.extractMimeType(imageStr));

        int imageId = imageRepository.save(image).getId();  // 获取插入的 image ID
        FishRecords fishRecords=fishRecordsRepository.findLatestFishRecord();
        fishRecords.setImageId(imageId);
        fishRecordsRepository.save(fishRecords);
    }
    public String getImage(int imageId) {
        Image image = imageRepository.findById(imageId).get();
        image.setImage(image.getImage());
        return image.getImageStr();
    }
    public String getRecentImage() {
        Image image = imageRepository.findLatestImage();
        image.setImage(image.getImage());
        return image.getImageStr();
    }
}
