#include <CoreFoundation/CoreFoundation.h>

const CFStringRef kCVImageBufferChromaLocationBottomFieldKey = CFSTR("CVImageBufferChromaLocationBottomField");
const CFStringRef kCVImageBufferChromaLocationTopFieldKey = CFSTR("CVImageBufferChromaLocationTopField");
const CFStringRef kCVImageBufferChromaLocation_Left = CFSTR("Left");
const CFStringRef kCVPixelBufferIOSurfacePropertiesKey = CFSTR("IOSurfaceProperties");
const CFStringRef kCVPixelBufferOpenGLCompatibilityKey = CFSTR("OpenGLCompatibility");
const CFStringRef kCVPixelBufferPixelFormatTypeKey = CFSTR("PixelFormatType");
const CFStringRef kCVPixelBufferMetalCompatibilityKey = CFSTR("MetalCompatibility");
const CFStringRef kCVPixelBufferBytesPerRowAlignmentKey = CFSTR("BytesPerRowAlignment");
const CFStringRef kCVPixelBufferHeightKey = CFSTR("Height");
const CFStringRef kCVPixelBufferWidthKey = CFSTR("Width");

const CFStringRef kCVImageBufferCleanApertureKey = CFSTR("CVImageBufferCleanAperture");
const CFStringRef kCVImageBufferCleanApertureHeightKey = CFSTR("Height");
const CFStringRef kCVImageBufferCleanApertureWidthKey = CFSTR("Width");
const CFStringRef kCVImageBufferCleanApertureHorizontalOffsetKey = CFSTR("HorizontalOffset");
const CFStringRef kCVImageBufferCleanApertureVerticalOffsetKey = CFSTR("VerticalOffset");

const CFStringRef kCVImageBufferColorPrimariesKey = CFSTR("CVImageBufferColorPrimaries");
const CFStringRef kCVImageBufferColorPrimaries_ITU_R_709_2 = CFSTR("ITU_R_709_2");
const CFStringRef kCVImageBufferColorPrimaries_EBU_3213 = CFSTR("EBU_3213");
const CFStringRef kCVImageBufferColorPrimaries_SMPTE_C = CFSTR("SMPTE_C");

const CFStringRef kCVImageBufferPixelAspectRatioKey = CFSTR("CVImageBufferPixelAspectRatio");			// CFDictionary with the following two keys
const CFStringRef kCVImageBufferPixelAspectRatioHorizontalSpacingKey = CFSTR("HorizontalSpacing"); // CFNumber
const CFStringRef kCVImageBufferPixelAspectRatioVerticalSpacingKey = CFSTR("VerticalSpacing");	// CFNumber

const CFStringRef kCVMetalTextureCacheMaximumTextureAgeKey = CFSTR("CVMetalTextureCacheMaximumTextureAge");
