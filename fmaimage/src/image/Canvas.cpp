#include <fma/util/sha256.hpp>
#include <image/Canvas.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <png.h>

using namespace FMA::image;

// ----------------------------------------------------------------------------
Canvas::Canvas()
  : bytes(NULL)
  , width(0)
  , height(0)
{
}

// ----------------------------------------------------------------------------
Canvas::~Canvas() {
  unload();
}

// ----------------------------------------------------------------------------
CanvasPtr Canvas::create(uint32_t width, uint32_t height, uint8_t bpp) {
  Canvas *img = new Canvas();
  img->width = width;
  img->height = height;
  img->bytesPerPixel = bpp;
  img->bytesPerRow = bpp * width;
  img->bytes = (unsigned char*)malloc(img->bytesPerRow * height);
  img->clear();
  return CanvasPtr(img);
}

// ----------------------------------------------------------------------------
CanvasPtr Canvas::createRaw(uint32_t width, uint32_t height, uint8_t bpp, const PalettePtr &palette, void *data) {
  Canvas *img = new Canvas();
  img->width = width;
  img->height = height;
  img->bytesPerPixel = bpp;
  img->bytesPerRow = bpp * width;
  img->bytes = (unsigned char*)data;
  img->palette = palette;
  return CanvasPtr(img);
}

// ----------------------------------------------------------------------------
CanvasPtr Canvas::clone() const {
  Canvas *img = new Canvas();
  img->width = width;
  img->height = height;
  img->bytesPerPixel = bytesPerPixel;
  img->bytesPerRow = bytesPerRow;
  img->bytes = (unsigned char*)malloc(bytesPerRow * height);
  memcpy(img->bytes, bytes, bytesPerRow * height);
  if (palette) {
    img->palette = palette->clone();
  }
  return CanvasPtr(img);
}

// ----------------------------------------------------------------------------
CanvasPtr Canvas::slice(uint32_t left, uint32_t top, uint32_t newWidth, uint32_t newHeight) const {
  Canvas *img = new Canvas();
  img->width = newWidth;
  img->height = newHeight;
  img->bytesPerPixel = bytesPerPixel;
  img->bytesPerRow = newWidth * bytesPerPixel;
  if (palette) {
    img->palette = palette->clone();
  }
  img->bytes = (unsigned char*)malloc(img->bytesPerRow * img->height);
  img->draw(this, 0, 0, left, top, newWidth, newHeight);
  return CanvasPtr(img);
}

// ----------------------------------------------------------------------------
void Canvas::draw(const Canvas *img, int32_t dx, int32_t dy, int32_t sx, int32_t sy, uint32_t _w, uint32_t _h) {
  int32_t w = _w;
  int32_t h = _h;

  if (dx < 0) {
    sx -= dx;
    w += dx;
    dx = 0;
  }

  if (dy < 0) {
    sy -= dy;
    h += dy;
    dy = 0;
  }

  if (sx < 0) {
    dx -= sx;
    w += sx;
    sx = 0;
  }

  if (sy < 0) {
    dy -= sy;
    h += sy;
    sy = 0;
  }

  if (w + dx > (int)width) {
    w = width - dx;
  }

  if (h + dy > (int)height) {
    h = height - dy;
  }

  if (w + sx > (int)img->width) {
    w = img->width - sx;
  }

  if (h + sy > (int)img->height) {
    h = img->height - sy;
  }

  if (w <= 0 || h <= 0) {
    return;
  }

  if (img->bytesPerPixel != bytesPerPixel) {
    std::cerr << "Error: draw with different bytesperpixel is not implemented yet" << std::endl;
    return;
  }
  
  unsigned char *src = img->bytes + sy * img->bytesPerRow + sx * img->bytesPerPixel;
  unsigned char *dest = bytes + dy * bytesPerRow + dx * bytesPerPixel;
  uint32_t copyPerRow = w * bytesPerPixel;
  
  for (int32_t y=0; y<h; y++) {
    memcpy(dest, src, copyPerRow);

    src += img->bytesPerRow;
    dest += bytesPerRow;
  }

}

// ----------------------------------------------------------------------------
void Canvas::clear() {
  memset(bytes, 0, bytesPerRow * height);
}

// ----------------------------------------------------------------------------
void Canvas::fill(uint8_t colorIndex) {
  if (bytesPerPixel != 1) {
    return;
  }

  memset(bytes, colorIndex, bytesPerRow * height);
}

// ----------------------------------------------------------------------------
void Canvas::unload() {
  if (bytes) {
    free(bytes);
    bytes = NULL;
  }

  palette.reset();

  width = 0;
  height = 0;
}

// ----------------------------------------------------------------------------
void Canvas::set(uint32_t col, uint32_t row, const Color &color) {
  if (col >= width || row >= height) {
    return;
  }

  unsigned char *ptr = getData(col, row);
  switch (bytesPerPixel) {
    case 3:
      *ptr++ = color.r;
      *ptr++ = color.g;
      *ptr++ = color.b;
      break;

    case 4:
      *ptr++ = color.r;
      *ptr++ = color.g;
      *ptr++ = color.b;
      *ptr++ = color.a;
      break;
  }
}

// ----------------------------------------------------------------------------
Color Canvas::get(uint32_t col, uint32_t row) {
  if (col >= width || row >= height) {
    return Color(0, 0, 0, 0);
  }

  unsigned char *ptr = getData(col, row);
  switch (bytesPerPixel) {
    case 1: return ptr[0] == 0 ? Color(0,0,0,0) : getColor(ptr[0]);
    case 3: return Color(ptr[0], ptr[1], ptr[2]);
    case 4: return Color(ptr[0], ptr[1], ptr[2], ptr[3]);
    default: return Color();
  }
}

// ----------------------------------------------------------------------------
std::string Canvas::hash() const {
  FMA::util::SHA256 sha;
  sha.add(&width, sizeof(width));
  sha.add(&height, sizeof(height));
  sha.add(&bytesPerRow, sizeof(bytesPerRow));
  sha.add(bytes, bytesPerRow * height);
  return sha.getHash();
}

// ----------------------------------------------------------------------------
std::string Canvas::hash(uint32_t left, uint32_t top, uint32_t hashWidth, uint32_t hashHeight) const {
  FMA::util::SHA256 sha;
  sha.add(&hashWidth, sizeof(hashWidth));
  sha.add(&hashHeight, sizeof(hashHeight));
  sha.add(&bytesPerRow, sizeof(bytesPerRow));

  uint32_t hashBytesPerRow = hashWidth * bytesPerPixel;
  unsigned char *data = bytes + left * bytesPerPixel + top * bytesPerRow;
  for (uint32_t y=0; y<hashHeight; y++) {
    sha.add(data, hashBytesPerRow);
    data += bytesPerRow;
  }
  return sha.getHash();
}

// ----------------------------------------------------------------------------
void Canvas::flipX() {
  for (uint32_t y=0; y<height; y++) {
    uint8_t data[4];
    unsigned char *left = bytes + y * bytesPerRow;
    unsigned char *right = left + bytesPerRow - bytesPerPixel;

    switch (bytesPerPixel) {
    case 1:
      while (right > left) {
        *data = *left;
        *left++ = *right;
        *right-- = *data;
      }
      break;
      
    case 3:
      while (right > left) {
        data[0] = left[0];
        data[1] = left[1];
        data[2] = left[2];
        left[0] = right[0];
        left[1] = right[1];
        left[2] = right[2];
        right[0] = data[0];
        right[1] = data[1];
        right[2] = data[2];
        right -= 3;
        left += 3;
      }
      break;
      
    case 4:
      while (right > left) {
        *(reinterpret_cast<uint32_t*>(data)) = *(reinterpret_cast<uint32_t*>(left));
        *(reinterpret_cast<uint32_t*>(left)) = *(reinterpret_cast<uint32_t*>(right));
        *(reinterpret_cast<uint32_t*>(right)) = *(reinterpret_cast<uint32_t*>(data));
        right -= 4;
        left += 4;
      }
      break;
    }
  }
}

// ----------------------------------------------------------------------------
void Canvas::flipY() {
  unsigned char *newData = (unsigned char*)malloc(height * bytesPerRow);

  uint32_t top = 0;
  uint32_t bottom = height - 1;
  
  while (top < bottom) {
    void *leftSrc   = bytes   + top    * bytesPerRow;
    void *rightSrc  = bytes   + bottom * bytesPerRow;
    void *leftDest  = newData + top    * bytesPerRow;
    void *rightDest = newData + bottom * bytesPerRow;

    memcpy(leftDest, rightSrc, bytesPerRow);
    memcpy(rightDest, leftSrc, bytesPerRow);

    top++;
    bottom--;
  }

  free(bytes);
  bytes = newData;
}

// ----------------------------------------------------------------------------
bool Canvas::loadFromPng(const char *fileName) {
  unload();
  uint8_t header[8];
  
  FILE *fh = fopen(fileName, "rb");
  if (!fh) {
    std::cerr << "Could not open file" << std::endl;
    return false;
  }

  if (fread(header, 1, 8, fh) < 8 || png_sig_cmp(header, 0, 8)) {
    std::cerr << "Invalid PNG file" << std::endl;
    fclose(fh);
    return false;
  }

  png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!pngPtr) {
    std::cerr << "Can not read PNG structure" << std::endl;
    fclose(fh);
    return false;
  }

  png_infop infoPtr = png_create_info_struct(pngPtr);
  if (!infoPtr) {
    std::cerr << "Can not create info structure" << std::endl;
    png_destroy_read_struct(&pngPtr, NULL, NULL);
    fclose(fh);
    return false;
  }

  if (setjmp(png_jmpbuf(pngPtr)))   {
    std::cerr << "Error during input IO" << std::endl;
    png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
    return false;
  }

  png_init_io(pngPtr, fh);
  png_set_sig_bytes(pngPtr, 8);
  png_read_info(pngPtr, infoPtr);

  png_uint_32 pngWidth;
  png_uint_32 pngHeight;
  int bitDepth = -1;
  int colorType = -1;

  png_get_IHDR(pngPtr, infoPtr, &pngWidth, &pngHeight, &bitDepth, &colorType, NULL, NULL, NULL);

  if (setjmp(png_jmpbuf(pngPtr))) {
    std::cerr << "Error reading image data" << std::endl;
    png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
    fclose(fh);
    return false;
  }

  if (bitDepth == -1 || colorType == -1) {
    std::cerr << "Could not read header for some reason. BitDepth: " << bitDepth << " ColorType: " << colorType << std::endl;
    png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
    fclose(fh);
    return false;
  }

  if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8) {
    png_set_expand(pngPtr);
  } else if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS)) {
    png_set_expand(pngPtr);
  }

  if (bitDepth == 16) {
    png_set_strip_16(pngPtr);
  } else if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA) {
    png_set_gray_to_rgb(pngPtr);
  }

  png_read_update_info(pngPtr, infoPtr);
  png_get_IHDR(pngPtr, infoPtr, &pngWidth, &pngHeight, &bitDepth, &colorType, NULL, NULL, NULL);

  width = pngWidth;
  height = pngHeight;

  if (colorType & PNG_COLOR_MASK_PALETTE) {
    int numPalette;
    png_colorp pngPalette;
    png_get_PLTE(pngPtr, infoPtr, &pngPalette, &numPalette);
    ColorList colors;
    for (int i=0; i<numPalette; i++) {
      colors.push_back(Color(pngPalette[i].red, pngPalette[i].green, pngPalette[i].blue));
    }
    palette = PalettePtr(new Palette(colors));
  }

  switch (colorType) {
    case PNG_COLOR_TYPE_PALETTE:
      bytesPerPixel = 1;
      break;

    case PNG_COLOR_TYPE_RGB:
      bytesPerPixel = 3;
      break;

    case PNG_COLOR_TYPE_RGBA:
      bytesPerPixel = 4;
      break;

    default:
      std::cerr << "Unsupported color type" << std::endl;
      png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
      fclose(fh);
      return false;
  }

  if (bitDepth != 8) {
      std::cerr << "Unsupported bit depth" << std::endl;
      png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
      fclose(fh);
      return false;
  }

  bytesPerRow = png_get_rowbytes(pngPtr, infoPtr);
  
  bytes = (unsigned char*)malloc(pngHeight * bytesPerRow);
  png_bytep *rowPointer = new png_bytep[pngHeight];
  for (png_uint_32 y=0; y<pngHeight; y++) {
    rowPointer[y] = (png_bytep)(bytes + bytesPerRow * y);
  }

  png_read_image(pngPtr, rowPointer);
  png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
  png_read_end(pngPtr, NULL);
  fclose(fh);
  delete[] rowPointer;

  return true;
}

// ----------------------------------------------------------------------------
void Canvas::convertToRgba() {
  if (bytesPerPixel == 4) {
    return;
  }

  uint64_t pixelsLeft = width * height;
  unsigned char *newData = (unsigned char*)malloc(pixelsLeft * 4);
  unsigned char *data = bytes;
  unsigned char *oldBytes = bytes;
  bytes = newData;
  switch (bytesPerPixel) {
    case 1:
      while (pixelsLeft--) {
        const Color &color = palette->getColor(*data++);
        *newData++ = color.r;
        *newData++ = color.g;
        *newData++ = color.b;
        *newData++ = color.a;
      }
      palette.reset();
      break;

    case 3:
      while (pixelsLeft--) {
        *newData++ = *data++;
        *newData++ = *data++;
        *newData++ = *data++;
        *newData++ = 255;
      }
      break;
  }

  bytesPerPixel = 4;
  bytesPerRow = width * 4;

  free(oldBytes);
}

// ----------------------------------------------------------------------------
void Canvas::applyPalette(const PalettePtr &palette) {
  unsigned char *newData = (unsigned char*)malloc(width * height);
  unsigned char *data = newData;

  for (uint32_t y=0; y<height; y++) {
    for (uint32_t x=0; x<width; x++) {
      *(data++) = palette->findBest(get(x, y));
    }
  }

  free(bytes);
  bytes = newData;
  bytesPerPixel = 1;
  bytesPerRow = width;
  this->palette = palette;
}