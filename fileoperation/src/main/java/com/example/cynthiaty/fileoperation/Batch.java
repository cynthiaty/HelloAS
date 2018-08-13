package com.example.cynthiaty.fileoperation;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.math.BigInteger;
import java.util.HashMap;
import java.util.Map;

import static java.lang.System.arraycopy;

/**
 * Created by ShangPingping on 2018/6/6.
 */

public class Batch {
    public static final String APP_CONFIG_PATH = "/storage/emulated/0/";

    /**
     * Crc16 check.
     * @param bytes The array of bytes to be verified.
     * @param length The length of the byte array to be verified.
     * @return Return check code.
     */
    public static byte[] crc16(byte[] bytes, int length) {
        int crc = 0x0000;
        int poly = 0xa001;

        for (int i = 0; i < length; i++) {
            crc ^= bytes[i];
            for (int j = 0; j < 8; j++) {
                if((crc & 1) > 0) {
                    crc >>= 1;
                    crc ^= poly;
                }
                else crc >>= 1;
            }
        }

        return intToByte(crc, 2);
    }

    /**
     * Data type conversion: int is converted to a len-byte byte array.
     * @param num Integer data to be converted.
     * @param len The length of the byte array.
     * @return Return a byte array of length len.
     */
    public static byte[] intToByte(int num, int len) {
        byte[] result = new byte[len];
        for (int i = 0; i < result.length; i++) {
            result[result.length - i - 1] = (byte) (num & 0xff);
            num = num >> 8;
        }

        return result;
    }

    /**
     * Data type conversion: byte array to int.
     * @param bytes The byte array to be converted.
     * @return Return int.
     */
    public static int byteToInt(byte[] bytes) {
        int result = 0;
        for (int i = 0; i < bytes.length; i++) {
            result = bytes[i] & 0xff;
            if(i < bytes.length - 1) {
                result <<= 8;
            }
        }

        return result;
    }
    /**
     * Data type conversion: byte array into a variety of hexadecimal strings.
     * @param bytes byte[]
     * @param radix The base can be converted into the range of,(Character.MIN_RADIX, Character.MAX_RADIX)，
     *              after the range is changed to Oct.
     * @return Converted string.
     */
    public static String binary(byte[] bytes, int radix) {
        // Here 1 represents a positive number.
        String result = new BigInteger(1, bytes).toString(radix);
        return result;
    }

    /**
     * Byte array assignment, when the length is not enough, the low bit fills 0.
     * @param child Sub array.
     * @param fatherLen The length of parent array.
     * @return Return the parent array after fills 0.
     */
    public static byte[] byteCoverAtStart(byte[] child, int fatherLen) {
        if(fatherLen == 0 || child.length > fatherLen) {
            return null;
        }

        byte[] bytes = new byte[fatherLen];
        arraycopy(new byte[fatherLen - child.length], 0, bytes, 0, fatherLen - child.length);
        arraycopy(child, 0, bytes, fatherLen - child.length, child.length);

        return bytes;
    }

    /**
     * Ping the serverIP every minute to determine whether the network is unblocked.
     * @param serverIP
     * @return
     */
    private boolean startPing(String serverIP) {
        boolean success = false;
        Process process = null;

        try {
            process = Runtime.getRuntime().exec("ping -c 1 -w 1 " + serverIP);
            int status = process.waitFor();
            if(status == 0) {
                success = true;
            } else {
                success = false;
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            process.destroy();
        }

        return success;
    }

    /**
     *
     * Read the file and parse it.
     * FileInputStream default read address：data/data/applicationId/files/fileName
     * Read address: /storage/emulated/0/fileName
     * @param fileName
     * @return
     */
    public static Map<String, String> readFileFromMemoryAndParse(String fileName) {
        FileInputStream fis = null;
        byte[] bytes = null;
        Map<String, String> mapConfig = new HashMap<>();

        // Read the configuration file.
        File file = new File(APP_CONFIG_PATH + fileName);
        if(file.exists()) {
            try {
                fis = new FileInputStream(file);
                bytes = new byte[fis.available()];
                while(fis.read(bytes) != -1) ;
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                try {
                    if(fis != null) fis.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        } else {
            return null;
        }

        // Data analysis.
        String configData = new String(bytes);
        String strLine;
        int pos;
        String key;
        while(!configData.isEmpty()) {
            if(configData.contains("\r\n")) {
                strLine = configData.substring(0, configData.indexOf("\r\n"));
                configData = configData.substring(configData.indexOf("\r\n") + 2);
            } else {
                strLine = configData;
                configData = "";
            }

            pos = strLine.indexOf("=");
            key = strLine.substring(0, pos);
            mapConfig.put(key, strLine.substring(pos + 1));
        }

        return mapConfig;
    }

    /**
     * Save the file to memory.
     * FileOutputStream default save address：data/data/applicationId/files/fileName
     * Save address: /storage/emulated/0/fileName
     * @param fileName
     * @param fileContent
     */
    public static void saveFileToMemory(String fileName, byte[] fileContent) {
        FileOutputStream fos = null;

        File file = new File(APP_CONFIG_PATH + fileName);
        if(!file.exists()) {
            try {
                file.createNewFile();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        try {
            fos = new FileOutputStream(file, true);
            fos.write(fileContent);
            fos.flush();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if(fos != null) fos.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
