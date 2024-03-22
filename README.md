# Advanced Programming Group Project
This C++ application is designed for image processing, offering both 2D and 3D processing capabilities. It provides a user-friendly interface for applying various image processing techniques such as edge detection, blurring, color correction, and more. Users can choose between 2D and 3D processing modes based on their requirements.

## Key Features

- User Interface: Utilize a command-line interface for user interaction and execution of processing techniques.
- 2D Processing: Apply image processing techniques to 2D images, including edge detection, blurring, and color correction.
- 3D Processing: Perform advanced image processing on volumetric data, offering techniques like volume rendering, projection, filtering, and slicing.
- Saving Output: Save processed images to designated directories.
- Integration: Incorporates efficient image processing classes for seamless operation.
- Unittesting: Implement unit tests to ensure the correctness and reliability of the implemented image processing algorithms.


## Folder structure:
- Report: Contains the group project report in PDF format.
- Assignment_PDF: Original assignment PDF for reference.
- Images: Directory for storing input images.
- Output: Directory for storing processed images.
- Scans: Directory for storing CT scan datasets.
- src: Contains the source code files.
- README.md: This README file.
- References.md: Contains references used in the project.
- LICENSE: License file detailing permissions and restrictions.
- dependencies.txt: Text file listing the project dependencies.
- Appendix_outputs/EdgeDetection: Contains test images for edge detection, when attempting to produce the most accurate/suitable images. E.g. RobertsCross_test contains images made with different pre/post processing techniques such as blur, threshold, etc.

Please see the pdf document ("GroupProject_Report_Selection-Sort.pdf") for overview.


## Usage

1. **Installation**: Clone the repository to your local machine.
    ```bash
    git clone <this_repository_url>
    ```

    ```bash
    cd <repository_directory>
    ```

2. **Dependencies**: Ensure you have the necessary dependencies installed:
    - C++ compiler supporting C++17 or later.
    - Standard libraries: e.g. iostream, filesystem, string.

3. **Compilation**: Compile the source files to build the executable:
    - For clang++
    ```bash
    clang++ -std=c++17 -o main main.cpp ImageBlur.cpp Image.cpp EdgeDetection.cpp ColourCorrection.cpp User_2D.cpp Projection.cpp Slice.cpp ThreeDFilter.cpp User_3D.cpp Volume.cpp User_unitTests.cpp ColourCorrectionTest.cpp EdgeDetectionTest.cpp ImageBlurTest.cpp ProjectionTest.cpp ThreeDFilterTest.cpp
    ```
    
    - For g++
    ```bash
    g++ -std=c++17 -o main main.cpp ImageBlur.cpp Image.cpp EdgeDetection.cpp ColourCorrection.cpp User_2D.cpp Projection.cpp Slice.cpp ThreeDFilter.cpp User_3D.cpp Volume.cpp User_unitTests.cpp ColourCorrectionTest.cpp EdgeDetectionTest.cpp ImageBlurTest.cpp ProjectionTest.cpp ThreeDFilterTest.cpp
    ```

4. **Execution**
    ```bash
    ./main
    ```
    Follow the on-screen instructions to interact with the application and process 2D or 3D datasets.
    - Mode Selection: Choose the mode you would like to use (2D or 3D)
    - Image Selection: Select an image from the provided list by entering the corresponding number.
    - Filter Selection: Choose a filter to apply to the selected image by entering the corresponding number.
    - Filter Parameters: If the selected filter requires parameters, enter them when prompted.
    - Output: View the processed image and optionally save it to a specified directory.
    - Exit: Choose the "Save and Exit" option to exit the program.


## Explanation of 2D Processing

The User_2D.cpp file implements user interactions for applying various image processing techniques in the 2D mode. Here's a breakdown of its functionalities:

- Selecting Images: Allows users to choose an image from a list presented in the command-line interface.
- Applying Filters: Provides options to apply different filters such as edge detection, blurring, and color correction to the selected image.
- Multiple Filters: Supports applying multiple filters to an image and saving the resulting images with descriptive filenames.
- Saving Processed Images: Enables users to save the processed images to a specific directory for further analysis or reference.


## Explanation of 3D Processing

The 3D processing functionalities of the application, implemented in the User_3D.cpp and User_3D.h files, enable users to perform advanced image processing on volumetric data. Below is an overview of the key functionalities:

- Dataset Selection: Allows users to choose a dataset ffrom a list presented in the command-line interface.
- Filter Application: Offers options for Gaussian and Median filters, or no filter and allows users to specify filter parameters such as kernel size and sigma value for Gaussian filter.
- Projection Generation: Generates projections of the processed volume data and supports different projection types based on the dataset (e.g., MIP, AIP).
- Slice Generation: Allows users to choose between default or custom slice indices from the processed volume data, and generates slices along the XZ and YZ planes and saves them to the output directory.
- Slab Generation: Offers options for default or custom slab indices from the processed bolume data, and generates slabs along the Z-axis and saves them to the output directory.


## Explanation of Unit Testing
The User_unitTests provides a framework for executing 2D and 3D image processing unit tests. This framework is designed to validate the correctness and performance of these image processing algorithms.

- Selecting Test Categories: Upon starting the program, a list of available test categories will be displayed, including Color Correction tests, Image Blur tests, Edge Detection tests, 3D Filter tests, and Projection tests.
- Running Specific Tests: After selecting a category, the specific test options available under that category will show up, such as Grayscale Conversion and Brightness Adjustment for Color Correction. Users can execute a test by entering the corresponding test number as prompted.
- View Test Results: Each test will execute its respective image processing operation and compare its output to see if it conforms to the characteristics of this filter. For instance, the output of Grayscale Conversion should only have 1 channel. Pass or Fail will be displayed along with the reasons for success or failure, and some parameter comparisons before and after image processing.
- Repeating or Exiting: After completing a test, users have the option to continue running other tests within the same category, return to the main menu to select a different category, or exit the test program.

<details>
  <summary>Click to expand/collapse to see the testing details</summary>

  ## Unit Testing Detail

  - Colour Correction Tests: For grayscale conversion, it verifies the image has been transformed to grayscale by checking for a single channel. Brightness adjustment is assessed by comparing the average brightness before and after the adjustment, expecting an increase. Histogram equalization's success is measured by an expanded intensity distribution, indicated by a greater standard deviation in the histogram. Thresholding tests ensure the image is properly segmented into binary format, with pixels being either black or white. Salt and pepper noise addition is evaluated by the expected increase in black and white pixels, ensuring the noise is accurately applied.
  
  - Image Blur Tests: For the Median Blur test, the test initially calculates the original noise level of the image by counting pixels at the extremes of the intensity spectrum. After applying the median blur filter to an image with 15% salt and pepper noise (renowned for its ability to reduce such noise), the test recalculates the noise level. A successful reduction of noise by 15% confirms the effectiveness of the filter. The Box Blur  test and Gaussian Blur test both start by calculating the standard deviation of the original image to establish a baseline for image contrast and sharpness. After the application of each blur filter, a new standard deviation is calculated. The expectation for a successful test is a lower standard deviation in the blurred image compared to the original, indicating the image has become smoother. This demonstrates the blur filters' capacity to soften the image, reduce details, and noise.

  - Edge Detection Tests: Each test applies its respective filter to an image and then evaluates the result by calculating the average gradient magnitude before and after the application, as color value changes between pixels will sharply increase after edge enhancement. A successful edge detection algorithm will increase the image's average gradient magnitude, indicating that the edges within the image have been enhanced and made more distinguishable.

  - 3D Filter Tests:  Gaussian Blur test applies a Gaussian blur with a specified kernel size and sigma value to the volume. The Gaussian blur is known for its ability to smooth features while preserving edges by giving more weight to the pixels closest to the center of the kernel. The test checks if the standard deviation of the volume's intensity values decreases, which would indicate the blur's effectiveness in smoothing the volume and reducing noise.  Median Blur test Implements a Median blur, which replaces each pixel with the median intensity value from its neighboring pixels within a defined kernel size. This filter is particularly effective against salt and pepper noise and does not assume a normal distribution of intensity values, making it robust for various applications. The test validates the filter's performance by observing a decrease in the standard deviation of the volume's intensity values, demonstrating its capability to reduce noise and smooth the data.

  - Projection Tests:  
    - For the MIP test, it applies the MIP method to a volume, creating an image where each pixel represents the maximum intensity encountered along a particular line of sight through the volume. The test verifies this by comparing the intensity of each pixel in the output image with the maximum intensity found across the corresponding z-stack in the volume data. If the intensities match, the test confirms the MIP projection's accuracy.

    - The MINIP test similarly projects the volume into a 2D image, but instead focuses on capturing the minimum intensity value found along each line of sight. This projection is particularly useful for highlighting darker features within a volume. The test checks if the output image's pixel intensities correctly represent the minimum intensity values from the volume, ensuring the MINIP projection is correctly implemented.

    - For the AIP test, the method computes an average intensity projection, which averages the intensity values encountered along each line of sight through the volume. This technique provides a more comprehensive view of the volume's internal structures. The test evaluates the accuracy of the AIP by comparing the output image's pixel intensities against the expected average intensities calculated from the volume data. A match confirms the AIP's effectiveness.think you will need to build 
</details>


## Expected Result

Upon running the program, users will be prompted to choose between 2D and 3D processing modes. Based on their selection, they can apply various image processing techniques to their images or volumetric data. The expected result includes enhanced or modified images in the case of 2D processing, and advanced analysis or visualization of volumetric data in the case of 3D processing.


## Dependencies

- User_2D.h
- User_3D.h
- Image.h
- EdgeDetection.h
- ImageBlur.h
- ColourCorrection.h
- Projection.h
- Slice.h
- ThreeDFilter.h
- Standard libraries: iostream, filesystem, string.


## Contributors

- Laurel Kyte-Buxton, edsml-lwk16
- Saffron Taylor, edsml-st2923
- Hung-Hsuan (Hanson) Shen, edsml-hs1623
- Yifei (Allison) Dou, acse-ad2123
- Xueling Gao, acse-xg1123
- Yue Wang, acse-yw3523


## Doxygen Documentation
The project includes Doxygen-generated documentation accessible via the index.html file. This file acts as a gateway to detailed documentation for all classes and functions within the project. It offers developers a structured overview of the codebase, facilitating easy navigation and understanding of the project's architecture and implementation details.


## CT Scans
Download CT Scan datasets here:
https://imperiallondon-my.sharepoint.com/:u:/g/personal/tmd02_ic_ac_uk/EafXMuNsbcNGnRpa8K62FjkBvIKvCswl1riz7hPDHpHdSQ


## References
Please see the [references.txt](https://github.com/ese-msc-2023/advanced-programming-group-selection-sort/blob/2D_Hanson/References.md) file.

## Licence
Please find the MIT License for this repository [here](https://github.com/ese-msc-2023/advanced-programming-group-selection-sort/blob/Final_Merge/LICENCE)