#include "esp_system.h" /* ESP_ERROR_CHECK                */
#include "esp_log.h"    /* ESP_LOGI                       */
#include "esp_netif.h"  /* esp_netif_init                 */
#include "esp_event.h"  /* esp_event_loop_create_default  */
#include "nvs_flash.h"  /* nvs_flash_init                 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <netdb.h>      /* AF_INET, SOCK_STREAM, addrinfo */
#include <sys/socket.h> /* setsockopt                     */

#define WEB_SERVER "example.com"
#define WEB_PORT   80
#define WEB_URL    "http://example.com"

static const char *TAG = "example";

static const char *REQUEST = "GET " WEB_URL " HTTP/1.0\r\n"
    "Host: "WEB_SERVER"\r\n"
    "User-Agent: esp-idf/1.0 esp32\r\n"
    "\r\n";

static void http_get_task(void *pvParameters)
{
    const struct addrinfo hints = {
        .ai_family   = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *res;
    struct in_addr  *addr;
    int s, r;
    char recv_buf[64];

    while (1) {
        int err = getaddrinfo(WEB_SERVER, "80", &hints, &res);

        if (err != 0 || res == NULL) {
            ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        /* inet_ntoa es no-reentrante. Es mejor ipaddr_ntoa_r */
        addr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
        ESP_LOGI(TAG, "DNS lookup succeeded. IP: %s", inet_ntoa(*addr));

        s = socket(res->ai_family, res->ai_socktype, 0);
        if (s < 0) {
            ESP_LOGE(TAG, "... Failed to allocate socket.");
            freeaddrinfo(res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        ESP_LOGI(TAG, "... allocated socket");

        if (connect(s, res->ai_addr, res->ai_addrlen) != 0) {
            ESP_LOGE(TAG, "... socket connect failed errno: %d", errno);
            close(s);
            freeaddrinfo(res);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }

        ESP_LOGI(TAG, "... connected");
        freeaddrinfo(res);

        if (write(s, REQUEST, strlen(REQUEST)) < 0) {
            ESP_LOGE(TAG, "... socket send failed");
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... socket send success");

        struct timeval receiving_timeout;
        receiving_timeout.tv_sec  = 5;
        receiving_timeout.tv_usec = 0;
        if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout,
                sizeof(receiving_timeout)) < 0) {
            ESP_LOGE(TAG, "... failed to set socket receiving timeout");
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... set socket receiving timeout success");

        /* lee la respuesta HTTP */
        do {
            bzero(recv_buf, sizeof(recv_buf));
            r = read(s, recv_buf, sizeof(recv_buf)-1);
            for (int i = 0; i < r; i++) {
                putchar(recv_buf[i]);
            }
        } while (r > 0);

        ESP_LOGI(TAG, "... done reading from socket.    Last read return: %d" \
                "errno: %d\r\n", r, errno);
        close(s);
        for (int countdown = 10; countdown >= 0; countdown--) {
            ESP_LOGI(TAG, "%d...", countdown);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        ESP_LOGI(TAG, "Starting again!");
    }
}

void app_main()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    xTaskCreate(&http_get_task, "http_get_task", 16384, NULL, 5, NULL);
}
