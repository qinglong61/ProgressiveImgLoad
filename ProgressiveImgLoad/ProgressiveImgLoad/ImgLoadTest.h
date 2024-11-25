//
//  ImgLoadTest.h
//  ProgressiveImgLoad
//
//  Created by 段清伦(qinglong61@163.com) on 2024/11/25.
//

typedef struct
{
    NSUInteger offset;
    NSUInteger length;
    NSUInteger count;
    NSMutableArray<NSNumber *> *chunks;
}
IDATInfo;

IDATInfo findIDATInfoInNSData(NSData *imageData)
{
    IDATInfo result = {0};
    result.chunks = NSMutableArray.new;
    const unsigned char *bytes = (const unsigned char *)imageData.bytes;
    NSUInteger dataLength = imageData.length;
    NSUInteger currentOffset = 8;
    while (currentOffset < dataLength) {
        NSUInteger chunkLength = CFSwapInt32BigToHost(*(const UInt32 *)(bytes + currentOffset));
        NSString *chunkType = [[NSString alloc] initWithBytes:&bytes[currentOffset + 4] length:4 encoding:NSASCIIStringEncoding];
        if ([chunkType isEqualToString:@"IDAT"]) {
            if (result.count == 0) {
                result.offset = currentOffset;
            }
            result.count++;
            [result.chunks addObject:@(chunkLength)];
        }
        if ([chunkType isEqualToString:@"IEND"]) {
            result.length = currentOffset - result.offset;
        }
        currentOffset += chunkLength + 12;
    }
    return result;
}

NSData *endData(void)
{
    Byte bytes[] = {0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82};
    return [NSData dataWithBytes:bytes length:12];
}

NSArray *extractIDATFromData(NSData *imageData)
{
    IDATInfo idatInfo = findIDATInfoInNSData(imageData);
    
    NSData *header = [imageData subdataWithRange:NSMakeRange(0, idatInfo.offset)];
    NSData *chunk = [imageData subdataWithRange:NSMakeRange(idatInfo.offset, idatInfo.length)];
    NSData *end = endData();
    
    NSMutableData *partData = NSMutableData.new;
    NSMutableArray *splitDataArray = NSMutableArray.new;
    NSUInteger n = idatInfo.count;
    NSUInteger offset = 0;
    for (NSUInteger i = 0; i < n; i++) {
        NSUInteger length = [idatInfo.chunks[i] unsignedIntegerValue] + 12;
        
        [partData appendData:[chunk subdataWithRange:NSMakeRange(offset, length)]];
        
        NSMutableData *tmp = NSMutableData.new;
        [tmp appendData:header];
        [tmp appendData:partData];
        [tmp appendData:end];
        
        [splitDataArray addObject:tmp];
        
        offset += length;
    }

    return splitDataArray;
}

void updateImg(UIImageView *imgv, NSArray *dataArr, int idx)
{
    imgv.image = [UIImage imageWithData:(dataArr[idx])];
    if (idx < dataArr.count-1) {
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            updateImg(imgv, dataArr, idx+1);
        });
    }
}

UIImage *testImgMake(CGSize size, UIColor *color, NSString *text)
{
    if (!color || size.width <= 0 || size.height <= 0) return nil;
    CGRect rect = CGRectMake(0.0f, 0.0f, size.width, size.height);
    UIGraphicsBeginImageContextWithOptions(rect.size, NO, 0);
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetFillColorWithColor(context, color.CGColor);
    CGContextFillRect(context, rect);
    [text drawAtPoint:CGPointMake(0, 0) withAttributes:@{
        NSFontAttributeName: [UIFont systemFontOfSize:200],
        NSForegroundColorAttributeName: UIColor.whiteColor,
    }];
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return image;
}

void testImgLoad(UIView *box)
{
    UIImageView *imgView = UIImageView.new;
    imgView.contentMode = UIViewContentModeScaleAspectFit;
    box.backgroundColor = UIColor.whiteColor;
    [box addSubview:imgView];
    imgView.frame = box.bounds;
    UIImage *image = testImgMake(CGSizeMake(1000, 1000), UIColor.blackColor, @"Progressive\nImage\nLoading\nTest");
    NSData *imgData = UIImagePNGRepresentation(image);
    NSArray *dataArr = extractIDATFromData(imgData);
    updateImg(imgView, dataArr, 0);
}

