
#import "RSSSettings.h"

typedef NS_ENUM(NSUInteger, RSSEuphoriaSet)
{
	RSSEuphoriaSetCustom=0,
	RSSEuphoriaSetRandom=1,
	RSSEuphoriaSetRegular=1027,
	RSSEuphoriaSetGrid=1028,
	RSSEuphoriaSetCubism=1029,
	RSSEuphoriaSetBadMath=1030,
	RSSEuphoriaSetMTheory=31,
	RSSEuphoriaSetUHFTEM=1032,						// ultra high frequency tunneling electron microscope
	RSSEuphoriaSetNowhere=1033,
	RSSEuphoriaSetEcho=1034,
	RSSEuphoriaSetKaleidoscope=1035
};

typedef NS_ENUM(NSUInteger, RSSEuphoriaTextureType)
{
	RSSEuphoriaTextureTypeNone=0,
	RSSEuphoriaTextureTypePlasma=1,
	RSSEuphoriaTextureTypeStringy=2,
	RSSEuphoriaTextureTypeLinear=3,
	RSSEuphoriaTextureTypeRandom=4
};

@interface RSSEuphoriaSettings : RSSSettings

@property RSSEuphoriaSet standardSet;


@property NSUInteger numberOfWisps;

@property NSUInteger numberOfBackgroundLayers;

@property NSUInteger meshDensity;

@property NSUInteger visibility;

@property NSUInteger speed;

@property NSUInteger feedback;

@property NSUInteger feedbackSpeed;

@property NSUInteger feedbackTextureSize;

@property RSSEuphoriaTextureType texture;

@property BOOL showWireframe;


- (void)resetSettingsToStandardSet:(RSSEuphoriaSet)inSet;

@end
