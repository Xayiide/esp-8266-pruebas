#include <sys/param.h>       /* MIN                           */
#include "esp_system.h"      /* ESP_ERROR_CHECK               */
#include "esp_log.h"         /* ESO_LOGI                      */
#include "esp_netif.h"       /* esp_netif_init                */
#include "esp_event.h"       /* esp_event_loop_create_default */
#include "nvs_flash.h"       /* nvs_flash_init                */
#include <esp_http_server.h> /* httpd_handle_t                */


static const char    *TAG    = "APP";
static httpd_handle_t server = NULL;

esp_err_t hello_get_handler(httpd_req_t *req);
esp_err_t ctrl_put_handler (httpd_req_t *req);
esp_err_t echo_post_handler(httpd_req_t *req);


httpd_uri_t hello = {
    .uri      = "/hello",
    .method   = HTTP_GET,
    .handler  = hello_get_handler,
    .user_ctx = "Hello, world!"
};

httpd_uri_t ctrl = {
    .uri      = "/ctrl",
    .method   = HTTP_PUT,
    .handler  = ctrl_put_handler,
    .user_ctx = NULL
};

httpd_uri_t echo = {
    .uri      = "/echo",
    .method   = HTTP_POST,
    .handler  = echo_post_handler,
    .user_ctx = NULL
};

esp_err_t hello_get_handler(httpd_req_t *req)
{
    char  *buf;
    size_t buf_len;

    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);

        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found header => Host: %s", buf);
        }
        free(buf);
    }

    buf_len = httpd_req_get_hdr_value_len(req, "Test-Header-2") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_hdr_value_str(req, "Test-Header-2",
                buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found header => Test-Header-2: %s", buf);
        }
        free(buf);
    }

    buf_len = httpd_req_get_hdr_value_len(req, "Test-Header-1") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_hdr_value_str(req, "Test-Header-1",
                buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found header => Test-Header-1: %s", buf);
        }
        free(buf);
    }



    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found URL query => %s", buf);
            char param[32];

            if (httpd_query_key_value(buf, "query1",
                    param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG, "Found URL query parameter => query1=%s", param);
            }
            if (httpd_query_key_value(buf, "query3",
                    param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG, "Found URL query parameer => query3=%s", param);
            }
            if (httpd_query_key_value(buf, "query2",
                    param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG, "Foun URL query parameter => query2=%s", param);
            }
        }
        free(buf);
    }

    httpd_resp_set_hdr(req, "Custom-Header-1", "Custom-Value-1");
    httpd_resp_set_hdr(req, "Custom-Header-2", "Custom-Value-2");

    const char *resp_str = (const char *) req->user_ctx;
    httpd_resp_send(req, resp_str, strlen(resp_str));

    if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
        ESP_LOGI(TAG, "Request headers lost");
    }

    return ESP_OK;
}

esp_err_t ctrl_put_handler(httpd_req_t *req)
{
    char buf;
    int  ret;

    if ((ret = httpd_req_recv(req, &buf, 1)) <= 0) {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            httpd_resp_send_408(req);
        }
        return ESP_FAIL;
    }

    if (buf == '0') {
        ESP_LOGI(TAG, "Unregistered /hello and /echo URIs");
        httpd_unregister_uri(req->handle, "/hello");
        httpd_unregister_uri(req->handle, "/echo");
    }
    else {
        ESP_LOGI(TAG, "Registering /hello and /echo URIs");
        httpd_register_uri_handler(req->handle, &hello);
        httpd_register_uri_handler(req->handle, &echo);
    }

    httpd_resp_send(req, NULL, 0);
    return ESP_OK;

}

esp_err_t echo_post_handler(httpd_req_t *req)
{
    char buf[100];
    int  ret, remaining = req->content_len;

    while (remaining > 0) {
        if ((ret = httpd_req_recv(req, buf,
                MIN(remaining, sizeof(buf)))) <= 0) {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
                continue;
            }
            return ESP_FAIL;
        }

        httpd_resp_send_chunk(req, buf, ret); /* Manda de vuelta lo mismo */
        remaining -= ret;

        ESP_LOGI(TAG, "=== RECEIVED DATA ===");
        ESP_LOGI(TAG, "%.*s", ret, buf);
        ESP_LOGI(TAG, "=====================");
    }

    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}


httpd_handle_t start_webserver(void)
{
    httpd_handle_t srv    = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    ESP_LOGI(TAG, "Starting server on port %d", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(srv, &hello);
        httpd_register_uri_handler(srv, &echo);
        httpd_register_uri_handler(srv, &ctrl);
        return srv;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

void stop_webserver(httpd_handle_t server)
{
    httpd_stop(server);
}

static void disconnect_handler(void            *arg,
                               esp_event_base_t event_base,
                               int32_t          event_id,
                               void            *event_data)
{
    httpd_handle_t *server = (httpd_handle_t *) arg;
    if (*server) {
        ESP_LOGI(TAG, "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }

}

static void connect_handler(void            *arg,
                            esp_event_base_t event_base,
                            int32_t          event_id,
                            void            *event_data)
{
    httpd_handle_t *server = (httpd_handle_t *) arg;
    if (*server == NULL) {
        ESP_LOGI(TAG, "Starting webserver");
        *server = start_webserver();
    }

}


void app_main()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
            &connect_handler, &server));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT,
            WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));
}
