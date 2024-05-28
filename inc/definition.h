// #define FILE_PLACE "../test/Test_Image_0.jpg"
// #define FILE_PLACE "../test/Test_Image_1.jpg"
// #define FILE_PLACE "../test/Test_Image_2.jpg"
// #define FILE_PLACE "../test/Test_Image_3.jpg"
// #define FILE_PLACE "../test/Test_Image_4.jpg"
// #define FILE_PLACE "../test/Test_Image_5.jpg"
#define FILE_PLACE "../test/Test_Image_4.jpg"

#define READ_LEN 600

#define JPEG_FORMAT_LEN 4
#define JPEG_FORMAT_START 6

// Start of Image
#define MARKER_SOI 0xd8
// JFIF application segment
#define MARKER_APP0 0xe0
#define MARKER_APPD 0xed
// #define MARKER_APPn
// Other APP segments

// Quantization Table
#define MARKER_DQT 0xdb
// Start of Frame
#define MARKER_SOF0 0xc0
// Huffman Table
#define MARKER_DHT 0xc4
// Start of Scan
#define MARKER_SOS 0xda
// End of Image
#define MARKER_EOI 0xd9

#define APP0_MARKER_LEN 2
#define APP0_LENGTH_LEN 2
#define APP0_IDENTIFIER_LEN 5
#define APP0_VERSION_LEN 2
#define APP0_UNIT_LEN 1
#define APP0_X_DENSITY_LEN 2
#define APP0_Y_DENSITY_LEN 2
#define APP0_X_THUMBNAIL_LEN 1
#define APP0_Y_THUMBNAIL_LEN 1

#define DQT_MARKER_LEN 2
#define DQT_LENGTH_LEN 2

#define DHT_MARKER_LEN 2
#define DHT_LENGTH_LEN 2

#define SOF_MARKER_LEN 2
#define SOF_LENGTH_LEN 2
#define SOF_PRECISION_LEN 1
#define SOF_HEIGHT_LEN 2
#define SOF_WIDTH_LEN 2
#define SOF_COMPONENTS_LEN 1

#define SOS_MARKER_LEN 2
#define SOS_LENGTH_LEN 2
#define SOS_COMPONENTS_LEN 1
#define SOS_START_SPECTRAL_LEN 1
#define SOS_END_SPECTRAL_LEN 1
#define SOS_APPROX_LEN 1
#define MATRIX_LEN 8;