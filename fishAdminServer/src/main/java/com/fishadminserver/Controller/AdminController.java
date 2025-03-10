package com.fishadminserver.Controller;

import com.fishadminserver.Entity.Admin;
import com.fishadminserver.Service.AdminService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/admin")
public class AdminController {
    @Autowired
    private  AdminService adminService;


    @GetMapping("/login")
    public String login(@RequestParam String username, @RequestParam String password) {
        if (!adminService.login(username, password).isEmpty()){
            return "success";
        }else {
            return "fail";
        }
    }

    @PostMapping("/register")
    public String  register(@RequestBody Admin admin) {
        adminService.register(admin);
        return "success";
    }
}
