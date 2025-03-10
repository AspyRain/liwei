package com.fishadminserver.Repository;

import com.fishadminserver.Entity.Admin;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface AdminRepository extends JpaRepository<Admin, Long> {
    public List<Admin> findAllByUsernameAndPassword(String username, String password);

}