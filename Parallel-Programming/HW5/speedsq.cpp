#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <chrono>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>

typedef struct {
    uint8_t R;
    uint8_t G;
    uint8_t B;
} RGB3;

typedef struct {
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t align;
} RGB4;

typedef union {
    RGB3 *rgb3;
    RGB4 *rgb4;
} RGB;

typedef struct {
    bool type;
    uint32_t size;
    uint32_t height;
    uint32_t weight;
    RGB data;
} Image;

class Timer {
   public:
    void start() {
        m_StartTime = std::chrono::system_clock::now();
        m_bRunning = true;
    }

    void stop() {
        m_EndTime = std::chrono::system_clock::now();
        m_bRunning = false;
    }

    double elapsedMilliseconds() {
        std::chrono::time_point<std::chrono::system_clock> endTime;

        if (m_bRunning) {
            endTime = std::chrono::system_clock::now();
        } else {
            endTime = m_EndTime;
        }

        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   endTime - m_StartTime)
            .count();
    }

    double elapsedSeconds() { return elapsedMilliseconds() / 1000.0; }

   private:
    std::chrono::time_point<std::chrono::system_clock> m_StartTime;
    std::chrono::time_point<std::chrono::system_clock> m_EndTime;
    bool m_bRunning = false;
};

Image *readbmp(const char *filename) {
    int fd = open(filename, O_RDONLY, 0x644);
    struct stat file_stat;
    fstat(fd, &file_stat);
    char *map =
        (char *)mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    uint32_t size = *(int *)&map[2];
    uint32_t offset = *(int *)&map[10];
    uint32_t w = *(int *)&map[18];
    uint32_t h = *(int *)&map[22];
    uint16_t depth = *(uint16_t *)&map[28];
    if (depth != 24 && depth != 32) {
        printf("we don't suppot depth with %d\n", depth);
        exit(0);
    }

    Image *ret = new Image();
    ret->height = h;
    ret->weight = w;
    ret->size = w * h;

    if (depth == 32) {
        ret->type = 1;
        ret->data.rgb4 = (RGB4 *)(map + offset);
    } else {
        ret->type = 0;
        ret->data.rgb3 = (RGB3 *)(map + offset);
    }

    return ret;
}

int writebmp(const char *filename, Image *img) {
    uint8_t header[54] = {
        0x42,           // identity : B
        0x4d,           // identity : M
        0,    0, 0, 0,  // file size
        0,    0,        // reserved1
        0,    0,        // reserved2
        54,   0, 0, 0,  // RGB data offset
        40,   0, 0, 0,  // struct BITMAPINFOHEADER size
        0,    0, 0, 0,  // bmp width
        0,    0, 0, 0,  // bmp height
        1,    0,        // planes
        32,   0,        // bit per pixel
        0,    0, 0, 0,  // compression
        0,    0, 0, 0,  // data size
        0,    0, 0, 0,  // h resolution
        0,    0, 0, 0,  // v resolution
        0,    0, 0, 0,  // used colors
        0,    0, 0, 0   // important colors
    };

    // file size
    uint32_t file_size = img->size * 4 + 54;
    header[2] = (unsigned char)(file_size & 0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;

    // width
    uint32_t width = img->weight;
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) & 0x000000ff;
    header[20] = (width >> 16) & 0x000000ff;
    header[21] = (width >> 24) & 0x000000ff;

    // height
    uint32_t height = img->height;
    header[22] = height & 0x000000ff;
    header[23] = (height >> 8) & 0x000000ff;
    header[24] = (height >> 16) & 0x000000ff;
    header[25] = (height >> 24) & 0x000000ff;

    std::ofstream fout;
    fout.open(filename, std::ios::binary);
    fout.write((char *)header, 54);
    fout.write((char *)img->data.rgb4, img->size * 4);
    fout.close();
}

void histogram(Image *img, uint32_t R[256], uint32_t G[256], uint32_t B[256]) {
    std::fill(R, R + 256, 0);
    std::fill(G, G + 256, 0);
    std::fill(B, B + 256, 0);

    for (int i = 0; i < img->size; i++) {
        if (img->type == 0) {
            RGB3 &pixel = img->data.rgb3[i];
            R[pixel.R]++;
            G[pixel.G]++;
            B[pixel.B]++;
        } else if (img->type == 1) {
            RGB4 &pixel = img->data.rgb4[i];
            R[pixel.R]++;
            G[pixel.G]++;
            B[pixel.B]++;
        }
    }
}

int main(int argc, char *argv[]) {
    char *filename;
    if (argc >= 2) {
        int many_img = argc - 1;
        for (int i = 0; i < many_img; i++) {
            Timer timer;

            filename = argv[i + 1];
            timer.start();
            Image *img = readbmp(filename);
            timer.stop();
            std::cout << "Seconds: " << timer.elapsedSeconds() << std::endl;

            std::cout << img->weight << ":" << img->height << "\n";

            uint32_t R[256];
            uint32_t G[256];
            uint32_t B[256];

            timer.start();
            histogram(img, R, G, B);
            timer.stop();
            std::cout << "Seconds: " << timer.elapsedSeconds() << std::endl;

            timer.start();
            uint32_t max = 0;
            for (int i = 0; i < 256; i++) {
                max = R[i] > max ? R[i] : max;
                max = G[i] > max ? G[i] : max;
                max = B[i] > max ? B[i] : max;
            }

            Image *ret = new Image();
            ret->type = 1;
            ret->height = 256;
            ret->weight = 256;
            ret->size = 256 * 256;
            ret->data.rgb4 = new RGB4[256 * 256];

            for (int i = 0; i < ret->height; i++) {
                for (int j = 0; j < 256; j++) {
                    if (R[j] * 256 / max > i)
                        ret->data.rgb4[256 * i + j].R = 255;
                    if (G[j] * 256 / max > i)
                        ret->data.rgb4[256 * i + j].G = 255;
                    if (B[j] * 256 / max > i)
                        ret->data.rgb4[256 * i + j].B = 255;
                }
            }

            std::cout << (int)ret->data.rgb4[0].R << (int)ret->data.rgb4[0].G
                      << (int)ret->data.rgb4[0].B << std::endl;
            std::string newfile = "hist_" + std::string(filename);
            writebmp(newfile.c_str(), ret);
            timer.stop();
            std::cout << "Seconds: " << timer.elapsedSeconds() << std::endl;
        }
    } else {
        printf("Usage: ./hist <img.bmp> [img2.bmp ...]\n");
    }
    return 0;
}
