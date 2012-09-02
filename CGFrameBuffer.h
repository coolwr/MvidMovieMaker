//
//  CGFrameBuffer.h
//
//  Created by Moses DeJong on 2/13/09.
//
//  License terms defined in License.txt.

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#define UIView NSView

@class DeltaBounds;
@class DeltaPixel;

// Avoid incorrect warnings from clang
#ifndef __has_feature      // Optional.
#define __has_feature(x) 0 // Compatibility with non-clang compilers.
#endif

#ifndef CF_RETURNS_RETAINED
#if __has_feature(attribute_cf_returns_retained)
#define CF_RETURNS_RETAINED __attribute__((cf_returns_retained))
#else
#define CF_RETURNS_RETAINED
#endif
#endif

@interface CGFrameBuffer : NSObject {
@protected
	char *m_pixels;
	size_t m_numBytes;
	size_t m_width;
	size_t m_height;
	size_t m_bitsPerPixel;
	size_t m_bytesPerPixel;
	int32_t m_isLockedByDataProvider;
	CGImageRef m_lockedByImageRef;
}

@property (readonly) char *pixels;
@property (readonly) size_t numBytes;
@property (readonly) size_t width;
@property (readonly) size_t height;
@property (readonly) size_t bitsPerPixel;
@property (readonly) size_t bytesPerPixel;

@property (nonatomic, assign) BOOL isLockedByDataProvider;
@property (nonatomic, readonly) CGImageRef lockedByImageRef;

+ (CGFrameBuffer*) cGFrameBufferWithBppDimensions:(NSInteger)bitsPerPixel width:(NSInteger)width height:(NSInteger)height;

- (id) initWithBppDimensions:(NSInteger)bitsPerPixel width:(NSInteger)width height:(NSInteger)height;

// Render the contents of a view as pixels. Returns TRUE
// is successful, otherwise FALSE. Note that the view
// must be opaque and render all of its pixels. 

- (BOOL) renderView:(UIView*)view;

// Create a Core Graphics image from the pixel data
// in this buffer. The hasDataProvider property
// will be TRUE while the CGImageRef is in use.

- (CGImageRef) createCGImageRef;

// Defines the pixel layout, could be overloaded in a derived class

- (CGBitmapInfo) getBitmapInfo;

- (NSData*) copyData;

- (NSArray*) calculateDeltaPixels:(CGFrameBuffer*)otherFrame;

// Convert pixels to a PNG image format that can be easily saved to disk.

- (NSData*) formatAsPNG;

// Copy data from another framebuffer into this one

- (void) copyPixels:(CGFrameBuffer *)anotherFrameBuffer;

// Use memcopy() as opposed to an OS level page copy

- (void) memcopyPixels:(CGFrameBuffer *)anotherFrameBuffer;

- (void) zeroCopyToPixels;

// Zero copy from an external read-only location if supported. Otherwise plain copy.

- (void) zeroCopyPixels:(void*)zeroCopyPtr mappedData:(NSData*)mappedData;

@end

// Util struct/object

@interface DeltaPixel : NSObject {
@public
	uint16_t x;
	uint16_t y;
	uint16_t oldValue;
	uint16_t newValue;
	DeltaBounds *deltaBounds;
}
@end

// Util struct/object

@interface DeltaBounds : NSObject {
@public
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
}
@end
