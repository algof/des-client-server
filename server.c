#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "des.h" 

#define PORT 9090 

int main() {
    WSADATA wsaData;
    int iResult;
    
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup gagal: %d\n", iResult);
        return 1;
    }
    
    uint8_t key[9] = "kunciku1";

    int server_fd, client_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    char buffer_kirim[1024] = {0};
    uint8_t buffer_terima[1024] = {0};

    
    printf("Host 1 (Server) memulai...\n");
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    
    printf("Menunggu koneksi dari Host 2...\n");
    client_sock = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    printf("Host 2 Terhubung!\n\n");

    
    while(1) {
        printf("Menunggu pesan dari Host 2...\n");
        int read_len = recv(client_sock, buffer_terima, 1024, 0);
        if (read_len <= 0) break; 

        printf("   [Ciphertext diterima dari Host 2 (%d bytes): ", read_len);
        for (int i = 0; i < read_len; i++) {
            printf("%02X ", buffer_terima[i]); 
        }
        printf("]\n   (Mendekripsi...)\n");

        uint8_t decrypted_msg[1024] = {0};
        int decrypted_len = des_decrypt_ecb(buffer_terima, read_len, key, decrypted_msg);
        
        if (decrypted_len >= 0) {
            decrypted_msg[decrypted_len] = '\0';
            printf("Host 2 berkata: %s\n", decrypted_msg);
        }
        
        printf("Ketik balasan (Host 1): ");
        fgets(buffer_kirim, 1024, stdin);
        buffer_kirim[strcspn(buffer_kirim, "\n")] = 0; 
        
        uint8_t encrypted_msg[1024] = {0};
        int encrypted_len = des_encrypt_ecb((uint8_t*)buffer_kirim, strlen(buffer_kirim), key, encrypted_msg);
        
        send(client_sock, encrypted_msg, encrypted_len, 0);
    }
    
    closesocket(client_sock);
    WSACleanup();
    return 0;
}