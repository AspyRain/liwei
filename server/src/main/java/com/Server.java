package com;

import java.io.*;
import java.net.*;

public class Server {
    private static Socket currentClient = null; // 维护当前的客户端连接

    public static void main(String[] args) {
        int port = 8888; // 服务器监听端口

        try (ServerSocket serverSocket = new ServerSocket(port)) {
            System.out.println("Server is listening on port " + port);

            while (true) {
                // 等待新的客户端连接
                Socket newClient = serverSocket.accept();

                // 如果已有连接，则关闭旧连接
                if (currentClient != null && !currentClient.isClosed()) {
                    System.out.println("Closing previous connection...");
                    try {
                        currentClient.close();
                    } catch (IOException e) {
                        System.out.println("Error closing previous client: " + e.getMessage());
                    }
                }

                // 处理新的客户端连接
                currentClient = newClient;
                System.out.println("New client connected: " + newClient.getInetAddress());

                // 启动新线程处理客户端通信
                new Thread(() -> handleClient(currentClient)).start();
            }
        } catch (IOException e) {
            System.out.println("Server error: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private static void handleClient(Socket client) {
        try (BufferedReader reader = new BufferedReader(new InputStreamReader(client.getInputStream()))) {
            String message;
            while ((message = reader.readLine()) != null) {
                System.out.println("Received message: " + message);
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
}
