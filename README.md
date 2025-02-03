# Recover

![Recover](https://play-lh.googleusercontent.com/lgt_gLGRBLmmpHy6nuSCiC3cSk17UuDVJ0MOTpT1TNME5ifWpsBxcxdThmYsMUnKu3k)

## Description

`Recover` is a C program designed to recover JPEG images from a forensic image of a memory card (`card.raw`).
Forensic image files are bit-by-bit copies of the original storage device and are often used in digital forensics to preserve the original data without altering it.
 When JPEG files are deleted from a memory card, the data often remains intact until it's overwritten. This program scans the forensic image, identifies JPEG signatures, and extracts the JPEG files, saving them as separate `.jpg` files.

## How It Works

- **JPEG Signatures**:
  - Even though JPEGs are more complicated than BMPs, JPEGs have “signatures,” patterns of bytes that can distinguish them from other file formats. Specifically, the first three bytes of JPEGs are
    ```
    0xff 0xd8 0xff
    ```
  - From the first byte to the third byte, left to right. The fourth byte, meanwhile, is either `0xe0`, `0xe1`, `0xe2`, `0xe3`, `0xe4`, `0xe5`, `0xe6`, `0xe7`, `0xe8`, `0xe9`, `0xea`, `0xeb`, `0xec`, `0xed`, `0xee`, or `0xef`.
  - Put another way, the fourth byte’s first four bits are `1110`.

- **Finding JPEGs**:
  - Odds are, if you find this pattern of four bytes on media known to store photos (e.g., a memory card), they demarcate the start of a JPEG.
  - To be fair, you might encounter these patterns on some disk purely by chance, so data recovery isn’t an exact science.

- **Storage on Memory Cards**:
  - Digital cameras tend to store photographs contiguously on memory cards, meaning each photo is stored immediately after the previously taken photo.
  - Accordingly, the start of a JPEG usually marks the end of another.

- **FAT File System and Block Size**:
  - Digital cameras often initialize cards with a FAT file system whose “block size” is 512 bytes (B).
  - This means cameras only write to those cards in units of 512 B.
  - A photo that’s 1 MB (i.e., 1,048,576 B) thus takes up 1048576 ÷ 512 = 2048 “blocks” on a memory card.
  - But so does a photo that’s, say, one byte smaller (i.e., 1,048,575 B)! The wasted space on disk is called “slack space.”
  - Forensic investigators often look at slack space for remnants of suspicious data.

- **Recovering JPEGs**:
  - The program iterates over a copy of the memory card, looking for JPEGs’ signatures.
  - Each time it finds a signature, it opens a new file for writing and starts filling that file with bytes from the memory card, closing that file only once it encounters another signature.
  - Rather than reading the memory card’s bytes one at a time, it reads 512 of them at a time into a buffer for efficiency.
  - Thanks to FAT, JPEGs’ signatures will be “block-aligned,” meaning you only need to look for those signatures in a block’s first four bytes.
  - JPEGs can span contiguous blocks, so the program handles them accordingly.
  - Any trailing 0s (from slack space) in the recovered JPEGs do not affect their integrity and should still be viewable.

- **Forensic Image**:
  - I’ve gone ahead and created a “forensic image” of the card, storing its contents, byte after byte, in a file called `card.raw`. So that you don’t waste time iterating over millions of 0s unnecessarily, I’ve only imaged the first few megabytes of the memory card. But you should ultimately find that the image contains 50 JPEGs.

## Files

- `recover.c`: The main C source code file containing the program logic.
- `card.raw`: The forensic image of the memory card used for recovering JPEGs.

## Compilation

To compile the program, use the `clang` compiler. Open your terminal, navigate to the `recover` directory, and execute the following command:

```bash
clang -o recover recover.c
```

This command compiles `recover.c` and creates an executable named `recover`.

## Usage

After compiling, run the program by providing the forensic image (`card.raw`) as a command-line argument. Use the following syntax:

```bash
./recover card.raw
```

**Example:**

```bash
./recover card.raw
```

Upon execution, the program will scan through `card.raw`, identify JPEG signatures, and recover the JPEG files. The recovered images will be saved in the current directory with filenames in the format `###.jpg` (e.g., `000.jpg`, `001.jpg`, ..., `049.jpg`).

## Output

- **Recovered JPEG Files:** Each recovered JPEG will be saved as a separate file named with a three-digit number followed by the `.jpg` extension (e.g., `000.jpg`, `001.jpg`, etc.).

## Notes

- **Block Size:** The program reads the forensic image in blocks of 512 bytes, which is standard for FAT file systems used in memory cards.
- **JPEG Signatures:** The program looks for specific byte patterns that indicate the start of a JPEG file:
  - First three bytes: `0xff 0xd8 0xff`
  - Fourth byte: `0xe0` to `0xef` (i.e., the first four bits are `1110`)
- **File Handling:** The program ensures that each new JPEG is properly closed before opening a new one to prevent data loss.

## Error Handling

- **Incorrect Usage:** If the program is not executed with exactly one command-line argument, it will display a usage message and exit.
- **File Opening Errors:** If the forensic image (`card.raw`) cannot be opened, the program will inform the user and terminate.

## Example Workflow

1. **Navigate to the Recover Directory:**

   ```bash
   cd recover
   ```

2. **Compile the Program:**

   ```bash
   clang -o recover recover.c
   ```

3. **Run the Program:**

   ```bash
   ./recover card.raw
   ```

4. **View Recovered JPEGs:**

   After running the program, you will find the recovered JPEG files (`000.jpg`, `001.jpg`, ..., `049.jpg`) in the `recover` directory.

## License

This project is released under the [MIT License](https://opensource.org/licenses/MIT).
