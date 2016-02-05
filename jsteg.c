#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <setjmp.h>

int encode_data(j_decompress_ptr srcinfo, j_compress_ptr dstinfo, 
    JDIMENSION x_crop_offset, jvirt_barray_ptr *src_coef_arrays){

    /**
      * So to change a single dct, you can use the following simple code: 
      * To access any dct coeff, you need to change four index, cx, bx, by, bi. 
      */
    return 0;
}

/** My attempt at implementing JSteg from scratch
  * Note: try using this to understand and access the DCT coefficients:
  * http://stackoverflow.com/questions/16186717/changing-dct-coefficients
  */

int main(int argc, char **argv) {
    if(argc != 4) {
        printf("Usage: ./jsteg <input file> <source jpeg> <destination jpeg>\n");
        printf("Invalid argument count. Expected 3 found %d\n", argc-1);
        exit(1);
    }

    char *in_filename = argv[1];
    char *src_jpg_name = argv[2];
    char *dst_jpg_name = argv[3];

    // allocate structures
    struct jpeg_decompress_struct src_cinfo;
    struct jpeg_compress_struct dst_cinfo;
    struct jpeg_error_mgr jsrcerr, jdsterr;

    // Initialize the JPEG decompression object with default error handling
    src_cinfo.err = jpeg_std_error(&jsrcerr);
    jpeg_create_decompress(&src_cinfo);

    // Initialize the JPEG comression object with default error handling
    dst_cinfo.err = jpeg_std_error(&jdsterr);
    jpeg_create_compress(&dst_cinfo);

    // open the source file
    FILE *fp;
    if((fp = fopen(src_jpg_name, "rb")) == NULL) {
        fprintf(stderr, "Couldn't open file %s for decompression\n", src_jpg_name);
        exit(1);
    }

    jpeg_stdio_src(&src_cinfo, fp);
    jpeg_read_header(&src_cinfo, TRUE);

    jvirt_barray_ptr *dct = jpeg_read_coefficients(&src_cinfo);

    JDIMENSION width = src_cinfo.output_width;
    JDIMENSION height = src_cinfo.output_height;

    JDIMENSION rows = height / 8;

    // do stuff
    JBLOCKARRAY buf = (src_cinfo.mem->access_virt_barray)((j_common_ptr)&src_cinfo, *dct, 0, 1, 0);

    /* NOTE: figure out what each block in array means. We know that block array is 128x128,
             with each block having 64 values (one entire DCT coefficient table)
             This means that the array contains 128^2 8x8 blocks.
             What we need to figure out is which elements of the array correspond to which channel
             In most JPEGs, the channels are YCbCr. 

             We are only interested in modifying the Y (luminance) channel.
    */

    jpeg_finish_decompress(&src_cinfo);


    fclose(fp);

    // Open destination file
    if((fp = fopen(src_jpg_name, "wb")) == NULL) {
        fprintf(stderr, "Couldn't open target JPEG file %s\n", src_jpg_name);
        exit(1);
    }

    // do stuff



    fclose(fp);

    return 0;

}