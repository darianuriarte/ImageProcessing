#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

struct Header{
    char  idLength;
    char  colorMapType;
    char  dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char  colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char  bitsPerPixel;
    char  imageDescriptor;
};

struct Pixel {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

class Image {

public :
    Header headerObject;
    string filePath;
    vector<Pixel> pixels;

    Image() {

    }

    Image(string filePath) {
        readingFile(filePath);
    }

    void readingFile(string filePath) {
        fstream file;
        file.open(filePath, ios::in | ios::binary);
        //Reading Header Data
        file.read((char *) &headerObject.idLength, sizeof(headerObject.idLength));
        file.read((char *) &headerObject.colorMapType, sizeof(headerObject.colorMapType));
        file.read((char *) &headerObject.dataTypeCode, sizeof(headerObject.dataTypeCode));
        file.read((char *) &headerObject.colorMapOrigin, sizeof(headerObject.colorMapOrigin));
        file.read((char *) &headerObject.colorMapLength, sizeof(headerObject.colorMapLength));
        file.read((char *) &headerObject.colorMapDepth, sizeof(headerObject.colorMapDepth));
        file.read((char *) &headerObject.xOrigin, sizeof(headerObject.xOrigin));
        file.read((char *) &headerObject.yOrigin, sizeof(headerObject.yOrigin));
        file.read((char *) &headerObject.width, sizeof(headerObject.width));
        file.read((char *) &headerObject.height, sizeof(headerObject.height));
        file.read((char *) &headerObject.bitsPerPixel, sizeof(headerObject.bitsPerPixel));
        file.read((char *) &headerObject.imageDescriptor, sizeof(headerObject.imageDescriptor));
        //Variable Calculations
        int pixelsSize = headerObject.width * headerObject.height;
        int totalBytes = pixelsSize * 3;
        //Reading Color Data
        for (int i = 0; i < pixelsSize; i++) {
            Pixel filePixel;
            file.read((char *) &filePixel.blue, sizeof(filePixel.blue));
            file.read((char *) &filePixel.green, sizeof(filePixel.green));
            file.read((char *) &filePixel.red, sizeof(filePixel.red));
            pixels.push_back(filePixel);
        }
        file.close();
    }
};
   class Filter {
   public:
       Image image1;
       Image image2;
       Image temporalImage;
       Image final;
       void writingFile(string fileName) {
           fstream file;
           file.open(fileName, ios::app | ios::binary);
           file.write((char *) &final.headerObject.idLength, sizeof(final.headerObject.idLength));
           file.write((char *) &final.headerObject.colorMapType, sizeof(final.headerObject.colorMapType));
           file.write((char *) &final.headerObject.dataTypeCode, sizeof(final.headerObject.dataTypeCode));
           file.write((char *) &final.headerObject.colorMapOrigin, sizeof(final.headerObject.colorMapOrigin));
           file.write((char *) &final.headerObject.colorMapLength, sizeof(final.headerObject.colorMapLength));
           file.write((char *) &final.headerObject.colorMapDepth, sizeof(final.headerObject.colorMapDepth));
           file.write((char *) &final.headerObject.xOrigin, sizeof(final.headerObject.xOrigin));
           file.write((char *) &final.headerObject.yOrigin, sizeof(final.headerObject.yOrigin));
           file.write((char *) &final.headerObject.width, sizeof(final.headerObject.width));
           file.write((char *) &final.headerObject.height, sizeof(final.headerObject.height));
           file.write((char *) &final.headerObject.bitsPerPixel, sizeof(final.headerObject.bitsPerPixel));
           file.write((char *) &final.headerObject.imageDescriptor, sizeof(final.headerObject.imageDescriptor));
           //Variable Calculations
           int pixelsSize = final.headerObject.width * final.headerObject.height;
           int totalBytes = pixelsSize * 3;
           Pixel filePixel;
           for (int i = 0; i < pixelsSize; i++) {
               Pixel filePixel;
               filePixel = final.pixels[i];
               file.write((char *) &filePixel.blue, sizeof(filePixel.blue));
               file.write((char *) &filePixel.green, sizeof(filePixel.green));
               file.write((char *) &filePixel.red, sizeof(filePixel.red));
           }
           file.close();
       }
       void Multiply() {
           //Keep Header
           final.headerObject = image1.headerObject;
           //Reset Pixel Values
           final.pixels.clear();
           for(int i = 0 ; i < image1.pixels.size(); i++) {
               Pixel temporal;
               float blue = ((image1.pixels.at(i).blue * image2.pixels.at(i).blue) / 255.0)+ 0.5f;
               float green = ((image1.pixels.at(i).green * image2.pixels.at(i).green) / 255.0)+ 0.5f;
               float red = ((image1.pixels.at(i).red * image2.pixels.at(i).red) / 255.0)+ 0.5f;
               temporal.blue = (unsigned char) blue;
               temporal.green = (unsigned char) green;
               temporal.red = (unsigned char) red;
               final.pixels.push_back(temporal);
           }
       }
       void Subtract() {
           //Keep Header
           final.headerObject = image1.headerObject;
           //Reset Pixel Values
           final.pixels.clear();
           for(int i = 0 ; i < image1.pixels.size(); i++)
           {
               Pixel temporal;
               int blue, green, red;
               if (image1.pixels.at(i).blue > image2.pixels.at(i).blue)
                   blue = image1.pixels.at(i).blue - image2.pixels.at(i).blue;
               else
                   blue = 0;
               if (image1.pixels.at(i).green > image2.pixels.at(i).green)
                   green = image1.pixels.at(i).green - image2.pixels.at(i).green;
               else
                   green = 0;
               if (image1.pixels.at(i).red > image2.pixels.at(i).red)
                   red = image1.pixels.at(i).red - image2.pixels.at(i).red;
               else
                   red = 0;
               temporal.blue =(uint8_t) blue;
               temporal.green =(uint8_t) green;
               temporal.red = (uint8_t)red;
               final.pixels.push_back(temporal);
           }
       }
       void Screen() {
           //Keep Header
           final.headerObject = image1.headerObject;
           //Reset Pixel Values
           final.pixels.clear();
           for(int i = 0 ; i < image1.pixels.size(); i++)
           {
               Pixel temporal;
               float blue = ((1.0f - (1.0f- (image1.pixels.at(i).blue / 255.0f)) * (1.0f - (image2.pixels.at(i).blue / 255.0f))) * 255.0f) + 0.5f;
               float green = ((1.0f - (1.0f- (image1.pixels.at(i).green / 255.0f)) * (1.0f - (image2.pixels.at(i).green / 255.0f))) * 255.0f) + 0.5f;
               float red = ((1.0f - (1.0f- (image1.pixels.at(i).red / 255.0f)) * (1.0f - (image2.pixels.at(i).red / 255.0f))) * 255.0f) + 0.5f;

               temporal.blue = (char)(int)blue;
               temporal.green =(char)(int) green;
               temporal.red =(char)(int) red;
               final.pixels.push_back(temporal);
           }
       }
       void Overlay() {
           //Keep Header
           final.headerObject = image1.headerObject;
           //Reset Pixel Values
           final.pixels.clear();
           float red, blue, green;
           for(int i = 0 ; i < image1.pixels.size(); i++)
           {
               Pixel temporal;
               if (((float)image2.pixels.at(i).blue / 255.0f) <= 0.5 & ((float)image2.pixels.at(i).green / 255.0f) <= 0.5 & ((float)image2.pixels.at(i).red / 255.0f) <= 0.5)
               {
                   blue = (2.0f * ((float)image1.pixels.at(i).blue / 255.0f) * ((float)image2.pixels.at(i).blue / 255.0f)) * 255.0f + 0.5f;
                   green = (2.0f * ((float)image1.pixels.at(i).green / 255.0f) * ((float)image2.pixels.at(i).green / 255.0f)) * 255.0f + 0.5f;
                   red = (2.0f * ((float)image1.pixels.at(i).red / 255.0f) * ((float)image2.pixels.at(i).red / 255.0f)) * 255.0f + 0.5f;
               }
               else
               {
                   blue = (1 - (2 * ((1.0f - ((float)image1.pixels.at(i).blue / 255.0f)) * (1.0f - ((float)image2.pixels.at(i).blue / 255.0f))))) * 255.0f + 0.5f;
                   green = (1 - (2 * ((1.0f - ((float)image1.pixels.at(i).green / 255.0f)) * (1.0f - ((float)image2.pixels.at(i).green / 255.0f))))) * 255.0f + 0.5f;
                   red = (1 - 2 * ((1.0f - ((float)image1.pixels.at(i).red / 255.0f)) * (1.0f - ((float)image2.pixels.at(i).red / 255.0f)))) * 255.0f + 0.5f;
               }
               temporal.blue = (char)(int)blue;
               temporal.green =(char)(int) green;
               temporal.red =(char)(int) red;
               final.pixels.push_back(temporal);
           }

       }
       void IncreaseGreenChannel(int amount){
           //Keep Header
           final.headerObject = image1.headerObject;
           //Reset Pixel Values
           final.pixels.clear();
           for(int i = 0 ; i < image1.pixels.size(); i++) {
               Pixel temporal;
               if (image1.pixels.at(i).green + amount > 255)
                   temporal.green = 255;
               else
                   temporal.green = image1.pixels.at(i).green + amount;

               temporal.blue = image1.pixels.at(i).blue;
               temporal.red = image1.pixels.at(i).red;
               final.pixels.push_back(temporal);
           }
       }
       void multiplyRedBlueChannel(int redAmount, int blueAmount){
           //Keep Header
           final.headerObject = image1.headerObject;
           //Reset Pixel Values
           final.pixels.clear();
           for(int i = 0 ; i < image1.pixels.size(); i++) {
               Pixel temporal;
               if (image1.pixels.at(i).red * redAmount > 255)
                   temporal.red = 255;
               else
                   temporal.red = image1.pixels.at(i).red * redAmount;

               if(image1.pixels.at(i).blue * blueAmount > 255)
                   temporal.blue = 255;
               else
                   temporal.blue = image1.pixels.at(i).blue * blueAmount;

               temporal.green = image1.pixels.at(i).green;
               final.pixels.push_back(temporal);
           }
       }
       void SeparatingChannels(string channel){
           //Keep Header
           final.headerObject = image1.headerObject;
           //Reset Pixel Values
           final.pixels.clear();
           for(int i = 0 ; i < image1.pixels.size(); i++) {
               Pixel temporal;
               if (channel == "green"){
                   temporal.green = image1.pixels.at(i).green;
                   temporal.blue = image1.pixels.at(i).green;
                   temporal.red = image1.pixels.at(i).green;
               }
               if (channel == "red"){
                   temporal.green = image1.pixels.at(i).red;
                   temporal.blue = image1.pixels.at(i).red;
                   temporal.red = image1.pixels.at(i).red;
               }
               if (channel == "blue"){
                   temporal.green = image1.pixels.at(i).blue;
                   temporal.blue = image1.pixels.at(i).blue;
                   temporal.red = image1.pixels.at(i).blue;
               }
               final.pixels.push_back(temporal);
           }
       }
       void Combine() {
           //Keep Header
           final.headerObject = image1.headerObject;
           //Reset Pixel Values
           final.pixels.clear();
           for (int i = 0; i < image1.pixels.size(); i++) {
               Pixel temporal;
               temporal.red = image1.pixels.at(i).red;
               temporal.green = image2.pixels.at(i).green;
               temporal.blue = temporalImage.pixels.at(i).blue;
               final.pixels.push_back(temporal);
           }
       }
       void Rotate () {
           //Keep Header
           final.headerObject = image1.headerObject;
           //Reset Pixel Values
           final.pixels.clear();

           for (int i = image1.pixels.size() -1 ; i >= 0; i--) {
               Pixel temporal;
               temporal.red = image1.pixels.at(i).red;
               temporal.green = image1.pixels.at(i).green;
               temporal.blue = image1.pixels.at(i).blue;
               final.pixels.push_back(temporal);

           }
       }
   };

   int main() {
       void Test(Image final, Image sample, int number);

           //*******************Task 1***********************************
           //Load Images
           Image topLayer("input/layer1.tga");
           Image bottomLayer("input/pattern1.tga");

           //Create Object
           Filter task1;
           task1.image1 = topLayer;
           task1.image2 = bottomLayer;

           //Perform Task
           task1.Multiply();
           //Output Files
           task1.writingFile("output/part1.tga");
           // Test
           Image sample1("examples/EXAMPLE_part1.tga");
           Test(task1.final, sample1, 1);

           //*******************Task 2 ***********************************
           //Load Images
           Image topLayer2("input/layer2.tga");
           Image bottomLayer2("input/car.tga");
           //Create Object
           Filter task2;
           task2.image2 = topLayer2;
           task2.image1 = bottomLayer2;
           //Perform Task
           task2.Subtract();
           //Output Files
           task2.writingFile("output/part2.tga");
           //Test
           Image sample2("examples/EXAMPLE_part2.tga");
           Test(task2.final, sample2, 2);

           //*******************Task 3 ***********************************
       //Load Images
       Image topLayer3("input/layer1.tga");
       Image bottomLayer3("input/pattern2.tga");
       //Multiply
       Filter task3;
       task3.image1 = topLayer3;
       task3.image2 = bottomLayer3;
       task3.Multiply();
       task3.temporalImage = task3.final;
       //Screen
       Image newLayer3 ("input/text.tga");
       task3.image1 = newLayer3;
       task3.image2 = task3.temporalImage;
       task3.Screen();
       //Output Files
       task3.writingFile("output/part3.tga");
       //Test
       Image sample3("examples/EXAMPLE_part3.tga");
       Test(task3.final, sample3, 3);

       //************************** Task 4 **************************************
       //Load Images
       Image topLayer4("input/layer2.tga");
       Image bottomLayer4("input/circles.tga");
       Image newLayer4 ("input/pattern2.tga");
       //Multiply
       Filter task4;
       task4.image1 = bottomLayer4;
       task4.image2 = topLayer4;
       task4.Multiply();
       task4.image1 = task4.final;
       task4.image2 = newLayer4;
       //Subtract Blending
       task4.Subtract();
       //Output Files
       task4.writingFile("output/part4.tga");
       //Test
       Image sample4("examples/EXAMPLE_part4.tga");
       Test(task4.final, sample4, 4);

       //********************* Task 5 **********************************************
       //Load Files
       Image topLayer5("input/layer1.tga");
       Image bottomLayer5("input/pattern1.tga");
       //Overlay
       Filter task5;
       task5.image1 = topLayer5;
       task5.image2 = bottomLayer5;
       task5.Overlay();
       //Output Files
       task5.writingFile("output/part5.tga");
       //Test
       Image sample5("examples/EXAMPLE_part5.tga");
       Test(task5.final, sample5, 5);

       //*************************** Task 6 ******************************************
       //Load Image
       Image topLayer6("input/car.tga");
       //Add 200 to green Channel
       Filter task6;
       task6.image1 = topLayer6;
       task6.IncreaseGreenChannel(200);
       //Output Files
       task6.writingFile("output/part6.tga");
       //Test
       Image sample6("examples/EXAMPLE_part6.tga");
       Test(task6.final, sample6, 6);
       //***************************** Task 7 *********************************************
       //Load Image
       Image topLayer7("input/car.tga");
       //Multiply Red channel by 4 and Blue Channel by 0
       Filter task7;
       task7.image1 = topLayer7;
       task7.multiplyRedBlueChannel(4,0);
       //Output Files
       task7.writingFile("output/part7.tga");
       //Test
       Image sample7("examples/EXAMPLE_part7.tga");
       Test(task7.final, sample7, 7);
       //****************************** Task 8 ********************************************
       //Load Image
       Image topLayer8("input/car.tga");
       //Separate Green Channel
       Filter task8;
       task8.image1 = topLayer8;
       task8.SeparatingChannels("green");
       //Output Files
       task8.writingFile("output/part8_g.tga");
       //Test
       Image sample8a("examples/EXAMPLE_part8_g.tga");
       Test(task8.final, sample8a, 8);
       //Separate Red Channel
       task8.SeparatingChannels("red");
       //Output Files
       task8.writingFile("output/part8_r.tga");
       //Separate Blue Channel
       task8.SeparatingChannels("blue");
       //Output Files
       task8.writingFile("output/part8_b.tga");

       //**************************** Task 9 **********************************
       //Load Image
       Image topLayer9("input/layer_red.tga");
       Image bottom9Layer9("input/layer_green.tga");
       Image otherLayer9 ("input/layer_blue.tga");
       //Combine Layers
       Filter task9;
       task9.image1 = topLayer9;
       task9.image2 = bottom9Layer9;
       task9.temporalImage = otherLayer9;
       task9.Combine();
       //Output Files
       task9.writingFile("output/part9.tga");
       //Test
       Image sample9("examples/EXAMPLE_part9.tga");
       Test(task9.final, sample9, 9);

       //********************************** Task 10 ****************************
       //Load Image
       Image topLayer10("input/text2.tga");
       //Rotate 180 degrees
       Filter task10;
       task10.image1 = topLayer10;
       task10.Rotate();
       //Output Files
       task10.writingFile("output/part10.tga");
       //Test
       Image sample10("examples/EXAMPLE_part10.tga");
       Test(task10.final, sample10, 10);
       
           return 0;
       }

void Test(Image final, Image sample, int number) {
    bool flag = false;
    for (int i = 0; i < sample.pixels.size(); i++) {
        if (final.pixels.at(i).red != sample.pixels.at(i).red) {
            flag = true;
        }
        if (final.pixels.at(i).blue != sample.pixels.at(i).blue) {
            flag = true;
        }
        if (final.pixels.at(i).green != sample.pixels.at(i).green) {
            flag = true;
        }
    }
    if (flag == true) {
        cout << "Task " << number << " failed" << endl;
    } else {
        cout << "Task " << number << " complete" << endl;
    }
};




