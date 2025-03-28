package com;

import java.io.*;
import java.net.*;

public class Client {
    public static void main(String[] args) {
        String serverAddress = "localhost"; // 服务器地址
        int port = 12345; // 服务器端口

        try (Socket socket = new Socket(serverAddress, port)) {
            // 获取输出流
            OutputStream outputStream = socket.getOutputStream();
            PrintWriter writer = new PrintWriter(outputStream, true);

            // 发送透传消息
            writer.println("Hello, server!");

            socket.close();
        } catch (IOException e) {
            System.out.println("Client error: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
