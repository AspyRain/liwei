package com.fishadminserver.Repository;

import com.fishadminserver.Entity.Image;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;


public interface ImageRepository extends JpaRepository<Image, Integer> {
    @Query("SELECT i FROM Image i ORDER BY i.id DESC LIMIT 1")
    Image findLatestImage();
}
