package com.fishadminserver.Util;
import java.util.Base64;

public class Base64Utils {
    public static byte[] decodeToByteArray(String base64String) {
        String[] parts = base64String.split(",");
        String encodedData = parts.length > 1 ? parts[1] : "";
        return Base64.getDecoder().decode(encodedData);
    }

    public static String encodeToString(byte[] byteArray, String mimeType) {
        String base64String = Base64.getEncoder().encodeToString(byteArray);
        return "data:" + mimeType + ";base64," + base64String;
    }

    public static String encodeToStringNotGiveType(byte[] byteArray){
        return Base64.getEncoder().encodeToString(byteArray);
    }
    public static String extractMimeType(String base64String) {
        String[] parts = base64String.split(":");
        if (parts.length > 1) {
            String mimeTypePart = parts[1];
            String[] mimeTypeParts = mimeTypePart.split(";");
            return mimeTypeParts[0];
        }
        return "";
    }
}
