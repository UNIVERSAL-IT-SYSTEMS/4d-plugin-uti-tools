/* --------------------------------------------------------------------------------
 #
 #	4DPlugin.cpp
 #	source generated by 4D Plugin Wizard
 #	Project : uti-tools
 #	author : miyako
 #	2015/08/11
 #
 # --------------------------------------------------------------------------------*/


#include "4DPluginAPI.h"
#include "4DPlugin.h"

void PluginMain(PA_long32 selector, PA_PluginParameters params)
{
	try
	{
		PA_long32 pProcNum = selector;
		sLONG_PTR *pResult = (sLONG_PTR *)params->fResult;
		PackagePtr pParams = (PackagePtr)params->fParameters;

		CommandDispatcher(pProcNum, pResult, pParams); 
	}
	catch(...)
	{

	}
}

void CommandDispatcher (PA_long32 pProcNum, sLONG_PTR *pResult, PackagePtr pParams)
{
	switch(pProcNum)
	{
// --- Path

		case 1 :
			PATH_GET_COMPONENTS(pResult, pParams);
			break;

		case 2 :
			PATH_Get_name(pResult, pParams);
			break;

		case 3 :
			PATH_Get_directory_path(pResult, pParams);
			break;

// --- Conversion

		case 4 :
			UTI_To_ostype(pResult, pParams);
			break;

		case 5 :
			UTI_To_mime(pResult, pParams);
			break;

		case 6 :
			UTI_To_extension(pResult, pParams);
			break;

		case 7 :
			UTI_From_ostype(pResult, pParams);
			break;

		case 8 :
			UTI_From_mime(pResult, pParams);
			break;

		case 9 :
			UTI_From_extension(pResult, pParams);
			break;

// --- Comparision

		case 10 :
			UTI_Equal(pResult, pParams);
			break;

		case 11 :
			UTI_Conforms_to(pResult, pParams);
			break;

// --- Information

		case 12 :
			UTI_Get_declaration(pResult, pParams);
			break;

		case 13 :
			UTI_Get_localized_description(pResult, pParams);
			break;

		case 14 :
			UTI_Get_icon(pResult, pParams);
			break;

		case 15 :
			UTI_Get_application(pResult, pParams);
			break;

		case 16 :
			UTI_Get_description(pResult, pParams);
			break;

// --- Path

		case 17 :
			PATH_Get_uti(pResult, pParams);
			break;

		case 18 :
			PATH_OPEN_WITH_APPLICATION(pResult, pParams);
			break;

		case 19 :
			PATH_Get_thumbnail(pResult, pParams);
			break;

	}
}

#pragma mark -

void PATH_OPEN_WITH_APPLICATION(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_TEXT Param2;
	C_LONGINT Param3;

	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);

    NSString *path = Param2.copyPath();
    NSURL *url = Param1.copyUrl();
    if(url){
        NSBundle *b = [NSBundle bundleWithPath:path];
        if(b){
            NSString *bundleIdentifier = [b bundleIdentifier];
            [[NSWorkspace sharedWorkspace]openURLs:[NSArray arrayWithObject:url]
                withAppBundleIdentifier:bundleIdentifier
                options:Param3.getIntValue()
                additionalEventParamDescriptor:[NSAppleEventDescriptor nullDescriptor]
                launchIdentifiers:nil];
        }
        [url release];
    }
    [path release];
}

void PATH_Get_thumbnail(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_REAL Param2;
	C_REAL Param3;

	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);

    CGFloat width = Param2.getDoubleValue();
    CGFloat height = Param3.getDoubleValue();
    
    NSURL *url = Param1.copyUrl();
    
    if(url){
        CFDictionaryRef options = CFDictionaryCreate(kCFAllocatorDefault, NULL, NULL, 0, NULL, NULL);
        CGImageRef image = QLThumbnailImageCreate(kCFAllocatorDefault, (CFURLRef)url, CGSizeMake(width, height), options);
        
        if(image){
            
            CFMutableDataRef data = CFDataCreateMutable(kCFAllocatorDefault, 0);
            CGImageDestinationRef destination = CGImageDestinationCreateWithData(data, kUTTypeTIFF, 1, NULL);
            CFMutableDictionaryRef properties = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, NULL, NULL);
            
            CGImageDestinationAddImage(destination, image, properties);
            CGImageDestinationFinalize(destination);
            
            //return picture without memory leak
            PA_Picture picture = PA_CreatePicture((void *)CFDataGetBytePtr(data), CFDataGetLength(data));
            *(PA_Picture*) pResult = picture;
            
            CFRelease(properties);
            CFRelease(destination);
            CFRelease(data);		
            
            CFRelease(image);
        }
        CFRelease(options);	
        [url release];			
    }

}

void PATH_GET_COMPONENTS(sLONG_PTR *pResult, PackagePtr pParams)
{
    C_TEXT Param1;
    ARRAY_TEXT Param2;
    
    Param1.fromParamAtIndex(pParams, 1);
    Param2.setSize(1);
    
    NSString *path = Param1.copyPath();
    NSArray *pathComponents = [path pathComponents];
    
    for(int i = 0; i < [pathComponents count]; ++i){
        if(![[pathComponents objectAtIndex:i]isEqualToString:@"/"])
            Param2.appendUTF16String([pathComponents objectAtIndex:i]);
    }
    
    [path release];
    
    Param2.toParamAtIndex(pParams, 2);
}

void PATH_Get_name(sLONG_PTR *pResult, PackagePtr pParams)
{
    C_TEXT Param1;
    C_TEXT Param2;
    C_TEXT Param3;
    C_TEXT returnValue;
    
    Param1.fromParamAtIndex(pParams, 1);
    
    NSString *path = Param1.copyPath();
    returnValue.setUTF16String([path lastPathComponent]);
    Param2.setUTF16String([[path lastPathComponent]stringByDeletingPathExtension]);
    Param3.setUTF16String([[path lastPathComponent]pathExtension]);
    [path release];
    
    Param2.toParamAtIndex(pParams, 2);
    Param3.toParamAtIndex(pParams, 3);
    returnValue.setReturn(pResult);
}

void PATH_Get_directory_path(sLONG_PTR *pResult, PackagePtr pParams)
{
    C_TEXT Param1;
    C_TEXT returnValue;
    
    Param1.fromParamAtIndex(pParams, 1);
    
    NSString *path = Param1.copyPath();
    NSString *directortyPath = [path stringByDeletingLastPathComponent];
    
    if([directortyPath length]){
        NSURL *url = [[NSURL alloc]initFileURLWithPath:directortyPath isDirectory:YES];
        directortyPath = (NSString *)CFURLCopyFileSystemPath((CFURLRef)url, kCFURLHFSPathStyle);
        if(directortyPath){
            returnValue.setUTF16String(directortyPath);
            [directortyPath release];
        }
    }    
    [path release];
    
    returnValue.setReturn(pResult);
}

void PATH_Get_uti(sLONG_PTR *pResult, PackagePtr pParams)
{
    C_TEXT Param1;
    C_TEXT returnValue;
    
    Param1.fromParamAtIndex(pParams, 1);
    
    NSString *path = Param1.copyPath();
    if([path length]){
        NSError *error;
        NSString *uti = [[NSWorkspace sharedWorkspace]typeOfFile:path error:&error];
        if(uti){
            returnValue.setUTF16String(uti);
        }
    }

    [path release];
    
    returnValue.setReturn(pResult);
}

// ---------------------------------- Conversion ----------------------------------

#pragma mark -

void UTI_To_ostype(sLONG_PTR *pResult, PackagePtr pParams)
{
    C_TEXT Param1;
    C_TEXT returnValue;
    
    Param1.fromParamAtIndex(pParams, 1);
    
    NSString *uti = Param1.copyUTF16String();
    NSString *type = (NSString *)UTTypeCopyPreferredTagWithClass((CFStringRef)uti, kUTTagClassOSType);
    if(type){
        returnValue.setUTF16String(type);
        [type release];
    }
    [uti release];
    
    returnValue.setReturn(pResult);
}

void UTI_To_mime(sLONG_PTR *pResult, PackagePtr pParams)
{
    C_TEXT Param1;
    C_TEXT returnValue;
    
    Param1.fromParamAtIndex(pParams, 1);
    
    NSString *uti = Param1.copyUTF16String();
    NSString *mime = (NSString *)UTTypeCopyPreferredTagWithClass((CFStringRef)uti, kUTTagClassMIMEType);
    if(mime){
        returnValue.setUTF16String(mime);
        [mime release];
    }
    [uti release];
    
    returnValue.setReturn(pResult);
}

void UTI_To_extension(sLONG_PTR *pResult, PackagePtr pParams)
{
    C_TEXT Param1;
    C_TEXT returnValue;
    
    Param1.fromParamAtIndex(pParams, 1);
    
    NSString *uti = Param1.copyUTF16String();
    NSString *extension = (NSString *)UTTypeCopyPreferredTagWithClass((CFStringRef)uti, kUTTagClassFilenameExtension);
    if(extension){
        returnValue.setUTF16String(extension);
        [extension release];
    }
    [uti release];
    
    returnValue.setReturn(pResult);
}

void UTI_From_ostype(sLONG_PTR *pResult, PackagePtr pParams)
{
    C_TEXT Param1;
    C_TEXT returnValue;
    
    Param1.fromParamAtIndex(pParams, 1);
    
    NSString *type = Param1.copyUTF16String();
    NSString *uti = (NSString *)UTTypeCreatePreferredIdentifierForTag(kUTTagClassOSType, (CFStringRef)type, NULL);
    if(uti){
        returnValue.setUTF16String(uti);
        [uti release];
    }
    [type release];
    
    returnValue.setReturn(pResult);
}

void UTI_From_mime(sLONG_PTR *pResult, PackagePtr pParams)
{
    C_TEXT Param1;
    C_TEXT returnValue;
    
    Param1.fromParamAtIndex(pParams, 1);
    
    NSString *mime = Param1.copyUTF16String();
    NSString *uti = (NSString *)UTTypeCreatePreferredIdentifierForTag(kUTTagClassMIMEType, (CFStringRef)mime, NULL);
    if(uti){
        returnValue.setUTF16String(uti);
        [uti release];
    }
    [mime release];
    
    returnValue.setReturn(pResult);
}

void UTI_From_extension(sLONG_PTR *pResult, PackagePtr pParams)
{
    C_TEXT Param1;
    C_TEXT returnValue;
    
    Param1.fromParamAtIndex(pParams, 1);
    
    NSString *extension = Param1.copyUTF16String();
    
    if([[extension lowercaseString]isEqualToString:@"key"]){
        returnValue.setUTF16String(@"com.apple.iwork.keynote.sffkey");
    }else if([[extension lowercaseString]isEqualToString:@"pages"]){
        returnValue.setUTF16String(@"com.apple.iwork.pages.sffpages");
    }else if([[extension lowercaseString]isEqualToString:@"numbers"]){
        returnValue.setUTF16String(@"com.apple.iwork.numbers.sffnumbers");
    }else{

        NSString *uti = (NSString *)UTTypeCreatePreferredIdentifierForTag(kUTTagClassFilenameExtension, (CFStringRef)extension, NULL);
        if(uti){
            returnValue.setUTF16String(uti);
            [uti release];
        }
        [extension release];

    }
    returnValue.setReturn(pResult);
}

// ---------------------------------- Comparision ---------------------------------

#pragma mark -

void UTI_Equal(sLONG_PTR *pResult, PackagePtr pParams)
{
    C_TEXT Param1;
    C_TEXT Param2;
    C_LONGINT returnValue;
    
    Param1.fromParamAtIndex(pParams, 1);
    Param2.fromParamAtIndex(pParams, 2);
    
    NSString *uti1 = Param1.copyUTF16String();
    NSString *uti2 = Param2.copyUTF16String();
    returnValue.setIntValue(UTTypeEqual((CFStringRef)uti1, (CFStringRef)uti2));
    [uti2 release];
    [uti1 release];
    
    returnValue.setReturn(pResult);
}

void UTI_Conforms_to(sLONG_PTR *pResult, PackagePtr pParams)
{
    C_TEXT Param1;
    C_TEXT Param2;
    C_LONGINT returnValue;
    
    Param1.fromParamAtIndex(pParams, 1);
    Param2.fromParamAtIndex(pParams, 2);
    
    NSString *uti1 = Param1.copyUTF16String();
    NSString *uti2 = Param2.copyUTF16String();
    returnValue.setIntValue(UTTypeConformsTo((CFStringRef)uti1, (CFStringRef)uti2));
    [uti2 release];
    [uti1 release];
    
    
    returnValue.setReturn(pResult);
}

// ---------------------------------- Information ---------------------------------

#pragma mark -

void UTI_Get_declaration(sLONG_PTR *pResult, PackagePtr pParams)
{
    C_TEXT Param1;
    C_TEXT returnValue;
    
    Param1.fromParamAtIndex(pParams, 1);
    
    NSString *uti = Param1.copyUTF16String();
    NSDictionary *dictionary = (NSDictionary *)UTTypeCopyDeclaration((CFStringRef)uti);
    if(dictionary){
        NSData *data = (NSData *)CFPropertyListCreateXMLData(kCFAllocatorDefault, dictionary);
        if(data){
            NSString *xml = [[NSString alloc]initWithBytes:[data bytes] length:[data length] encoding:NSUTF8StringEncoding];
            returnValue.setUTF16String(xml);
            [xml release];
            [data release];
        }
        
        [dictionary release];
    }
    
    [uti release];
    
    returnValue.setReturn(pResult);
}

void UTI_Get_localized_description(sLONG_PTR *pResult, PackagePtr pParams)
{
    C_TEXT Param1;
    C_TEXT returnValue;
    
    Param1.fromParamAtIndex(pParams, 1);
    
    NSString *uti = Param1.copyUTF16String();
    returnValue.setUTF16String([[NSWorkspace sharedWorkspace]localizedDescriptionForType:uti]);
    [uti release];
    
    returnValue.setReturn(pResult);
}

void UTI_Get_icon(sLONG_PTR *pResult, PackagePtr pParams)
{
    C_TEXT Param1;
    
    Param1.fromParamAtIndex(pParams, 1);
    
    NSString *uti = Param1.copyUTF16String();
    NSImage *icon = [[NSWorkspace sharedWorkspace]iconForFileType:uti];
    NSRect imageRect = NSMakeRect(0, 0, DEFAULT_ICON_SIZE, DEFAULT_ICON_SIZE);
    CGImageRef image = [icon CGImageForProposedRect:(NSRect *)&imageRect context:NULL hints:NULL];
    CFMutableDataRef data = CFDataCreateMutable(kCFAllocatorDefault, 0);
    CGImageDestinationRef destination = CGImageDestinationCreateWithData(data, kUTTypeTIFF, 1, NULL);
    CFMutableDictionaryRef properties = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, NULL, NULL);
    CGImageDestinationAddImage(destination, image, properties);
    CGImageDestinationFinalize(destination);
    
    //return picture without memory leak
    PA_Picture picture = PA_CreatePicture((void *)CFDataGetBytePtr(data), CFDataGetLength(data));
    *(PA_Picture*) pResult = picture;

    CFRelease(destination);
    CFRelease(properties);
    CFRelease(data);
    
    [uti release];
}

void UTI_Get_application(sLONG_PTR *pResult, PackagePtr pParams)
{
    C_TEXT Param1;
    C_TEXT returnValue;
    
    Param1.fromParamAtIndex(pParams, 1);
    
    NSString *uti = Param1.copyUTF16String();
    NSString *mime = (NSString *)UTTypeCopyPreferredTagWithClass((CFStringRef)uti, kUTTagClassMIMEType);
    if(mime){
        CFURLRef url;
        //use LSCopyDefaultApplicationURLForContentType in 10.10
        if(noErr == LSCopyApplicationForMIMEType((CFStringRef)mime, kLSRolesAll, &url)){
            NSString *path = (NSString *)CFURLCopyFileSystemPath((CFURLRef)url, kCFURLHFSPathStyle);
            if(path){
                returnValue.setUTF16String(path);
                [path release];
            }
        }
        
        [mime release];
    }
    
    [uti release];
    
    returnValue.setReturn(pResult);
}

void UTI_Get_description(sLONG_PTR *pResult, PackagePtr pParams)
{
    C_TEXT Param1;
    C_TEXT returnValue;
    
    Param1.fromParamAtIndex(pParams, 1);
    
    NSString *uti = Param1.copyUTF16String();
    NSString *description = (NSString *)UTTypeCopyDescription((CFStringRef)uti);
    if(description){
        returnValue.setUTF16String(description);
        [description release];
    }
    [uti release];
    
    returnValue.setReturn(pResult);
}

