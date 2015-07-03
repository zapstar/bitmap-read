#include <stdio.h>

//Unsigned 32-bit data type
#ifndef ubit32_t
#define ubit32_t unsigned int
#endif
//NOTE: Change unsigned int to whatever 32-bit
//data type which is unsigned (only if needed)
//Change all the data types to whatever needed

//Unsigned 16-bit data type
#ifndef ubit16_t
#define ubit16_t unsigned short
#endif

#ifndef ubit8_t
#define ubit8_t unsigned char
#endif

//Signed 32-bit data type
#ifndef sbit32_t
#define sbit32_t signed int
#endif

//Signed 16-bit data type
#ifndef sbit16_t
#define sbit16_t signed short
#endif

//Signed 8-bit data type
#ifndef sbit8_t
#define sbit8_t signed char
#endif

//Unsigned 32-bit byte manipulation union
union my_u32_union
{
	ubit32_t dword;
	ubit8_t byte[4];
};

//Unsigned 16-bit byte manipulation union
union my_u16_union
{
	ubit16_t word;
	ubit8_t byte[2];
};

//Signed 32-bit byte manipulation union
union my_s32_union
{
	sbit32_t dword;
	ubit8_t byte[4];
};

//Signed 16-bit byte manipulation union
union my_s16_union
{
	sbit16_t word;
	ubit8_t byte[4];
};

//BMP File Magic Structure
struct bmpfile_magic
{
	ubit8_t magic[2];
};

struct bmpfile_header
{
	ubit32_t filesz;
	ubit16_t creator1;
	ubit16_t creator2;
	ubit32_t bmp_offset;
};

struct dib_header
{
	ubit32_t header_sz;
	sbit32_t width;
	sbit32_t height;
	ubit16_t nplanes;
	ubit16_t bitspp;
	ubit32_t compress_type;
	ubit32_t bmp_bytesz;
	sbit32_t hres;
	sbit32_t vres;
	ubit32_t ncolors;
	ubit32_t nimpcolors;
};

//Block read data type declration
union my_u32_union temp_u32;
union my_u16_union temp_u16;
union my_s32_union temp_s32;
union my_s16_union temp_s16;

//BMP HEADER data types declaration
struct bmpfile_magic my_bmpfile_magic;
struct bmpfile_header  my_bmpfile_header;
struct dib_header my_dib_header;

void clear_unions()
{
	temp_u32.dword = 0;
	temp_s32.dword = 0;
	temp_u16.word = 0;
	temp_s16.word = 0;
}

void get_bmp_magic(FILE *fp, struct bmpfile_magic* bmp_magic)
{
	//Iterator variable
	unsigned int i;

	//Go to the correct FP offset
	fseek(fp,0L,SEEK_SET);

	//Get the Bitmap Magic Data
	for(i=0;i<2;i++)
		bmp_magic->magic[i] = (ubit8_t) getc(fp);

	//Return the FP offset to the start of file
	rewind(fp);
}

void get_bmp_header(FILE *fp, struct bmpfile_header* header)
{
	//Iterator variable
	unsigned int i;

	//Go to the correct FP offset
	fseek(fp,0x2L,SEEK_SET);
	clear_unions();

	//Bitmap Filesize retrieval start
	for(i=0;i<4;i++)
		temp_u32.byte[i] = (ubit8_t) getc(fp);
	header->filesz = temp_u32.dword;
	clear_unions();
	//Bitmap Filesize retrieval end
	
	//Bitmap File Creator 1 retrieval start
	for(i=0;i<2;i++)
		temp_u16.byte[i] = (ubit8_t) getc(fp);
	header->creator1 = temp_u16.word;
	clear_unions();
	//Bitmap File Creator 1 retrieval end

	//Bitamp File Creator 2 retrieval start
	for(i=0;i<2;i++)
		temp_u16.byte[i] = (ubit8_t) getc(fp);
	header->creator2 = temp_u16.word;
	clear_unions();
	//Bitamp File Creator 2 retrieval end

	//Bitmap Pixel Offset retrieval ubit8_t
	for(i=0;i<4;i++)
		temp_u32.byte[i] = (ubit8_t) getc(fp);
	header->bmp_offset = temp_u32.dword;
	clear_unions();
	//Bitmap Pixel Offset retrieval end

	//Return the FP offset to the start of the file
	rewind(fp);
}

void get_dib_header(FILE *fp,struct dib_header* header)
{
	//Iterator variables
	unsigned int i;

	//Set the File Pointer to tha appropriate offset
	fseek(fp,0xEL,SEEK_SET);
	clear_unions();

	//DIB Header Length retrieval start
	for(i=0;i<4;i++)
		temp_u32.byte[i] = (ubit8_t) getc(fp);
	header->header_sz = temp_u32.dword;
	//DIB Header Length retrieval end

	//Bitmap width in pixels retrieval start
	for(i=0;i<4;i++)
		temp_s32.byte[i] = (ubit8_t) getc(fp);
	header->width = temp_s32.dword;
	clear_unions();
	//Bitmap width in pixels retrieval end

	//Bitmap height in pixels retrieval start
	for(i=0;i<4;i++)
		temp_s32.byte[i] = (ubit8_t) getc(fp);
	header->height = temp_s32.dword;
	clear_unions();
	//Bitmap height in pixels retrieval end

	//No of color planes retrieval start
	for(i=0;i<2;i++)
		temp_u16.byte[i] = (ubit8_t) getc(fp);
	header->nplanes = temp_u16.word;
	clear_unions();
	//No of color planes retrieval end

	//No of bits per pixel retrieval start
	for(i=0;i<2;i++)
		temp_u16.byte[i] = (ubit8_t) getc(fp);
	header->bitspp = temp_u16.word;
	clear_unions();
	//No of bits per pixel retrieval end

	//Compression Type retrieval start
	for(i=0;i<4;i++)
		temp_u32.byte[i] = (ubit8_t) getc(fp);
	header->compress_type = temp_u32.dword;
	clear_unions();
	//Compression Type retrieval end

	//Raw bitmap Data size retrieval start
	for(i=0;i<4;i++)
		temp_u32.byte[i] = (ubit8_t) getc(fp);
	header->bmp_bytesz = temp_u32.dword;
	clear_unions();
	//Raw bitmap Data size retrieval end

	//Horizontal resolution in pixels per meter retrieval start
	for(i=0;i<4;i++)
		temp_s32.byte[i] = (ubit8_t) getc(fp);
	header->hres = temp_s32.dword;
	clear_unions();
	//Horizontal resolution in pixels per meter retrieval end

	//Vertical resolution in pixels per meter retrieval start
	for(i=0;i<4;i++)
		temp_s32.byte[i] = (ubit8_t) getc(fp);
	header->vres = temp_s32.dword;
	clear_unions();
	//Vertical resolution in pixels per meter retrieval end

	//No. of colors in palette retrieval start
	for(i=0;i<4;i++)
		temp_u32.byte[i] = (ubit8_t) getc(fp);
	header->ncolors = temp_u32.dword;
	clear_unions();
	//No. of colors in palette retrieval end

	//No. of important colors retrieval start
	for(i=0;i<4;i++)
		temp_u32.byte[i] = (ubit8_t) getc(fp);
	header->nimpcolors = temp_u32.dword;
	clear_unions();
	//No. of important colors retrieval end

	//Reset the File Pointer to the start of file
	rewind(fp);
}

void print_compression_type(struct dib_header* header)
{
	ubit32_t c_type = header->compress_type;
	switch(c_type)
	{
	case 0: printf("None\n");
		break;
	case 1: printf("RLE-8bit/pixel\n");
		break;
	case 2: printf("RLE-4bit/pixel\n");
		break;
	case 3: printf("Bit Field or Huffman 1D\n");
		break;
	case 4: printf("JPEG or RLE-24\n");
		break;
	case 5: printf("PNG\n");
		break;
	case 6: printf("Bit Field\n");
		break;
	default: printf("Error while recogonizing the Compression Type.\n");
		break;

	}
}

void print_pixel_data(FILE *fp)
{
	//Iterator variable
	unsigned int i;

	//Picture height and width variables
	sbit32_t height,width;

	//Pixel Data Offset
	ubit32_t pixel_offset;

	//No of bits per pixel
	ubit16_t bits_pp;

	//No of bytes per pixels
	ubit16_t bytes_pp;

	//Bytes per pixel iterator
	ubit16_t j;
	
	//Picture traversal iterators
	sbit32_t x,y;

	//Go to where you can retrieve height and width of picture
	fseek(fp,0x12L,SEEK_SET);
	clear_unions();

	//Retrieve picture width in pixels
	for(i=0;i<4;i++)
		temp_s32.byte[i] = (ubit8_t) getc(fp);
	width = temp_s32.dword;
	clear_unions();

	//Retrieve picture height in pixels
	for(i=0;i<4;i++)
		temp_s32.byte[i] = (ubit8_t) getc(fp);
	height = temp_s32.dword;
	clear_unions();

	//Find the starting address of the pixel data
	fseek(fp,0xAL,SEEK_SET);
	for(i=0;i<4;i++)
		temp_u32.byte[i] = (ubit8_t) getc(fp);
	pixel_offset = temp_u32.dword;
	clear_unions();

	//Find the number of byptes per pixel
	fseek(fp,0x1CL,SEEK_SET);
	for(i=0;i<2;i++)
		temp_u16.byte[i] = (ubit8_t) getc(fp);
	bits_pp = temp_u16.word;
	clear_unions();
	bytes_pp = bits_pp/8;
	
	//Go to the starting address of the pixel data
	fseek(fp,(long) pixel_offset,SEEK_SET);
	
	//Print the pixel data using picture height and width
	printf("PIXEL DATA from Bottom Left to Top Right\n\n");
	for(x=0;x<height;x++)
	{
		for(y=0;y<width;y++)
		{
			printf("<\t%d,\t%d> [ ",x,y);
			for(j=0;j<bytes_pp;j++)
			{
				printf("%#04x ",(ubit8_t) getc(fp));
			}
			printf("]\n");
		}
	}
	//Return the File pointer to start of file
	rewind(fp);
}

int main(int argc,char **argv)
{
	//File operation data types declration
	FILE *fp;

	//If no command line argument, display usage
	if(argc != 2)
	{
		printf("Usage: <program name> <BMP File path>.\n");
		return -1;
	}
	
	//Open the file
	fp = fopen(argv[1],"r");

	if(fp == NULL)
	{
		printf("Error: %s could not be opened.\n",argv[1]);
		return -1;
	}
	
	printf("**** BMP FILE START ****\n\n");


	printf("**** BMP HEADER START ****\n");
	
	//BMP Magic Start
	get_bmp_magic(fp,&my_bmpfile_magic);
	printf("(2 bytes)::BMP Magic: ");
	printf("%c %c\n",my_bmpfile_magic.magic[0],my_bmpfile_magic.magic[1]);
	//BMP Magic End

	//Get the BMP Header
	get_bmp_header(fp,&my_bmpfile_header);
	
	//BMP Filesize Start
	printf("(2 bytes)::BMP Filesize: ");
	printf("%u\n",my_bmpfile_header.filesz);
	//BMP Filesize End
	
	//BMP Creator 1 Start
	printf("(2 bytes)::BMP Creator 1: ");
	printf("%u\n",my_bmpfile_header.creator1);
	//BMP Creator 1 End
	
	//BMP Creator 2 Start
	printf("(2 bytes)::BMP Creator 2: ");
	printf("%u\n",my_bmpfile_header.creator2);
	//BMP Creator 2 End
	
	//BMP Pixel Offset Start
	printf("(4 bytes)::BMP Pixel Offset: ");
	printf("%u\n",my_bmpfile_header.bmp_offset);
	//BMP Pixel Offset End

	printf("**** BMP HEADER END   ****\n\n");


	printf("**** DIB HEADER START ****\n");
	
	//Get the DIB Header
	get_dib_header(fp,&my_dib_header);

	//DIB Header Length Start
	printf("(4 bytes)::DIB Header Length: %u\n",my_dib_header.header_sz);
	//DIB Header Length End

	//DIB Width in pixels start
	printf("(4 bytes)::Width in pixels: %d\n",my_dib_header.width);
	//DIB Width in pixels end

	//DIB Height in pixels start
	printf("(4 bytes)::Height in pixels: %d\n",my_dib_header.height);
	//DIB Height in pixels end

	//DIB No. of color planes start
	printf("(2 bytes)::No. of color planes: %u\n",my_dib_header.nplanes);
	//DIB No. of color planes end

	//DIB No. of bits per pixel start
	printf("(2 bytes)::No. of bits per pixel: %u\n",my_dib_header.bitspp);
	//DIB No. of bits per pixel end

	//Compression Type start
	printf("(4 bytes)::Compression Type: %u >> ",my_dib_header.compress_type);
	print_compression_type(&my_dib_header);
	//Compression Type end

	//Raw bitmap data size start
	printf("(4 bytes)::Raw bitmap data size: %u\n",my_dib_header.bmp_bytesz);
	//Raw bitmap data size end

	//Horizontal resolution in pixels per meter start
	printf("(4 bytes)::Horizontal resolution (pixel/m): %d\n",my_dib_header.hres);
	//Horizontal resolution in pixels per meter end

	//Vertical resolution in pixels per meter start
	printf("(4 bytes)::Vertical resolution (pixel/m): %d\n",my_dib_header.vres);
	//Vertical resolution in pixels per meter end

	//No. of colors in the palette start
	printf("(4 bytes)::No of colors in the palette: %u",my_dib_header.ncolors);
	if(my_dib_header.ncolors == 0)
		printf(" (DEFAULT)\n");
	else
		printf("\n");
	//No. of colors in the palette end

	//No. of important colors start
	printf("(4 bytes)::No. of important colors: %u",my_dib_header.nimpcolors);
	if(my_dib_header.nimpcolors == 0)
		printf(" (ALL COLORS ARE IMPORTANT\n");
	else
		printf("\n");
	//No. of important colors end

	printf("**** DIB HEADER END   ****\n\n");


	printf("**** PIXEL DATA START ****\n");
	print_pixel_data(fp);
	printf("**** PIXEL DATA END   ****\n");
	
	printf("**** BMP FILE END  ****\n\n");

	return 0;
}