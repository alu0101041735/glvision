#include "imageprocessing.h"

Imageprocessing::Imageprocessing()
{
    if (!clSetup())
    {
        exit(-1);
    }

    if (!clKernelSetup())
    {
       exit(-1);
    }

    if (!runKernel())
    {
        exit(-1);
    }

}


Imageprocessing::Imageprocessing(QImage image): m_qimage(image)
{

    if (!configImage())
    {
        exit(-1);
    }

    if (!clSetup())
    {
        exit(-1);
    }

    if (!clKernelSetup())
    {
       exit(-1);
    }

    if (!runKernel())
    {
        exit(-1);
    }

}

bool Imageprocessing::clSetup()
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

bool Imageprocessing::clProcessImage()
{

    return true;
}

bool Imageprocessing::clKernelSetup()
{

    // kernel calculates for each element C=A+B
    std::string kernel_code=
        "   void kernel simple_add(global const int* A, global const int* B, global int* C){       "
        "       C[get_global_id(0)]=A[get_global_id(0)]+B[get_global_id(0)];                 "
        "   }                                                                               ";
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

bool Imageprocessing::runKernel()
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

bool Imageprocessing::configImage()
{
   int width = m_qimage.width();
   int height = m_qimage.height();

   m_image.resize(width * height);

   int v = 0;
   for (int i = 0 ; i < width; i++) {
       for (int j = 0; j < height; j++) {
          QRgb pixel = m_qimage.pixel(i, j);

          auto pixelcolor = std::make_tuple(
                      static_cast<uint8_t>(QColor(pixel).red()),
                      static_cast<uint8_t>(QColor(pixel).green()),
                      static_cast<uint8_t>(QColor(pixel).red()));
          m_image[v]	= pixelcolor;

          v++;
       }
   }

   return true;
}

bool Imageprocessing::saveImage()
{

    return true;
}




