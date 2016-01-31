#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <setjmp.h>

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
	char *jpeg_filename = argv[2];

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	FILE *in_file;
	if((in_file = fopen(jpeg_filename, "rb")) == NULL) {
		fprintf(stderr, "Couldn't open file %s for decompression\n", jpeg_filename);
		exit(1);
	}

	jpeg_stdio_src(&cinfo, in_file);
	jpeg_read_header(&cinfo, TRUE);

	jvirt_barray_ptr *dct = jpeg_read_coefficients(&cinfo);

	JDIMENSION width = cinfo.output_width;
	JDIMENSION height = cinfo.output_height;

	JDIMENSION rows = height / 8;

	// do stuff
	JBLOCKARRAY buf = (cinfo.mem->access_virt_barray)((j_common_ptr)&cinfo, *dct, 0, 1, 0);

	/* NOTE: figure out what each block in array means. We know that block array is 128x128,
	         with each block having 64 values (one entire DCT coefficient table)
	         This means that the array contains 128^2 8x8 blocks.
	         What we need to figure out is which elements of the array correspond to which channel
	         In most JPEGs, the channels are YCbCr. 

	         We are only interested in modifying the Y (luminance) channel.
	*/

	jpeg_finish_decompress(&cinfo);

	return 0;

}