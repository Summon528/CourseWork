#pragma GCC optimization("O3")

#include <CL/cl.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <chrono>
#include <cstring>
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
    void *map;
    size_t map_len;
} Image;

#ifdef DEBUG
#define CL_ERROR_GUARD                                                     \
    if (status != CL_SUCCESS) {                                            \
        fprintf(stderr, "*** line %d failed with error '%d'.\n", __LINE__, \
                status);                                                   \
        abort();                                                           \
    }
#else
#define CL_ERROR_GUARD ;
#endif

const char *histogram =
    "\
    typedef struct {\n\
        uchar R;\n\
        uchar G;\n\
        uchar B;\n\
        uchar align;\n\
    } RGB4;\n\
    typedef struct {\n\
        uchar R;\n\
        uchar G;\n\
        uchar B;\n\
    } RGB3;\n\
    __kernel void\n\
    histogram(__global RGB3* rgb3data,\n\
              __global RGB4* rgb4data,\n\
              __global uint* R,\n\
              __global uint* G,\n\
              __global uint* B) {\n\
    int idx = get_global_id(0);\n\
    if (rgb4data != NULL) {\n\
        RGB4 pixel = rgb4data[idx];\n\
        atomic_inc(&R[pixel.R]);\n\
        atomic_inc(&G[pixel.G]);\n\
        atomic_inc(&B[pixel.B]);\n\
    } else {\n\
        RGB3 pixel = rgb3data[idx];\n\
        atomic_inc(&R[pixel.R]);\n\
        atomic_inc(&G[pixel.G]);\n\
        atomic_inc(&B[pixel.B]);\n\
    }\n\
}";

Image *readbmp(const char *filename) {
    int fd = open(filename, O_RDONLY, 0x644);
    struct stat file_stat;
    fstat(fd, &file_stat);
    Image *ret = new Image();
    ret->map_len = file_stat.st_size;
    ret->map = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    char *map = (char *)ret->map;
    uint32_t size = *(int *)&map[2];
    uint32_t offset = *(int *)&map[10];
    uint32_t w = *(int *)&map[18];
    uint32_t h = *(int *)&map[22];
    uint16_t depth = *(uint16_t *)&map[28];
    if (depth != 24 && depth != 32) {
        printf("we don't suppot depth with %d\n", depth);
        exit(0);
    }

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

int main(int argc, char *argv[]) {
    cl_int status = 0;
    cl_platform_id platform;
    status = clGetPlatformIDs(1, &platform, NULL);
    CL_ERROR_GUARD

    cl_device_id device;
    status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    CL_ERROR_GUARD

    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &status);
    CL_ERROR_GUARD

    cl_command_queue queue =
        clCreateCommandQueueWithProperties(context, device, NULL, &status);
    CL_ERROR_GUARD

    size_t srcLen = strlen(histogram);
    cl_program program = clCreateProgramWithSource(
        context, 1, (const char **)&histogram, &srcLen, &status);
    CL_ERROR_GUARD

    status = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    CL_ERROR_GUARD

    cl_kernel kernel = clCreateKernel(program, "histogram", &status);
    CL_ERROR_GUARD

    char *filename;
    if (argc >= 2) {
        int many_img = argc - 1;
        for (int i = 0; i < many_img; i++) {
            filename = argv[i + 1];
            Image *img = readbmp(filename);

            std::cout << img->weight << ":" << img->height << "\n";

            uint32_t R[256] = {};
            uint32_t G[256] = {};
            uint32_t B[256] = {};

            size_t img_size = img->size;
            cl_mem_flags buf_flag = CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR;
            cl_mem clData;
            if (img->type == 1) {
                clData =
                    clCreateBuffer(context, buf_flag, img_size * sizeof(RGB4),
                                   img->data.rgb4, &status);
            } else {
                clData =
                    clCreateBuffer(context, buf_flag, img_size * sizeof(RGB3),
                                   img->data.rgb3, &status);
            }
            CL_ERROR_GUARD

            size_t buf_size = sizeof(uint32_t) * 256;
            cl_mem clR =
                clCreateBuffer(context, buf_flag, buf_size, &R, &status);
            CL_ERROR_GUARD

            cl_mem clB =
                clCreateBuffer(context, buf_flag, buf_size, &B, &status);
            CL_ERROR_GUARD

            cl_mem clG =
                clCreateBuffer(context, buf_flag, buf_size, &G, &status);
            CL_ERROR_GUARD

            if (img->type == 0) {
                status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &clData);
            } else {
                status = clSetKernelArg(kernel, 0, sizeof(cl_mem), NULL);
            }
            CL_ERROR_GUARD

            if (img->type == 0) {
                status = clSetKernelArg(kernel, 1, sizeof(cl_mem), NULL);
            } else {
                status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &clData);
            }
            CL_ERROR_GUARD

            status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &clR);
            CL_ERROR_GUARD

            status = clSetKernelArg(kernel, 3, sizeof(cl_mem), &clG);
            CL_ERROR_GUARD

            status = clSetKernelArg(kernel, 4, sizeof(cl_mem), &clB);
            CL_ERROR_GUARD

            status = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &img_size,
                                            NULL, 0, NULL, NULL);
            CL_ERROR_GUARD
            clFinish(queue);

            status = 0;
            status = clEnqueueReadBuffer(queue, clR, CL_FALSE, 0, buf_size, &R,
                                         0, NULL, NULL);
            CL_ERROR_GUARD

            status = clEnqueueReadBuffer(queue, clG, CL_FALSE, 0, buf_size, &G,
                                         0, NULL, NULL);
            CL_ERROR_GUARD

            status = clEnqueueReadBuffer(queue, clB, CL_FALSE, 0, buf_size, &B,
                                         0, NULL, NULL);
            CL_ERROR_GUARD
            clFinish(queue);

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
            ret->data.rgb4 = new RGB4[256 * 256]{};

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

            std::string newfile = "hist_" + std::string(filename);
            writebmp(newfile.c_str(), ret);

            delete ret->data.rgb4;
            delete ret;
            munmap(img->map, img->map_len);
            delete img;
            clReleaseMemObject(clData);
            clReleaseMemObject(clR);
            clReleaseMemObject(clG);
            clReleaseMemObject(clB);
        }
    } else {
        printf("Usage: ./hist <img.bmp> [img2.bmp ...]\n");
    }
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    return 0;
}
