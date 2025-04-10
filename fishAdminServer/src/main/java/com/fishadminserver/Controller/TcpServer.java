package com.fishadminserver.Controller;

import com.fishadminserver.Entity.FishRecords;
import com.fishadminserver.Entity.WaterQualityRecords;
import com.fishadminserver.Service.FishWaterService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import java.io.*;
import java.net.*;
import java.time.LocalDateTime;

@Component
public class TcpServer implements Runnable {

    private static Socket currentClient1 = null;
    private static Socket currentClient2 = null;

    // 用于存储数据
    private static Float temperature = null;
    private static Float phValue = null;
    private static Float turbidity = null;
    private static Float speed = null;
    private static Float size = null;

    // 上一条数据，用于计算变化幅度
    private static Float lastTemperature = null;
    private static Float lastPhValue = null;
    private static Float lastTurbidity = null;
    private static Float lastSpeed = null;
    private static Float lastSize = null;

    // 用户可设置的幅度限制
    private static final float MAX_TEMP_CHANGE = 5.0f; // 温度最大变化幅度（单位：摄氏度）
    private static final float MAX_PH_CHANGE = 20f;   // pH 值最大变化幅度
    private static final float MAX_TURBIDITY_CHANGE = 1000.0f; // 浑浊度最大变化幅度
    private static final float MAX_SPEED_CHANGE = 5.0f; // 速度最大变化幅度（单位：cm/s）
    private static final float MAX_SIZE_CHANGE = 5.0f;   // 尺寸最大变化幅度（单位：cm）

    @Autowired
    private FishWaterService fishWaterService;

    public TcpServer() {
        Thread serverThread = new Thread(this);
        serverThread.setDaemon(true);
        serverThread.start();
        scheduleDataSaving();
    }

    @Override
    public void run() {
        int port1 = 8887; // 用来接收温度、pH 值和浑浊度
        int port2 = 8889; // 用来接收速度和尺寸

        try {
            // 开启第一个端口
            ServerSocket serverSocket1 = new ServerSocket(port1);
            System.out.println("Server is listening on port " + port1);
            // 开启第二个端口
            ServerSocket serverSocket2 = new ServerSocket(port2);
            System.out.println("Server is listening on port " + port2);

            while (true) {
                // 接受连接
                Socket newClient1 = serverSocket1.accept();
                if (currentClient1 != null && !currentClient1.isClosed()) {
                    System.out.println("Closing previous connection on port " + port1);
                    try {
                        currentClient1.close();
                    } catch (IOException e) {
                        System.out.println("Error closing previous client: " + e.getMessage());
                    }
                }
                currentClient1 = newClient1;
                System.out.println("New client connected on port " + port1);
                new Thread(() -> handleClient(currentClient1, 1)).start();

                // 接受连接
                Socket newClient2 = serverSocket2.accept();
                if (currentClient2 != null && !currentClient2.isClosed()) {
                    System.out.println("Closing previous connection on port " + port2);
                    try {
                        currentClient2.close();
                    } catch (IOException e) {
                        System.out.println("Error closing previous client: " + e.getMessage());
                    }
                }
                currentClient2 = newClient2;
                System.out.println("New client connected on port " + port2);
                new Thread(() -> handleClient(currentClient2, 2)).start();
            }
        } catch (IOException e) {
            System.out.println("Server error: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private void handleClient(Socket client, int port) {
        try (BufferedReader reader = new BufferedReader(new InputStreamReader(client.getInputStream()))) {
            String message;
            while ((message = reader.readLine()) != null) {
                System.out.println("Received message: " + message);

                // 根据端口处理数据
                if (port == 1) {
                    // 端口1用于接收温度、ph、浑浊度
                    processDataForWaterQuality(message);
                } else if (port == 2) {
                    // 端口2用于接收速度和尺寸
                    processDataForFish(message);
                }
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

    private void processDataForWaterQuality(String message) {
        try {
            if (!message.startsWith("data:")) {
                System.out.println("Invalid data format: " + message);
                return;
            }

            String[] parts = message.substring(5).split(",");
            if (parts.length != 3) {
                System.out.println("Invalid data format, expected 3 values: " + message);
                return;
            }

            float cur_temperature = Float.parseFloat(parts[0]);
            float cur_phValue = Float.parseFloat(parts[1]);
            float cur_turbidity = Float.parseFloat(parts[2]);

            // 首次赋值时进行范围检查
            if ( (cur_temperature < -10 || cur_temperature > 100)) {
                System.out.println("Temperature out of range on first input, ignoring data: " + cur_temperature);
                return;
            }
            if ((cur_phValue < 0 || cur_phValue > 14)) {
                System.out.println("pH value out of range on first input, ignoring data: " + cur_phValue);
                return;
            }
            if ((cur_turbidity < 0 || cur_turbidity > 2000)) {
                System.out.println("Turbidity out of range on first input, ignoring data: " + cur_turbidity);
                return;
            }

            // 检查数据变化幅度
            if (lastTemperature != null && (Math.abs(cur_temperature - lastTemperature) > MAX_TEMP_CHANGE) ) {
                System.out.println("Temperature change too large, ignoring data: " + cur_temperature);
                return;
            }
            if (lastPhValue != null && (Math.abs(cur_phValue - lastPhValue) > MAX_PH_CHANGE)) {
                System.out.println("pH value change too large, ignoring data: " + cur_phValue);
                return;
            }
            if (lastTurbidity != null && (Math.abs(cur_turbidity - lastTurbidity) > MAX_TURBIDITY_CHANGE)) {
                System.out.println("Turbidity change too large, ignoring data: " + cur_turbidity);
                return;
            }

            // 更新数据
            temperature = cur_temperature;
            phValue = cur_phValue;
            turbidity = cur_turbidity;

            lastTemperature = cur_temperature;
            lastPhValue = cur_phValue;
            lastTurbidity = cur_turbidity;

            System.out.printf("Received water quality data - Temperature: %s, pH: %s, Turbidity: %s\n", temperature, phValue, turbidity);

        } catch (Exception e) {
            System.out.println("Error processing water quality data: " + e.getMessage());
        }
    }

    private void processDataForFish(String message) {
        try {
            if (!message.startsWith("data:")) {
                System.out.println("Invalid data format: " + message);
                return;
            }

            String[] parts = message.substring(5).split(",");
            if (parts.length != 2) {
                System.out.println("Invalid data format, expected 2 values: " + message);
                return;
            }

            float cur_speed = Float.parseFloat(parts[0]);
            float cur_size = Float.parseFloat(parts[1]);

            // 首次赋值时进行范围检查
            if ((cur_speed < 0 || cur_speed > 100)) {
                System.out.println("Speed out of range on first input, ignoring data: " + cur_speed);
                return;
            }
            if ((cur_size < 0 || cur_size > 100)) {
                System.out.println("Size out of range on first input, ignoring data: " + cur_size);
                return;
            }

            // 检查数据变化幅度
            if (lastSpeed != null && Math.abs(cur_speed - lastSpeed) > MAX_SPEED_CHANGE) {
                System.out.println("Speed change too large, ignoring data: " + cur_speed);
                return;
            }
            if (lastSize != null && (Math.abs(cur_size - lastSize) > MAX_SIZE_CHANGE) && Math.abs(cur_size - lastSize) != 0 ) {
                System.out.println("Size change too large, ignoring data: " + cur_size);
                return;
            }

            // 更新数据
            speed = cur_speed;
            size = cur_size;

            lastSpeed = cur_speed;
            lastSize = cur_size;

            System.out.printf("Received fish data - Speed: %s, Size: %s\n", speed, size);

        } catch (Exception e) {
            System.out.println("Error processing fish data: " + e.getMessage());
        }
    }


    // 总控程序，每隔 5 秒钟打包并保存数据
    private void scheduleDataSaving() {
        new Thread(() -> {
            while (true) {
                try {
                    // 每隔 5 秒执行一次保存
                    Thread.sleep(60000);
                    saveDataToDatabase();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }).start();
    }

    private void saveDataToDatabase() {
        if (temperature != null && phValue != null && turbidity != null && speed != null && size != null) {
            try {
                // 创建记录对象并保存
                WaterQualityRecords waterRecord = new WaterQualityRecords();
                waterRecord.setTemperature(temperature);
                waterRecord.setPhValue(phValue);
                waterRecord.setTurbidity(turbidity);

                FishRecords fishRecord = new FishRecords();
                fishRecord.setSpeed(speed);
                fishRecord.setSize(size);

                // 保存到数据库
                fishWaterService.saveFishWaterRecord(fishRecord, waterRecord);
                System.out.println("Data saved successfully.");
            } catch (Exception e) {
                System.out.println("Error saving data to database: " + e.getMessage());
            }
        }
    }
}
