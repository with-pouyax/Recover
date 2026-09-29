# JPEG recovery from a card image

A C exercise in binary file I/O. The program scans a raw card image in 512-byte blocks, detects JPEG headers and writes numbered `.jpg` files.

## Run

```sh
cc -Wall -Wextra -Werror recover.c -o recover
mkdir recovered
cd recovered
../recover ../card.raw
```

Running in a dedicated directory keeps the recovered images separate. Files are named `000.jpg`, `001.jpg`, and so on.

The header check uses `FF D8 FF` followed by an `E0`–`EF` marker nibble. The implementation assumes JPEGs start on a 512-byte boundary and writes full blocks until the next header or EOF. It does not infer the exact JPEG end marker, so output may include trailing bytes. Source: [`recover.c`](recover.c). [License](LICENSE).
