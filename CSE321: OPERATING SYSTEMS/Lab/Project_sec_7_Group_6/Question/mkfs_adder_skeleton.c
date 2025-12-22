#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <time.h>
#include <assert.h>

#define BS 4096u
#define INODE_SIZE 128u
#define ROOT_INO 1u
#define DIRECT_MAX 12

#pragma pack(push, 1)
typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t block_size;
    uint64_t total_blocks;
    uint64_t inode_count;
    uint64_t inode_bitmap_start;
    uint64_t inode_bitmap_blocks;
    uint64_t data_bitmap_start;
    uint64_t data_bitmap_blocks;
    uint64_t inode_table_start;
    uint64_t inode_table_blocks;
    uint64_t data_region_start;
    uint64_t data_region_blocks;
    uint64_t root_inode;
    uint64_t mtime_epoch;
    uint32_t flags;
    uint32_t checksum;
} superblock_t;
#pragma pack(pop)
_Static_assert(sizeof(superblock_t) == 116, "superblock must fit in one block");

#pragma pack(push,1)
typedef struct {
    uint16_t mode;
    uint16_t links;
    uint32_t uid;
    uint32_t gid;
    uint64_t size_bytes;
    uint64_t atime;
    uint64_t mtime;
    uint64_t ctime;
    uint32_t direct[DIRECT_MAX];
    uint32_t reserved_0;
    uint32_t reserved_1;
    uint32_t reserved_2;
    uint32_t proj_id;
    uint32_t uid16_gid16;
    uint64_t xattr_ptr;
    uint64_t inode_crc;
} inode_t;
#pragma pack(pop)
_Static_assert(sizeof(inode_t)==INODE_SIZE, "inode size mismatch");

#pragma pack(push,1)
typedef struct {
    uint32_t inode_no;
    uint8_t type;
    char name[58];
    uint8_t checksum;
} dirent64_t;
#pragma pack(pop)
_Static_assert(sizeof(dirent64_t)==64, "dirent size mismatch");

// ==========================CRC32====================================
uint32_t CRC32_TAB[256];
void crc32_init(void){
    for (uint32_t i=0;i<256;i++){
        uint32_t c=i;
        for(int j=0;j<8;j++) c = (c&1)?(0xEDB88320u^(c>>1)):(c>>1);
        CRC32_TAB[i]=c;
    }
}
uint32_t crc32(const void* data, size_t n){
    const uint8_t* p=(const uint8_t*)data; uint32_t c=0xFFFFFFFFu;
    for(size_t i=0;i<n;i++) c = CRC32_TAB[(c^p[i])&0xFF] ^ (c>>8);
    return c ^ 0xFFFFFFFFu;
}
static uint32_t superblock_crc_finalize(superblock_t *sb) {
    sb->checksum = 0;
    uint32_t s = crc32((void *) sb, BS - 4);
    sb->checksum = s;
    return s;
}
void inode_crc_finalize(inode_t* ino){
    uint8_t tmp[INODE_SIZE]; memcpy(tmp, ino, INODE_SIZE);
    memset(&tmp[120], 0, 8);
    uint32_t c = crc32(tmp, 120);
    ino->inode_crc = (uint64_t)c;
}
void dirent_checksum_finalize(dirent64_t* de) {
    const uint8_t* p = (const uint8_t*)de;
    uint8_t x = 0;
    for (int i = 0; i < 63; i++) x ^= p[i];
    de->checksum = x;
}

// ==========================UTILITY FUNCTIONS=========================
void print_usage() {
    fprintf(stderr, "Usage: mkfs_adder --input <out.img> --output <out2.img> --file <file>\n");
}
int find_free_inode(const uint8_t* inode_bitmap, uint64_t inode_count) {
    for (uint64_t i = 0; i < inode_count; ++i) {
        if (!((inode_bitmap[i / 8] >> (i % 8)) & 1)) {
            return i + 1;
        }
    }
    return -1;
}
int find_free_data_block(const uint8_t* data_bitmap, uint64_t data_region_blocks) {
    for (uint64_t i = 0; i < data_region_blocks; ++i) {
        if (!((data_bitmap[i / 8] >> (i % 8)) & 1)) {
            return i;
        }
    }
    return -1;
}

// ==========================MAIN LOGIC=========================
int main(int argc, char *argv[]) {
    crc32_init();

    char* input_image_path = NULL;
    char* output_image_path = NULL;
    char* file_to_add_path = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--input") == 0) {
            if (++i < argc) input_image_path = argv[i];
            else { fprintf(stderr, "Error: --input requires a value.\n"); print_usage(); return EXIT_FAILURE; }
        } else if (strcmp(argv[i], "--output") == 0) {
            if (++i < argc) output_image_path = argv[i];
            else { fprintf(stderr, "Error: --output requires a value.\n"); print_usage(); return EXIT_FAILURE; }
        } else if (strcmp(argv[i], "--file") == 0) {
            if (++i < argc) file_to_add_path = argv[i];
            else { fprintf(stderr, "Error: --file requires a value.\n"); print_usage(); return EXIT_FAILURE; }
        } else {
            fprintf(stderr, "Error: Unknown argument '%s'.\n", argv[i]);
            print_usage(); return EXIT_FAILURE;
        }
    }

    if (!input_image_path || !output_image_path || !file_to_add_path) {
        fprintf(stderr, "Error: All required arguments must be provided.\n");
        print_usage(); return EXIT_FAILURE;
    }

    FILE* input_fp = fopen(input_image_path, "rb");
    if (!input_fp) {
        fprintf(stderr, "Error: Could not open input file '%s': %s\n", input_image_path, strerror(errno));
        return EXIT_FAILURE;
    }
    fseek(input_fp, 0, SEEK_END);
    size_t image_size = ftell(input_fp);
    fseek(input_fp, 0, SEEK_SET);

    uint8_t* disk_image = (uint8_t*)malloc(image_size);
    if (!disk_image) { fclose(input_fp); return EXIT_FAILURE; }
    if (fread(disk_image, 1, image_size, input_fp) != image_size) {
        fprintf(stderr, "Error: Failed to read the complete disk image.\n");
        free(disk_image); fclose(input_fp); return EXIT_FAILURE;
    }
    fclose(input_fp);

    FILE* file_to_add_fp = fopen(file_to_add_path, "rb");
    if (!file_to_add_fp) {
        fprintf(stderr, "Error: Could not open file to add '%s': %s\n", file_to_add_path, strerror(errno));
        free(disk_image); return EXIT_FAILURE;
    }
    fseek(file_to_add_fp, 0, SEEK_END);
    size_t file_size = ftell(file_to_add_fp);
    fseek(file_to_add_fp, 0, SEEK_SET);

    uint8_t* file_data = (uint8_t*)malloc(file_size);
    if (!file_data) { free(disk_image); fclose(file_to_add_fp); return EXIT_FAILURE; }
    if (fread(file_data, 1, file_size, file_to_add_fp) != file_size) {
        fprintf(stderr, "Error: Failed to read the complete file data.\n");
        free(disk_image); free(file_data); fclose(file_to_add_fp); return EXIT_FAILURE;
    }
    fclose(file_to_add_fp);

    superblock_t* sb = (superblock_t*)disk_image;
    uint8_t* inode_bitmap = disk_image + (sb->inode_bitmap_start * BS);
    uint8_t* data_bitmap = disk_image + (sb->data_bitmap_start * BS);
    inode_t* inode_table = (inode_t*)(disk_image + (sb->inode_table_start * BS));

    int new_inode_no = find_free_inode(inode_bitmap, sb->inode_count);
    if (new_inode_no == -1) {
        fprintf(stderr, "Error: No free inodes available.\n");
        free(disk_image); free(file_data); return EXIT_FAILURE;
    }

    uint64_t required_blocks = (file_size + BS - 1) / BS;
    if (required_blocks > DIRECT_MAX) {
        fprintf(stderr, "Warning: File too large, truncating to %d blocks.\n", DIRECT_MAX);
        required_blocks = DIRECT_MAX;
    }

    uint32_t data_blocks[DIRECT_MAX] = {0};
    for(uint64_t i = 0; i < required_blocks; ++i) {
        int free_data_block = find_free_data_block(data_bitmap, sb->data_region_blocks);
        if (free_data_block == -1) {
            fprintf(stderr, "Error: Not enough free data blocks.\n");
            free(disk_image); free(file_data); return EXIT_FAILURE;
        }
        data_bitmap[free_data_block / 8] |= (1u << (free_data_block % 8));
        data_blocks[i] = sb->data_region_start + free_data_block; // store absolute block number

        uint8_t* data_block_ptr = disk_image + (uint64_t)data_blocks[i] * BS;
        size_t bytes_to_copy = (i == required_blocks - 1) ? (file_size - i*BS) : BS;
        memcpy(data_block_ptr, file_data + i*BS, bytes_to_copy);
    }

    inode_bitmap[(new_inode_no - 1) / 8] |= (1u << ((new_inode_no - 1) % 8));

    inode_t* new_inode = &inode_table[new_inode_no - 1];
    time_t now = time(NULL);

    memset(new_inode, 0, INODE_SIZE);
    new_inode->mode = 0100000u; // regular file
    new_inode->links = 1;
    new_inode->uid = 0;
    new_inode->gid = 0;
    new_inode->size_bytes = file_size;
    new_inode->atime = now;
    new_inode->mtime = now;
    new_inode->ctime = now;
    memcpy(new_inode->direct, data_blocks, sizeof(uint32_t) * DIRECT_MAX);
    inode_crc_finalize(new_inode);

    inode_t* root_inode = &inode_table[ROOT_INO - 1];
    uint8_t* root_data_block = disk_image + (uint64_t)root_inode->direct[0] * BS;

    dirent64_t* new_dir_entry = NULL;
    for (size_t i = 0; i < BS / sizeof(dirent64_t); ++i) {
        if (((dirent64_t*)root_data_block)[i].inode_no == 0) {
            new_dir_entry = &((dirent64_t*)root_data_block)[i];
            break;
        }
    }
    if (!new_dir_entry) {
        fprintf(stderr, "Error: Root directory full.\n");
        free(disk_image); free(file_data); return EXIT_FAILURE;
    }

    new_dir_entry->inode_no = new_inode_no;
    new_dir_entry->type = 1;
    memset(new_dir_entry->name, 0, 58);
    const char* filename = strrchr(file_to_add_path, '/');
    filename = filename ? filename + 1 : file_to_add_path;
    strncpy(new_dir_entry->name, filename, 58);
    new_dir_entry->name[57] = '\0';
    dirent_checksum_finalize(new_dir_entry);

    root_inode->size_bytes += sizeof(dirent64_t);
    root_inode->links++;
    root_inode->mtime = now;
    inode_crc_finalize(root_inode);

    superblock_crc_finalize(sb);

    FILE* output_fp = fopen(output_image_path, "wb");
    if (!output_fp) {
        fprintf(stderr, "Error: Could not open output file '%s': %s\n", output_image_path, strerror(errno));
        free(disk_image); free(file_data); return EXIT_FAILURE;
    }
    if (fwrite(disk_image, 1, image_size, output_fp) != image_size) {
        fprintf(stderr, "Error: Failed to write output image.\n");
        fclose(output_fp); free(disk_image); free(file_data); return EXIT_FAILURE;
    }
    fclose(output_fp); free(disk_image); free(file_data);

    printf("Successfully added file '%s' to '%s'. Output saved as '%s'.\n",
            file_to_add_path, input_image_path, output_image_path);

    return EXIT_SUCCESS;
}


