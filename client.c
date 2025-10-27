#define _WIN32_WINNT 0x0600

#include <winsock2.h>     
#include <ws2tcpip.h>     
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    
    int sock = 0;
    struct sockaddr_in serv_addr;
    
    char buffer_kirim[1024] = {0};
    uint8_t buffer_terima[1024] = {0};
    
    printf("Host 2 (Client) memulai...\n");
    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Koneksi gagal. Pastikan server sudah jalan.\n");
        return -1;
    }
    printf("Terhubung ke Host 1!\n\n");

    while(1) {
        
        printf("Ketik pesan (Host 2): ");
        fgets(buffer_kirim, 1024, stdin);
        buffer_kirim[strcspn(buffer_kirim, "\n")] = 0; 

        uint8_t encrypted_msg[1024] = {0};
        int encrypted_len = des_encrypt_ecb((uint8_t*)buffer_kirim, strlen(buffer_kirim), key, encrypted_msg);
        
        send(sock, encrypted_msg, encrypted_len, 0);
        
        printf("Menunggu balasan dari Host 1...\n");
        int read_len = recv(sock, buffer_terima, 1024, 0);
        if (read_len <= 0) break; 

        printf("\t[Ciphertext diterima dari Host 1 (%d bytes): ", read_len);
        for (int i = 0; i < read_len; i++) {
            printf("%02X ", buffer_terima[i]); 
        }
        printf("]\n\t(Mendekripsi...)\n");

        uint8_t decrypted_msg[1024] = {0};
        int decrypted_len = des_decrypt_ecb(buffer_terima, read_len, key, decrypted_msg);
        
        if (decrypted_len >= 0) {
            decrypted_msg[decrypted_len] = '\0';
            printf("Host 1 berkata: %s\n", decrypted_msg);
        }
    }
    
    closesocket(sock);
    WSACleanup();
    return 0;
}