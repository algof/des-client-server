#include "des.h"
#include <stdlib.h>
#include <stdio.h>

const int initial_permutation_table[64] = {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

const int final_permutation_table[64] = {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
};

const int expansion_table[48] = {
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
};

const int s_boxes[8][4][16] = {
    {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
     {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
     {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
     {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},

    {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
     {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
     {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
     {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},

    {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
     {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
     {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
     {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},

    {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
     {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
     {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
     {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},

    {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
     {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
     {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
     {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},

    {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
     {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
     {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
     {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},

    {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
     {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
     {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
     {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},

    {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
     {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
     {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
     {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}
};

const int p_box_table[32] = {
    16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
};

const int pc1_table[56] = {
    57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
};

const int pc2_table[48] = {
    14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

const int key_shifts[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

void permute_bits(const uint8_t *input, uint8_t *output, const int *table, int size){
    for (int i = 0; i < size; i++){ // size menyesuaikan predefined table yang digunakan
        int pos = table[i] - 1; // mengambil posisi dari tabel, -1 karena tabel dimulai dari 1, sedangkan array dari 0
        int in_byte_pos = pos / 8; // cari untuk pos tersebut dia ada di byte ke berapa
        int in_bit_pos = 7 - (pos % 8); // cari untuk pos tersebut dia ada di bit ke berapa dalam byte
        /*
        "7-" Membalik urutannya. 
        Ini mengasumsikan bit 0 adalah Most Significant Bit (MSB, paling kiri) dan bit 7 adalah Least Significant Bit (LSB, paling kanan).
        */

        int out_byte_pos = i / 8; // cari byte ke berapa untuk output
        int out_bit_pos = 7 - (i % 8); // cari bit ke berapa dalam byte untuk output

        if ((input[in_byte_pos] >> in_bit_pos) & 1){ // cari byte di input, geser ke kanan sesuai bit pos, lalu AND dengan 1 untuk mendapatkan nilai bitnya
            // jika bitnya 1
            // (1 << out_bit_pos) menggeser 1 ke kiri sesuai posisi bit output
            // Lalu dengan OR kita memasukkan 1 tersebut ke dalam output
            output[out_byte_pos] = output[out_byte_pos] | (1 << out_bit_pos);
        }
        else{
            // jika bitnya 0
            // ~(1 << out_bit_pos) menggeser 1 ke kiri sesuai posisi bit output
            // Lalu dengan AND kita memasukkan 0 tersebut ke dalam output
            // Memasukkan 0 bisa dilakukan dengan meng-AND-kan bit tersebut dengan 0
            // Untuk memastikan bit lain tidak terpengaruh, kita gunakan bit 1 sehingga jika pada awalnya nilai bit output 1 akan tetap 1
            // Contoh (1 << out_bit_pos) = 00010000
            // ~(1 << out_bit_pos) = 11101111
            output[out_byte_pos] = output[out_byte_pos] & ~(1 << out_bit_pos);
        }
    }
}

void left_shift(uint8_t *data, int shifts){
    uint32_t val = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]; // menggabungkan 4 byte menjadi 28 bit dalam satu variabel 32 bit
    /*
    Start geser 24 karena data[0] ada 8 bit, sehingga untuk memindahkannya ke posisi paling kiri (bit 31-24) perlu digeser 24 bit ke kiri
    Lalu data[1] digeser 16 bit ke kiri agar berada di posisi bit 23-16
    data[2] digeser 8 bit ke kiri agar berada di posisi bit 15-8
    data[3] tetap di posisi bit 7-0
    */
    val <<= shifts; // geser kiri sesuai shifts
    val |= (val >> 28); // ambil bit yang overflow dari kiri dan masukkan ke kanan
    val &= 0x0FFFFFFF; // bersihkan 4 bit paling kiri agar hanya tersisa 28 bit, misalkan ada sisa 1111 agar menjadi 0000
    
    // pisahkan kembali menjadi 4 byte sambil dibersihkan
    data[0] = (val >> 24) & 0xFF;
    data[1] = (val >> 16) & 0xFF;
    data[2] = (val >> 8) & 0xFF;
    data[3] = val & 0xFF;
}

void generate_sub_keys(const uint8_t key[8], uint8_t sub_keys[16][6]){
    uint8_t permuted_key_56[7] = {0}; // menyimpan 56 bit key setelah permutasi PC-1
    permute_bits(key, permuted_key_56, pc1_table, 56); // permutasi PC-1

    // membagi 56 bit menjadi 2 bagian 28 bit
    uint8_t c[4] = {permuted_key_56[0], permuted_key_56[1], permuted_key_56[2], permuted_key_56[3] & 0xF0/*11110000*/};
    uint8_t d[4] = {permuted_key_56[3] & 0x0F/*00001111*/, permuted_key_56[4], permuted_key_56[5], permuted_key_56[6]};

    for (int i = 0; i < 16; i++)    {
        // circular left shift dengan pergeseran sesuai predefined table key_shifts
        left_shift(c, key_shifts[i]);
        left_shift(d, key_shifts[i]);

        uint8_t combined_key[7];

        // menggabungkan kiri dan kanan menjadi 56 bit
        combined_key[0] = c[0]; 
        combined_key[1] = c[1];
        combined_key[2] = c[2];
        combined_key[3] = c[3] | d[0];
        combined_key[4] = d[1];
        combined_key[5] = d[2];
        combined_key[6] = d[3];

        memset(sub_keys[i], 0, 6); // inisialisasi sub key ke- i dengan 0
        permute_bits(combined_key, sub_keys[i], pc2_table, 48); // permutasi PC-2 untuk mendapatkan sub key ke- i
    }
}

void feistel_function(const uint8_t r[4], uint8_t f_out[4], const uint8_t sub_key[6]){
    uint8_t expanded_r[6] = {0}; // variabel menyimpan hasil ekpansi dari 32 bit ke 48 bit, inisiasi dengan 0
    permute_bits(r, expanded_r, expansion_table, 48); // permutasi ekspansi dengan predefined table expansion table

    for (int i = 0; i < 6; i++)    {
        expanded_r[i] = expanded_r[i] ^ sub_key[i]; // XOR antara hasil ekspansi dengan sub key
    }

    uint8_t s_box_output[4] = {0};
    for (int i = 0; i < 8; i++){ // 8 S-boxes
        uint8_t six_bits = 0; // menyimpan 6 bit untuk setiap S-box, inisiasi dengan 0
        six_bits = (expanded_r[i * 6 / 8] << (i * 6 % 8)) | (expanded_r[i * 6 / 8 + 1] >> (8 - (i * 6 % 8))); // mengambil 6 bit dari expanded_r sesuai indeks S-box
        six_bits >>= 2; // 6 bit disimpan di posisi paling kiri, geser ke kanan 2 bit untuk menghilangkan bit ekstra
        six_bits &= 0x3F; // membersihkan 2 bit paling kiri yang sudah dibuang
// 0011
        int row = ((six_bits & 0x20) >> 4) | (six_bits & 0x01); // ambil bit 1 dan 6 untuk menentukan row di S-box
        /*
        0x20 = 00100000 (bit ke-6)
        0x01 = 00000001 (bit ke-1)
        6 bit: b1 b2 b3 b4 b5 b6
        ngambil bit pakai AND
        lalu untuk yang bit ke-6 digeser ke kanan 4 posisi agar menjadi bit ke-2
        sehingga hasilnya menjadi 0000b1b6
        */
        int col = (six_bits & 0x1E) >> 1;
        /*
        0x1E = 00011110 (bit ke-2, 3, 4, 5)
        6 bit: b1 b2 b3 b4 b5 b6
        ngambil bit pakai AND
        lalu digeser ke kanan 1 posisi agar menjadi 000b2b3b4b5
        */

        uint8_t val = s_boxes[i][row][col]; // ambil nilai dari S-box ke-i sesuai row dan col 

        if (i % 2 == 0){ // 0, 2, 4, 6
            s_box_output[i / 2] |= (val << 4); // i-nya genap isi dari paling kiri
        }
        else{ // 1, 3, 5, 7
            s_box_output[i / 2] |= val; // i-nya ganjil gabung dengan yang kiri 
        }
    }
    permute_bits(s_box_output, f_out, p_box_table, 32); // permutasi P-box dengan predefined table p_box_table
}

void des_crypt(const uint8_t input[8], uint8_t output[8], const uint8_t sub_keys[16][6], int mode){
    uint8_t permuted_input[8] = {0}; // menyimpan hasil initial permutation
    permute_bits(input, permuted_input, initial_permutation_table, 64); // initial permutation dengan predefined table

    uint8_t l[4], r[4]; // L dan R masing-masing 32 bit (4 byte)
    memcpy(l, permuted_input, 4); // L diisi 4 byte pertama dari permuted_input
    memcpy(r, permuted_input + 4, 4); // R diisi 4 byte terakhir dari permuted_input

    for (int i = 0; i < 16; i++){
        uint8_t temp_l[4]; // variabel L_lama
        memcpy(temp_l, l, 4); // temp_l = L_lama

        // mode 0 for encryption, 1 for decryption
        // kalau enkripsi mulai dari sub key 0 sampai 15
        // kalau dekripsi mulai dari sub key 15 sampai 0
        int sub_key_index = (mode == 0) ? i : (15 - i);

        uint8_t f_result[4] = {0}; // menyimpan hasil f(R_lama, Key_i)
        feistel_function(r, f_result, sub_keys[sub_key_index]);

        memcpy(l, r, 4); // L_baru = R_lama
        for (int j = 0; j < 4; j++){
            r[j] = temp_l[j] ^ f_result[j]; // R_baru = L_lama XOR f(R_lama, Key_i)
        }
    }

    uint8_t combined[8];
    memcpy(combined, r, 4); // combined byte 1-4 diisi R_baru
    memcpy(combined + 4, l, 4); // combined byte 5-8 diisi L_baru

    permute_bits(combined, output, final_permutation_table, 64); // final permutation dengan predefined table
}

int des_encrypt_ecb(const uint8_t *plaintext, int plaintext_len, const uint8_t key[8], uint8_t *ciphertext){
    uint8_t sub_keys[16][6]; // menyimpan 16 sub key dengan masing-masing 48 bit (6 byte)
    generate_sub_keys(key, sub_keys); // generate 16 sub key dari key utama

    int num_blocks = plaintext_len / 8; //  hitung banyak blok 8 byte penuh
    int remaining_bytes = plaintext_len % 8; // hitung sisa byte yang tidak penuh 8 byte

    for (int i = 0; i < num_blocks; i++){
        des_crypt(plaintext + (i * 8), ciphertext + (i * 8), sub_keys, 0); // enkripsi setiap 8 byte blok
    }

    uint8_t last_block[8]; // variabel untuk menyimpan blok terakhir dengan padding
    int padding_val = 8 - remaining_bytes; // nilai padding yang digunakan, antara 1 sampai 8 didapat dari sisa byte

    if (remaining_bytes > 0){ // ada sisa byte yang perlu dipadding
        memcpy(last_block, plaintext + (num_blocks * 8), remaining_bytes); // salin sisa byte ke last_blocks
    }

    for (int i = remaining_bytes; i < 8; i++){
        last_block[i] = padding_val; // tambahkan padding sesuai nilai padding_val
    }

    des_crypt(last_block, ciphertext + (num_blocks * 8), sub_keys, 0); // enkripsi blok terakhir yang sudah dipadding

    return (num_blocks + 1) * 8; // kembalikan panjang total ciphertext
}

int des_decrypt_ecb(const uint8_t *ciphertext, int ciphertext_len, const uint8_t key[8], uint8_t *decryptedtext){

    if (ciphertext_len % 8 != 0){ // pastikan panjang ciphertext adalah kelipatan 8
        printf("Error: Panjang ciphertext harus kelipatan 8.\n");
        return -1;
    }

    uint8_t sub_keys[16][6]; // menyimpan 16 sub key dengan masing-masing 48 bit (6 byte)
    generate_sub_keys(key, sub_keys); // generate 16 sub key dari key utama

    int num_blocks = ciphertext_len / 8; // hitung banyak blok 8 byte penuh
    
    for (int i = 0; i < num_blocks; i++){
        des_crypt(ciphertext + (i * 8), decryptedtext + (i * 8), sub_keys, 1); // dekripsi setiap 8 byte blok
    }
    
    int last_byte_pos = ciphertext_len - 1; // posisi byte terakhir dalam decrypted text
    int padding_val = decryptedtext[last_byte_pos]; // ambil nilai padding dari byte terakhir

    if (padding_val < 1 || padding_val > 8){ // jika nilai padding bukan antara 1 sampai 8, berarti padding tidak valid
        printf("Error: Padding tidak valid.\n");
        return ciphertext_len; 
    }

    return ciphertext_len - padding_val; // kembalikan panjang asli plaintext tanpa padding
}