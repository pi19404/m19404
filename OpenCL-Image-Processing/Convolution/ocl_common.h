#include <iostream>
#include <cstdlib>
#include "string.h"
#include <CL/cl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctime>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <vector>
#ifndef OCL_COMMON_H
#define OCL_COMMON_H
#define FAILURE -1
#define SUCCESS 0
#define MAX_SOURCE_SIZE (0x100000)
#define MAX_PLATFORMS 2
#define NAME 10000
#define MAX_DEVICES 4
#define uchar unsigned char
using namespace std;

//dir containing kernel functions
#define CHECK_OPENCL_ERROR(actual, msg) \
    if(checkVal(actual, CL_SUCCESS, msg)) \
        { \
            std::cout << "Location : " << __FILE__ << ":" << __LINE__<< std::endl; \
            return -1; \
        }


#define CHECK_ALLOCATION(actual, msg) \
        if(actual == NULL) \
        { \
            error(msg); \
            std::cout << "Location : " << __FILE__ << ":" << __LINE__<< std::endl; \
             return FAILURE; \
        }




/**
    function to display opencv Error based on the error code
  */
template<typename T>
const char* getOpenCLErrorCodeStr(T input)
{
    int errorCode = (int)input;
    switch(errorCode)
    {
        case CL_DEVICE_NOT_FOUND:
            return "CL_DEVICE_NOT_FOUND";
        case CL_DEVICE_NOT_AVAILABLE:
            return "CL_DEVICE_NOT_AVAILABLE";
        case CL_COMPILER_NOT_AVAILABLE:
            return "CL_COMPILER_NOT_AVAILABLE";
        case CL_MEM_OBJECT_ALLOCATION_FAILURE:
            return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
        case CL_OUT_OF_RESOURCES:
            return "CL_OUT_OF_RESOURCES";
        case CL_OUT_OF_HOST_MEMORY:
            return "CL_OUT_OF_HOST_MEMORY";
        case CL_PROFILING_INFO_NOT_AVAILABLE:
            return "CL_PROFILING_INFO_NOT_AVAILABLE";
        case CL_MEM_COPY_OVERLAP:
            return "CL_MEM_COPY_OVERLAP";
        case CL_IMAGE_FORMAT_MISMATCH:
            return "CL_IMAGE_FORMAT_MISMATCH";
        case CL_IMAGE_FORMAT_NOT_SUPPORTED:
            return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
        case CL_BUILD_PROGRAM_FAILURE:
            return "CL_BUILD_PROGRAM_FAILURE";
        case CL_MAP_FAILURE:
            return "CL_MAP_FAILURE";
        case CL_MISALIGNED_SUB_BUFFER_OFFSET:
            return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
        case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
            return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
        case CL_INVALID_VALUE:
            return "CL_INVALID_VALUE";
        case CL_INVALID_DEVICE_TYPE:
            return "CL_INVALID_DEVICE_TYPE";
        case CL_INVALID_PLATFORM:
            return "CL_INVALID_PLATFORM";
        case CL_INVALID_DEVICE:
            return "CL_INVALID_DEVICE";
        case CL_INVALID_CONTEXT:
            return "CL_INVALID_CONTEXT";
        case CL_INVALID_QUEUE_PROPERTIES:
            return "CL_INVALID_QUEUE_PROPERTIES";
        case CL_INVALID_COMMAND_QUEUE:
            return "CL_INVALID_COMMAND_QUEUE";
        case CL_INVALID_HOST_PTR:
            return "CL_INVALID_HOST_PTR";
        case CL_INVALID_MEM_OBJECT:
            return "CL_INVALID_MEM_OBJECT";
        case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
            return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
        case CL_INVALID_IMAGE_SIZE:
             return "CL_INVALID_IMAGE_SIZE";
        case CL_INVALID_SAMPLER:
            return "CL_INVALID_SAMPLER";
        case CL_INVALID_BINARY:
            return "CL_INVALID_BINARY";
        case CL_INVALID_BUILD_OPTIONS:
            return "CL_INVALID_BUILD_OPTIONS";
        case CL_INVALID_PROGRAM:
            return "CL_INVALID_PROGRAM";
        case CL_INVALID_PROGRAM_EXECUTABLE:
            return "CL_INVALID_PROGRAM_EXECUTABLE";
        case CL_INVALID_KERNEL_NAME:
            return "CL_INVALID_KERNEL_NAME";
        case CL_INVALID_KERNEL_DEFINITION:
            return "CL_INVALID_KERNEL_DEFINITION";
        case CL_INVALID_KERNEL:
            return "CL_INVALID_KERNEL";
        case CL_INVALID_ARG_INDEX:
            return "CL_INVALID_ARG_INDEX";
        case CL_INVALID_ARG_VALUE:
            return "CL_INVALID_ARG_VALUE";
        case CL_INVALID_ARG_SIZE:
            return "CL_INVALID_ARG_SIZE";
        case CL_INVALID_KERNEL_ARGS:
            return "CL_INVALID_KERNEL_ARGS";
        case CL_INVALID_WORK_DIMENSION:
            return "CL_INVALID_WORK_DIMENSION";
        case CL_INVALID_WORK_GROUP_SIZE:
            return "CL_INVALID_WORK_GROUP_SIZE";
        case CL_INVALID_WORK_ITEM_SIZE:
            return "CL_INVALID_WORK_ITEM_SIZE";
        case CL_INVALID_GLOBAL_OFFSET:
            return "CL_INVALID_GLOBAL_OFFSET";
        case CL_INVALID_EVENT_WAIT_LIST:
            return "CL_INVALID_EVENT_WAIT_LIST";
        case CL_INVALID_EVENT:
            return "CL_INVALID_EVENT";
        case CL_INVALID_OPERATION:
            return "CL_INVALID_OPERATION";
        case CL_INVALID_GL_OBJECT:
            return "CL_INVALID_GL_OBJECT";
        case CL_INVALID_BUFFER_SIZE:
            return "CL_INVALID_BUFFER_SIZE";
        case CL_INVALID_MIP_LEVEL:
            return "CL_INVALID_MIP_LEVEL";
        case CL_INVALID_GLOBAL_WORK_SIZE:
            return "CL_INVALID_GLOBAL_WORK_SIZE";
        case CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR:
            return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
        case CL_PLATFORM_NOT_FOUND_KHR:
            return "CL_PLATFORM_NOT_FOUND_KHR";
        //case CL_INVALID_PROPERTY_EXT:
        //    return "CL_INVALID_PROPERTY_EXT";
        case CL_DEVICE_PARTITION_FAILED_EXT:
            return "CL_DEVICE_PARTITION_FAILED_EXT";
        case CL_INVALID_PARTITION_COUNT_EXT:
            return "CL_INVALID_PARTITION_COUNT_EXT";
        default:
            return "unknown error code";
    }

    return "unknown error code";
}

/**
    OpenCL class containing common initialization functions and kernel calling functions
  **/
class OCLX
{

public:

    OCLX()
    {
        flag=0;
    }


   //function to display error message
    void error(const char* errorMsg)
    {
        std::cout<<"Error: "<<errorMsg<<std::endl;
    }

    //function to display error message
    void error(std::string errorMsg)
    {
        std::cout<<"Error: "<<errorMsg<<std::endl;
    }



    //platform information
    cl_platform_id platform;
    cl_uint num_devices;
    string platform_name;

    //device information
    cl_device_id * devices;
    std::vector <string> device_name;
    std::vector <string> device_version;
    std::vector <string> opencl_version;


   //execution evvironment parameters
    cl_context context;
    cl_command_queue queue;
    cl_uint maxComputeUnits;

    //pointer for kernel and programs
    cl_kernel *kernel;
    cl_program *program;


    template<typename T>
    int checkVal(
        T input,
        T reference,
        std::string message,
        bool isAPIerror=true) const
    {
        if(input==reference)
        {
            return 0;
        }
       else
        {
            if(isAPIerror)
            {
                std::cout<<"Error: "<< message << " Error code : ";
                std::cout << getOpenCLErrorCodeStr(input) << std::endl;
            }
            else
                std::cout << message;
            return 1;
        }
    }






    /**
     * @brief randomInit method populates the matrix with random values
     * @param data
     * @param size
     */
    void randomInit(float* data, int size)
    {
        int min=0;
        int max=10;
       for (int i = 0; i < size; ++i)
       data[i] = min + (rand() % (int)(max - min + 1));
    }

    /**
     * @brief checkErr method checks for error and exits the application in case of error
     * @param clErr
     * @param filename
     * @param line
     */
    void checkErr(cl_int clErr, char* filename, int line)
    {
    if (clErr!=CL_SUCCESS)
    {
    printf("OpenCL Error %i at line %i of%s\n",clErr,line,filename);
    exit(EXIT_FAILURE);
    }
    }

    /**
     * @brief checkErr method checks for error and exits the application in case of error
     * @param clErr
     * @param filename
     * @param line
     */
    void checkErr(cl_int clErr, const char* filename, int line)
    {
    if (clErr!=CL_SUCCESS)
    {
    printf("OpenCL Error %i at line %i of%s\n",clErr,line,filename);
    exit(EXIT_FAILURE);
    }
    }


    //function to get the platform information
    cl_int getPlatform(cl_platform_id &platform,int platformId,bool platformIdEnabled)
    {
        cl_int clErr;
        cl_uint numPlatforms;
        clErr = clGetPlatformIDs(0,NULL,&numPlatforms);

        if(0<numPlatforms)
        {
           cl_platform_id* platforms = new cl_platform_id[numPlatforms];
           // get platform IDs
           clErr = clGetPlatformIDs(numPlatforms,platforms,NULL);
           checkErr(clErr,(char*)__FILE__,__LINE__);




        if(platformIdEnabled)
         {
             platform = platforms[platformId];
         }
         else
         {
             char platformName[100];
             for (unsigned i = 0; i < numPlatforms; ++i)
             {


                 size_t size;
                 clErr = clGetPlatformInfo(platforms[i],CL_PLATFORM_VENDOR,0,NULL,&size);
                 checkErr(clErr,(char*)__FILE__,__LINE__);

                 clErr = clGetPlatformInfo(platforms[i],CL_PLATFORM_VENDOR,size,platformName,NULL);
                 checkErr(clErr,(char*)__FILE__,__LINE__);
                 printf("Platform %i: %s\n",i,platformName);


                 platform = platforms[i];
                 if (!strcmp(platformName, "Advanced Micro Devices, Inc."))
                 {
                     break;
                 }
             }
             std::cout << "Platform found : " << platformName << "\n";
         }
        delete[] platforms;

        }
        else
        {
             std::cout<<"Error: No available platform found!"<<std::endl;
             return FAILURE;
        }

    }

  //function to initialize the device information
    cl_int getDevice(cl_platform_id platform,cl_uint & count,cl_device_type deviceType,cl_device_id  **devices)
    {
        cl_int status;
        char platformVendor[1024];
        status = clGetPlatformInfo(platform, CL_PLATFORM_VENDOR, sizeof(platformVendor), platformVendor, NULL);
        CHECK_OPENCL_ERROR(status, "clGetPlatformInfo failed");
        std::cout << "\nSelected Platform Vendor : " << platformVendor << std::endl;

        // Get number of devices available
        cl_uint deviceCount = 0;
        status = clGetDeviceIDs(platform, deviceType, 0, NULL, &deviceCount);
        CHECK_OPENCL_ERROR(status, "clGetDeviceIDs failed");
        if (deviceCount == 0)
        {
              std::cout << "No device available\n" << endl;
              return FAILURE;
        }

        std::cout << "Number of Devices Found " << deviceCount << endl;
        count=deviceCount;
        //deviceIds.resize(count);



        *devices=(cl_device_id*)malloc(count*sizeof(cl_device_id));
        CHECK_ALLOCATION(*devices, "Failed to allocate memory(deviceIds)");

        // Get device ids
        status = clGetDeviceIDs(platform,deviceType,deviceCount,*devices, NULL);
        CHECK_OPENCL_ERROR(status, "clGetDeviceIDs failed");

        // Print device index and device names
        for(cl_uint i = 0; i < deviceCount; ++i)
        {
            char deviceName[1024];
            char device_version[1024];
            char opencl_version[1024];
            cl_int maxComputUnits;

            size_t size;
            status = clGetDeviceInfo(*devices[i],CL_DEVICE_NAME,0,NULL,&size);
            status = clGetDeviceInfo(*devices[i], CL_DEVICE_NAME, sizeof(deviceName), deviceName, NULL);
            CHECK_OPENCL_ERROR(status, "clGetDeviceInfo CL_DEVICE_NAME failed");

            status = clGetDeviceInfo(*devices[i],CL_DEVICE_VERSION,0,NULL,&size);
            status= clGetDeviceInfo(*devices[i],CL_DEVICE_VERSION,size,device_version,NULL);
            CHECK_OPENCL_ERROR(status, "clGetDeviceInfo  CL_DEVICE_VERSION failed");

            clGetDeviceInfo(*devices[i], CL_DEVICE_OPENCL_C_VERSION, 0, NULL, &size);
            clGetDeviceInfo(*devices[i], CL_DEVICE_OPENCL_C_VERSION,size,opencl_version, NULL);

            //deviceIds.push_back(devices[i]);
            //devicename.push_back(deviceName);
            //deviceversion.push_back(device_version);;

            clGetDeviceInfo(*devices[i], CL_DEVICE_MAX_COMPUTE_UNITS,sizeof(maxComputUnits), &maxComputUnits, NULL);


            std::cout << "Device -->" << i << " : " << deviceName <<endl;
            std::cout << "Device Version -->" << device_version << std::endl;
            std::cout << "OpenCL version -->" << opencl_version << endl;
            std::cout << "max compute units -->" << maxComputUnits << endl;


        }

        //free(devices);

        return SUCCESS;

    }

    /**
     * @brief show_platform_info function shows platform and device info
     */
    cl_int init()
    {


        cl_int status;
        unsigned int i;
        getPlatform(platform,1,false);
        getDevice(platform,num_devices,CL_DEVICE_TYPE_ALL,&devices);


        //define desired context properties list
        cl_context_properties properties[] = {CL_CONTEXT_PLATFORM,
                                              (cl_context_properties) platform,0};


        // create context for devices
        context = clCreateContext(properties,num_devices,devices,NULL,NULL,&status);
        CHECK_OPENCL_ERROR(status, "clCreateContext failed");

        // create queue for devices
        queue = clCreateCommandQueue(context,devices[0],0,&status);
        CHECK_OPENCL_ERROR(status, "clCreateCommandQueue failed");

    }


    /**
     * @brief destroy function releases resourcees
     */
    cl_int destroy()
    {
        cl_int status;



        free(devices);



        status=clFlush(queue);
        CHECK_OPENCL_ERROR(status, "clFlush failed");


        status=clFinish(queue);
        CHECK_OPENCL_ERROR(status, "clFinish failed");



        status= clReleaseCommandQueue(queue);
        CHECK_OPENCL_ERROR(status, "clReleaseCommandQueue failed");


        status= clReleaseContext(context);
        CHECK_OPENCL_ERROR(status, "clReleaseContext failed");



    }




    /**
     * @brief create_global_buffer function creates a global write buffer in device global memory
     */

    cl_mem create_write_buffer(size_t bytes)
    {
        cl_int clErr;
        cl_mem buffer = clCreateBuffer(context,CL_MEM_WRITE_ONLY|CL_MEM_ALLOC_HOST_PTR ,bytes,NULL,&clErr);
        checkErr(clErr,(char*)__FILE__,__LINE__);
        return buffer;
    }

    /**
     * @brief create_read_buffer method creates a read buffer in device global memory
     * @param bytes
     * @return
     */
    cl_mem create_read_buffer(size_t bytes)
    {

        cl_mem_flags flags = CL_MEM_READ_ONLY ;//| CL_MEM_USE_HOST_PTR |CL_MEM_USE_PERSISTENT_MEM_AMD;

        cl_int clErr;
        cl_mem buffer = clCreateBuffer(context,flags,bytes,NULL,&clErr);
        checkErr(clErr,(char*)__FILE__,__LINE__);
        //printf("-------%d",CL_SUCCESS);
        return buffer;

    }

    //creates a global  write memory which is mapped to a host memory and can be accessed by read/write to host pointer mem
    cl_mem create_write_buffer(size_t bytes,void **mem,void **ptr)
    {
        cl_int status;
        cl_mem buffer = clCreateBuffer(context,CL_MEM_WRITE_ONLY|CL_MEM_USE_HOST_PTR ,bytes,*mem,&status);
        checkErr(status,(char*)__FILE__,__LINE__);
        *ptr = clEnqueueMapBuffer(queue,buffer,CL_FALSE, CL_MAP_READ, 0,bytes, 0,NULL,NULL, &status);
        checkErr(status,(char*)__FILE__,__LINE__);
        return buffer;
    }

    //creates a global  read memory which is mapped to a host memory and can be accessed by read/write to host pointer mem
    cl_mem create_read_buffer(size_t bytes,void *buffer1,uint id,cl_event *event)
    {
        cl_int status;

        cl_mem_flags flags = CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR ;

         cl_mem bufferx = clCreateBuffer(context,flags,bytes,buffer1,&status);
         checkErr(status,(char*)__FILE__,__LINE__);
         void *mem = clEnqueueMapBuffer(queue,bufferx,CL_FALSE,CL_MAP_READ, 0,bytes,0,NULL,NULL, &status);
         checkErr(status,(char*)__FILE__,__LINE__);
         memcpy(mem,buffer1,bytes);
         status = clEnqueueUnmapMemObject(queue, bufferx, mem, 0, NULL,&event[id]);
         checkErr(status,(char*)__FILE__,__LINE__);
        //CHECK_OPENCL_ERROR(status,"");
        //printf("-------%d",CL_SUCCESS);
        return bufferx;

    }

//creates a global  read write memory which is mapped to a host memory and can be accessed by read/write to host pointer buffer1
    cl_mem create_rw_buffer(size_t bytes,void *buffer1,uint id,cl_event *event)
    {
        cl_int status;


        cl_mem_flags flags = CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR ;

         cl_mem bufferx = clCreateBuffer(context,flags,bytes,buffer1,&status);
         checkErr(status,(char*)__FILE__,__LINE__);


        return bufferx;

    }




//function to write data to global buffer from host->device
    cl_int write_buffer(cl_mem dbuffer,size_t bytes,void *hbuffer,cl_event *event,int id)
    {
         cl_int clErr;
        if(id==0)
            clErr = clEnqueueWriteBuffer(queue,dbuffer,CL_FALSE,0,bytes,(void*)hbuffer,0,NULL,&event[id]);
        else
            clErr = clEnqueueWriteBuffer(queue,dbuffer,CL_FALSE,0,bytes,(void*)hbuffer,0,NULL,&event[id]);
        checkErr(clErr,__FILE__,__LINE__);


        return clErr;
    }

    /**
     * @brief read_buffer reads data to host from the device global memory
     * @param dbuffer
     * @param bsize
     * @param hbuffer
     */

    cl_int read_buffer(cl_mem dbuffer,size_t bsize,void *hbuffer,cl_event *event,int id)
    {
        cl_int clErr;
        if(id==0)
        clErr = clEnqueueReadBuffer(queue,dbuffer,CL_FALSE,0,bsize,(void*)hbuffer,0,NULL,NULL);
        else
        clErr = clEnqueueReadBuffer(queue,dbuffer,CL_FALSE,0,bsize,(void*)hbuffer,0,NULL,&event[id]);
        return clErr;

    }



    /**
     * @brief function to read the source file containing kernel code and performing compilation of kernel code
     * @param path
     * @param name
     * @return
     */


    cl_int read_program(char * path,cl_program *program)
    {

        //cl_kernel kernel;
        FILE *fp;
        char *source = (char*)malloc(MAX_SOURCE_SIZE);

        cl_int status;
        fp = fopen(path, "r");
        if (!fp) {
            fprintf(stderr, "Failed to load kernel %s\n",path);
            exit(1);
        }
        cerr << "Reading Kernel File " << path << endl;

    //read the kernel source from the file
    size_t size = fread( source, 1, MAX_SOURCE_SIZE, fp);
    fflush(fp);
    fclose( fp );
    if(size <=0)
        return FAILURE;


    // Create a program from the kernel source

    *program = clCreateProgramWithSource(context, 1,(const char **)&source, (const size_t *)&size, &status);
    CHECK_OPENCL_ERROR(status,"clCreateProgramWithSource");

    //JIT compilation of kernel
    status = clBuildProgram(*program, 1,&devices[0], "-g", NULL, NULL);
    CHECK_OPENCL_ERROR(status,"");

    //get build infor log
    size_t lsize;
    status = clGetProgramBuildInfo(*program,devices[0],CL_PROGRAM_BUILD_LOG,0,NULL,&lsize);
     CHECK_OPENCL_ERROR(status,"");


    char build_log[lsize];
    status = clGetProgramBuildInfo(*program,devices[0],CL_PROGRAM_BUILD_LOG,lsize,build_log,NULL);
     CHECK_OPENCL_ERROR(status,"");
    printf("\nBuild Log:\n\n%s\n\n",build_log);





    }

    cl_int read_program(char * path,cl_program *program,string options)
    {

        //cl_kernel kernel;
        FILE *fp;
        char *source = (char*)malloc(MAX_SOURCE_SIZE);

        cl_int status;
        fp = fopen(path, "r");
        if (!fp) {
            fprintf(stderr, "Failed to load kernel %s\n",path);
            exit(1);
        }
        cerr << "Reading Kernel File " << path << endl;

    //read the kernel source from the file
    size_t size = fread( source, 1, MAX_SOURCE_SIZE, fp);
    fflush(fp);
    fclose( fp );
    if(size <=0)
        return FAILURE;


    // Create a program from the kernel source

    *program = clCreateProgramWithSource(context, 1,(const char **)&source, (const size_t *)&size, &status);
    CHECK_OPENCL_ERROR(status,"clCreateProgramWithSource");

    //JIT compilation of kernel
    status = clBuildProgram(*program, 1,&devices[0], options.c_str(), NULL, NULL);
    CHECK_OPENCL_ERROR(status,"");

    //get build infor log
    size_t lsize;
    status = clGetProgramBuildInfo(*program,devices[0],CL_PROGRAM_BUILD_LOG,0,NULL,&lsize);
     CHECK_OPENCL_ERROR(status,"");


    char build_log[lsize];
    status = clGetProgramBuildInfo(*program,devices[0],CL_PROGRAM_BUILD_LOG,lsize,build_log,NULL);
     CHECK_OPENCL_ERROR(status,"");
    printf("\nBuild Log:\n\n%s\n\n",build_log);





    }


    //function to create kernel
    cl_int read_kernel(cl_program program,cl_kernel *kernel,char *name)
    {
        // Create the OpenCL kernel
        cl_int status;
        *kernel = clCreateKernel(program,name, &status);
         CHECK_OPENCL_ERROR(status,"");


    }

    //fuction to release kernel and program data structures
    cl_int release(cl_kernel k,cl_program p)
    {
          cl_int  status;

       status=clReleaseKernel(k);
        CHECK_OPENCL_ERROR(status, "");

       status=clReleaseProgram(p);
        CHECK_OPENCL_ERROR(status, "");

    }




    /**
        function that reads the kernel program filter and loads the kernel
      */
    cl_int conv2d(cl_program *program,cl_kernel *kernel)
    {
        string source="../Convolution/conv1.cl";
        string name[10];
        name[0]="conv2D_1";
        name[1]="conv2D_2";
        //name[2]="conv1D_3";

        read_program((char *)source.c_str(),program);
        read_kernel(*program,&kernel[0],(char *)name[0].c_str());
        read_kernel(*program,&kernel[1],(char *)name[1].c_str());
        //read_kernel(*program,&kernel[2],(char *)name[2].c_str());

    }

    cl_int conv2d(cl_program *program,cl_kernel *kernel,string options)
    {
        string source="../Convolution/conv1.cl";
        string name[10];
        name[0]="conv2D_1";
        name[1]="conv2D_2";
        //name[2]="conv1D_3";

        read_program((char *)source.c_str(),program,options);
        read_kernel(*program,&kernel[0],(char *)name[0].c_str());
        read_kernel(*program,&kernel[1],(char *)name[1].c_str());
        //read_kernel(*program,&kernel[2],(char *)name[2].c_str());

    }


    //function to call the launch kernel on the device and set the arguments for kernel execution

    //commmon host memory pointer
    void *memory1;

    //common global memory pointer
    cl_mem dimage1,dimage2,dimage3;
    //initialization flag for code to be run during first execution
    int flag;


    /**
        funcation that call the kernel code for 2D convolution operation
      */
    cl_int kernel_conv2(uchar *input,int width,int height,uchar *mask,int mwidth,int mheight,uchar *output,cl_kernel *ke,cl_program program)
    {


        cl_kernel kernel=ke[0];
        cl_int status;



                 cl_event event[5];
                 size_t bytes1=width*height*sizeof(unsigned char);
                 size_t bytes2=mwidth*mheight*sizeof(unsigned char);


                 if(flag==0)
                 {
                      dimage1=create_rw_buffer(bytes1,input,0,NULL);
                      dimage2=create_rw_buffer(bytes2,mask,0,NULL);
                      dimage3=create_rw_buffer(bytes1,output,0,NULL);
                      flag=1;
                 }

                 size_t global[3] = {width,height,3};
                 size_t local[3] = {16,16,1};

                 //setting the kernel arguments
                 status= clSetKernelArg(kernel, 0, sizeof(cl_mem), &dimage1);
                 CHECK_OPENCL_ERROR(status,"");

                 status= clSetKernelArg(kernel, 1, sizeof(cl_int),&width);
                 CHECK_OPENCL_ERROR(status,"");

                 status= clSetKernelArg(kernel, 2, sizeof(cl_int),&height);
                 CHECK_OPENCL_ERROR(status,"");


                 status= clSetKernelArg(kernel, 3, sizeof(cl_mem), &dimage2);
                 CHECK_OPENCL_ERROR(status,"");


                 status= clSetKernelArg(kernel, 4, sizeof(cl_int),&mwidth);
                 CHECK_OPENCL_ERROR(status,"");

                 status= clSetKernelArg(kernel, 5, sizeof(cl_int),&mheight);
                 CHECK_OPENCL_ERROR(status,"");


                 status= clSetKernelArg(kernel, 6, sizeof(cl_mem), &dimage3);
                 CHECK_OPENCL_ERROR(status,"");


                  status = clEnqueueNDRangeKernel(queue,kernel,3,NULL,global,local,0,NULL,&event[0]);
                  CHECK_OPENCL_ERROR(status,"");

                 //waiting for the kernel to complete
                  status = clWaitForEvents(1,&event[0]);
                  CHECK_OPENCL_ERROR(status,"");


    }



    /**
        host code for 2D kernel convolution
      */
    void host_conv2(uchar *data,int width,int height,uchar *mask,int mask_width,int mask_height,uchar *output)
    {


        for(int y=0;y<height;y++)
        {
        for(int x=0;x<width;x++)
        {


            if(x<width && y <height )
            {
            //performs computation for pixels in the valid range
            int value=0; //local variable used  to store convolution sum
            for(int i=0;i<mask_height;i++)
            {
            //loop over the rows of the pixel neighborhood
            for(int j=0;j<mask_width;j++)
            {
            //loop over the columns of the pixels neighborhood

                if((y+i-(mask_height/2)>=0) && (x+j-(mask_width/2)>=0) && ((y+i-mask_height/2)< height) && ((x+j-mask_width/2) <width))
                {
                    //condition defines pixels lying within the image borders
                    value=value+data[(y+i-mask_height/2)*width+(x+j-mask_width/2)]*mask[(i)*(mask_width)+j];
                //reading the data from global input memory and computing the convolution sum over the neighborhood

               }
            }
            }
            //copying the data to global output memory
            output[y*width+x]=value;


        }


        }

        }
}
};


#endif // OCL_COMMON_H
