# Tugas 2 Keamanan Informasi

| Name           | NRP        | Kelas     |
| ---            | ---        | ----------|
| Algof Kristian Zega | 5025231235 | Keamanan Informasi (B) |

# DES (Data Encryption Standard) Client-Server Implementation

[![C](https://img.shields.io/badge/Language-C-lightgrey.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Cara Compile dan Run

Contoh compile menggunakan gcc dalam environment Windows (terminal Windows Powershell)
- Untuk server

```sh
gcc server.c des.c -o server -lws2_32
```
```sh
./server
```

- Untuk client

```sh
gcc client.c des.c -o client -lws2_32
```
```sh
./client
```