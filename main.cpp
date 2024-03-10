
#include <iostream>
extern "C"
{
#include <libavformat/avformat.h>
}

int open_input_media_file(AVFormatContext** format_context, const char* input_file);
void clean_memory(AVFormatContext* format_context);

int open_input_media_file(AVFormatContext** format_context, const char* input_file)
{
    int response;

    *format_context = avformat_alloc_context();
    if(!(*format_context)) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate memory for the input format context.\n");
        return -2;
    }

    response = avformat_open_input(format_context, input_file, nullptr, nullptr);
    if(response != 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open the input file: %s.\n", input_file);
        return -3;
    }

    response = avformat_find_stream_info(*format_context, nullptr);
    if(response < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not find the input stream information.\n");
        return -4;
    }

    return 0;
}

void clean_memory(AVFormatContext* format_context)
{
    if(format_context != nullptr) {
        avformat_close_input(&format_context);
        avformat_free_context(format_context);
    }
}

int main(int argc, char* argv[])
{
    AVFormatContext* format_context = nullptr;

    if(argc != 2) {
        av_log(nullptr, AV_LOG_ERROR, "Usage: <input file>\n");
        return -1;
    }

    const char* input_file = argv[1];
    int response = open_input_media_file(&format_context, input_file);
    if(response != 0) {
        clean_memory(format_context);
        return response;
    }


    unsigned int count = format_context->nb_streams;
    std::cout <<"Количество потоков = " <<count<< std::endl;

    int64_t duration = format_context->duration + 5000;
    int64_t secs  = duration / AV_TIME_BASE;
    int64_t secs1 = secs;
    int64_t us    = duration % AV_TIME_BASE;
    us = 100*us/ AV_TIME_BASE;
    int64_t mins  = secs / 60;
    secs %= 60;
    int64_t hours = mins / 60;
    mins %= 60;

    std::cout << "Длительность файла "<<hours <<":" << mins  <<":" << secs <<"." << us  << std::endl;
    std::cout << "Длительность файла sec "<< secs1<< std::endl;

    int64_t  frame = format_context->streams[0]->nb_frames;
    std::cout <<"Количество кадров видео потока = " <<frame<< std::endl;

    for(int idx{0}; idx < count; ++idx) {
        AVMediaType type_id = avcodec_get_type(format_context->streams[idx]->codecpar->codec_id);
        const char *nameType = av_get_media_type_string(type_id);
        const char *nameCodec = avcodec_get_name(format_context->streams[idx]->codecpar->codec_id);

        std::cout <<nameType << " : тип кодека - "<< nameCodec<< std::endl;
    }

    clean_memory(format_context);
    return 0;
}



