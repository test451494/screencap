#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/jpeg/jpeglib.h"
#include <pthread.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/mman.h>
#define JPEG_QUALITY 95
//--------------------------------------------------------------------------

static char *screen_context = NULL;
static pthread_t talk_savePic_threadId;
static int fb_dev_fd = -1;
int fb_Width = 0;
int	fb_Height = 0;
int fb_Depth = 0;
char work_dir[100];
char *framebuffer_data;

void* savePic_thread (void* unused);
void screen_savePic();

int main(int argc, char *argv[])
//int main_start(int argc, char *argv[])
{
if(	fb_init() == 0){

    printf("Capture the screen..\n");
//	capFamebuffer();
	if(argc == 1){	//default storage path
	
		if ( getcwd(work_dir, sizeof(work_dir)) != NULL)
		{
printf("work dir is %s",work_dir);

			char* tmp = "screen";
			strncpy(work_dir,tmp,sizeof(tmp));	
			screen_savePic();	
		}else{
			printf("can't get present work directory\n");
			exit(1);
		}

	}else if( argc == 2){	//argv[1] is the designated path
//	strncpy( work_dir, argv[1],sizeof(argv[1]))	
			
	}else{	//templary not allow multiple storage path
	exit(1);
	}

return 0;

}else{
	//fb init fail
	printf("framebuffer init fail!\n");
	exit(1);
	}

}
void screen_savePic()
{
  //  pthread_attr_t attr;
printf("save_savePic\n");
    //create thread to save file
  //  pthread_attr_init(&attr);
  //  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&talk_savePic_threadId, /*&attr*/ NULL, (void *)savePic_thread, NULL);
   // pthread_attr_destroy(&attr);
   pthread_join(talk_savePic_threadId,NULL);
}
/*
void capFrameBuffer(){

//open the framebuffer 
    int data_size = fb_Width * fb_Height * fb_Depth;
    int fd;
    char *framebuffer_data;
    int len, i;
    int index=0;

    if ((fd = open("/dev/fb0", O_RDWR)) == -1) {
        printf("Can't open /dev/fb0\n");
        return -1;
    }
    framebuffer_data = (char *)malloc(data_size);
    len=read(fd, framebuffer_data, data_size);


//	if(NULL == screen_context){
// allocate a mem for pic to store	
 //		 screen_context = malloc (fb_mgr_getSaveWdith()*fb_mgr_getSaveHeight()*depth);
//	}


}*/
int fb_init(void){
	struct fb_fix_screeninfo fb_fix_info;
    struct fb_var_screeninfo fb_var_info;
   
   	fb_dev_fd = open("/dev/fb0", O_RDWR);
printf("fb %d",fb_dev_fd);	
	if(!fb_dev_fd){
	printf("Fail!\n");
	exit(1);
	}

   // if ((fb_dev_fd = open("/dev/fb0", O_RDONLY)) == -1) {
    //        printf("Can't open /dev/fb0\n");
      //      return -1;
   // }
    if (ioctl(fb_dev_fd, FBIOGET_VSCREENINFO, &fb_var_info) ) {
        printf("%s : Can't ioctl FBIOGET_VSCREENINFO\n", __FUNCTION__);
        return -1;
    }
    ioctl(fb_dev_fd,FBIOGET_FSCREENINFO,&fb_fix_info);
    fb_Depth = fb_var_info.bits_per_pixel / 8;
    fb_Width = fb_var_info.xres;
    fb_Height = fb_var_info.yres;

	//////get the screenshot
    int data_size = fb_Width * fb_Height * fb_Depth;
    //char *framebuffer_data;
    int len, i;

    framebuffer_data = (char *)malloc(data_size);
    len=read(fb_dev_fd, framebuffer_data, data_size);
printf("\nclose fb\n");
	close(fb_dev_fd);
	return 0;
}


void* savePic_thread (void* unused)
{
		printf("savePic_thread\n");
   // char outFullFileName[64];
//    int depth = fb_mgr_getDepth();
  //  int year, month, day, hour, min, sec;

   // setting_getTime(&year, &month, &day, &hour, &min, &sec);
  //  sprintf(outstr, "%04d%02d%02d_%02d%02d%02d.jpg",year, month, day, hour, min, sec);
   // sprintf(outFullFileName, "%s\%s", PIC_FILE_PATH, outstr);
    if (NULL != framebuffer_data) {
        if (0 == rawData2Jpg(framebuffer_data, work_dir, fb_Width, fb_Height, fb_Depth)) {
            freePic();
        }    
    }
    return NULL;
}

/////////////////////////////
void freePic()
{
    if(NULL != framebuffer_data) {
        free(framebuffer_data);
        framebuffer_data = NULL;
    }
	
}
////////////////////////////
int rawData2Jpg(char *data, const char *jpg_file, int width, int height, int fb_depth)
{

		printf("Testing\n");
    int depth = 3;
    JSAMPROW * row_pointer;
    int i=0, j=0;
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *outfile;
    unsigned char * line[800];
    char tempbuf[fb_depth];

    if (4 != fb_depth) {
        printf("%s not support this fb_depth %d\n",__FUNCTION__, fb_depth);
        return -1;
    }
    //Convert BMP to JPG
	printf("test1\n");
    cinfo.err = jpeg_std_error(&jerr);
    //* Now we can initialize the JPEG compression object.
    jpeg_create_compress(&cinfo);
	printf("test11\n");

    if ((outfile = fopen(jpg_file, "wb")) == NULL)
    {
        fprintf(stderr, "can't open %s\n", jpg_file);
        printf("%s: open file %s error\n", __FUNCTION__, jpg_file);
        return -1;
    }
    jpeg_stdio_dest(&cinfo, outfile);
	printf("test2\n");
    cinfo.image_width = width;             //* image width and height, in pixels
    cinfo.image_height = height;
    cinfo.input_components = depth;    //* # of color components per pixel
    cinfo.in_color_space = JCS_RGB;     //* colorspace of input image
    jpeg_set_defaults(&cinfo);

     //Now you can set any non-default parameters you wish to.
     //Here we just illustrate the use of quality (quantization table) scaling:
    jpeg_set_quality(&cinfo, JPEG_QUALITY, TRUE ); //* limit to baseline-JPEG values
    jpeg_start_compress(&cinfo, TRUE);
    row_pointer = malloc(height*width*depth);
    for(i=0;i<height;i++) {
        unsigned char * lineData = NULL;
        lineData = malloc(width*depth);
        line[i]=lineData;
        for(j=0;j<width;j++) {
            memcpy(tempbuf,  data+i*width*fb_depth+j*fb_depth, fb_depth);
            lineData[j*3+0] = tempbuf[2];  // R
            lineData[j*3+1] = tempbuf[1]; // G
            lineData[j*3+2] = tempbuf[0];  // B
        }
        row_pointer[i] = lineData;
    }
    jpeg_write_scanlines(&cinfo, row_pointer, height);
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    for (i=0; i<height; i++)
    {
        free(line[i]);
    }
    free(row_pointer);
    fclose(outfile);
    return 0;
}
