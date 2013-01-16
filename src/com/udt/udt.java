package com.udt;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class udt extends Activity
{
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
    public native int send(int socket, byte[] buffer, int flags);
    public native int recv(int socket, byte[] buffer, int len, int flags);

    // application port from the appclient
    private void appclient() {
        int result = 0;

        result = startup();
        Log.e("Giggle", "startup result = " + result);

        int handle = socket();
        Log.e("Giggle", "handle = " + handle);

        result = connect(handle, "192.168.1.100", 9000);
        Log.e("Giggle", "connect result = " + result);

        byte[] buffer = new byte[100];
        for (int i = 0; i < 100; i++) buffer[i] = (byte)i;

        result = send(handle, buffer, 0);
        Log.e("Giggle", "send result = " + result);
    }

    static {
        System.loadLibrary("stlport_shared");
        System.loadLibrary("udt");
        System.loadLibrary("udtjni");
    }
}
