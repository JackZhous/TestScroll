#include "com_jz_testscroll_av_AVUtil.h"
#include <stdio.h>
#include <time.h>

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/log.h"



void custom_log(void *ptr, int level, const char *log, va_list vl){
	FILE *fp = fopen("/storage/emulated/0/my_log.txt","a+");

	if(fp){
		vfprintf(fp,log,vl);					//格式化写入
		fflush(fp);
		fclose(fp);
	}
}




/*
 * Class:     com_jack_ffmpegdecorder_MainActivity
 * Method:    decode
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT jstring JNICALL Java_com_jz_testscroll_av_AVUtil_decode(JNIEnv *env, jobject object, jstring input, jstring output){

	AVFormatContext *pFormatContext;
	int i,  videoIndex;
	AVCodecContext *pCodecCtx;
	AVCodec *pCodec;
	//AVFrame是存放解码之后音频或视频数据，它必须用av_frame_alloc进行内存分配，av_frame_free释放内存
	AVFrame *pFrame,*pFrameYUV;
	uint8_t *out_buffer;
	//AVPacket存放是解码之前的压缩数据，av_read_frame得到压缩的数据，在用avcodec_decode_video2解码得到AVFrame
	AVPacket *packet;
	int y_size;
	int ret, got_picture;
	struct SwsContext *img_convert_ctx;
	FILE *fp_yuv;
	int frame_cnt;
	clock_t time_start,time_finish;
	double time_duration = 0.0;

	char input_url[500] = {0};
	char output_char[500] = {0};
	char info[4000] = {0};

	    sprintf(input_url,"%s",(*env)->GetStringUTFChars(env,input, NULL));
	    sprintf(output_char,"%s",(*env)->GetStringUTFChars(env,output, NULL));

	printf("^_^  This is Ffmpeg Log\n");


	//注册日志回调
	av_log_set_callback(custom_log);


	av_register_all();									//libavformat 中，注册所有的复合器和逆复合器 muxer将音频/视频/字母文件合并成一个媒体文件  dumuxer就是其逆过程

	avformat_network_init();				//播放网络视频

	pFormatContext = avformat_alloc_context();						//分配一个AVFormatContext结构体  最后不需要的时候要yogaavformat_free_context进行内存释放

	if(avformat_open_input(&pFormatContext, input_url,NULL,NULL) != 0){					//打开视频文件

		return (*env)->NewStringUTF(env,"解码失败，打开输入流失败\n");
	}

	if(avformat_find_stream_info(pFormatContext,  NULL) < 0){
		return (*env)->NewStringUTF(env,"解码失败，视频文件没有发现文件流\n");
	}

	videoIndex = -1;
	for(i = 0 ; i < pFormatContext->nb_streams; i++){
		if(pFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){						//发现视频流
			videoIndex = i;
		}
	}

	if(videoIndex == -1){
		return (*env)->NewStringUTF(env,"解码失败，该文件无视频流\n");
	}

	pCodecCtx = pFormatContext->streams[videoIndex]->codec;
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);										//寻找解码器id
	if(pCodec == NULL){
		return  (*env)->NewStringUTF(env,"解码失败，没有该视频对应的解码器\n");
	}

	if(avcodec_open2(pCodecCtx, pCodec, NULL) < 0){
		return  (*env)->NewStringUTF(env,"解码失败，无法打开解码器\n");
	}

	pFrame = av_frame_alloc();
	pFrameYUV = av_frame_alloc();
	out_buffer = (uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
	//pFrameYUV和out_buffer都有自己的一块内存，此函数自动将pFrameYUV按照YUV420格式关联到out_buffer上去
	avpicture_fill((AVPicture *)pFrameYUV, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
	packet = (AVPacket *)av_malloc(sizeof(AVPacket));

	//用于视频转换，图像色彩/分辨率和滤波，前三个参数是输入视频的宽高格式，然后转换后视频的宽高格式，后面参数依次是缩放算法、输入输出滤波信息、缩放算法的参数，返回缩放的结构体,当前函数只是对sws库初始哈u，真正的转换在sws_scale函数里面实现
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);


	 sprintf(info,   "[Input     ]%s\n", input_url);
	  sprintf(info, "%s[Output    ]%s\n",info,output_char);
	  sprintf(info, "%s[Format    ]%s\n",info, pFormatContext->iformat->name);
	  sprintf(info, "%s[Codec     ]%s\n",info, pCodecCtx->codec->name);
	  sprintf(info, "%s[Resolution]%dx%d\n",info, pCodecCtx->width,pCodecCtx->height);


	  fp_yuv = fopen(output_char,"wb+");
	  if(fp_yuv == NULL){
		  return  (*env)->NewStringUTF(env,"解码失败，无法打出输出文件\n");
	  }

	  frame_cnt = 0;
	  //启动程序到现在的时间
	  time_start = clock();

	  while(av_read_frame(pFormatContext,packet) >= 0){
		  if(packet->stream_index == videoIndex){
			  ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
			  if(ret  < 0){
				  return   (*env)->NewStringUTF(env,"解码失败，解码异常\n");
			  }
			  if(got_picture){
					//参数1是前面初始化的返回值，2、3时间输入图像信息分别是图像数据地址和图像颜色通道的每行存储的字节数组，4, 输入图像从第几行开始扫描，5 要扫描多少行，输出图像的地址和颜色通道每行数组
			                 sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
			                     pFrameYUV->data, pFrameYUV->linesize);
					//y通道占用大小，YUV420 U、V通道占Y/4,因为宽高各除以2就是4
			                 y_size=pCodecCtx->width*pCodecCtx->height;

			                 fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);    //Y
			                 fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U
			                 fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V
			                 //Output info 鉴别IBP帧，B会参考前后帧数据压缩率较高，P帧只参考前面的帧，压缩率低同时解码也快
			                 char pictype_str[10]={0};
			                 switch(pFrame->pict_type){
			                     case AV_PICTURE_TYPE_I:sprintf(pictype_str,"I");break;
			                   case AV_PICTURE_TYPE_P:sprintf(pictype_str,"P");break;
			                     case AV_PICTURE_TYPE_B:sprintf(pictype_str,"B");break;
			                     default:sprintf(pictype_str,"Other");break;
			                 }
			                // LOGI("Frame Index: %5d. Type:%s",frame_cnt,pictype_str);
			                 frame_cnt++;
			             }
		  }
		  av_free_packet(packet);
	  }

	  while (1) {
	         ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
	         if (ret < 0)
	             break;
	         if (!got_picture)
	             break;
	         sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
	             pFrameYUV->data, pFrameYUV->linesize);
	         int y_size=pCodecCtx->width*pCodecCtx->height;
	         fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);    //Y
	         fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U
	         fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V
	         //Output info
	         char pictype_str[10]={0};
	         switch(pFrame->pict_type){
	             case AV_PICTURE_TYPE_I:sprintf(pictype_str,"I");break;
	           case AV_PICTURE_TYPE_P:sprintf(pictype_str,"P");break;
	             case AV_PICTURE_TYPE_B:sprintf(pictype_str,"B");break;
	             default:sprintf(pictype_str,"Other");break;
	         }
	      //   LOGI("Frame Index: %5d. Type:%s",frame_cnt,pictype_str);
	         frame_cnt++;
	     }
	     time_finish = clock();
	     time_duration=(double)(time_finish - time_start);


	     sprintf(info, "%s[Time      ]%fms\n",info,time_duration);
	     sprintf(info, "%s[Count     ]%d\n",info,frame_cnt);


	     sws_freeContext(img_convert_ctx);
	     fclose(fp_yuv);

	        av_frame_free(&pFrameYUV);
	        av_frame_free(&pFrame);
	        avcodec_close(pCodecCtx);
	        avformat_close_input(&pFormatContext);
		return (*env)->NewStringUTF(env, "解码成功");
}
