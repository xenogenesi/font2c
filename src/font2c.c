#include <stdio.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <ftbitmap.h>

#define WIDTH   640
#define BYTEWIDTH (WIDTH)/8
#define HEIGHT  480

static unsigned char image[HEIGHT][BYTEWIDTH];

static FT_Library library;
static FT_Face face;
static FT_Error err;
static FT_Bitmap tempbitmap;

static void to_bitmap( FT_Bitmap*  bitmap, FT_Int x, FT_Int y) {

	FT_Int  i, j, p, q;
	FT_Int  x_max = x + bitmap->width;
	FT_Int  y_max = y + bitmap->rows;

	for ( i = x, p = 0; i < x_max; i++, p++ ) {
		for ( j = y, q = 0; j < y_max; j++, q++ ) {
			if ( (i < 0) || (j < 0) || (i >= WIDTH || j >= HEIGHT) )
				continue;
			image[j][i >> 3] |= (bitmap->buffer[q * bitmap->width + p]) << (i & 7);
		}
	}
}

static void draw_glyph(unsigned char glyph, int *x, int *y) {
	FT_UInt  glyph_index;
	FT_GlyphSlot  slot = face->glyph;

	glyph_index = FT_Get_Char_Index( face, glyph );

	if ((err = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT ))) {
		fprintf( stderr, "warning: failed FT_Load_Glyph 0x%x %d\n", glyph, err);
		return;
	}

    if ((err = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_MONO ))) {
    	fprintf( stderr, "warning: failed FT_Render_Glyph 0x%x %d\n", glyph, err);
    	return;
    }

	FT_Bitmap_New(&tempbitmap);
	FT_Bitmap_Convert( library, &slot->bitmap, &tempbitmap, 1);

    //to_bitmap( &slot->bitmap, *x + slot->bitmap_left, *y - slot->bitmap_top );
    to_bitmap( &tempbitmap, *x, *y );

	FT_Bitmap_Done( library, &tempbitmap );

    *x += slot->advance.x >> 6;
}

static void out_xbm(int w, int h) {
	int x, y;
	printf("#define BMP_width %d\n", WIDTH);
	printf("#define BMP_height %d\n", h);
	printf("static char BMP_bits[] = {\n");
	for (y=0; y < h; y++) {
		printf("\t");
		for (x=0; x < w; x++) {
			printf("0x%x, ", image[y][x]);
		}
		printf("\n");
	}
	printf("\n}\n");
}

int main(int argc, char **argv) {
	char *filename;
	int x = 0, y = 0;
	int g;

	memset (image, 0, BYTEWIDTH*HEIGHT);

	if (argc < 2) {
		fprintf( stderr, "usage: font2c [font]\n");
		exit(1);
	}
	filename = argv[1];

	if ((err = FT_Init_FreeType( &library ))) {
		fprintf( stderr, "error: Init_Freetype failed %d\n", err);
		exit(1);
	}
	if ((err = FT_New_Face( library, filename, 0, &face ))) {
		fprintf( stderr, "error: FT_New_Face failed %d\n", err);
		exit(1);
	}

	for (g = 0; g < 256; g++) {
		if (x+8 >= WIDTH) {
				x = 0;
				y += 15; // FIXME get ascender
		}
		draw_glyph(g, &x, &y);
	}

	out_xbm(BYTEWIDTH, HEIGHT);

	FT_Done_Face( face );
	FT_Done_FreeType( library );

	return 0;
}
