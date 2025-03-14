package com.fishadminserver.Repository;

import com.fishadminserver.Entity.DataCheck;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface DataCheckRepository extends JpaRepository<DataCheck, String> { }
