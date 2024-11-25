#  ProgressiveImgLoad

ProgressiveImgLoad is a project that enables progressive loading of images, providing a smoother user experience by gradually displaying the image as more data becomes available.

## Features

- **Progressive Image Loading**: Loads images in chunks to display a preview quickly and then updates with higher quality as more data is received.
- **Customizable**: Offers flexibility to adjust parameters such as the number of chunks and the delay between updates.

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/qinglong61/ProgressiveImgLoad.git
   ```
2. Open the project in Xcode.

## Usage

1. Import the `ImgLoadTest.h` header in your view controller:
   ```objc
   #import "ImgLoadTest.h"
   ```
2. Call the `testImgLoad` function and pass the view where you want to display the image. For example, in the `viewDidLoad` method of your view controller:
   ```objc
   - (void)viewDidLoad {
       [super viewDidLoad];
       testImgLoad(self.view);
   }
   ```

## How It Works

The project uses the `findIDATInfoInNSData` function to extract information about the IDAT chunks in the image data. These chunks contain the actual image data. The `extractIDATFromData` function then splits the image data into multiple parts based on the IDAT chunks. The `updateImg` function is used to update the image view with the progressively loaded image data.

## Example

Here is an example of how to use the library to load a test image:

```objc
UIImageView *imgView = UIImageView.new;
imgView.contentMode = UIViewContentModeScaleAspectFit;
UIView *box = UIView.new;
box.backgroundColor = UIColor.whiteColor;
[box addSubview:imgView];
imgView.frame = box.bounds;
UIImage *image = testImgMake(CGSizeMake(1000, 1000), UIColor.blackColor, @"Progressive\nImage\nLoading\nTest");
NSData *imgData = UIImagePNGRepresentation(image);
NSArray *dataArr = extractIDATFromData(imgData);
updateImg(imgView, dataArr, 0);
```

## Contributing

Contributions are welcome! Please fork the repository and create a pull request with your changes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
