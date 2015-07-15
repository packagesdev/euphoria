
#import "RSSEuphoriaConfigurationWindowController.h"

#import "RSSEuphoriaSettings.h"

#import "RSSCollectionView.h"

#import "RSSCollectionViewItem.h"

@interface RSSEuphoriaConfigurationWindowController () <RSSCollectionViewDelegate>
{
	IBOutlet RSSCollectionView *_settingsCollectionView;
	
	IBOutlet NSSlider * _numberOfWispsSlider;
	IBOutlet NSTextField * _numberOfWispsLabel;
	IBOutlet NSSlider * _numberOfBackgroundLayersSlider;
	IBOutlet NSTextField * _numberOfBackgroundLayersLabel;
	
	IBOutlet NSSlider * _meshDensitySlider;
	IBOutlet NSTextField * _meshDensityLabel;
	IBOutlet NSSlider * _visibilitySlider;
	IBOutlet NSTextField * _visibilityLabel;
	IBOutlet NSSlider * _speedSlider;
	IBOutlet NSTextField * _speedLabel;
	
	IBOutlet NSSlider * _feedbackSlider;
	IBOutlet NSTextField * _feedbackLabel;
	IBOutlet NSSlider * _feedbackSpeedSlider;
	IBOutlet NSTextField * _feedbackSpeedLabel;
	IBOutlet NSSlider * _feedbackTextureSizeSlider;
	IBOutlet NSTextField * _feedbackTextureSizeLabel;
	IBOutlet NSPopUpButton * _textureTypePopupButton;
	IBOutlet NSButton *_wireframeCheckbox;
	
	NSNumberFormatter * _numberFormatter;
}

- (void)_selectCollectionViewItemWithTag:(NSInteger)inTag;

- (void)_setAsCustomSet;

- (IBAction)setNumberOfWisps:(id)sender;
- (IBAction)setNumberOfBackgroundLayers:(id)sender;

- (IBAction)setMeshDensity:(id)sender;
- (IBAction)setVisibility:(id)sender;
- (IBAction)setSpeed:(id)sender;

- (IBAction)setFeedback:(id)sender;
- (IBAction)setFeedbackSpeed:(id)sender;
- (IBAction)setFeedbackTextureSize:(id)sender;
- (IBAction)setTextureType:(id)sender;
- (IBAction)setWireframe:(id)sender;

@end

@implementation RSSEuphoriaConfigurationWindowController

- (void)dealloc
{
	[[NSDistributedNotificationCenter defaultCenter] removeObserver:self];
}

- (Class)settingsClass
{
	return [RSSEuphoriaSettings class];
}

- (void)windowDidLoad
{
	[super windowDidLoad];
	
	_numberFormatter=[[NSNumberFormatter alloc] init];
	
	if (_numberFormatter!=nil)
	{
		_numberFormatter.hasThousandSeparators=YES;
		_numberFormatter.localizesFormat=YES;
	}
	
	NSBundle * tBundle=[NSBundle bundleForClass:[self class]];
	
	NSArray * tStandardSettingsArray=@[@{
										   RSSCollectionViewRepresentedObjectThumbnail : @"regular_thumbnail",
										   RSSCollectionViewRepresentedObjectTag : @(RSSEuphoriaSetRegular),
										   RSSCollectionViewRepresentedObjectName : NSLocalizedStringFromTableInBundle(@"Regular",@"Localized",tBundle,@"")
										   },
									   @{
										   RSSCollectionViewRepresentedObjectThumbnail : @"grid_thumbnail",
										   RSSCollectionViewRepresentedObjectTag : @(RSSEuphoriaSetGrid),
										   RSSCollectionViewRepresentedObjectName : NSLocalizedStringFromTableInBundle(@"Grid",@"Localized",tBundle,@"")
										   },
									   @{
										   RSSCollectionViewRepresentedObjectThumbnail : @"cubism_thumbnail",
										   RSSCollectionViewRepresentedObjectTag : @(RSSEuphoriaSetCubism),
										   RSSCollectionViewRepresentedObjectName : NSLocalizedStringFromTableInBundle(@"Cubism",@"Localized",tBundle,@"")
										   },
									   @{
										   RSSCollectionViewRepresentedObjectThumbnail : @"badmath_thumbnail",
										   RSSCollectionViewRepresentedObjectTag : @(RSSEuphoriaSetBadMath),
										   RSSCollectionViewRepresentedObjectName : NSLocalizedStringFromTableInBundle(@"Bad Math",@"Localized",tBundle,@"")
										   },
									   @{
										   RSSCollectionViewRepresentedObjectThumbnail : @"mtheory_thumbnail",
										   RSSCollectionViewRepresentedObjectTag : @(RSSEuphoriaSetMTheory),
										   RSSCollectionViewRepresentedObjectName : NSLocalizedStringFromTableInBundle(@"M-Theory",@"Localized",tBundle,@"")
										   },
									   @{
										   RSSCollectionViewRepresentedObjectThumbnail : @"uhftem_thumbnail",
										   RSSCollectionViewRepresentedObjectTag : @(RSSEuphoriaSetUHFTEM),
										   RSSCollectionViewRepresentedObjectName : NSLocalizedStringFromTableInBundle(@"UHFTEM",@"Localized",tBundle,@"")
										   },
									   @{
										   RSSCollectionViewRepresentedObjectThumbnail : @"nowhere_thumbnail",
										   RSSCollectionViewRepresentedObjectTag : @(RSSEuphoriaSetNowhere),
										   RSSCollectionViewRepresentedObjectName : NSLocalizedStringFromTableInBundle(@"Nowhere",@"Localized",tBundle,@"")
										   },
									   @{
										   RSSCollectionViewRepresentedObjectThumbnail : @"echo_thumbnail",
										   RSSCollectionViewRepresentedObjectTag : @(RSSEuphoriaSetEcho),
										   RSSCollectionViewRepresentedObjectName : NSLocalizedStringFromTableInBundle(@"Echo",@"Localized",tBundle,@"")
										   },
									   @{
										   RSSCollectionViewRepresentedObjectThumbnail : @"kaleidoscope_thumbnail",
										   RSSCollectionViewRepresentedObjectTag : @(RSSEuphoriaSetKaleidoscope),
										   RSSCollectionViewRepresentedObjectName : NSLocalizedStringFromTableInBundle(@"Kaleidoscope",@"Localized",tBundle,@"")
										   },
									   @{
										   RSSCollectionViewRepresentedObjectThumbnail : @"random_thumbnail",
										   RSSCollectionViewRepresentedObjectTag : @(RSSEuphoriaSetRandom),
										   RSSCollectionViewRepresentedObjectName : NSLocalizedStringFromTableInBundle(@"Random",@"Localized",tBundle,@"")
										   },
									   @{
										   RSSCollectionViewRepresentedObjectThumbnail : @"custom_thumbnail",
										   RSSCollectionViewRepresentedObjectTag : @(RSSEuphoriaSetCustom),
										   RSSCollectionViewRepresentedObjectName : NSLocalizedStringFromTableInBundle(@"Custom",@"Localized",tBundle,@"")
										   }];
	
	[_settingsCollectionView setContent:tStandardSettingsArray];
	
	/*[[NSDistributedNotificationCenter defaultCenter] addObserver:self
														selector:@selector(preferredScrollerStyleDidChange:)
															name:NSPreferredScrollerStyleDidChangeNotification
														  object:nil
											  suspensionBehavior:NSNotificationSuspensionBehaviorDeliverImmediately];*/
}

- (void)restoreUI
{
	[super restoreUI];
	
	RSSEuphoriaSettings * tEuphoriaSettings=(RSSEuphoriaSettings *) sceneSettings;
	
	[self _selectCollectionViewItemWithTag:tEuphoriaSettings.standardSet];
	
	
	[_numberOfWispsSlider setIntegerValue:tEuphoriaSettings.numberOfWisps];
	[_numberOfWispsLabel setIntegerValue:tEuphoriaSettings.numberOfWisps];
	
	[_numberOfBackgroundLayersSlider setIntegerValue:tEuphoriaSettings.numberOfBackgroundLayers];
	[_numberOfBackgroundLayersLabel setIntegerValue:tEuphoriaSettings.numberOfBackgroundLayers];

	
	[_meshDensitySlider setIntegerValue:tEuphoriaSettings.meshDensity];
	[_meshDensityLabel setIntegerValue:tEuphoriaSettings.meshDensity];
	
	[_visibilitySlider setIntegerValue:tEuphoriaSettings.visibility];
	[_visibilityLabel setIntegerValue:tEuphoriaSettings.visibility];
	
	[_speedSlider setIntegerValue:tEuphoriaSettings.speed];
	[_speedLabel setIntegerValue:tEuphoriaSettings.speed];
	
	
	[_feedbackSlider setIntegerValue:tEuphoriaSettings.feedback];
	[_feedbackLabel setIntegerValue:tEuphoriaSettings.feedback];
	
	[_feedbackSpeedSlider setIntegerValue:tEuphoriaSettings.feedbackSpeed];
	[_feedbackSpeedLabel setIntegerValue:tEuphoriaSettings.feedbackSpeed];
	
	[_feedbackTextureSizeSlider setIntegerValue:tEuphoriaSettings.feedbackTextureSize];
	[_feedbackTextureSizeLabel setIntegerValue:tEuphoriaSettings.feedbackTextureSize];
	
	[_textureTypePopupButton selectItemWithTag:tEuphoriaSettings.texture];
	
	[_wireframeCheckbox setState:(tEuphoriaSettings.texture==YES) ? NSOnState : NSOffState];
}

#pragma mark -

- (void)_selectCollectionViewItemWithTag:(NSInteger)inTag
{
	[_settingsCollectionView.content enumerateObjectsUsingBlock:^(NSDictionary * bDictionary,NSUInteger bIndex,BOOL * bOutStop){
		NSNumber * tNumber=bDictionary[RSSCollectionViewRepresentedObjectTag];
		
		if (tNumber!=nil)
		{
			if (inTag==[tNumber integerValue])
			{
				[_settingsCollectionView RSS_selectItemAtIndex:bIndex];
				
				*bOutStop=YES;
			}
		}
	}];
}

- (void)_setAsCustomSet
{
	RSSEuphoriaSettings * tEuphoriaSettings=(RSSEuphoriaSettings *) sceneSettings;
	
	if (tEuphoriaSettings.standardSet!=RSSEuphoriaSetCustom)
	{
		tEuphoriaSettings.standardSet=RSSEuphoriaSetCustom;
		
		[self _selectCollectionViewItemWithTag:tEuphoriaSettings.standardSet];
	}
}

- (IBAction)setNumberOfWisps:(id)sender
{
	RSSEuphoriaSettings * tEuphoriaSettings=(RSSEuphoriaSettings *) sceneSettings;
	
	if (tEuphoriaSettings.numberOfWisps!=[sender integerValue])
	{
		tEuphoriaSettings.numberOfWisps=[sender integerValue];
		
		[_numberOfWispsLabel setIntegerValue:tEuphoriaSettings.numberOfWisps];
		
		[self _setAsCustomSet];
	}
}

- (IBAction)setNumberOfBackgroundLayers:(id)sender
{
	RSSEuphoriaSettings * tEuphoriaSettings=(RSSEuphoriaSettings *) sceneSettings;
	
	if (tEuphoriaSettings.numberOfBackgroundLayers!=[sender integerValue])
	{
		tEuphoriaSettings.numberOfBackgroundLayers=[sender integerValue];
		
		[_numberOfBackgroundLayersLabel setIntegerValue:tEuphoriaSettings.numberOfBackgroundLayers];
		
		[self _setAsCustomSet];
	}
}

- (IBAction)setMeshDensity:(id)sender
{
	RSSEuphoriaSettings * tEuphoriaSettings=(RSSEuphoriaSettings *) sceneSettings;
	
	if (tEuphoriaSettings.meshDensity!=[sender integerValue])
	{
		tEuphoriaSettings.meshDensity=[sender integerValue];
		
		[_meshDensityLabel setIntegerValue:tEuphoriaSettings.meshDensity];
		
		[self _setAsCustomSet];
	}
}

- (IBAction)setVisibility:(id)sender
{
	RSSEuphoriaSettings * tEuphoriaSettings=(RSSEuphoriaSettings *) sceneSettings;
	
	if (tEuphoriaSettings.visibility!=[sender integerValue])
	{
		tEuphoriaSettings.visibility=[sender integerValue];
		
		[_visibilityLabel setIntegerValue:tEuphoriaSettings.visibility];
		
		[self _setAsCustomSet];
	}
}

- (IBAction)setSpeed:(id)sender
{
	RSSEuphoriaSettings * tEuphoriaSettings=(RSSEuphoriaSettings *) sceneSettings;
	
	if (tEuphoriaSettings.speed!=[sender integerValue])
	{
		tEuphoriaSettings.speed=[sender integerValue];
		
		[_speedLabel setIntegerValue:tEuphoriaSettings.speed];
		
		[self _setAsCustomSet];
	}
}

- (IBAction)setFeedback:(id)sender
{
	RSSEuphoriaSettings * tEuphoriaSettings=(RSSEuphoriaSettings *) sceneSettings;
	
	if (tEuphoriaSettings.feedback!=[sender integerValue])
	{
		tEuphoriaSettings.feedback=[sender integerValue];
		
		[_feedbackLabel setIntegerValue:tEuphoriaSettings.feedback];
		
		[self _setAsCustomSet];
	}
}

- (IBAction)setFeedbackSpeed:(id)sender
{
	RSSEuphoriaSettings * tEuphoriaSettings=(RSSEuphoriaSettings *) sceneSettings;
	
	if (tEuphoriaSettings.feedbackSpeed!=[sender integerValue])
	{
		tEuphoriaSettings.feedbackSpeed=[sender integerValue];
		
		[_feedbackSpeedLabel setIntegerValue:tEuphoriaSettings.feedbackSpeed];
		
		[self _setAsCustomSet];
	}
}

- (IBAction)setFeedbackTextureSize:(id)sender
{
	RSSEuphoriaSettings * tEuphoriaSettings=(RSSEuphoriaSettings *) sceneSettings;
	
	if (tEuphoriaSettings.feedbackTextureSize!=[sender integerValue])
	{
		tEuphoriaSettings.feedbackTextureSize=[sender integerValue];
		
		[_feedbackTextureSizeLabel setIntegerValue:tEuphoriaSettings.feedbackTextureSize];
		
		[self _setAsCustomSet];
	}
}

- (IBAction)setTextureType:(id)sender
{
	RSSEuphoriaSettings * tEuphoriaSettings=(RSSEuphoriaSettings *) sceneSettings;
	
	if (tEuphoriaSettings.texture!=[sender selectedTag])
	{
		tEuphoriaSettings.texture=[sender selectedTag];
		
		[self _setAsCustomSet];
	}
}

- (IBAction)setWireframe:(id)sender
{
	RSSEuphoriaSettings * tEuphoriaSettings=(RSSEuphoriaSettings *) sceneSettings;
	
	if (tEuphoriaSettings.showWireframe!=([sender state]==NSOnState))
	{
		tEuphoriaSettings.showWireframe=([sender state]==NSOnState);
		
		[self _setAsCustomSet];
	}
}

#pragma mark -

- (BOOL)RSS_collectionView:(NSCollectionView *)inCollectionView shouldSelectItemAtIndex:(NSInteger)inIndex
{
	RSSCollectionViewItem * tCollectionViewItem=(RSSCollectionViewItem *)[_settingsCollectionView itemAtIndex:inIndex];
	
	if (tCollectionViewItem!=nil)
	{
		NSInteger tTag=tCollectionViewItem.tag;
		
		if (tTag==RSSEuphoriaSetCustom)
			return NO;
	}
	
	return YES;
}

- (void)RSS_collectionViewSelectionDidChange:(NSNotification *)inNotification
{
	if (inNotification.object==_settingsCollectionView)
	{
		NSIndexSet * tIndexSet=[_settingsCollectionView selectionIndexes];
		NSUInteger tIndex=[tIndexSet firstIndex];
		
		RSSCollectionViewItem * tCollectionViewItem=(RSSCollectionViewItem *)[_settingsCollectionView itemAtIndex:tIndex];
		
		if (tCollectionViewItem!=nil)
		{
			NSInteger tTag=tCollectionViewItem.tag;
			RSSEuphoriaSettings * tEuphoriaSettings=(RSSEuphoriaSettings *) sceneSettings;
			
			if (tEuphoriaSettings.standardSet!=tTag)
			{
				tEuphoriaSettings.standardSet=tTag;
				
				if (tTag!=RSSEuphoriaSetRandom)
				{
					[tEuphoriaSettings resetSettingsToStandardSet:tTag];
					
					[self restoreUI];
				}
			}
		}
	}
}

@end
