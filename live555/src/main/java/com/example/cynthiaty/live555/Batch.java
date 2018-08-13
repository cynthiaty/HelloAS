package com.example.cynthiaty.live555;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Created by Administrator on 2018/8/13.
 */

public class Batch {
    public static final String APP_CONFIG_PATH = "/storage/emulated/0/";
    public static ExecutorService exec = Executors.newCachedThreadPool();

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
