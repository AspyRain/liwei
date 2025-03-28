package com.fishadminserver.Controller;

import java.io.*;
import java.net.*;
import java.time.LocalDateTime;
import java.util.concurrent.atomic.AtomicInteger;

import com.fishadminserver.Entity.FishRecords;
import com.fishadminserver.Entity.WaterQualityRecords;
import com.fishadminserver.Service.FishWaterService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class TcpServer implements Runnable {
    private static Socket currentClient = null;

    @Autowired
    private FishWaterService fishWaterService;

    public TcpServer() {
        Thread serverThread = new Thread(this);
        serverThread.setDaemon(true);
        serverThread.start();
    }

    @Override
    public void run() {
        int port = 8888;

        try (ServerSocket serverSocket = new ServerSocket(port)) {
            System.out.println("Server is listening on port " + port);

            while (true) {
                Socket newClient = serverSocket.accept();

                if (currentClient != null && !currentClient.isClosed()) {
                    System.out.println("Closing previous connection...");
                    try {
                        currentClient.close();
                    } catch (IOException e) {
                        System.out.println("Error closing previous client: " + e.getMessage());
                    }
                }

                currentClient = newClient;
                System.out.println("New client connected: " + newClient.getInetAddress());

                new Thread(() -> handleClient(currentClient)).start();
            }
        } catch (IOException e) {
            System.out.println("Server error: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private void handleClient(Socket client) {
        try (BufferedReader reader = new BufferedReader(new InputStreamReader(client.getInputStream()))) {
            String message;
            while ((message = reader.readLine()) != null) {
                System.out.println("Received message: " + message);

                // 解析数据并存入数据库
                processData(message);
            }
        } catch (IOException e) {
            System.out.println("Client disconnected: " + e.getMessage());
        } finally {
            try {
                client.close();
            } catch (IOException ignored) {
            }
        }
    }

    private void processData(String message) {
        try {
            // 确保消息符合 "data:0.0,0.0,0.0,0.0,0.0" 格式
            if (!message.startsWith("data:")) {
                System.out.println("Invalid data format: " + message);
                return;
            }

            // 提取数据部分
            String[] parts = message.substring(5).split(",");
            if (parts.length != 5) {
                System.out.println("Invalid data format, expected 5 values: " + message);
                return;
            }

            // 解析数据
            float temperature = Float.parseFloat(parts[0]);
            float phValue = Float.parseFloat(parts[1]);
            float turbidity = Float.parseFloat(parts[2]);
            float speed = Float.parseFloat(parts[3]);
            float size = Float.parseFloat(parts[4]);

            // 创建 WaterQualityRecords 记录
            WaterQualityRecords waterRecord = new WaterQualityRecords();
            waterRecord.setTemperature(temperature);
            waterRecord.setPhValue(phValue);
            waterRecord.setTurbidity(turbidity);

            // 创建 FishRecords 记录
            FishRecords fishRecord = new FishRecords();
            fishRecord.setSpeed(speed);
            fishRecord.setSize(size);

            // 保存到数据库
            fishWaterService.saveFishWaterRecord(fishRecord, waterRecord);

            System.out.println("Data saved successfully.");
        } catch (Exception e) {
            System.out.println("Error processing data: " + e.getMessage());
        }
    }
}
