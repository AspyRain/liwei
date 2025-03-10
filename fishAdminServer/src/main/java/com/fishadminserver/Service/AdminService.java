package com.fishadminserver.Service;

import com.fishadminserver.Entity.Admin;
import com.fishadminserver.Repository.AdminRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class AdminService {
    @Autowired
    private AdminRepository adminRepository;

    public List<Admin> login(String username, String password) {
        return adminRepository.findAllByUsernameAndPassword(username,password);
    }

    public void register(Admin admin) {
        adminRepository.save(admin);
    }
}
