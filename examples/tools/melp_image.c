/* melp_image.c — MELP için basit JPEG/PNG image wrapper
 * MELP FFI (scalar sadece) uyumlu: int/double döndürür
 * Build: gcc -O2 -shared -fPIC -o melp_image.so melp_image.c -ljpeg -lpng
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>
#include <png.h>

/* ─── Shared image handle ─────────────────────────────────── */

#define MAX_HANDLES 8

typedef struct {
    int      valid;
    int      width;
    int      height;
    int      channels;  /* 3=RGB, 4=RGBA */
    unsigned char *data; /* row-major, channels bytes per pixel */
} ImageHandle;

static ImageHandle g_handles[MAX_HANDLES];

static int alloc_handle() {
    for (int i = 0; i < MAX_HANDLES; i++) {
        if (!g_handles[i].valid) return i;
    }
    return -1;
}

/* ─── JPEG ────────────────────────────────────────────────── */

long long jpeg_open(const char *filename) {
    int h = alloc_handle();
    if (h < 0) return -1;

    FILE *fp = fopen(filename, "rb");
    if (!fp) return -1;

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, fp);
    jpeg_read_header(&cinfo, TRUE);
    cinfo.out_color_space = JCS_RGB;
    jpeg_start_decompress(&cinfo);

    int w = cinfo.output_width;
    int hh = cinfo.output_height;
    int ch = cinfo.output_components;  /* 3 for RGB */

    unsigned char *data = (unsigned char *)malloc(w * hh * ch);
    JSAMPROW row_pointer[1];
    int row = 0;
    while (cinfo.output_scanline < (unsigned)hh) {
        row_pointer[0] = data + row * w * ch;
        jpeg_read_scanlines(&cinfo, row_pointer, 1);
        row++;
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(fp);

    g_handles[h].valid    = 1;
    g_handles[h].width    = w;
    g_handles[h].height   = hh;
    g_handles[h].channels = ch;
    g_handles[h].data     = data;

    return (long long)h;
}

/* ─── PNG ─────────────────────────────────────────────────── */

long long png_open(const char *filename) {
    int h = alloc_handle();
    if (h < 0) return -1;

    FILE *fp = fopen(filename, "rb");
    if (!fp) return -1;

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) { fclose(fp); return -1; }

    png_infop info = png_create_info_struct(png);
    if (!info) { png_destroy_read_struct(&png, NULL, NULL); fclose(fp); return -1; }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        return -1;
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    int w  = png_get_image_width(png, info);
    int hh = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth  = png_get_bit_depth(png, info);

    /* Normalize to 8-bit RGBA */
    if (bit_depth == 16) png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
    if (color_type == PNG_COLOR_TYPE_RGB  ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    int ch = 4; /* RGBA after normalization */
    unsigned char *data = (unsigned char *)malloc(w * hh * ch);
    png_bytep *row_ptrs = (png_bytep *)malloc(hh * sizeof(png_bytep));
    for (int r = 0; r < hh; r++)
        row_ptrs[r] = data + r * w * ch;

    png_read_image(png, row_ptrs);
    free(row_ptrs);

    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);

    g_handles[h].valid    = 1;
    g_handles[h].width    = w;
    g_handles[h].height   = hh;
    g_handles[h].channels = ch;
    g_handles[h].data     = data;

    return (long long)h;
}

/* ─── Common accessors ───────────────────────────────────── */

long long img_width(long long handle) {
    if (handle < 0 || handle >= MAX_HANDLES || !g_handles[handle].valid) return -1;
    return g_handles[handle].width;
}

long long img_height(long long handle) {
    if (handle < 0 || handle >= MAX_HANDLES || !g_handles[handle].valid) return -1;
    return g_handles[handle].height;
}

long long img_channels(long long handle) {
    if (handle < 0 || handle >= MAX_HANDLES || !g_handles[handle].valid) return -1;
    return g_handles[handle].channels;
}

/* x=column, y=row — ch: 0=R 1=G 2=B 3=A */
long long img_pixel(long long handle, long long x, long long y, long long ch) {
    if (handle < 0 || handle >= MAX_HANDLES || !g_handles[handle].valid) return -1;
    ImageHandle *ih = &g_handles[handle];
    if (x < 0 || x >= ih->width || y < 0 || y >= ih->height) return -1;
    if (ch < 0 || ch >= ih->channels) return -1;
    return ih->data[(y * ih->width + x) * ih->channels + ch];
}

long long img_close(long long handle) {
    if (handle < 0 || handle >= MAX_HANDLES) return -1;
    if (g_handles[handle].valid) {
        free(g_handles[handle].data);
        g_handles[handle].valid = 0;
        g_handles[handle].data  = NULL;
    }
    return 0;
}
