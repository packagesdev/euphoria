#import "RSSEuphoriaSettings.h"

NSString * const RSSEuphoria_Settings_StandardSetKey=@"Standard set";
NSString * const RSSEuphoria_Settings_WispsCountKey=@"Wisps count";
NSString * const RSSEuphoria_Settings_BackgroundLayersCountKey=@"Background count";
NSString * const RSSEuphoria_Settings_MeshDensityKey=@"Density";
NSString * const RSSEuphoria_Settings_VisibilityKey=@"Visibility";
NSString * const RSSEuphoria_Settings_SpeedKey=@"Speed";
NSString * const RSSEuphoria_Settings_FeedbackKey=@"Feedback";
NSString * const RSSEuphoria_Settings_FeedbackSpeedKey=@"Feedback speed";
NSString * const RSSEuphoria_Settings_FeedbackTextureSizeKey=@"Feedback size";
NSString * const RSSEuphoria_Settings_TextureKey=@"Texture";
NSString * const RSSEuphoria_Settings_ShowWireframeKey=@"Wireframe";

@implementation RSSEuphoriaSettings

- (id)initWithDictionaryRepresentation:(NSDictionary *)inDictionary
{
	self=[super init];
	
	if (self!=nil)
	{
		NSNumber * tNumber=inDictionary[RSSEuphoria_Settings_StandardSetKey];
		
		if (tNumber==nil)
			_standardSet=RSSEuphoriaSetRegular;
		else
			_standardSet=[tNumber unsignedIntegerValue];
		
		if (_standardSet==RSSEuphoriaSetCustom)
		{
			_numberOfWisps=[inDictionary[RSSEuphoria_Settings_WispsCountKey] unsignedIntegerValue];
			_numberOfBackgroundLayers=[inDictionary[RSSEuphoria_Settings_BackgroundLayersCountKey] unsignedIntegerValue];
			_meshDensity=[inDictionary[RSSEuphoria_Settings_MeshDensityKey] unsignedIntegerValue];
			_visibility=[inDictionary[RSSEuphoria_Settings_VisibilityKey] unsignedIntegerValue];
			_speed=[inDictionary[RSSEuphoria_Settings_SpeedKey] unsignedIntegerValue];
			_feedback=[inDictionary[RSSEuphoria_Settings_FeedbackKey] unsignedIntegerValue];
			_feedbackSpeed=[inDictionary[RSSEuphoria_Settings_FeedbackSpeedKey] unsignedIntegerValue];
			_feedbackTextureSize=[inDictionary[RSSEuphoria_Settings_FeedbackTextureSizeKey] unsignedIntegerValue];
			_texture=[inDictionary[RSSEuphoria_Settings_TextureKey] unsignedIntegerValue];
			_showWireframe=[inDictionary[RSSEuphoria_Settings_ShowWireframeKey] boolValue];
		}
		else if (_standardSet!=RSSEuphoriaSetRandom)
		{
			[self resetSettingsToStandardSet:_standardSet];
		}
	}
	
	return self;
}

- (NSDictionary *)dictionaryRepresentation
{
	NSMutableDictionary * tMutableDictionary=[NSMutableDictionary dictionary];
	
	if (tMutableDictionary!=nil)
	{
		tMutableDictionary[RSSEuphoria_Settings_StandardSetKey]=@(_standardSet);
		
		tMutableDictionary[RSSEuphoria_Settings_WispsCountKey]=@(_numberOfWisps);
		tMutableDictionary[RSSEuphoria_Settings_BackgroundLayersCountKey]=@(_numberOfBackgroundLayers);
		tMutableDictionary[RSSEuphoria_Settings_MeshDensityKey]=@(_meshDensity);
		tMutableDictionary[RSSEuphoria_Settings_VisibilityKey]=@(_visibility);
		tMutableDictionary[RSSEuphoria_Settings_SpeedKey]=@(_speed);
		tMutableDictionary[RSSEuphoria_Settings_FeedbackKey]=@(_feedback);
		tMutableDictionary[RSSEuphoria_Settings_FeedbackSpeedKey]=@(_feedbackSpeed);
		tMutableDictionary[RSSEuphoria_Settings_FeedbackTextureSizeKey]=@(_feedbackTextureSize);
		tMutableDictionary[RSSEuphoria_Settings_TextureKey]=@(_texture);
		tMutableDictionary[RSSEuphoria_Settings_ShowWireframeKey]=@(_showWireframe);
	}
	
	return [tMutableDictionary copy];
}

#pragma mark -

- (void)resetSettings
{
	_standardSet=RSSEuphoriaSetRegular;
	
	[self resetSettingsToStandardSet:_standardSet];
}

- (void)resetSettingsToStandardSet:(RSSEuphoriaSet)inSet;
{
	switch(inSet)
	{
		case RSSEuphoriaSetRegular:
			
			_numberOfWisps=5;
			_numberOfBackgroundLayers=0;
			_meshDensity=25;
			_visibility=35;
			_speed=15;
			
			_feedback=0;
			_feedbackSpeed=1;
			_feedbackTextureSize=8;
			_texture=RSSEuphoriaTextureTypeStringy;
			
			_showWireframe=NO;
			
			break;
			
		case RSSEuphoriaSetGrid:
			
			_numberOfWisps=4;
			_numberOfBackgroundLayers=1;
			_meshDensity=25;
			_visibility=70;
			_speed=15;
			
			_feedback=0;
			_feedbackSpeed=1;
			_feedbackTextureSize=8;
			_texture=RSSEuphoriaTextureTypeNone;
			
			_showWireframe=YES;
			
			break;
			
		case RSSEuphoriaSetCubism:
			
			_numberOfWisps=15;
			_numberOfBackgroundLayers=0;
			_meshDensity=4;
			_visibility=15;
			_speed=10;
			
			_feedback=0;
			_feedbackSpeed=1;
			_feedbackTextureSize=8;
			_texture=RSSEuphoriaTextureTypeNone;
			
			_showWireframe=NO;
			
			break;
			
		case RSSEuphoriaSetBadMath:
			
			_numberOfWisps=2;
			_numberOfBackgroundLayers=2;
			_meshDensity=20;
			_visibility=40;
			_speed=30;
			
			_feedback=40;
			_feedbackSpeed=5;
			_feedbackTextureSize=8;
			_texture=RSSEuphoriaTextureTypeStringy;
			
			_showWireframe=YES;
			
			break;
			
		case RSSEuphoriaSetMTheory:
			
			_numberOfWisps=3;
			_numberOfBackgroundLayers=0;
			_meshDensity=25;
			_visibility=15;
			_speed=20;
			
			_feedback=40;
			_feedbackSpeed=20;
			_feedbackTextureSize=8;
			_texture=RSSEuphoriaTextureTypeNone;
			
			_showWireframe=NO;
			
			break;
			
		case RSSEuphoriaSetUHFTEM:
			
			_numberOfWisps=0;
			_numberOfBackgroundLayers=3;
			_meshDensity=35;
			_visibility=5;
			_speed=50;
			
			_feedback=0;
			_feedbackSpeed=1;
			_feedbackTextureSize=8;
			_texture=RSSEuphoriaTextureTypeNone;
			
			_showWireframe=NO;
			
			break;
		
		case RSSEuphoriaSetNowhere:
			
			_numberOfWisps=0;
			_numberOfBackgroundLayers=3;
			_meshDensity=30;
			_visibility=40;
			_speed=20;
			
			_feedback=80;
			_feedbackSpeed=10;
			_feedbackTextureSize=8;
			_texture=RSSEuphoriaTextureTypeLinear;
			
			_showWireframe=YES;
			
			break;
			
		case RSSEuphoriaSetEcho:
			
			_numberOfWisps=3;
			_numberOfBackgroundLayers=0;
			_meshDensity=25;
			_visibility=30;
			_speed=20;
			
			_feedback=85;
			_feedbackSpeed=30;
			_feedbackTextureSize=8;
			_texture=RSSEuphoriaTextureTypePlasma;
			
			_showWireframe=NO;
			
			break;
			
		case RSSEuphoriaSetKaleidoscope:
			
			_numberOfWisps=3;
			_numberOfBackgroundLayers=0;
			_meshDensity=25;
			_visibility=40;
			_speed=15;
			
			_feedback=90;
			_feedbackSpeed=3;
			_feedbackTextureSize=8;
			_texture=RSSEuphoriaTextureTypeNone;
			
			_showWireframe=NO;
			
			break;
			
		default:
			
			NSLog(@"This should not be invoked for set: %u",(unsigned int)inSet);
			
			break;
	}
}

@end

