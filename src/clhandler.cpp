#include "clhandler.h"

clHandler::clHandler()
{
    if (!clSetup())
    {
        exit(-1);
    }


}


clHandler::clHandler(QImage image): m_qimage(image)
{

    if (!configImage())
    {
        exit(-1);
    }

    if (!clSetup())
    {
        exit(-1);
    }

}

clHandler::~clHandler()
{

}

bool clHandler::clSetup()
{

    //get all platforms (drivers)
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0){
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    m_default_platform=all_platforms[0];
    std::cout << "Using platform: "<<m_default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    //get default device of the default platform
    std::vector<cl::Device> all_devices;
    m_default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0){
        std::cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }
    m_default_device=all_devices[0];
    std::cout<< "Using device: "<<m_default_device.getInfo<CL_DEVICE_NAME>()<<"\n";

    new(&m_context) cl::Context({m_default_device});

    return true;
}

bool clHandler::clProcessImage()
{
    return true;
}

bool clHandler::clKernelSetup(TransformationFlags transformation)
{
    std::string path;
    if (!std::filesystem::exists("../glvision/src/kernel/test.cl")) {
        std::cout << "File does not exist\n";
        exit(-1);
    }
    switch(transformation) {
        case GAUSSIAN:
            path = "../glvision/src/kernel/gaussian.cl";
            if (!std::filesystem::exists(path)) {
                std::cout << "File does not exist\n";
                exit(-1);
            }
            break;
        case GRAYSCALE:
            path = "../glvision/src/kernel/grayscale.cl";
            if (!std::filesystem::exists(path)) {
                std::cout << "File does not exist\n";
                exit(-1);
            }
            break;
        case BLUR:
            path = "../glvision/src/kernel/blur.cl";
            if (!std::filesystem::exists(path)) {
                std::cout << "File does not exist\n";
                exit(-1);
            }
            break;
        case TEST:
            path = "../glvision/src/kernel/test.cl";
            if (!std::filesystem::exists(path)) {
                std::cout << "File does not exist\n";
                exit(-1);
            }
            break;
        default:
            break;
    }

    std::string kernel_code;
    std::ifstream f(path);

    if(f) {
        std::ostringstream ss;
        ss << f.rdbuf(); // reading data
        kernel_code = ss.str();
    }
    std::cout << "the kernel code is: " << kernel_code << "\n\n\n";

    // kernel calculates for each element C=A+B
    m_sources.push_back({kernel_code.c_str(),kernel_code.length()});


    new(&m_program) cl::Program(m_context,m_sources);

    cl::Program program(m_context,m_sources);
    if(m_program.build({m_default_device})!=CL_SUCCESS){
        std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(m_default_device)<<"\n";
        exit(1);
    }


    // create buffers on the device
    return true;


}

bool clHandler::runKernel()
{

    cl::Buffer buffer_A(m_context,CL_MEM_READ_WRITE,sizeof(int)*10);
    cl::Buffer buffer_B(m_context,CL_MEM_READ_WRITE,sizeof(int)*10);
    cl::Buffer buffer_C(m_context,CL_MEM_READ_WRITE,sizeof(int)*10);

    int A[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int B[] = {0, 1, 2, 0, 1, 2, 0, 1, 2, 0};

    //create queue to which we will push commands for the device.
    cl::CommandQueue queue(m_context, m_default_device);

    //write arrays A and B to the device
    queue.enqueueWriteBuffer(buffer_A,CL_TRUE,0,sizeof(int)*10,A);
    queue.enqueueWriteBuffer(buffer_B,CL_TRUE,0,sizeof(int)*10,B);
    //run the kernel
    //alternative way to run the kernel
    cl::Kernel kernel_add=cl::Kernel(m_program,"simple_add");
    kernel_add.setArg(0,buffer_A);
    kernel_add.setArg(1,buffer_B);
    kernel_add.setArg(2,buffer_C);
    queue.enqueueNDRangeKernel(kernel_add,cl::NullRange,cl::NDRange(10),cl::NullRange);
    queue.finish();

    int C[10];
    //read result C from the device to array C
    queue.enqueueReadBuffer(buffer_C,CL_TRUE,0,sizeof(int)*10,C);

    std::cout<<" result: \n";
    for(int i=0;i<10;i++){
        std::cout<<C[i]<<" ";
    }
    return true;
}

bool clHandler::configImage()
{
   int width = m_qimage.width();
   int height = m_qimage.height();

   m_image = new Rgba[width*height];

   int v = 0;
   for (int i = 0 ; i < width; i++) {
       for (int j = 0; j < height; j++) {
          QRgb pixel = m_qimage.pixel(i, j);

          Rgba pixelcolor;
          //pixelcolor.red = static_cast<uint8_t>(QColor(pixel).red());
          //pixelcolor.green = static_cast<uint8_t>(QColor(pixel).green());
          //pixelcolor.blue  = static_cast<uint8_t>(QColor(pixel).blue());

          pixelcolor.red = QColor(pixel).red();
          pixelcolor.green = QColor(pixel).green();
          pixelcolor.blue  = QColor(pixel).blue();

          m_image[v]	= pixelcolor;
          v++;
       }
   }

   /*
   for (int i = 0; i < (width*height); i++) {
       std::cout << "Pixel color: red(" << m_image[i].red
                    << ") green(" << m_image[i].green
                    << ") blue(" << m_image[i].blue << ")\n";
   }
   */

   return true;
}

bool clHandler::saveImage()
{
    return true;
}




