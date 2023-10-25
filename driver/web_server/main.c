#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "ssi.h"
#include "cgi.h"
#include "lwip/ip.h"
#include "lwip/netif.h"

#include "sensor_data.h"

// WIFI Credentials - take care if pushing to github!
const char WIFI_SSID[] = "skyish";
const char WIFI_PASSWORD[] = "totoro222";

void print_ip_address() {
    struct netif *netif = netif_default; // Get the default network interface (usually the Wi-Fi interface)

    if (netif != NULL) {
        printf("IP Address: %u.%u.%u.%u\n\n",
               ip4_addr1(netif_ip4_addr(netif)),
               ip4_addr2(netif_ip4_addr(netif)),
               ip4_addr3(netif_ip4_addr(netif)),
               ip4_addr4(netif_ip4_addr(netif)));
    } else {
        printf("No network interface found.\n");
    }
}

int main() {
    stdio_init_all();

    cyw43_arch_init();

    cyw43_arch_enable_sta_mode();
    // Set as AP mode
    // cyw43_arch_enable_ap_mode(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_MIXED_PSK);

    // Connect to the WiFI network - loop until connected
    while(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0){
        printf("Attempting to connect...\n");
    }
    // Print a success message once connected
    printf("Connected! \n");
    print_ip_address();

    // Initialise web server
    httpd_init();
    printf("Http server initialised\n");

    // Configure SSI and CGI handler
    ssi_init(false); 
    printf("\nSSI Handler initialised\n");
    cgi_init();
    printf("CGI Handler initialised\n");
    
    // Infinite loop
    while(1);
}