package com.udt;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class udt extends Activity
{
    private static final String TAG = "UDT-Activity";

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        appclient();
    }

    public native int startup();
    public native int cleanup();
    public native int socket();
    public native int connect(int socket, String ip, int port);
    public native int close(int socket);
    public native int send(int socket, byte[] buffer, int offset, int size, int flags);
    public native int recv(int socket, byte[] buffer, int offset, int size, int flags);

    // application port from the appclient
    private void appclient() {
        int result = 0;

        result = startup();
        Log.e(TAG, "startup result = " + result);

        int handle = socket();
        Log.e(TAG, "handle = " + handle);

        result = connect(handle, "localhost", 9000);
        Log.e(TAG, "connect result = " + result);

        int data_size = 100000;
        byte[] buffer = new byte[data_size];
        for (int i = 0; i < data_size; i++) buffer[i] = (byte)i;

        int sent_size = 0;
        while (sent_size != data_size)
        {
            int send_count = send(handle, buffer, sent_size, data_size-sent_size, 0);
            Log.e(TAG, "send count = " + send_count);
            sent_size += send_count;
        }

        int recv_size = 0;
        byte[] recv_back = new byte[data_size];
        while (recv_size != data_size)
        {
            int size = recv(handle, recv_back, recv_size, data_size-recv_size, 0);
            Log.e(TAG, "   recv size = " + size);
            recv_size += size;
        }

        // verify the send result
        boolean verify_result = true;
        for (int i = 0; i < data_size; i++)
        {
            if (buffer[i] != recv_back[i]) 
            {
                Log.e(TAG, "Fail on entry " + i);
                verify_result = false;
                break;
            }
        }
        if (verify_result) 
            Log.e(TAG, "Verify ok!");
        else
            Log.e(TAG, "Verify fail!");

        result = close(handle);
        Log.e(TAG, "close result = " + result);

        result = cleanup();
        Log.e(TAG, "cleanup result = " + result);
    }

    static {
        System.loadLibrary("stlport_shared");
        System.loadLibrary("udt");
        System.loadLibrary("udtjni");
    }
}
