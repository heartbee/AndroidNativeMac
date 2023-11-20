#include <jni.h>
#include <string>
#include <netdb.h>
#include "ifaddrs.h"
#include <android/log.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <map>


using namespace std ;

string getipv6(){
    string ipv6;
    struct ifaddrs *ifap, *ifaptr;
    if (mygetifaddrs(&ifap) == 0) {
        for (ifaptr = ifap; ifaptr != nullptr; ifaptr = (ifaptr)->ifa_next) {
            if (ifaptr->ifa_addr != nullptr) {
                sa_family_t family = ((ifaptr)->ifa_addr)->sa_family;
                if (family == AF_INET || family == AF_INET6 ) {
                    char host[NI_MAXHOST];
                    int ret = getnameinfo(ifaptr->ifa_addr,
                                          (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                          sizeof(struct sockaddr_in6),
                                          host, NI_MAXHOST,
                                          nullptr, 0, NI_NUMERICHOST);
                    if (ret != 0) {
                        __android_log_print(ANDROID_LOG_ERROR, "wocao",
                                            "AF_INET6  getnameinfo() failed   %s  ",
                                            gai_strerror(ret));
                    }
                    __android_log_print(ANDROID_LOG_ERROR, "wocao", "AF_INET6 %s %s  ",
                                        (ifaptr)->ifa_name, host);
                }
            }
        }
        myfreeifaddrs(ifap);
    }
    return ipv6;
}

string getmacaddrs() {
    string macaddress="";
    struct ifaddrs *ifap, *ifaptr;
    if (mygetifaddrs(&ifap) == 0) {
        for (ifaptr = ifap; ifaptr != nullptr; ifaptr = (ifaptr)->ifa_next) {
            if(ifaptr->ifa_addr!= nullptr) {
                sa_family_t family = ((ifaptr)->ifa_addr)->sa_family;
                if (family == AF_PACKET) {
                    char macp[INET6_ADDRSTRLEN];
                    auto *sockadd = (struct sockaddr_ll *) (ifaptr->ifa_addr);
                    int len = 0;
                    if(strcmp(ifaptr->ifa_name,"wlan0")==0 || strcmp(ifaptr->ifa_name,"eth0")){
                        for (int i = 0; i < sockadd->sll_halen; i++) {
                            len += sprintf(macp + len, "%02X%s", sockadd->sll_addr[i],(i < 5 ? ":" : ""));
                        }
                        myfreeifaddrs(ifap);
                        macaddress=macp;
                        return macaddress;
                    }
                }
            }

        }
        myfreeifaddrs(ifap);
        return "";
    } else {
        return "";
    }
}

extern "C" JNIEXPORT jstring JNICALL
        Java_com_example_macdemo_MainActivity_getMacFromJNI(JNIEnv* env,
                                                            jobject jobject){
    string mac=getmacaddrs();
    string defaultmac = "02:00:00:00:00:00";
    if(mac.length()>0){
        return  env->NewStringUTF(mac.c_str());
    }
    return env->NewStringUTF(defaultmac.c_str());
}



extern "C" JNIEXPORT jstring JNICALL
Java_com_example_macdemo_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    string hello = "Hello from C++";
    return  env->NewStringUTF(hello.c_str());


}