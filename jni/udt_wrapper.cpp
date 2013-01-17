#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>

#include "udt.h"
#include "com_udt_udt.h"
#include <android/log.h>

#include <string>

namespace {

    std::string to_string(int value) 
    {
        char buffer[100] = {0};
        sprintf(buffer, "%d", value);

        return std::string(buffer);
    }
}

const char TAG[] = "UDT-JNI";

jint JNICALL Java_com_udt_udt_startup(JNIEnv *, jobject)
{
    return UDT::startup();
}

jint JNICALL Java_com_udt_udt_cleanup(JNIEnv *, jobject)
{
    return UDT::cleanup();
}

jint JNICALL Java_com_udt_udt_socket(JNIEnv *, jobject)
{
    struct addrinfo hints, *local;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int result = 0;
    if ((result = getaddrinfo(NULL, "9000", &hints, &local)) != 0)
    {
        std::string error = "incorrect network address" + to_string(result);
        __android_log_write(ANDROID_LOG_ERROR, TAG, error.c_str());
        return 0;
    }

    int handle = UDT::socket(local->ai_family, local->ai_socktype, local->ai_protocol);
    freeaddrinfo(local);

    return handle;
}

jint JNICALL Java_com_udt_udt_connect(JNIEnv* env, jobject thiz, jint handle, jstring ip, jint port)
{
    const char *ip_address = env->GetStringUTFChars(ip, NULL);

    struct addrinfo hints, *peer;
    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    std::string port_str = to_string(port);
    if (0 != getaddrinfo(ip_address, port_str.c_str(), &hints, &peer))
    {
        __android_log_write(ANDROID_LOG_ERROR, TAG, "incorrect server/peer address. ");
        return 0;
    }

    // connect to the server, implict bind
    int connect_result = 0;
    if ((connect_result = UDT::connect(handle, peer->ai_addr, peer->ai_addrlen)) == UDT::ERROR)
    {
        __android_log_write(ANDROID_LOG_ERROR, TAG, "connect error");
        return 0;
    }

    freeaddrinfo(peer);
    return connect_result;
}

jint JNICALL Java_com_udt_udt_close(JNIEnv *env, jobject thiz, jint handle)
{
    return UDT::close(handle);
}

jint JNICALL Java_com_udt_udt_send(JNIEnv *env, jobject thiz, jint handle, jbyteArray buffer, jint offset, jint max_send, jint flag)
{
    //
    // TODO: Consider to use GetPrimitiveArrayCritical which though may stall GC. The current implementation
    // may copy the whole buffer.
    //
    jbyte *local_buffer = new jbyte[max_send];
    env->GetByteArrayRegion(buffer, offset, max_send, local_buffer);

    int sent_size = UDT::send(handle, (const char*)local_buffer, max_send, flag);
    if (sent_size == UDT::ERROR)
    {
        __android_log_write(ANDROID_LOG_ERROR, TAG, UDT::getlasterror().getErrorMessage());
    }

    delete local_buffer;
    return sent_size;
}

jint JNICALL Java_com_udt_udt_recv(JNIEnv *env, jobject thiz, jint handle, jbyteArray buffer, jint offset, jint max_read, jint flags)
{
    //
    // TODO: Consider to use GetPrimitiveArrayCritical which though may stall GC. The current implementation
    // may copy the whole buffer.
    //
    jbyte *local_buffer = new jbyte[max_read];

    int recv_size = 0;
    if (UDT::ERROR == (recv_size = UDT::recv(handle, (char*)local_buffer, max_read, flags)))
    {
        recv_size = 0;
        __android_log_write(ANDROID_LOG_ERROR, TAG, UDT::getlasterror().getErrorMessage());
    }

    env->SetByteArrayRegion(buffer, offset, recv_size, local_buffer);

    delete local_buffer;
    return recv_size;
}
